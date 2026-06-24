#!/usr/bin/env python3
import sys
import json
from pathlib import Path

SCRIPT_DIR = Path(__file__).resolve().parent

if len(sys.argv) > 1:
    INPUT_JSON = Path(sys.argv[1])
else:
    INPUT_JSON = SCRIPT_DIR / "reverse_lut_default.json"

OUTPUT_HPP = SCRIPT_DIR / "lut_gen.hpp"

pattern_counter = 0


def escape_cpp(s: str) -> str:
    return s.replace("\\", "\\\\").replace('"', '\\"')


def split_patterns(key: str):
    return [p.strip() for p in key.split("|")]


def gen_match_any(arg, patterns, indent):
    global pattern_counter

    name = f"patterns_{pattern_counter}"
    pattern_counter += 1

    ind = " " * indent

    code = []
    code.append(f"{ind}constexpr std::string_view {name}[] = {{")
    for p in patterns:
        code.append(f'{ind}    "{escape_cpp(p)}",')
    code.append(f"{ind}}};")
    code.append(f"{ind}if (matchAny({arg}, {name})) {{")

    return code, name


def generate():

    if not INPUT_JSON.exists():
        raise FileNotFoundError(f"{INPUT_JSON} missing")

    with open(INPUT_JSON) as f:
        data = json.load(f)

    lookup = data["lookup"]
    fallback = int(data.get("fallthrough_afid", 0))

    with open(OUTPUT_HPP, "w") as out:

        # ===== HEADER =====
        out.write("// Auto-generated. Do not edit.\n\n")
        out.write("#pragma once\n\n")
        out.write("#include <string_view>\n\n")

        out.write("struct AFIDResult {\n")
        out.write("    int afid;\n")
        out.write("    std::string_view origin;\n")
        out.write("};\n\n")

        # ===== MATCH UTILS =====
        out.write("constexpr bool starts_with(std::string_view s, std::string_view p) {\n")
        out.write("    return s.substr(0, p.size()) == p;\n}\n\n")

        out.write("constexpr bool ends_with(std::string_view s, std::string_view p) {\n")
        out.write("    return s.size() >= p.size() && s.substr(s.size()-p.size()) == p;\n}\n\n")

        out.write("constexpr bool matchPattern(std::string_view str, std::string_view pat) {\n")
        out.write("    if (pat == \"*\") return true;\n")
        out.write("    auto s = pat.find('*');\n")
        out.write("    if (s == std::string_view::npos) return str == pat;\n")
        out.write("    auto prefix = pat.substr(0, s);\n")
        out.write("    auto suffix = pat.substr(s + 1);\n")
        out.write("    if (!prefix.empty() && str.substr(0, prefix.size()) != prefix) return false;\n")
        out.write("    if (!suffix.empty() && str.substr(str.size()-suffix.size()) != suffix) return false;\n")
        out.write("    return true;\n}\n\n")

        out.write("template <size_t N>\n")
        out.write("constexpr bool matchAny(std::string_view v, const std::string_view(&arr)[N]) {\n")
        out.write("    for (auto p : arr) if (matchPattern(v, p)) return true;\n")
        out.write("    return false;\n}\n\n")

        # ===== FUNCTION =====
        out.write("constexpr AFIDResult lookupAFID(\n")
        out.write("    std::string_view registry,\n")
        out.write("    std::string_view msgId,\n")
        out.write("    std::string_view arg1,\n")
        out.write("    std::string_view arg2,\n")
        out.write("    std::string_view arg3,\n")
        out.write("    std::string_view arg4)\n")
        out.write("{\n")

        # suppress warnings
        out.write("    (void)arg3;\n")
        out.write("    (void)arg4;\n\n")

        # ===== GENERATION =====
        for reg, reg_val in lookup.items():

            out.write(f'    if (registry == "{escape_cpp(reg)}") {{\n')

            for msg, msg_val in reg_val.items():

                if msg != "*":
                    out.write(f'        if (msgId == "{escape_cpp(msg)}") {{\n')
                else:
                    out.write("        // wildcard messageId\n")

                for a1, a1_val in msg_val.items():

                    if a1 != "*":
                        code, _ = gen_match_any("arg1", split_patterns(a1), 12)
                        out.write("\n".join(code) + "\n")
                    else:
                        out.write("            if (true) {\n")

                    if isinstance(a1_val, dict) and "afid" not in a1_val:

                        for a2, leaf in a1_val.items():
                            code, name = gen_match_any("arg2", split_patterns(a2), 16)
                            out.write("\n".join(code) + "\n")

                            out.write(
                                f'                return {{ {leaf["afid"]}, "{escape_cpp(leaf["originOfCondition"])}" }};\n'
                            )

                            out.write("                }\n")  # close arg2 if

                    else:
                        out.write(
                            f'                return {{ {a1_val["afid"]}, "{escape_cpp(a1_val["originOfCondition"])}" }};\n'
                        )

                    out.write("            }\n")  # close arg1 if

                if msg != "*":
                    out.write("        }\n")

            out.write("    }\n\n")

        # fallback
        out.write(f"    return {{ {fallback}, \"\" }};\n")
        out.write("}\n")

    print(f"Generated {OUTPUT_HPP}")


if __name__ == "__main__":
    generate()