from distutils.command.build import build
from Rom import Rom
from Utils import *

def ci4_to_rgba16(rom:Rom, address, length, palette):
    newPixels = []
    texture = rom.read_bytes(address, int(length/2))
    for byte in texture:
        newPixels.append(palette[(byte & 0xF0) >> 4])
        newPixels.append(palette[byte & 0x0F])
    return newPixels

def rgba16_to_ci8(rgba16_texture):
    ci8_texture = []
    palette = get_colors_from_rgba16(rgba16_texture)
    if len(palette) > 0x100:
        raise(Exception("RGB Texture exceeds maximum of 256 colors"))
    print(len(palette))
    if len(palette) < 0x100: #Pad the palette with 0x0001
        for i in range(0, 0x100 - len(palette)):
            palette.append(0x0001)
    print(len(palette))
    for pixel in rgba16_texture:
        if pixel in palette:
            ci8_texture.append(palette.index(pixel))
    return (ci8_texture, palette)

def load_palette(rom:Rom, address, length):
    palette = []
    for i in range(0, length):
        palette.append(rom.read_int16(address + 2*i))
    return palette

def get_colors_from_rgba16(rgba16_texture):
    colors = []
    for pixel in rgba16_texture:
        if pixel not in colors:
            colors.append(pixel)
    return colors

def apply_rgba16_patch(rgba16_texture, rgba16_patch):
    if rgba16_patch is not None and (len(rgba16_texture) != len(rgba16_patch)):
        raise(Exception("OG Texture and Patch not the same length!"))
    
    new_texture = []
    if not rgba16_patch:
        for i in range(0, len(rgba16_texture)):
            new_texture.append(rgba16_texture[i])
        return new_texture
    for i in range(0, len(rgba16_texture)):
        new_texture.append(rgba16_texture[i] ^ rgba16_patch[i])
    return new_texture

def save_rgba16_texture(rgba16_texture, fileStr):
    file = open(fileStr, 'wb')
    bytes = bytearray()
    for pixel in rgba16_texture:
        bytes.extend(pixel.to_bytes(2, 'big'))
    file.write(bytes)
    file.close()

def save_ci8_texture(ci8_texture, fileStr):
    file = open(fileStr, 'wb')
    bytes = bytearray()
    for pixel in ci8_texture:
        bytes.extend(pixel.to_bytes(1, 'big'))
    file.write(bytes)
    file.close()

def load_rgba16_texture(fileStr, size):
    texture = []
    file = open(fileStr, 'rb')
    for i in range(0, size):
        texture.append(int.from_bytes(file.read(2), 'big'))

    file.close()
    return(texture)

def ci4_texture_apply_rgba16patch_and_convert_to_ci8(rom, base_texture_address, base_palette_address, size, patchfile):
    palette = load_palette(rom, base_palette_address, 16) #load the original palette from rom
    base_texture_rgba16 = ci4_to_rgba16(rom, base_texture_address, size, palette) #load the original texture from rom and convert to ci8
    patch_rgba16 = None
    if patchfile:
        patch_rgba16 = load_rgba16_texture(patchfile, size)
    new_texture_rgba16 = apply_rgba16_patch(base_texture_rgba16, patch_rgba16)
    ci8_texture, ci8_palette = rgba16_to_ci8(new_texture_rgba16)
    #merge the palette and the texture
    bytes = bytearray()
    for pixel in ci8_palette:
        bytes.extend(int.to_bytes(pixel, 2, 'big'))
    for pixel in ci8_texture:
        bytes.extend(int.to_bytes(pixel, 1, 'big'))
    return bytes

def build_crate_ci8_patches():


    #load crate textures from rom
    object_kibako2_addr = 0x018B6000
    SIZE_CI4_32X128 = 4096
    rom = Rom("ZOOTDEC.z64")
    crate_palette = load_palette(rom, object_kibako2_addr + 0x00, 16)
    crate_texture_rgba16 = ci4_to_rgba16(rom, object_kibako2_addr + 0x20,SIZE_CI4_32X128, crate_palette)

    #load new textures
    crate_texture_gold_rgba16 = load_rgba16_texture('crate_gold_rgba16.bin', 0x1000)
    crate_texture_skull_rgba16 = load_rgba16_texture('crate_skull_rgba16.bin', 0x1000)
    crate_texture_key_rgba16 = load_rgba16_texture('crate_key_rgba16.bin', 0x1000)
    crate_texture_bosskey_rgba16 = load_rgba16_texture('crate_bosskey_rgba16.bin', 0x1000)

    #create patches
    gold_patch = apply_rgba16_patch(crate_texture_rgba16, crate_texture_gold_rgba16)
    key_patch = apply_rgba16_patch(crate_texture_rgba16, crate_texture_key_rgba16)
    skull_patch = apply_rgba16_patch(crate_texture_rgba16, crate_texture_skull_rgba16)
    bosskey_patch = apply_rgba16_patch(crate_texture_rgba16, crate_texture_bosskey_rgba16)

    #save patches
    save_rgba16_texture(gold_patch, 'crate_gold_rgba16_patch.bin')
    save_rgba16_texture(key_patch, 'crate_key_rgba16_patch.bin')
    save_rgba16_texture(skull_patch, 'crate_skull_rgba16_patch.bin')
    save_rgba16_texture(bosskey_patch, 'crate_bosskey_rgba16_patch.bin')

    #create ci8s
    default_ci8, default_palette = rgba16_to_ci8(crate_texture_rgba16)
    gold_ci8, gold_palette = rgba16_to_ci8(crate_texture_gold_rgba16)
    key_ci8, key_palette = rgba16_to_ci8(crate_texture_key_rgba16)
    skull_ci8, skull_palette = rgba16_to_ci8(crate_texture_skull_rgba16)
    bosskey_ci8, bosskey_palette = rgba16_to_ci8(crate_texture_bosskey_rgba16)

    #save ci8 textures
    save_ci8_texture(default_ci8, 'crate_default_ci8.bin')
    save_ci8_texture(gold_ci8, 'crate_gold_ci8.bin')
    save_ci8_texture(key_ci8, 'crate_key_ci8.bin')
    save_ci8_texture(skull_ci8, 'crate_skull_ci8.bin')
    save_ci8_texture(bosskey_ci8, 'crate_bosskey_ci8.bin')

    #save palettes
    save_rgba16_texture(default_palette, 'crate_default_palette.bin')
    save_rgba16_texture(gold_palette, 'crate_gold_palette.bin')
    save_rgba16_texture(key_palette, 'crate_key_palette.bin')
    save_rgba16_texture(skull_palette, 'crate_skull_palette.bin')
    save_rgba16_texture(bosskey_palette, 'crate_bosskey_palette.bin')

    patchfiles = [None, 'crate_gold_rgba16_patch.bin', 'crate_key_rgba16_patch.bin', 'crate_skull_rgba16_patch.bin', 'crate_bosskey_rgba16_patch.bin']

    crate_textures = [
        (5, 'texture_crate_default', 0x18B6000 + 0x20, 0x018B6000, 4096, ci4_texture_apply_rgba16patch_and_convert_to_ci8, None),
        (6, 'texture_crate_gold'   , 0x18B6000 + 0x20, 0x018B6000, 4096, ci4_texture_apply_rgba16patch_and_convert_to_ci8, 'crate_gold_rgba16_patch.bin'),
        (7, 'texture_crate_key', 0x18B6000 + 0x20, 0x018B6000, 4096, ci4_texture_apply_rgba16patch_and_convert_to_ci8, 'crate_key_rgba16_patch.bin'),
        (8, 'texture_crate_skull',  0x18B6000 + 0x20, 0x018B6000, 4096, ci4_texture_apply_rgba16patch_and_convert_to_ci8, 'crate_skull_rgba16_patch.bin'),
        (9, 'texture_crate_bosskey', 0x18B6000 + 0x20, 0x018B6000, 4096, ci4_texture_apply_rgba16patch_and_convert_to_ci8, 'crate_bosskey_rgba16_patch.bin')
        ]

    for texture_id, texture_name, rom_address_base, rom_address_palette, size,func, patchfile in crate_textures:
        texture = func(rom, rom_address_base, rom_address_palette, size, patchfile)
        file = open(texture_name, 'wb')
        file.write(texture)
        file.close()
        print(texture)

build_crate_ci8_patches()