import sys

import subprocess

against = sys.argv[1]

subprocess.run([sys.executable, 'update-presets.py', '--hook'], check=True)

old_version_py = subprocess.run(['git', 'show', f'{against}:version.py'], stdout=subprocess.PIPE, encoding='utf-8', check=True).stdout
with open('version.py', encoding='utf-8') as f:
    new_version_py = f.read()

for old_line, new_line in zip(old_version_py.splitlines(), new_version_py.splitlines()):
    if old_line.startswith('supplementary_version = '):
        if old_line == new_line:
            sys.exit('Missing version bump')
