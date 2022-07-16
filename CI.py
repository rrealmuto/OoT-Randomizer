# This script is called by GitHub Actions, see .github/workflows/python.yml

import json
import sys
from SettingsList import logic_tricks
from Utils import data_path

num_errors = 0

def error(msg):
    global num_errors

    print(msg, file=sys.stderr)
    num_errors += 1

with open(data_path('presets_default.json'), encoding='utf-8') as f:
    presets = json.load(f)
    for trick in logic_tricks.values():
        if trick['name'] not in presets['Hell Mode']['allowed_tricks']:
            error(f'Logic trick {trick["name"]!r} missing from Hell Mode preset.')

if num_errors > 0:
    print(f'CI failed with {num_errors} errors.', file=sys.stderr)
    sys.exit(1)
else:
    print(f'CI checks successful.')
