#!/usr/bin/env python3
import os
import sys
import subprocess
import argparse
import time
from glob import glob
from dataclasses import dataclass
from typing import List, Optional
from enum import Enum, auto
from collections import defaultdict
import json  # <-- Added for JSON output

class TestResult(Enum):
    PASS = auto()
    FAIL = auto()
    ERROR = auto()

@dataclass
class TestMetadata:
    author: Optional[str] = None
    importance: Optional[int] = None
    tags: List[str] = None
    description: Optional[str] = None
    category: Optional[str] = None
    benchmark: bool = False

class Colors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

    @staticmethod
    def green(text): return f"{Colors.OKGREEN}{text}{Colors.ENDC}"
    @staticmethod
    def red(text): return f"{Colors.FAIL}{text}{Colors.ENDC}"
    @staticmethod
    def yellow(text): return f"{Colors.WARNING}{text}{Colors.ENDC}"

@dataclass
class TestCase:
    path: str
    expected_output_path: str
    expected_error_path: str
    metadata: TestMetadata = None

def parse_metadata(file_path: str) -> TestMetadata:
    metadata = TestMetadata(tags=[])
    with open(file_path, 'r', encoding='utf-8') as f:
        for line in f:
            line = line.strip()
            if not line.startswith("// @"):
                continue
            if line.startswith("// @author"):
                metadata.author = line.partition("@author")[2].strip()
            elif line.startswith("// @importance"):
                try:
                    metadata.importance = int(line.partition("@importance")[2].strip())
                except ValueError:
                    pass
            elif line.startswith("// @tag"):
                tag = line.partition("@tag")[2].strip()
                if tag:
                    metadata.tags.append(tag)
            elif line.startswith("// @description"):
                metadata.description = line.partition("@description")[2].strip()
    return metadata

def parse_args():
    parser = argparse.ArgumentParser(description="Run Zia tests.")
    parser.add_argument("binary", help="Path to Zia binary")
    parser.add_argument("tests", help="Path to test directory")
    parser.add_argument("--tag", action="append", help="Run only tests with specific tag(s)")
    parser.add_argument("--min-importance", type=int, help="Minimum importance level")
    parser.add_argument("--interactive", action="store_true", help="Interactive mode")
    parser.add_argument("--html", help="Generate HTML report to the given file")
    parser.add_argument("--parallel", type=int, help="Run tests in parallel using N workers")
    parser.add_argument("--only", help="Run only a specific .zia file")
    return parser.parse_args()

def find_test_files(root_dir: str, category_filter: Optional[str] = None) -> List[TestCase]:
    test_cases = []
    for dirpath, _, filenames in os.walk(root_dir):
        if "expected" in dirpath:
            continue
        for filename in filenames:
            if not filename.endswith('.zia'):
                continue
            full_path = os.path.join(dirpath, filename)
            metadata = parse_metadata(full_path)
            category = metadata.tags[0] if metadata.tags else os.path.basename(os.path.dirname(full_path))
            metadata.category = category
            if category_filter and category != category_filter:
                continue
            output_path, error_path = get_expected_files(full_path)
            test_cases.append(TestCase(
                path=full_path,
                expected_output_path=output_path,
                expected_error_path=error_path,
                metadata=metadata
            ))
    return sorted(test_cases, key=lambda tc: tc.path)

def get_expected_files(test_path: str) -> tuple[str, str]:
    rel_path = os.path.relpath(test_path, start="tests")
    base_name = os.path.splitext(rel_path)[0]
    output_path = os.path.join("tests", "expected", f"{base_name}.out")
    error_path = os.path.join("tests", "expected", f"{base_name}.outf")
    return output_path, error_path

def read_expected_file(path: str) -> List[str]:
    if not os.path.exists(path):
        return []
    with open(path, 'r', encoding='utf-8') as f:
        content = f.read()
        if content.startswith('[') and content.endswith(']'):
            import ast
            try:
                return ast.literal_eval(content)
            except:
                return [line.strip(' "\'') for line in content[1:-1].split(',')]
        # Only strip newlines, preserve other whitespace
        return [line.rstrip('\n') for line in content.splitlines()]

def compare_error_messages(actual: List[str], expected: List[str]) -> bool:
    normalized_actual = [msg.strip(' "\'') for msg in actual]
    normalized_expected = [msg.strip(' "\'') for msg in expected]
    return normalized_actual == normalized_expected

def run_test(binary: str, test_case: TestCase) -> tuple[TestResult, str, float]:
    try:
        start = time.perf_counter()
        result = subprocess.run(
            [binary, test_case.path],
            capture_output=True,
            text=True,
            timeout=5
        )
        duration = time.perf_counter() - start

        expected_output = read_expected_file(test_case.expected_output_path)
        expected_errors = read_expected_file(test_case.expected_error_path)
        output_lines = [line.rstrip('\n') for line in result.stdout.splitlines() if line.strip()]
        if output_lines != expected_output:
            return (
                TestResult.FAIL,
                f"Output mismatch\nExpected: {expected_output}\nGot: {output_lines}",
                duration
            )
        error_lines = [line.rstrip('\n') for line in result.stderr.splitlines() if line.strip()]
        if not compare_error_messages(error_lines, expected_errors):
            return (
                TestResult.FAIL,
                f"Error mismatch\nExpected: {expected_errors}\nGot: {error_lines}",
                duration
            )
        return (TestResult.PASS, "", duration)

    except subprocess.TimeoutExpired:
        return (TestResult.ERROR, "Timeout after 5 seconds", 5.0)
    except Exception as e:
        return (TestResult.ERROR, f"Execution failed: {str(e)}", 0.0)

def print_result(test_case: TestCase, result: TestResult, message: str, duration: float):
    rel_path = os.path.relpath(test_case.path)
    time_info = f"{duration:.2f}s"
    if result == TestResult.PASS:
        print(f"{Colors.green('PASS')} {rel_path} [{time_info}]")
    elif result == TestResult.FAIL:
        print(f"{Colors.red('FAIL')} {rel_path}: {message} [{time_info}]")
    else:
        print(f"{Colors.yellow('ERROR')} {rel_path}: {message} [{time_info}]")

def main():
    args = parse_args()
    binary_path = args.binary
    tests_dir = args.tests

    # Handle --only single test file
    if args.only:
        if not os.path.isfile(args.only) or not args.only.endswith(".zia"):
            print(f"{Colors.red('✖')} Invalid file path for --only: {args.only}")
            sys.exit(1)
        metadata = parse_metadata(args.only)
        output_path, error_path = get_expected_files(args.only)
        test_case = TestCase(
            path=args.only,
            expected_output_path=output_path,
            expected_error_path=error_path,
            metadata=metadata
        )
        test_files = [test_case]
    else:
        test_files = find_test_files(tests_dir)

    all_tests: List[TestCase] = []

    for test in test_files:
        metadata = test.metadata

        # Filtering
        if args.tag and not any(tag in metadata.tags for tag in args.tag):
            continue
        if args.min_importance and (metadata.importance or 0) < args.min_importance:
            continue

        all_tests.append(test)

    if not all_tests:
        print("No tests matched your filters.")
        sys.exit(1)

    passed, failed, errors = 0, 0, 0
    tag_stats = defaultdict(int)
    test_results = []

    for test_case in all_tests:
        result, message, duration = run_test(binary_path, test_case)
        print_result(test_case, result, message, duration)

        if result == TestResult.PASS:
            passed += 1
            for tag in test_case.metadata.tags:
                tag_stats[tag] += 1
        elif result == TestResult.FAIL:
            failed += 1
        else:
            errors += 1

        test_results.append({
            "file": os.path.relpath(test_case.path),
            "status": result.name,
            "time": duration,
            "author": test_case.metadata.author or "",
            "tags": test_case.metadata.tags,
            "description": test_case.metadata.description or "",
        })

    print("\nTest Summary:")
    print(f"{Colors.green('PASSED')}: {passed}")
    print(f"{Colors.red('FAILED')}: {failed}")
    print(f"{Colors.yellow('ERRORS')}: {errors}")

    if tag_stats:
        print("\nTags Summary:")
        for tag, count in tag_stats.items():
            print(f"  {tag}: {count} passed")

    if args.html:
        summary = {
            "total": len(test_results),
            "passed": passed,
            "failed": failed,
            "errors": errors
        }
        results_json = {
            "summary": summary,
            "tests": test_results
        }
        with open(args.html, "w", encoding="utf-8") as f:
            json.dump(results_json, f, indent=2, ensure_ascii=False)
        print(f"\n✅ JSON test results saved to {args.html}")

    sys.exit(1 if failed or errors else 0)

if __name__ == "__main__":
    main()
