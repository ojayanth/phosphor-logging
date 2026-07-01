#!/usr/bin/env python3

import json
import argparse
import sys
import re


def is_leaf(node):
    return isinstance(node, dict) and "afid" in node


def split_patterns(pattern):
    return pattern.split("|")


def is_prefix(pattern):
    return pattern.endswith("*")


def check_overlap(patterns):
    """Detect overlapping patterns (basic prefix conflict check)."""
    expanded = []
    for p in patterns:
        expanded.extend(split_patterns(p))

    for i in range(len(expanded)):
        for j in range(i + 1, len(expanded)):
            a = expanded[i]
            b = expanded[j]

            if a == "*" or b == "*":
                continue

            if is_prefix(a) and is_prefix(b):
                if a[:-1].startswith(b[:-1]) or b[:-1].startswith(a[:-1]):
                    raise ValueError(f"Overlapping prefix patterns: '{a}' and '{b}'")

            elif is_prefix(a):
                if b.startswith(a[:-1]):
                    raise ValueError(f"Pattern overlap: '{a}' matches '{b}'")

            elif is_prefix(b):
                if a.startswith(b[:-1]):
                    raise ValueError(f"Pattern overlap: '{b}' matches '{a}'")

            elif a == b:
                raise ValueError(f"Duplicate exact pattern: '{a}'")


def validate_node(node, depth=0):
    """Recursively validate LUT structure."""

    if is_leaf(node):
        if "originOfCondition" not in node:
            raise ValueError("Leaf node missing 'originOfCondition'")
        return

    if not isinstance(node, dict):
        raise ValueError(f"Invalid node type at depth {depth}")

    keys = list(node.keys())

    # Check overlap at this level
    check_overlap(keys)

    for key, child in node.items():
        if not isinstance(key, str):
            raise ValueError(f"Invalid key type: {key}")

        validate_node(child, depth + 1)


def validate_lookup_structure(lookup):
    """Validate full LUT structure."""

    if not isinstance(lookup, dict):
        raise ValueError("lookup must be a dictionary")

    for registry, messages in lookup.items():
        if not isinstance(messages, dict):
            raise ValueError(f"Invalid registry structure: {registry}")

        for msg, node in messages.items():
            if not isinstance(node, dict):
                raise ValueError(f"Invalid message node: {msg}")

            # Enforce at least arg1 level
            if is_leaf(node):
                raise ValueError(
                    f"Message '{msg}' must contain arg1 level (cannot be leaf directly)"
                )

            validate_node(node, depth=0)


def validate_json(data):
    """Top-level validation."""

    if "lookup" not in data:
        raise ValueError("Missing 'lookup' section")

    validate_lookup_structure(data["lookup"])

    if "fallthrough_afid" not in data:
        raise ValueError("Missing 'fallthrough_afid'")

    if not isinstance(data["fallthrough_afid"], int):
        raise ValueError("'fallthrough_afid' must be integer")


def main():
    parser = argparse.ArgumentParser(description="Validate AEL LUT JSON")
    parser.add_argument("--input", required=True, help="Path to LUT JSON file")

    args = parser.parse_args()

    try:
        with open(args.input, "r") as f:
            data = json.load(f)

        validate_json(data)

        print(f"[AEL] Validation PASSED: {args.input}")
        sys.exit(0)

    except Exception as e:
        print(f"[AEL] Validation FAILED: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()