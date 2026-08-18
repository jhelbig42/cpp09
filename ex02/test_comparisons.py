#!/usr/bin/env python3
"""Tester for ex02 PmergeMe: checks the reported comparison counts.

For a range of input sizes it:
  - verifies the program's output is actually sorted
  - verifies std::vector and std::list report the same comparison count
    (both run the exact same engine, so they must always match)
  - computes the Ford-Johnson worst-case comparison count via Knuth's
    closed-form formula and checks no measured count ever exceeds it
"""

import random
import re
import subprocess
import sys
from pathlib import Path

BINARY = Path(__file__).resolve().parent / "PmergeMe"

OUTPUT_RE = re.compile(
    r"Before:(?P<before>.*)\n"
    r"After:(?P<after>.*)\n"
    r"Comparisons for std::vector with \d+ elements : (?P<vec>\d+)\n"
    r"Comparisons for std::list with \d+ elements : (?P<lst>\d+)"
)


def ceil_log2(m: int) -> int:
    """ceil(log2(m)) for integer m >= 1, computed exactly (no floats)."""
    if m <= 1:
        return 0
    return (m - 1).bit_length()


def ford_johnson_worst_case(n: int) -> int:
    """Knuth's closed-form worst-case comparison count for merge-insertion
    sort of n elements: sum_{k=1..n} ceil(log2(3k/4))."""
    return sum(ceil_log2(3 * k) - 2 for k in range(1, n + 1))


def run(args):
    result = subprocess.run(
        [str(BINARY)] + [str(a) for a in args],
        capture_output=True, text=True, timeout=30,
    )
    if result.returncode != 0:
        raise RuntimeError(f"program failed: {result.stderr.strip()}")
    match = OUTPUT_RE.search(result.stdout)
    if not match:
        raise RuntimeError(f"unexpected output:\n{result.stdout}")
    before = list(map(int, match["before"].split()))
    after = list(map(int, match["after"].split()))
    return before, after, int(match["vec"]), int(match["lst"])


def check_case(values, label, worst_case_cache):
    before, after, vec_cmp, lst_cmp = run(values)
    n = len(values)

    if sorted(before) != after:
        return f"FAIL [{label}]: output not sorted correctly", vec_cmp, n

    if vec_cmp != lst_cmp:
        return (f"FAIL [{label}]: vector/list comparison counts differ "
                f"({vec_cmp} vs {lst_cmp}) -- should always be identical"), vec_cmp, n

    if n not in worst_case_cache:
        worst_case_cache[n] = ford_johnson_worst_case(n)
    limit = worst_case_cache[n]

    if vec_cmp > limit:
        return (f"FAIL [{label}]: {vec_cmp} comparisons exceeds the "
                f"Ford-Johnson worst-case bound of {limit} for n={n}"), vec_cmp, n

    return None, vec_cmp, n


def main():
    if not BINARY.exists():
        print(f"error: {BINARY} not found -- run `make` in ex02 first", file=sys.stderr)
        return 1

    random.seed(42)
    failures = []
    worst_case_cache = {}
    max_seen = {}
    n_values = list(range(1, 41))
    trials_per_n = 15
    total_cases = 0

    for n in n_values:
        cases = [
            (f"random n={n} #{t}", [random.randint(1, 10_000_000) for _ in range(n)])
            for t in range(trials_per_n)
        ]
        cases += [
            (f"sorted n={n}", list(range(1, n + 1))),
            (f"reverse n={n}", list(range(n, 0, -1))),
            (f"duplicates n={n}", [1] * n),
        ]

        for label, values in cases:
            total_cases += 1
            err, vec_cmp, _ = check_case(values, label, worst_case_cache)
            if err:
                failures.append(err)
            else:
                max_seen[n] = max(max_seen.get(n, 0), vec_cmp)

    print(f"ran {total_cases} cases for n=1..{n_values[-1]}")

    if failures:
        print(f"\n{len(failures)} FAILURE(S):")
        for f in failures:
            print(f"  {f}")
        return 1

    print("all cases passed: output sorted, vector/list comparisons match, "
          "and no case exceeded the Ford-Johnson worst-case bound.\n")

    print("max comparisons observed vs. theoretical worst case:")
    print(f"{'n':>4} {'max seen':>10} {'theoretical':>12}")
    for n in n_values:
        if n in max_seen:
            print(f"{n:>4} {max_seen[n]:>10} {worst_case_cache[n]:>12}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
