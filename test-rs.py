"""Test the randomizer using plando.json.

Usage:
  test.py [options]
  test.py (-h | --help)

Options:
  -E, --no-emu                  don't automatically launch BizHawk
  -c, --cosmetics               include cosmetics from cosmetics-plando.json
  -e, --everdrive               test world 1 on EverDrive
  -h, --help                    show this help message and exit
  -l, --log                     open the spoiler log in VS Code
  -n, --worlds=<count>          generate a multiworld seed for this many players [default: 1] [type: int]
  -o, --output=<path>           save randomizer output to the given file
  -p, --preset=<preset>         use this settings preset instead of or in addition to plando.json
  -v, --verbose                 always show randomizer output
  --mhmw-stage                  test using the gitdir stage of Mido's House Multiworld
  --no-rebuild                  don't recompile ASM/C/Rust even if it was changed
  --no-wait                     don't wait for input before deleting the seed
  --patch                       generate a .zpf file
  --player=<id>                 only launch the given world [type: int]
  --release                     compile Rust code with optimizations
  --seed=<seed>                 generate a set seed
  --seeds=<count>               generate the given number of seeds and count successes and failures. Implies --no-emu [default: 1] [type: int]
  --settings=<settings_string>  use this settings string instead of or in addition to plando.json
"""

import sys

import json
import os
import pathlib
import shutil
import subprocess

import type_docopt # PyPI: type-docopt

BIZHAWK_PATH = pathlib.Path.home() / 'bin' / 'BizHawk'
EVERDRIVE_PATH = pathlib.Path.home() / 'bin' / 'usb64-v1.0.0.3'

arguments = type_docopt.docopt(__doc__)

settings = {
    'rom': 'C:\\Users\\fenhl\\games\\zelda\\oot\\oot-ntscu-1.0.z64',
    'pal_rom': 'C:\\Users\\fenhl\\games\\zelda\\oot\\oot-pal-1.0.z64',
    'output_dir': 'C:\\Users\\fenhl\\games\\zelda\\oot',
    'enable_distribution_file': True,
    'distribution_file': 'C:\\Users\\fenhl\\git\\github.com\\fenhl\\OoT-Randomizer\\stage\\plando.json',
    'enable_cosmetic_file': arguments['--cosmetics'],
    'cosmetic_file': 'C:\\Users\\fenhl\\git\\github.com\\fenhl\\OoT-Randomizer\\stage\\cosmetics-plando.json',
    'create_spoiler': True,
    'create_cosmetics_log': False,
    'create_patch_file': arguments['--patch'],
    'create_compressed_rom': not arguments['--patch'],
    'world_count': arguments['--worlds'],
}

seed_condition = lambda spoiler: True

if not arguments['--no-rebuild']:
    subprocess.run([sys.executable, 'crate/rust-n64-test/assets/build.py'], check=True)
    subprocess.run(['cargo', 'build', '--lib', *(['--release'] if arguments['--release'] else [])], check=True)
    shutil.copy(f'target/{"release" if arguments["--release"] else "debug"}/rs.dll', 'rs.pyd')
    if subprocess.run(['git', 'diff', '--quiet', 'HEAD', '--', 'ASM']).returncode != 0: # any staged or unstaged changes to the ASM directory?
        subprocess.run(['wsl', 'ASM/build.py', '--compile-c'], check=True)

successes = 0
failures = 0
for _ in range(arguments['--seeds']):
    failed = False
    while True:
        seed = arguments['--seed']
        rom_paths = []
        for world_id in range(1, arguments['--worlds'] + 1):
            iter_settings = {**settings, 'player_num': world_id}
            #rando_args = ['target/debug/ootr', '--settings=-']
            rando_args = [sys.executable]
            rando_args += ['.\\OoTRandomizer.py', '--settings=-']
            with open(settings['distribution_file'], encoding='utf-8') as f:
                if json.load(f) == {}:
                    # don't show “Plandomizer” in file select since nothing has actually been plando'd
                    del iter_settings['distribution_file']
                    iter_settings['enable_distribution_file'] = False
            if arguments['--preset']:
                rando_args.append(f'--settings_preset={arguments["--preset"]}')
            if arguments['--settings']:
                rando_args.append(f'--settings_string={arguments["--settings"]}')
            if world_id == 1 and iter_settings['enable_distribution_file'] and (arguments['--preset'] or arguments['--settings']):
                input('plando.json will override settings preset/string, press return to confirm')
            if seed:
                rando_args.append(f'--seed={seed}')
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
                    elif line.startswith('Creating Patch File: '):
                        rom_path = pathlib.Path(settings['output_dir'], line[len('Creating Patch File: '):])
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
            if base_name.endswith(f'W{arguments["--worlds"]}P{world_id}'):
                base_name = base_name[:-2]
            if seed is None:
                spoiler_path = rom_path.parent / f'{base_name}_Spoiler.json'
                with spoiler_path.open() as spoiler_f:
                    spoiler = json.load(spoiler_f)
                seed = spoiler[':seed']
            rom_paths.append(rom_path)
        if failed:
            break
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
        if os.environ['TERM_PROGRAM'] == 'vscode':
            vscode = 'code.cmd'
        else:
            vscode = 'C:/Program Files/Microsoft VS Code/bin/code.cmd'
        code = subprocess.Popen([vscode, '--wait', str(spoiler_path)])
    if arguments['--no-emu'] or arguments['--patch'] or arguments['--seeds'] > 1:
        if arguments['--seeds'] == 1 and not arguments['--log']:
            for rom_path in rom_paths:
                print(rom_path)
            if not arguments['--no-wait']:
                input('press return to delete the seed')
    else:
        if arguments['--everdrive']:
            #if (returncode := subprocess.run([str(EVERDRIVE_PATH / 'usb64.exe'), '-cp', str(rom_paths[0]), 'sd:test.z64']).returncode) != 0:
            #    sys.exit(returncode)
            if (returncode := subprocess.run(['C:/Users/fenhl/git/github.com/JasonCampbell256/ED64/main/usb64/usb64/bin/Debug/net5.0/usb64.exe', str(rom_paths[arguments.get('--player', 1) - 1])]).returncode) != 0:
                sys.exit(returncode)
        if arguments['--worlds'] > 1:
            if arguments['--mhmw-stage']:
                BIZHAWK_PATH = pathlib.Path.home() / 'git' / 'github.com' / 'midoshouse' / 'ootr-multiworld' / 'stage' / 'crate' / 'multiworld-bizhawk' / 'OotrMultiworld' / 'BizHawk'
            emu_processes = [
                subprocess.Popen([str(BIZHAWK_PATH / 'EmuHawk.exe'), '--open-ext-tool-dll=OotrMultiworld', str(rom_path)], cwd=BIZHAWK_PATH)
                for i, rom_path in enumerate(rom_paths)
                if (i > 0 or not arguments['--everdrive'])
                and ('--player' not in arguments or i == arguments['--player'] - 1)
            ]
            for emu in emu_processes:
                if emu.wait() != 0:
                    sys.exit(emu.returncode)
        elif not arguments['--everdrive']:
            if (returncode := subprocess.run([str(BIZHAWK_PATH / 'EmuHawk.exe'), '--open-ext-tool-dll=OotrTestHelper', str(rom_paths[0])], cwd=BIZHAWK_PATH).returncode) != 0:
                sys.exit(returncode)
    if arguments['--log']:
        if code.wait() != 0:
            sys.exit(code.returncode)
    for rom_path in rom_paths:
        rom_path.unlink()
    spoiler_path.unlink()
    if iter_settings['enable_distribution_file']:
        (rom_path.parent / f'{base_name}_Distribution.json').unlink()
    #TODO clear logs
if arguments['--seeds'] > 1:
    print(f'{successes} successes, {failures} failures')
    if failures > 0:
        sys.exit(1)
