import sys

import shutil
import subprocess

against = sys.argv[1]

subprocess.run([sys.executable, 'update-presets.py', '--hook'], check=True)

old_cargo_toml = subprocess.run(['git', 'show', f'{against}:Cargo.toml'], stdout=subprocess.PIPE, encoding='utf-8', check=True).stdout
with open('Cargo.toml', encoding='utf-8') as f:
    new_cargo_toml = f.read()

for old_line, new_line in zip(old_cargo_toml.splitlines(), new_cargo_toml.splitlines()):
    if old_line.startswith('version = '):
        if old_line == new_line:
            sys.exit('Missing version bump in Cargo.toml')

old_cargo_lock = subprocess.run(['git', 'show', f'{against}:Cargo.lock'], stdout=subprocess.PIPE, encoding='utf-8', check=True).stdout
with open('Cargo.lock', encoding='utf-8') as f:
    new_cargo_lock = f.read()

if old_cargo_lock == new_cargo_lock: #TODO more precisely compare the version field
    sys.exit('Missing version bump in Cargo.lock')

subprocess.run(['cargo', 'build', '--lib', '--release'], check=True)
subprocess.run(['cargo', 'build', '--release', '--bin=ootr'], check=True)
shutil.copyfile('target/release/rs.dll', 'rs.pyd')
