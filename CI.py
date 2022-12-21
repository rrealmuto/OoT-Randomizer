# This script is called by GitHub Actions, see .github/workflows/python.yml
# To fix code style errors, run: python3 ./CI.py --fix --no_unit_tests

import json
import sys
from io import StringIO
import unittest
import os.path
import pathlib
import argparse

import Unittest as Tests
from SettingsList import logic_tricks
from Utils import data_path


def error(msg):
    if not hasattr(error, "count"):
        error.count = 0
    print(msg, file=sys.stderr)
    error.count += 1


def run_unit_tests():
    # Run Unit Tests
    stream = StringIO()
    runner = unittest.TextTestRunner(stream=stream)
    suite = unittest.defaultTestLoader.loadTestsFromModule(Tests)
    result = runner.run(suite)
    print(f'Tests run: {result.testsRun}.')
    stream.seek(0)
    print(f'Test output:\n{stream.read()}')
    if result.errors:
        error('Unit Tests had an error, see output above.')


def check_hell_mode_tricks():
    # Check for tricks missing from Hell Mode preset.
    with open(data_path('presets_default.json'), encoding='utf-8') as f:
        presets = json.load(f)
        for trick in logic_tricks.values():
            if trick['name'] not in presets['Hell Mode']['allowed_tricks']:
                error(f'Logic trick {trick["name"]!r} missing from Hell Mode preset.')


def check_code_style(fix_errors=False):
    # Check for code style errors
    repo_dir = pathlib.Path(os.path.dirname(os.path.realpath(__file__)))

    def check_file_format(path):
        fixed = ''
        with path.open(encoding='utf-8', newline='') as file:
            path = path.relative_to(repo_dir)
            for i, line in enumerate(file, start=1):
                if not line.endswith('\n'):
                    error(f'Missing line break at end of {path}')
                    line += '\n'
                if line.endswith('\r\n'):
                    error(f'Line {i} of {path} ends with CRLF')
                    line = line.rstrip('\r\n') + '\n'
                line = line.rstrip('\n')
                if '\t' in line:
                    error(f'Hard tab on line {i} of {path}')
                    fixed_line = ''
                    for c in line:
                        if c == '\t':
                            fixed_line += ' ' * (4 - len(fixed_line) % 4)
                        else:
                            fixed_line += c
                    line = fixed_line
                if line.endswith(' '):
                    error(f'Trailing whitespace on line {i} of {path}')
                    line = line.rstrip(' ')
                fixed += line + '\n'
        if fix_errors:
            with path.open('w', encoding='utf-8', newline='') as file:
                file.write(fixed)

    for path in repo_dir.iterdir():
        if path.suffix == '.py':
            check_file_format(path)
    for path in (repo_dir / 'ASM').iterdir():
        if path.suffix == '.py':
            check_file_format(path)
    for path in (repo_dir / 'ASM' / 'c').iterdir():
        if path.suffix in ('.c', '.h'):
            check_file_format(path)
    for path in (repo_dir / 'ASM' / 'src').iterdir():
        if path.suffix == '.asm':
            check_file_format(path)


def run_ci_checks():
    parser = argparse.ArgumentParser()
    parser.add_argument('--no_unit_tests', help="Skip unit tests", action='store_true')
    parser.add_argument('--only_unit_tests', help="Only run unit tests", action='store_true')
    parser.add_argument('--fix', help='Automatically apply fixes where possible', action='store_true')
    args = parser.parse_args()

    if args.only_unit_tests:
        run_unit_tests()
        exit_ci()

    if not args.no_unit_tests:
        run_unit_tests()
    check_hell_mode_tricks()
    check_code_style(args.fix)
    exit_ci()


def exit_ci():
    if hasattr(error, "count") and error.count:
        print(f'CI failed with {error.count} errors.', file=sys.stderr)
        sys.exit(1)
    else:
        print(f'CI checks successful.')
        sys.exit(0)


if __name__ == '__main__':
    run_ci_checks()
