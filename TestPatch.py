from Rom import Rom
from N64Patch import *

class FakeSettings:
    def __init__(self, file: str):
        self.patch_file = file
        self.repatch_cosmetics = False

rom = Rom("ZOOTDEC.z64")

settings = FakeSettings("input.zpf")

apply_patch_file(rom, settings)

settings = FakeSettings("OoTR_Cosmetic.zpf")

rom.original = rom.copy()
apply_patch_file(rom, settings)

rom.write_to_file("test_uncompressed.z64")