#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0

import argparse
import json
from pathlib import Path


# ==========================================================
# CLI
# ==========================================================
def parse_args():
    p = argparse.ArgumentParser()
    p.add_argument("--input", required=True)
    p.add_argument("--output", required=True)
    return p.parse_args()


# ==========================================================
# Helpers
# ==========================================================
def is_leaf(n):
    return isinstance(n, dict) and "afid" in n


def split_patterns(k):
    return [p.strip() for p in k.split("|")]


def normalize_pattern(p):
    return p.replace("*", "")


def is_overlap(p1, p2):
    """
    Detect prefix overlap:
    BRB_* vs BRB_L_*
    """
    s1 = normalize_pattern(p1)
    s2 = normalize_pattern(p2)

    return s1.startswith(s2) or s2.startswith(s1)


# ==========================================================
# Overlap Detection
# ==========================================================
def detect_overlaps(lookup):
    overlaps = []

    def check(node, path):

        if is_leaf(node):
            return

        keys = list(node.keys())

        for i in range(len(keys)):
            for j in range(i + 1, len(keys)):
                p1 = keys[i]
                p2 = keys[j]

                for a in split_patterns(p1):
                    for b in split_patterns(p2):
                        if a != b and is_overlap(a, b):
                            overlaps.append((path, a, b))

        for k, v in node.items():
            check(v, f"{path}->{k}")

    check(lookup, "root")
    return overlaps


# ==========================================================
# Positive Case Generator
# ==========================================================
def generate_positive_cases(lookup):

    cases = []

    def walk(reg, msg, node, args, depth):

        if is_leaf(node):
            cases.append(
                {
                    "registry": reg,
                    "msg": msg,
                    "args": args.copy(),
                    "afid": node["afid"],
                    "origin": node["originOfCondition"],
                }
            )
            return

        for k, v in node.items():

            if depth >= 4:
                continue

            if k == "*":
                val = f"ANY{depth}"
            else:
                val = split_patterns(k)[0].replace("*", "X")

            new_args = args.copy()
            new_args[depth] = val

            walk(reg, msg, v, new_args, depth + 1)

    for reg in sorted(lookup):
        for msg in sorted(lookup[reg]):
            walk(reg, msg, lookup[reg][msg], ["", "", "", ""], 0)

    return cases


# ==========================================================
# Negative Cases
# ==========================================================
def generate_negative_cases(lookup, fallback):

    cases = []

    # Invalid registry
    cases.append(
        {
            "registry": "INVALID.REG",
            "msg": "INVALID_MSG",
            "args": ["", "", "", ""],
            "afid": fallback,
        }
    )

    for reg in lookup:

        # Invalid message
        cases.append(
            {
                "registry": reg,
                "msg": "INVALID_MSG",
                "args": ["", "", "", ""],
                "afid": fallback,
            }
        )

    return cases


# ==========================================================
# Overlap Behavior Tests
# ==========================================================
def generate_overlap_tests(lookup):

    tests = []

    def walk(node, path):

        if is_leaf(node):
            return

        keys = list(node.keys())

        for i in range(len(keys)):
            for j in range(i + 1, len(keys)):
                p1 = keys[i]
                p2 = keys[j]

                for a in split_patterns(p1):
                    for b in split_patterns(p2):
                        if a != b and is_overlap(a, b):

                            # create specific vs generic test
                            tests.append(
                                {"pattern_big": a, "pattern_small": b}
                            )

        for v in node.values():
            walk(v, path)

    walk(lookup, "")
    return tests


# ==========================================================
# Emit C++
# ==========================================================
def emit_cpp(pos, neg, overlap, output, fallback):

    with open(output, "w") as out:

        out.write("// SPDX-License-Identifier: Apache-2.0\n")
        out.write("// AUTO-GENERATED TESTS\n\n")

        out.write("#include <gtest/gtest.h>\n")
        out.write('#include "amd_ael_decoder.hpp"\n\n')

        out.write("using phosphor::logging::amd::getAFID;\n\n")

        # ---------------- Positive ----------------
        out.write("TEST(AELTest, PositiveCases) {\n")

        for i, c in enumerate(pos):
            a = c["args"]

            out.write(f"    // Case {i}\n")
            out.write("    {\n")
            out.write(
                f'    auto r = getAFID("{c["registry"]}", "{c["msg"]}", '
                f'{{"{a[0]}", "{a[1]}", "{a[2]}", "{a[3]}"}});\n'
            )
            out.write("    ASSERT_TRUE(r.has_value());\n")
            out.write(f"    EXPECT_EQ(r->afid, {c['afid']});\n")
            out.write("    }\n")

        out.write("}\n\n")

        # ---------------- Negative ----------------
        out.write("TEST(AELTest, NegativeCases) {\n")

        for i, c in enumerate(neg):
            a = c["args"]

            out.write(f"    // Negative {i}\n")
            out.write("    {\n")
            out.write(
                f'    auto r = getAFID("{c["registry"]}", "{c["msg"]}", '
                f'{{"{a[0]}", "{a[1]}", "{a[2]}", "{a[3]}"}});\n'
            )
            out.write("    ASSERT_TRUE(r.has_value());\n")
            out.write(f"    EXPECT_EQ(r->afid, {fallback});\n")
            out.write("    }\n")

        out.write("}\n\n")

        # ---------------- Overlap ----------------
        out.write("TEST(AELTest, OverlapDetection) {\n")

        if len(overlap) == 0:
            out.write("    SUCCEED();\n")

        for i, t in enumerate(overlap):
            out.write(
                f'    ADD_FAILURE() << "Overlap detected: {t["pattern_big"]} vs {t["pattern_small"]}";\n'
            )

        out.write("}\n")


# ==========================================================
# MAIN
# ==========================================================
if __name__ == "__main__":
    args = parse_args()

    with open(args.input) as f:
        data = json.load(f)

    lookup = data["lookup"]
    fallback = int(data.get("fallthrough_afid", -1))

    overlaps = detect_overlaps(lookup)

    if overlaps:
        print("[AEL] ERROR: overlapping patterns detected:")
        for o in overlaps:
            print(o)
        exit(1)

    pos = generate_positive_cases(lookup)
    neg = generate_negative_cases(lookup, fallback)
    ov = generate_overlap_tests(lookup)

    Path(args.output).parent.mkdir(parents=True, exist_ok=True)

    emit_cpp(pos, neg, ov, args.output, fallback)

    print(f"[AEL] Generated tests: {len(pos)} pos, {len(neg)} neg")
