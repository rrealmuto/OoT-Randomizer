# Maybe just compile gLinkAdultSkel.c from Fast64 export and then link in the other parts

# Regex: (.*)Limb\s*gLinkAdultSkelLimb_(.*)\s*=\s*\{\s*\{\s*-?[0-9]*\s*,\s*-?[0-9]*\s*,\s*-?[0-9]*\s*\}\s*,\s*-?[0-9]*\s*,\s*-?[0-9]*\s*,\s*(.*)\s*\}\s*;

import re
import argparse
import os
import sys
import subprocess

from ModelHelpers import *

SKEL_DL_PREFIX_ADULT = "gLinkAdultSkelLimb_"
SKEL_DL_PREFIX_CHILD = "gLinkChildSkelLimb_"

reg_adult = re.compile(r"(.*)Limb\s*" + SKEL_DL_PREFIX_ADULT + r"(.*)\s*=(\s*\{\s*\{\s*-?[0-9]*\s*,\s*-?[0-9]*\s*,\s*-?[0-9]*\s*\}\s*,\s*-?[0-9]*\s*,\s*-?[0-9]*\s*),\s*(.*)\s*\}\s*;")
reg_child = re.compile(r"(.*)Limb\s*" + SKEL_DL_PREFIX_CHILD + r"(.*)\s*=(\s*\{\s*\{\s*-?[0-9]*\s*,\s*-?[0-9]*\s*,\s*-?[0-9]*\s*\}\s*,\s*-?[0-9]*\s*,\s*-?[0-9]*\s*),\s*(.*)\s*\}\s*;")

regexs = {
    "child": (reg_child, SKEL_DL_PREFIX_CHILD),
    "adult": (reg_adult, SKEL_DL_PREFIX_ADULT)
}

LUTs = {
    "child": "linkChildLUT.c",
    "adult": "linkAdultLUT.c"
}

#reg_adult = re.compile("(.*)Limb\s*(.*)_([0-9]*)\s*=(\s*\{\s*\{\s*-?[0-9]*\s*,\s*-?[0-9]*\s*,\s*-?[0-9]*\s*\}\s*,\s*-?[0-9]*\s*,\s*-?[0-9]*\s*),\s*(.*)\s*\}\s*;")

# 1 = Limb Type
# 2 = Limb Index
# 3 = Limb variables before the DL
# 4 = DL symbol name

# Read in gLinkAdultSkel.c and convert standard limbs to LodLimbs
def convertLod(skel_c_lines:list[str], age:str) -> tuple[list[str], dict[int,str]]:
    out_lines: list[str] = []
    bone_symbols: dict[int,str] = {}
    
    reg, DL_PREFIX = regexs[age]
    for line in skel_c_lines:
        match = reg.match(line)
        if match:
            limb_type = match.group(1).strip()
            limb_index = match.group(2).strip()
            limb_vars = match.group(3).strip()
            limb_dl = match.group(4).strip()
            # If it's a standard limb, convert to Lod
            if limb_type == "Standard":
                # Warn if any of the limbs are missing DLs
                if "NULL" in limb_dl and limb_index not in ("000", "002", "009"):
                    print(f"Warning: Limb {limb_index} missing DL")
                new_line = f"LodLimb {DL_PREFIX}{limb_index} ={limb_vars}, {limb_dl}, {limb_dl} }};\n"
                out_lines.append(new_line)
            elif limb_type == "Lod":
                out_lines.append(line)
            bone_symbols[int(limb_index)] = limb_dl

        else:
            out_lines.append(line)
    return out_lines, bone_symbols

def addLUT(skel_lines: list[str], age: str):
    # Add the LUT to the second line in the file
    skel_lines.insert(1, f'#include "{LUTs[age]}"')
    return skel_lines



def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--in_file", required=True)
    parser.add_argument("--is_link", action="store_true", help="Use this argument to build link models. Place corresponding LUT .c and .h files in the same directory as the model. linkChildLUT for Child and linkAdultLUT for Adult")
    parser.add_argument("--keep", action="store_true", help="Keep the intermediate files created by the script")
    parser.add_argument("--link_script", help="Define an additional linker script to be used during linking")
    parser.add_argument("--segment", default=0x06, help="Define the segment to be used when compiling this object. Default to 0x06 for most objects. Set to 0x05 for field/dangeon keep")
    parser.add_argument("--rom", default="baseroms/ntsc-1.0/baserom-decompressed.z64", help="Specify NTSC 1.0 decompressed ROM path. Defaults to baseroms/ntsc-1.0/baserom-decompressed.z64")
    parser.add_argument("--build_prefix", default="mips64-ultra-elf", help="Prefix to gcc/ld/objcopy tools. Don't include the final '-' ex. mips-linux-gnu")
    args = parser.parse_args()

    in_file:str = args.in_file
    is_link: bool = args.is_link
    keep: bool = args.keep
    link_script: str = args.link_script
    segment: int = int(args.segment)
    rom_path: str = args.rom
    build_prefix: str = args.build_prefix + "-"

    print(f"in_file: {in_file}")
    print(f"is_link: {is_link}")
    print(f"keep: {keep}")
    print(f"link_script: {link_script}")

    if is_link:
        age = None

        basename = os.path.basename(in_file)

        if "Child" in basename:
            age = "child"
        elif "Adult" in basename:
            age = "adult"

        if not age:
            raise Exception("Could not determine age from input file")

        f = open(in_file, 'r')
        skel_lines = f.readlines()
        f.close()

        skel_lines, bones = convertLod(skel_lines, age)
        skel_lines = addLUT(skel_lines, age)

        out_file, _ = os.path.splitext(in_file)
        out_file_c = out_file + "_ootr.c"
        out_file_o = out_file + "_ootr.o"
        out_file_elf = out_file + "_ootr.elf"
        out_file_zobj = out_file + ".zobj"
        out_file_sym = out_file + ".sym"

        with open(out_file_c, 'w') as out_file:
            out_file.writelines(skel_lines)
    else:
        out_file, _ = os.path.splitext(in_file)
        out_file_c = in_file
        out_file_o = out_file + "_ootr.o"
        out_file_elf = out_file + "_ootr.elf"
        out_file_zobj = out_file + ".zobj"
        out_file_sym = out_file + ".sym"

    ret = subprocess.run([f"{build_prefix}gcc", out_file_c, "-c", "-O0", "-G0", "-fno-reorder-blocks", "-fno-zero-initialized-in-bss", "-fno-toplevel-reorder", "-march=vr4300", "-mtune=vr4300", "-mabi=32", "-mno-gpopt",
                    "-mdivide-breaks", "-mexplicit-relocs", "-I.", "-Iinclude", "-Iinclude/libc", "-Iextracted/ntsc-1.0", "-Ibuild/ntsc-1.0", "-Isrc", "-DF3DEX_GBI_2", f"-o{out_file_o}"])

    if ret.returncode != 0:
        raise Exception(f"Compilation error {ret}")

    segment_addr = segment * 0x01000000

    link_command = [f"{build_prefix}ld", out_file_o, f"--section-start=.data={hex(segment_addr)}", f"-o{out_file_elf}"]
    if link_script:
        link_command.extend(["-T", link_script])
    ret = subprocess.run(link_command)
    if ret.returncode != 0:
        raise Exception(f"Link error {ret}")
    ret = subprocess.run([f"{build_prefix}objcopy", "-O", "binary", "--only-section=.data", out_file_elf, out_file_zobj])
    if ret.returncode != 0:
        raise Exception(f"Objcopy error {ret}")
    ret = subprocess.run([f"{build_prefix}objdump", out_file_elf, "--syms"])

    # If we're building a link model, load all of the vanilla parts from the base ROM
    if is_link:
        # Open the compiled .zobj
        zobj = None
        with open(out_file_zobj, 'rb') as f:
            zobj = f.read()
            zobj = bytearray(zobj)
        
        # Open vanilla ROM
        rom = None
        with open(rom_path, 'rb') as f:
            rom = f.read()

        LoadVanillaPiecesToZOBJ(zobj, rom, 1 if age == "child" else 0)
        with open(out_file_zobj, 'wb') as f:
            f.write(zobj)

    if not keep:
        os.remove(out_file_o)
        os.remove(out_file_elf)
        if is_link:
            os.remove(out_file_c)

if __name__ == "__main__":
    main()