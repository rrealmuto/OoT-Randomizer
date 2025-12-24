from texture_util import *
from Rom import *
import zlib

rom = Rom("ZOOTDEC.z64")

# Read the original title texture from the ROM
title_address = 0x01795300
title_size = 160*160*4 # rgba32 (4 bytes per pixel) 160x160

original_title_bytes = rom.original.buffer[title_address:title_address+title_size]

# read the new texture png
new_title_bytes = rgba32_from_png(rom, 0,0,0, "gTitleZeldaShieldLogoTex.rgba32.png")

patch_bytes = bytes([a ^ b for (a, b) in zip(original_title_bytes, new_title_bytes)])

# Save the patch
with open("title_patch.bin", 'wb') as f:
    compressed = zlib.compress(patch_bytes)
    f.write(compressed)