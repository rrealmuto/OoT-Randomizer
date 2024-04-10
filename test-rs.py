"""Test the randomizer using plando.json.

Usage:
  test.py [options]
  test.py (-h | --help)

Options:
  -E, --no-emu                  don't automatically launch BizHawk
  -h, --help                    show this help message and exit
  -l, --log                     open the spoiler log in VS Code
  -n, --worlds=<count>          generate a multiworld seed for this many players [default: 1] [type: int]
  -o, --output=<path>           save randomizer output to the given file
  -v, --verbose                 always show randomizer output
  --patch                       generate a .zpf file
  --preset=<preset>             use this settings preset instead of or in addition to plando.json
  --seeds=<count>               generate the given number of seeds and count successes and failures. Implies --no-emu [default: 1] [type: int]
  --settings=<settings_string>  use this settings string instead of or in addition to plando.json
"""

import sys

import json
import pathlib
import shutil
import subprocess

import type_docopt # PyPI: type-docopt

BIZHAWK_PATH = pathlib.Path.home() / 'bin' / 'BizHawk'

arguments = type_docopt.docopt(__doc__)

subprocess.run(['cargo', 'build'], check=True)
shutil.copy('target/debug/rs.dll', 'rs.pyd')

settings = {
    'rom': 'C:\\Users\\fenhl\\games\\zelda\\oot\\oot-ntscu-1.0.z64',
    'output_dir': 'C:\\Users\\fenhl\\games\\zelda\\oot',
    'enable_distribution_file': True,
    'distribution_file': 'C:\\Users\\fenhl\\git\\github.com\\fenhl\\OoT-Randomizer\\stage\\plando.json',
    'create_spoiler': True,
    'create_cosmetics_log': False,
    'create_patch_file': arguments['--patch'],
    'create_compressed_rom': not arguments['--patch'],
    'world_count': arguments['--worlds'],
    'player_num': 1,
}

seed_condition = lambda spoiler: True

successes = 0
failures = 0
for _ in range(arguments['--seeds']):
    failed = False
    while True:
        iter_settings = settings.copy()
        rando_args = ['target/debug/ootr', '--settings=-']
        with open(settings['distribution_file'], encoding='utf-8') as f:
            if json.load(f) == {}:
                # don't show “Plandomizer” in file select since nothing has actually been plando'd
                del iter_settings['distribution_file']
                iter_settings['enable_distribution_file'] = False
        if arguments['--preset']:
            rando_args.append(f'--settings_preset={arguments["--preset"]}')
        if arguments['--settings']:
            rando_args.append(f'--settings_string={arguments["--settings"]}')
        if iter_settings['enable_distribution_file'] and (arguments['--preset'] or arguments['--settings']):
            input('plando.json will override settings preset/string, press return to confirm')
        process = subprocess.run(rando_args, input=json.dumps(iter_settings), stderr=None if arguments['--verbose'] else subprocess.PIPE, encoding='utf-8')
        if process.returncode != 0:
            if arguments['--seeds'] > 1:
                failed = True
                break
            else:
                if arguments['--output']:
                    stderr = open(arguments['--output'], 'w', encoding='utf-8')
                else:
                    stderr = sys.stderr
                print(process.stderr, file=stderr)
                if arguments['--output']:
                    stderr.close()
                sys.exit(process.returncode)
        for line in reversed(process.stderr.splitlines()): #TODO fix for --verbose
            if arguments['--patch']:
                if line.startswith('Created patch file archive at: '):
                    rom_path = pathlib.Path(line[len('Created patch file archive at: '):])
                    break
            else:
                if line.lower().startswith('created compressed rom at: '):
                    rom_path = pathlib.Path(line[len('Created compressed ROM at: '):])
                    break
                elif line.lower().startswith('created uncompressed rom at: '):
                    rom_path = pathlib.Path(line[len('Created uncompressed ROM at: '):])
                    break
        else:
            sys.exit(f'Randomizer did not report {"patch" if arguments["--patch"] else "rom"} location')
        base_name = rom_path.stem.rstrip('-comp')
        if base_name.endswith(f'W{arguments["--worlds"]}P1'):
            base_name = base_name[:-2]
        spoiler_path = rom_path.parent / f'{base_name}_Spoiler.json'
        with spoiler_path.open() as spoiler_f:
            spoiler = json.load(spoiler_f)
        if seed_condition(spoiler):
            break
        else:
            rom_path.unlink()
            spoiler_path.unlink()
            if iter_settings['enable_distribution_file']:
                (rom_path.parent / f'{base_name}_Distribution.json').unlink()
    if failed:
        failures += 1
        continue
    successes += 1
    if arguments['--log']:
        code = subprocess.Popen(['C:/Program Files/Microsoft VS Code/bin/code.cmd', '--wait', str(spoiler_path)])
    if arguments['--no-emu'] or arguments['--patch'] or arguments['--seeds'] > 1:
        if arguments['--seeds'] == 1 and not arguments['--log']:
            input('press return to delete the seed')
    else:
        subprocess.run([str(BIZHAWK_PATH / 'EmuHawk.exe'), '--open-ext-tool-dll=OotrTestHelper', str(rom_path)], cwd=BIZHAWK_PATH, check=True)
    if arguments['--log']:
        if code.wait() != 0:
            sys.exit(code.returncode)
    rom_path.unlink()
    spoiler_path.unlink()
    if iter_settings['enable_distribution_file']:
        (rom_path.parent / f'{base_name}_Distribution.json').unlink()
if arguments['--seeds'] > 1:
    print(f'{successes} successes, {failures} failures')
    if failures > 0:
        sys.exit(1)
