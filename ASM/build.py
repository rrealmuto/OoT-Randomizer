#!/usr/bin/env python3
import sys
import os
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)), '..'))

import argparse
import json
import re
from subprocess import check_call as call
from rom_diff import create_diff
from ntype import BigStream
from crc import calculate_crc

parser = argparse.ArgumentParser()
parser.add_argument('--pj64sym', help="Output path for Project64 debugging symbols")
parser.add_argument('--compile-c', action='store_true', help="Recompile C modules. This is the default")
parser.add_argument('--no-compile-c', action='store_true', help="Do not recompile C modules")
parser.add_argument('--dump-obj', action='store_true', help="Dumps extra object info for debugging purposes. Does nothing with --no-compile-c")
parser.add_argument('--diff-only', action='store_true', help="Creates diff output without running armips")

args = parser.parse_args()
pj64_sym_path = args.pj64sym
compile_c = not args.no_compile_c
dump_obj = args.dump_obj
diff_only = args.diff_only

root_dir = os.path.dirname(os.path.realpath(__file__))
tools_dir = os.path.join(root_dir, 'tools')
# Makes it possible to use the "tools" directory as the prefix for the toolchain
tools_bin_dir = os.path.join(tools_dir, 'bin')
# Makes it possible to copy the full toolchain prefix into the "tools" directory
n64_bin_dir = os.path.join(tools_dir, "n64", "bin")
os.environ['PATH'] = os.pathsep.join([tools_dir, tools_bin_dir, n64_bin_dir, os.environ['PATH']])

run_dir = root_dir

# Compile code

os.chdir(run_dir)

base_rom_size = os.stat('roms/base.z64').st_size
if base_rom_size != 0x400_0000:
    sys.exit(f'build.py: roms/base.z64 should be 0x4000000 bytes (64 MiB), but yours is 0x{base_rom_size:x} bytes ({base_rom_size / (1024 ** 2)} MiB). Make sure you have an uncompressed base ROM (see ../bin/Decompress).')

if compile_c:
    clist = ['make']
    if dump_obj:
        clist.append('RUN_OBJDUMP=1')
    call(clist)

if not diff_only:
    os.chdir(run_dir + '/src')
    call(['armips', '-sym2', '../build/asm_symbols.txt', 'build.asm'])

os.chdir(run_dir)

with open('build/asm_symbols.txt', 'rb') as f:
    asm_symbols_content = f.read()
asm_symbols_content = asm_symbols_content.replace(b'\r\n', b'\n')
asm_symbols_content = asm_symbols_content.replace(b'\x1A', b'')
with open('build/asm_symbols.txt', 'wb') as f:
    f.write(asm_symbols_content)

# Parse symbols

c_sym_types = {}

with open('build/c_symbols.txt', 'r') as f:
    for line in f:
        m = re.match(r'''
                ^
                [0-9a-fA-F]+
                .*
                \.
                ([^\s]+)
                \s+
                [0-9a-fA-F]+
                \s+
                ([^.$][^\s]+)
                \s+$
            ''', line, re.VERBOSE)
        if m:
            sym_type = m.group(1)
            name = m.group(2)
            c_sym_types[name] = 'code' if sym_type == 'text' else 'data'

symbols = {}

with open('build/asm_symbols.txt', 'r') as f:
    for line in f:
        parts = line.strip().split(' ')
        if len(parts) < 2:
            continue
        address, sym_name = parts
        if address[0] != '8':
            continue
        if sym_name[0] in ['.', '@']:
            continue
        sym_type = c_sym_types.get(sym_name) or ('data' if sym_name.isupper() else 'code')
        symbols[sym_name] = {
            'type': sym_type,
            'address': address,
        }

# Loop through a second time, add lengths to each data symbol
# This could probably be optimized to run in a single pass :)
with open('build/asm_symbols.txt', 'r') as f:
    for line in f:
        parts = line.strip().split(' ')
        if len(parts) < 2:
            continue
        address, sym_name = parts
        if sym_name.startswith('.'):
            # split on the ':' to get the length, in hex
            type, hex_length = sym_name.split(':')
            for symbol, sym_data in symbols.items():
                if sym_data['address'] == address and sym_data['type'] == 'data':
                    sym_data['length'] = int(hex_length, 16)

# Output symbols

os.chdir(run_dir)

PAYLOAD_START = int(symbols['PAYLOAD_START']['address'], 16)
PAYLOAD_END = int(symbols['PAYLOAD_END']['address'], 16)
data_symbols = {}
code_symbols = {}
for (name, sym) in symbols.items():
    if sym['type'] == 'data':
        addr = int(sym['address'], 16)
        if PAYLOAD_START <= addr < PAYLOAD_END:
            addr = addr - 0x80400000 + 0x03480000
        else:
            continue
        data_symbols[name] = {
            'address': f'{addr:08X}',
            'length': sym.get('length', 0),
        }
    elif sym['type'] == 'code':
        addr = int(sym['address'], 16)
        sym_name = name
        if ',' in name:
            sym_name = name.split(',')[0]
        code_symbols[sym_name] = addr

with open('../data/generated/symbols.json', 'w') as f:
    json.dump(data_symbols, f, indent=4, sort_keys=True)
with open('../data/generated/code_symbols.json', 'w') as f:
    json.dump(code_symbols, f, indent=4, sort_keys=True)


if pj64_sym_path:
    pj64_sym_path = os.path.realpath(pj64_sym_path)
    with open(pj64_sym_path, 'w') as f:
        key = lambda pair: pair[1]['address']
        for sym_name, sym in sorted(symbols.items(), key=key):
            f.write('{0},{1},{2}\n'.format(sym['address'], sym['type'], sym_name))


with open('roms/patched.z64', 'r+b') as stream:
    buffer = bytearray(stream.read(0x101000))
    crc = calculate_crc(BigStream(buffer))
    stream.seek(0x10)
    stream.write(bytearray(crc))

# Diff ROMs
create_diff('roms/base.z64', 'roms/patched.z64', '../data/generated/rom_patch.txt')
