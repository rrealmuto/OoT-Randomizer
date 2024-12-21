from __future__ import annotations
import copy
import json
import os
import platform
import subprocess
from collections.abc import Iterator, Sequence
from typing import Optional

from Models import restrictiveBytes
from Utils import is_bundled, subprocess_args, local_path, data_path, get_version_bytes
from crc import calculate_crc
from ntype import BigStream
from version import base_version, branch_identifier, supplementary_version

DMADATA_START: int = 0x7430  # NTSC 1.0/1.1: 0x7430, NTSC 1.2: 0x7960, Debug: 0x012F70
OVERLAY_TABLE_START: int = 0xB5E490 # NTSC 1.0
OVERLAY_TABLE_OFFSET: int = 0
OVERLAY_TABLE_ENTRY_SIZE: int = 0x20
PAUSE_PLAYER_OVERLAY_TABLE_START: int = 0xB743E0 
PAUSE_PLAYER_OVERLAY_TABLE_ENTRY_SIZE: int = 0x1C
PAUSE_PLAYER_OVERLAY_TABLE_OFFSET: int = 4

NUM_OVERLAY_ENTRIES: int = 0x1D7
NUM_PAUSE_PLAYER_OVERLAY_ENTRIES: int = 2

class Rom(BigStream):
    def __init__(self, file: Optional[str] = None, verify_crc: bool = True) -> None:
        super().__init__(bytearray())

        self.original: Rom = self
        self.changed_address: dict[int, int] = {}
        self.changed_dma: dict[int, tuple[int, int, int]] = {}
        self.force_patch: list[int] = []
        self.dma: DMAIterator = DMAIterator(self, DMADATA_START)

        with open(data_path('generated/symbols.json'), 'r') as stream:
            symbols = json.load(stream)
            self.symbols: dict[str, int] = {name: {'address': int(sym['address'], 16), 'length': sym['length']} for name, sym in symbols.items()}

        with open(data_path('generated/patch_symbols.json'), 'r') as stream:
            self.patch_symbols = json.load(stream)

        if file is None:
            return

        decompressed_file: str = local_path('ZOOTDEC.z64')

        os.chdir(local_path())
        if file:
            self.read_rom(file, decompressed_file, verify_crc=verify_crc)
        elif os.path.isfile(decompressed_file):
            # Try to read from previously decompressed rom if one exists.
            try:
                self.read_rom(decompressed_file)
            except (FileNotFoundError, RuntimeError):
                # Decompress the provided file.
                if not file:
                    raise FileNotFoundError('Must specify path to base ROM')
                self.read_rom(file, decompressed_file)
        else:
            raise FileNotFoundError('Must specify path to base ROM')

        # Add file to maximum size
        self.buffer.extend(bytearray([0x00] * (0x4000000 - len(self.buffer))))
        self.original = self.copy()
        self.overlay_table = OverlayTable.read_overlay_table(self, OVERLAY_TABLE_START, OVERLAY_TABLE_OFFSET, OVERLAY_TABLE_ENTRY_SIZE, NUM_OVERLAY_ENTRIES) + OverlayTable.read_overlay_table(self, PAUSE_PLAYER_OVERLAY_TABLE_START, PAUSE_PLAYER_OVERLAY_TABLE_OFFSET, PAUSE_PLAYER_OVERLAY_TABLE_ENTRY_SIZE, NUM_PAUSE_PLAYER_OVERLAY_ENTRIES)
        # Add version number to header.
        self.write_version_bytes()

    def copy(self) -> Rom:
        new_rom: Rom = Rom()
        new_rom.buffer = copy.copy(self.buffer)
        new_rom.changed_address = copy.copy(self.changed_address)
        new_rom.changed_dma = copy.copy(self.changed_dma)
        new_rom.force_patch = copy.copy(self.force_patch)
        return new_rom

    def read_rom(self, input_file: str, output_file: Optional[str] = None, verify_crc: bool = True) -> None:
        try:
            with open(input_file, 'rb') as stream:
                self.buffer = bytearray(stream.read())
        except FileNotFoundError as ex:
            raise FileNotFoundError(f'Invalid path to Base ROM: "{input_file}"')

        # Validate ROM file
        if not verify_crc:
            return

        valid_crc = [
            [0xEC, 0x70, 0x11, 0xB7, 0x76, 0x16, 0xD7, 0x2B], # Compressed
            [0x70, 0xEC, 0xB7, 0x11, 0x16, 0x76, 0x2B, 0xD7], # Byteswap compressed
            [0x93, 0x52, 0x2E, 0x7B, 0xE5, 0x06, 0xD4, 0x27], # Decompressed
        ]

        file_name = os.path.splitext(input_file)
        rom_crc = list(self.buffer[0x10:0x18])
        if rom_crc not in valid_crc:
            # Bad CRC validation
            raise RuntimeError(f'ROM file {input_file} is not a valid OoT 1.0 US ROM.')
        elif len(self.buffer) < 0x2000000 or len(self.buffer) > 0x4000000 or file_name[1].lower() not in ('.z64', '.n64'):
            # ROM is too big, or too small, or a bad type
            raise RuntimeError(f'ROM file {input_file} is not a valid OoT 1.0 US ROM.')
        elif len(self.buffer) == 0x2000000:
            # If Input ROM is compressed, then Decompress it
            if output_file:
                self.decompress_rom(input_file, output_file, verify_crc)
            else:
                raise RuntimeError('ROM was unable to be decompressed. Please supply an already decompressed ROM.')
        else:
            # ROM file is a valid and already uncompressed
            pass

    def decompress_rom(self, input_file: str, output_file: str, verify_crc: bool = True) -> None:
        sub_dir = "./" if is_bundled() else "bin/Decompress/"

        if platform.system() == 'Windows':
            if platform.machine() == 'AMD64':
                subcall = [sub_dir + "Decompress.exe", input_file, output_file]
            elif platform.machine() == 'ARM64':
                subcall = [sub_dir + "Decompress_ARM64.exe", input_file, output_file]
            else:
                subcall = [sub_dir + "Decompress32.exe", input_file, output_file]
        elif platform.system() == 'Linux':
            if platform.machine() in ('arm64', 'aarch64', 'aarch64_be', 'armv8b', 'armv8l'):
                subcall = [sub_dir + "Decompress_ARM64", input_file, output_file]
            elif platform.machine() in ('arm', 'armv7l', 'armhf'):
                subcall = [sub_dir + "Decompress_ARM32", input_file, output_file]
            else:
                subcall = [sub_dir + "Decompress", input_file, output_file]
        elif platform.system() == 'Darwin':
            if platform.machine() == 'arm64':
                subcall = [sub_dir + "Decompress_ARM64.out", input_file, output_file]
            else:
                subcall = [sub_dir + "Decompress.out", input_file, output_file]
        else:
            raise RuntimeError('Unsupported operating system for decompression. Please supply an already decompressed ROM.')

        subprocess.call(subcall, **subprocess_args())
        self.read_rom(output_file, verify_crc=verify_crc)

    def write_byte(self, address: int, value: int) -> None:
        super().write_byte(address, value)
        self.changed_address[self.last_address-1] = value

    def write_bytes_restrictive(self, start: int, size: int, values: Sequence[int]) -> None:
        for i in range(size):
            address = start + i
            should_write = True
            for restrictiveBlock in restrictiveBytes:
                # If i is between the start of restrictive zone [0] and start + size [1]
                if restrictiveBlock[0] <= address < restrictiveBlock[0] + restrictiveBlock[1]:
                    should_write = False
                    break
            if should_write:
                self.write_byte(address, values[i])

    def write_bytes(self, address: int, values: Sequence[int]) -> None:
        super().write_bytes(address, values)
        self.changed_address.update(zip(range(address, address + len(values)), values))

    def revert_patch(self, patch_name: str) -> None:
        # Get the _START and _END symbols
        patch_start = OverlayTable.VRAM_2_VROM(self.overlay_table, self.patch_symbols[patch_name + "_START"])
        patch_end = OverlayTable.VRAM_2_VROM(self.overlay_table, self.patch_symbols[patch_name + "_END"])
        orig_bytes = self.original.read_bytes(patch_start, patch_end - patch_start)
        self.write_bytes(patch_start, orig_bytes)

    def restore(self) -> None:
        self.buffer = copy.copy(self.original.buffer)
        self.changed_address = {}
        self.changed_dma = {}
        self.force_patch = []
        self.last_address = 0
        self.write_version_bytes()

    def sym(self, symbol_name: str) -> int:
        return self.symbols[symbol_name]['address']

    def sym_length(self, symbol_name: str) -> int:
        return self.symbols[symbol_name]['length']

    def write_to_file(self, file: str) -> None:
        self.verify_dmadata()
        self.update_header()
        with open(file, 'wb') as outfile:
            outfile.write(self.buffer)

    def update_header(self) -> None:
        crc = calculate_crc(self)
        self.write_bytes(0x10, crc)

    def write_version_bytes(self) -> None:
        version_bytes = get_version_bytes(base_version, branch_identifier, supplementary_version)
        self.write_bytes(0x19, version_bytes[:5])
        self.write_bytes(0x35, version_bytes[:3])
        self.force_patch.extend([0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x35, 0x36, 0x37])

    def read_version_bytes(self) -> bytearray:
        version_bytes = self.read_bytes(0x19, 5)
        secondary_version_bytes = self.read_bytes(0x35, 3)
        for i in range(3):
            if secondary_version_bytes[i] != version_bytes[i]:
                return secondary_version_bytes
        return version_bytes
    
    def dump_file_system(self, output_directory: str) -> None:
        i = 0
        if not os.path.exists(output_directory):
            os.mkdir(output_directory)
        for dma_entry in self.dma:
            filename = dma_table_names[i] if i in dma_table_names.keys() else str(i)
            with open(os.path.join(output_directory, filename), 'wb') as f:
                f.write(dma_entry.file_bytes())
            i += 1

    # dmadata/file management helper functions

    def verify_dmadata(self) -> None:
        overlapping_records = []
        dma_data = []

        for dma_entry in self.dma:
            this_start, this_end, this_size = dma_entry.as_tuple()

            if this_start == 0 and this_end == 0:
                break

            dma_data.append((this_start, this_end, this_size))

        dma_data.sort()

        for i in range(0, len(dma_data) - 1):
            this_start, this_end, this_size = dma_data[i]
            next_start, next_end, next_size = dma_data[i + 1]

            if this_end > next_start:
                overlapping_records.append(
                    f'0x{this_start:08X} - 0x{this_end:08X} (Size: 0x{this_size:04X})\n0x{next_start:08X} - 0x{next_end:08X} (Size: 0x{next_size:04X})'
                )

        if len(overlapping_records) > 0:
            raise Exception("Overlapping DMA Data Records!\n%s" %
                            '\n-------------------------------------\n'.join(overlapping_records))

    # update dmadata record with start vrom address "key"
    # if key is not found, then attempt to add a new dmadata entry
    def update_dmadata_record_by_key(self, key: Optional[int], start: int, end: int, from_file: Optional[int] = None) -> None:
        dma_entry = self.dma.get_dmadata_record_by_key(key)
        if dma_entry is None:
            raise Exception(f"dmadata update failed: key {key:{'x' if key else ''}} not found in dmadata and dma table is full.")

        if from_file is None:
            from_file = -1 if key is None else key
        dma_entry.update(start, end, from_file)

    # This will scan for any changes that have been made to the DMA table
    # By default, this assumes any changes here are new files, so this should only be called
    # after patching in the new files, but before vanilla files are repointed
    def scan_dmadata_update(self, preserve_from_file: bool = False, assume_move: bool = False) -> None:
        for dma_entry in self.dma:
            dma_start, dma_end, dma_size = dma_entry.as_tuple()
            old_dma_start, old_dma_end, old_dma_size = self.original.dma[dma_entry.index].as_tuple()
            if (dma_start == 0 and dma_end == 0) and (old_dma_start == 0 and old_dma_end == 0):
                break

            # If the entries do not match, the flag the changed entry
            if not (dma_start == old_dma_start and dma_end == old_dma_end):
                from_file = -1
                if preserve_from_file and dma_entry.index in self.changed_dma:
                    from_file = self.changed_dma[dma_entry.index][0]
                elif assume_move and dma_entry.index < 1496:
                    from_file = old_dma_start
                self.changed_dma[dma_entry.index] = (from_file, dma_start, dma_end - dma_start)

    # This will rescan the entire ROM, compare to original ROM, and repopulate changed_address.
    def rescan_changed_bytes(self) -> None:
        self.changed_address = {}
        size = len(self.buffer)
        original_size = len(self.original.buffer)
        for i, byte in enumerate(self.buffer):
            if i >= original_size:
                self.changed_address[i] = byte
                continue
            orig_byte = self.original.read_byte(i)
            if byte != orig_byte:
                self.changed_address[i] = byte
        if size < original_size:
            self.changed_address.update(zip(range(size, original_size-1), [0]*(original_size-size)))

class DMAEntry:
    def __init__(self, rom: Rom, index: int) -> None:
        self.rom = rom
        self.index = index
        if self.index < 0 or self.index > self.rom.dma.dma_entries:
            raise ValueError(f"DMAEntry: Index out of range: {self.index}")

    @property
    def start(self) -> int:
        return self.rom.read_int32(self.rom.dma.dma_start + (self.index * 0x10))

    @property
    def end(self) -> int:
        return self.rom.read_int32(self.rom.dma.dma_start + (self.index * 0x10) + 0x04)

    @property
    def size(self) -> int:
        return self.end - self.start

    def as_tuple(self) -> tuple[int, int, int]:
        start, end = self.start, self.end
        return start, end, end - start

    def file_bytes(self) -> bytearray:
        start, end, size = self.as_tuple()
        return self.rom.read_bytes(start, size)

    def update(self, start: int, end: int, from_file: Optional[int] = None):
        if from_file is None:
            if self.index in self.rom.changed_dma:
                from_file = self.rom.changed_dma[self.index][0]
            elif self.start and self.end:
                from_file = self.start
            else:
                from_file = -1
        self.rom.write_int32s(self.rom.dma.dma_start + (self.index * 0x10), [start, end, start, 0])
        self.rom.changed_dma[self.index] = (from_file, start, end - start)


class DMAIterator:
    def __init__(self, rom: Rom, dma_start: int) -> None:
        self.rom: Rom = rom
        self.dma_start: int = dma_start
        self.dma_index: int = 0
        self.dma_end: int = 0
        self._dma_entries: int = 0

    @property
    def dma_entries(self) -> int:
        if not self._dma_entries:
            self._calculate_dma_entries()
        return self._dma_entries

    def _calculate_dma_entries(self) -> None:
        i = start = -1
        while start != self.dma_start:
            i += 1
            if i > 2000:
                dma_bytes = self.rom.read_bytes(self.rom.dma.dma_start, 160).hex(' ', 4)
                raise Exception(f"DMA entry for DMA table not found. Attempted to find DMA entry starting at {self.dma_start}. First 160 bytes of DMA table: {dma_bytes}")
            start = self.rom.read_int32(self.rom.dma.dma_start + (i * 0x10))
        self.dma_index = i
        self.dma_end = self.rom.read_int32(self.dma_start + (self.dma_index * 0x10) + 0x04)
        self._dma_entries = (self.dma_end - self.dma_start) >> 4

    def __getitem__(self, item: int) -> DMAEntry:
        if not isinstance(item, int):
            raise ValueError("DMAIterator only supports integer keys.")
        if item < 0:
            item = self.dma_entries + item
        if item > self.dma_entries:
            raise ValueError(f"Attempted to get DMA entry exceeding the table size: {item}")

        return DMAEntry(self.rom, item)

    def __iter__(self) -> Iterator[DMAEntry]:
        for item in range(0, self.dma_entries):
            yield self[item]

    # Gets a dmadata entry by the file start position.
    def get_dmadata_record_by_key(self, key: Optional[int]) -> DMAEntry:
        i = 0
        for dma_entry in self:
            if key is None and dma_entry.end == 0 and dma_entry.start == 0:
                return dma_entry
            elif dma_entry.start == key:
                return dma_entry
            i += 1
        raise Exception(f"`get_dmadata_record_by_key`: DMA Start '{key}' not found in the DMA Table.")

    def get_dmadata_record_by_index(self, index: int) -> DMAEntry:
        return DMAEntry()

    # Gets the last used byte of rom defined in the DMA table
    def end_of_data(self) -> int:
        max_end = 0
        for dma_entry in self:
            max_end = max(max_end, dma_entry.end)

        max_end = ((max_end + 0x0F) >> 4) << 4
        return max_end

    # Finds the smallest suitable place between current files. If size is None, find the largest span of free space.
    def free_space(self, size: Optional[int] = None) -> int:
        free_space = []  # List of tuples containing size of free space and start of free space.

        # Get DMA entries in tuple form and then sort them.
        files = sorted([dma_entry.as_tuple() for dma_entry in self])

        # Find free space between files.
        for i in range(len(files)):
            end_current = ((files[i][1] + 0x0F) >> 4) << 4
            start_next = ((files[i+1][0] + 0x0F) >> 4) << 4 if i+1 < len(files) else len(self.rom.buffer)
            if end_current < start_next:
                free_space.append((start_next - end_current, end_current))

        free_space.sort()
        if not free_space:
            raise Exception(f"No free space in ROM. This should never happen. DMA entries: {self.dma_entries}")

        if size is None:
            # Return the largest free space.
            return free_space[-1][1]
        else:
            # Return the smallest area of free space that fits size.
            try:
                return next(filter(lambda f: f[0] >= size, free_space))[1]
            except StopIteration:
                raise Exception(f"Not enough free space in ROM to fit a file of size {size}. Largest region of free space available: {free_space[-1][0]}.")

class OverlayEntry:
    def __init__(self, vrom_start, vrom_end, vram_start, vram_end) -> None:
        self.vrom_start = vrom_start
        self.vrom_end = vrom_end
        self.vram_start = vram_start
        self.vram_end = vram_end

class OverlayTable:
    def read_overlay_table(rom: Rom, ovl_table_start: int, offset: int, entry_size: int, num_entries: int) -> None:
        overlay_entries: list[OverlayEntry] = []
        # Read the overlay table from the ROM
        for i in range(0, num_entries):
            entry_bytes = rom.read_bytes(ovl_table_start + i * entry_size, entry_size)
            vrom_start = int.from_bytes(entry_bytes[offset + 0:offset +4], 'big')
            vrom_end = int.from_bytes(entry_bytes[offset + 4:offset +8], 'big')
            vram_start = int.from_bytes(entry_bytes[offset + 8:offset +12], 'big')
            vram_end = int.from_bytes(entry_bytes[offset + 12:offset +16], 'big')
            overlay_entries.append(OverlayEntry(vrom_start, vrom_end, vram_start, vram_end))
        return overlay_entries

    def VRAM_2_VROM(overlay_entries: list[OverlayEntry], vram_address: int) -> int:
        # Loop through overlay table and find the entry containing the address
        for overlay_entry in overlay_entries:
            if (overlay_entry.vram_start <= vram_address) and (overlay_entry.vram_end > vram_address):
                return vram_address - overlay_entry.vram_start + overlay_entry.vrom_start
        raise Exception("Overlay address not found in table")

dma_table_names = {
    0: 'makerom',
    1: 'boot',
    2: 'dmadata',
    3: 'Audiobank',
    4: 'Audioseq',
    5: 'Audiotable',
    6: 'kanji',
    7: 'link_animetion',
    8: 'icon_item_static',
    9: 'icon_item_24_static',
    10: 'icon_item_field_static',
    11: 'icon_item_dungeon_static',
    12: 'icon_item_gameover_static',
    13: 'icon_item_jpn_static',
    14: 'icon_item_nes_static',
    15: 'item_name_static',
    16: 'map_name_static',
    17: 'do_action_static',
    18: 'message_static',
    19: 'message_data_static',
    20: 'message_texture_static',
    21: 'nes_font_static',
    22: 'nes_message_data_static',
    23: 'staff_message_data_static',
    24: 'map_grand_static',
    25: 'map_i_static',
    26: 'map_48x85_static',
    27: 'code',
    28: 'n64dd',
    29: 'ovl_title',
    30: 'ovl_select',
    31: 'ovl_opening',
    32: 'ovl_file_choose',
    33: 'ovl_kaleido_scope',
    34: 'ovl_player_actor',
    35: 'ovl_map_mark_data',
    36: 'ovl_En_Test',
    37: 'ovl_En_GirlA',
    38: 'ovl_En_Part',
    39: 'ovl_En_Light',
    40: 'ovl_En_Door',
    41: 'ovl_En_Box',
    42: 'ovl_En_Poh',
    43: 'ovl_En_Okuta',
    44: 'ovl_En_Bom',
    45: 'ovl_En_Wallmas',
    46: 'ovl_En_Dodongo',
    47: 'ovl_En_Firefly',
    48: 'ovl_En_Horse',
    49: 'ovl_En_Arrow',
    50: 'ovl_En_Elf',
    51: 'ovl_En_Niw',
    52: 'ovl_En_Tite',
    53: 'ovl_En_Reeba',
    54: 'ovl_En_Peehat',
    55: 'ovl_En_Holl',
    56: 'ovl_En_Scene_Change',
    57: 'ovl_En_Zf',
    58: 'ovl_En_Hata',
    59: 'ovl_Boss_Dodongo',
    60: 'ovl_Boss_Goma',
    61: 'ovl_En_Zl1',
    62: 'ovl_En_Viewer',
    63: 'ovl_En_Goma',
    64: 'ovl_Bg_Pushbox',
    65: 'ovl_En_Bubble',
    66: 'ovl_Door_Shutter',
    67: 'ovl_En_Dodojr',
    68: 'ovl_En_Bdfire',
    69: 'ovl_En_Boom',
    70: 'ovl_En_Torch2',
    71: 'ovl_En_Bili',
    72: 'ovl_En_Tp',
    73: 'ovl_En_St',
    74: 'ovl_En_Bw',
    75: 'ovl_En_Eiyer',
    76: 'ovl_En_River_Sound',
    77: 'ovl_En_Horse_Normal',
    78: 'ovl_En_Ossan',
    79: 'ovl_Bg_Treemouth',
    80: 'ovl_Bg_Dodoago',
    81: 'ovl_Bg_Hidan_Dalm',
    82: 'ovl_Bg_Hidan_Hrock',
    83: 'ovl_En_Horse_Ganon',
    84: 'ovl_Bg_Hidan_Rock',
    85: 'ovl_Bg_Hidan_Rsekizou',
    86: 'ovl_Bg_Hidan_Sekizou',
    87: 'ovl_Bg_Hidan_Sima',
    88: 'ovl_Bg_Hidan_Syoku',
    89: 'ovl_En_Xc',
    90: 'ovl_Bg_Hidan_Curtain',
    91: 'ovl_Bg_Spot00_Hanebasi',
    92: 'ovl_En_Mb',
    93: 'ovl_En_Bombf',
    94: 'ovl_Bg_Hidan_Firewall',
    95: 'ovl_Bg_Dy_Yoseizo',
    96: 'ovl_En_Zl2',
    97: 'ovl_Bg_Hidan_Fslift',
    98: 'ovl_En_OE2',
    99: 'ovl_Bg_Ydan_Hasi',
    100: 'ovl_Bg_Ydan_Maruta',
    101: 'ovl_Boss_Ganondrof',
    102: 'ovl_En_Am',
    103: 'ovl_En_Dekubaba',
    104: 'ovl_En_M_Fire1',
    105: 'ovl_En_M_Thunder',
    106: 'ovl_Bg_Ddan_Jd',
    107: 'ovl_Bg_Breakwall',
    108: 'ovl_En_Jj',
    109: 'ovl_En_Horse_Zelda',
    110: 'ovl_Bg_Ddan_Kd',
    111: 'ovl_Door_Warp1',
    112: 'ovl_Obj_Syokudai',
    113: 'ovl_Item_B_Heart',
    114: 'ovl_En_Dekunuts',
    115: 'ovl_Bg_Menkuri_Kaiten',
    116: 'ovl_Bg_Menkuri_Eye',
    117: 'ovl_En_Vali',
    118: 'ovl_Bg_Mizu_Movebg',
    119: 'ovl_Bg_Mizu_Water',
    120: 'ovl_Arms_Hook',
    121: 'ovl_En_fHG',
    122: 'ovl_Bg_Mori_Hineri',
    123: 'ovl_En_Bb',
    124: 'ovl_Bg_Toki_Hikari',
    125: 'ovl_En_Yukabyun',
    126: 'ovl_Bg_Toki_Swd',
    127: 'ovl_En_Fhg_Fire',
    128: 'ovl_Bg_Mjin',
    129: 'ovl_Bg_Hidan_Kousi',
    130: 'ovl_Door_Toki',
    131: 'ovl_Bg_Hidan_Hamstep',
    132: 'ovl_En_Bird',
    133: 'ovl_En_Wood02',
    134: 'ovl_En_Lightbox',
    135: 'ovl_En_Pu_box',
    136: 'ovl_En_Trap',
    137: 'ovl_En_Arow_Trap',
    138: 'ovl_En_Vase',
    139: 'ovl_En_Ta',
    140: 'ovl_En_Tk',
    141: 'ovl_Bg_Mori_Bigst',
    142: 'ovl_Bg_Mori_Elevator',
    143: 'ovl_Bg_Mori_Kaitenkabe',
    144: 'ovl_Bg_Mori_Rakkatenjo',
    145: 'ovl_En_Vm',
    146: 'ovl_Demo_Effect',
    147: 'ovl_Demo_Kankyo',
    148: 'ovl_Bg_Hidan_Fwbig',
    149: 'ovl_En_Floormas',
    150: 'ovl_En_Heishi1',
    151: 'ovl_En_Rd',
    152: 'ovl_En_Po_Sisters',
    153: 'ovl_Bg_Heavy_Block',
    154: 'ovl_Bg_Po_Event',
    155: 'ovl_Obj_Mure',
    156: 'ovl_En_Sw',
    157: 'ovl_Boss_Fd',
    158: 'ovl_Object_Kankyo',
    159: 'ovl_En_Du',
    160: 'ovl_En_Fd',
    161: 'ovl_En_Horse_Link_Child',
    162: 'ovl_Door_Ana',
    163: 'ovl_Bg_Spot02_Objects',
    164: 'ovl_Bg_Haka',
    165: 'ovl_Magic_Wind',
    166: 'ovl_Magic_Fire',
    167: 'ovl_En_Ru1',
    168: 'ovl_Boss_Fd2',
    169: 'ovl_En_Fd_Fire',
    170: 'ovl_En_Dh',
    171: 'ovl_En_Dha',
    172: 'ovl_En_Rl',
    173: 'ovl_En_Encount1',
    174: 'ovl_Demo_Du',
    175: 'ovl_Demo_Im',
    176: 'ovl_Demo_Tre_Lgt',
    177: 'ovl_En_Fw',
    178: 'ovl_Bg_Vb_Sima',
    179: 'ovl_En_Vb_Ball',
    180: 'ovl_Bg_Haka_Megane',
    181: 'ovl_Bg_Haka_MeganeBG',
    182: 'ovl_Bg_Haka_Ship',
    183: 'ovl_Bg_Haka_Sgami',
    184: 'ovl_En_Heishi2',
    185: 'ovl_En_Encount2',
    186: 'ovl_En_Fire_Rock',
    187: 'ovl_En_Brob',
    188: 'ovl_Mir_Ray',
    189: 'ovl_Bg_Spot09_Obj',
    190: 'ovl_Bg_Spot18_Obj',
    191: 'ovl_Boss_Va',
    192: 'ovl_Bg_Haka_Tubo',
    193: 'ovl_Bg_Haka_Trap',
    194: 'ovl_Bg_Haka_Huta',
    195: 'ovl_Bg_Haka_Zou',
    196: 'ovl_Bg_Spot17_Funen',
    197: 'ovl_En_Syateki_Itm',
    198: 'ovl_En_Syateki_Man',
    199: 'ovl_En_Tana',
    200: 'ovl_En_Nb',
    201: 'ovl_Boss_Mo',
    202: 'ovl_En_Sb',
    203: 'ovl_En_Bigokuta',
    204: 'ovl_En_Karebaba',
    205: 'ovl_Bg_Bdan_Objects',
    206: 'ovl_Demo_Sa',
    207: 'ovl_Demo_Go',
    208: 'ovl_En_In',
    209: 'ovl_En_Tr',
    210: 'ovl_Bg_Spot16_Bombstone',
    211: 'ovl_Bg_Hidan_Kowarerukabe',
    212: 'ovl_Bg_Bombwall',
    213: 'ovl_En_Ru2',
    214: 'ovl_Obj_Dekujr',
    215: 'ovl_Bg_Mizu_Uzu',
    216: 'ovl_Bg_Spot06_Objects',
    217: 'ovl_Bg_Ice_Objects',
    218: 'ovl_Bg_Haka_Water',
    219: 'ovl_En_Ma2',
    220: 'ovl_En_Bom_Chu',
    221: 'ovl_En_Horse_Game_Check',
    222: 'ovl_Boss_Tw',
    223: 'ovl_En_Rr',
    224: 'ovl_En_Ba',
    225: 'ovl_En_Bx',
    226: 'ovl_En_Anubice',
    227: 'ovl_En_Anubice_Fire',
    228: 'ovl_Bg_Mori_Hashigo',
    229: 'ovl_Bg_Mori_Hashira4',
    230: 'ovl_Bg_Mori_Idomizu',
    231: 'ovl_Bg_Spot16_Doughnut',
    232: 'ovl_Bg_Bdan_Switch',
    233: 'ovl_En_Ma1',
    234: 'ovl_Boss_Ganon',
    235: 'ovl_Boss_Sst',
    236: 'ovl_En_Ny',
    237: 'ovl_En_Fr',
    238: 'ovl_Item_Shield',
    239: 'ovl_Bg_Ice_Shelter',
    240: 'ovl_En_Ice_Hono',
    241: 'ovl_Item_Ocarina',
    242: 'ovl_Magic_Dark',
    243: 'ovl_Demo_6K',
    244: 'ovl_En_Anubice_Tag',
    245: 'ovl_Bg_Haka_Gate',
    246: 'ovl_Bg_Spot15_Saku',
    247: 'ovl_Bg_Jya_Goroiwa',
    248: 'ovl_Bg_Jya_Zurerukabe',
    249: 'ovl_Bg_Jya_Cobra',
    250: 'ovl_Bg_Jya_Kanaami',
    251: 'ovl_Fishing',
    252: 'ovl_Obj_Oshihiki',
    253: 'ovl_Bg_Gate_Shutter',
    254: 'ovl_Eff_Dust',
    255: 'ovl_Bg_Spot01_Fusya',
    256: 'ovl_Bg_Spot01_Idohashira',
    257: 'ovl_Bg_Spot01_Idomizu',
    258: 'ovl_Bg_Po_Syokudai',
    259: 'ovl_Bg_Ganon_Otyuka',
    260: 'ovl_Bg_Spot15_Rrbox',
    261: 'ovl_Bg_Umajump',
    262: 'ovl_En_Insect',
    263: 'ovl_En_Butte',
    264: 'ovl_En_Fish',
    265: 'ovl_Bg_Spot08_Iceblock',
    266: 'ovl_Item_Etcetera',
    267: 'ovl_Arrow_Fire',
    268: 'ovl_Arrow_Ice',
    269: 'ovl_Arrow_Light',
    270: 'ovl_Obj_Kibako',
    271: 'ovl_Obj_Tsubo',
    272: 'ovl_En_Wonder_Item',
    273: 'ovl_En_Ik',
    274: 'ovl_Demo_Ik',
    275: 'ovl_En_Skj',
    276: 'ovl_En_Skjneedle',
    277: 'ovl_En_G_Switch',
    278: 'ovl_Demo_Ext',
    279: 'ovl_Demo_Shd',
    280: 'ovl_En_Dns',
    281: 'ovl_Elf_Msg',
    282: 'ovl_En_Honotrap',
    283: 'ovl_En_Tubo_Trap',
    284: 'ovl_Obj_Ice_Poly',
    285: 'ovl_Bg_Spot03_Taki',
    286: 'ovl_Bg_Spot07_Taki',
    287: 'ovl_En_Fz',
    288: 'ovl_En_Po_Relay',
    289: 'ovl_Bg_Relay_Objects',
    290: 'ovl_En_Diving_Game',
    291: 'ovl_En_Kusa',
    292: 'ovl_Obj_Bean',
    293: 'ovl_Obj_Bombiwa',
    294: 'ovl_Obj_Switch',
    295: 'ovl_Obj_Elevator',
    296: 'ovl_Obj_Lift',
    297: 'ovl_Obj_Hsblock',
    298: 'ovl_En_Okarina_Tag',
    299: 'ovl_En_Yabusame_Mark',
    300: 'ovl_En_Goroiwa',
    301: 'ovl_En_Ex_Ruppy',
    302: 'ovl_En_Toryo',
    303: 'ovl_En_Daiku',
    304: 'ovl_En_Nwc',
    305: 'ovl_En_Blkobj',
    306: 'ovl_Item_Inbox',
    307: 'ovl_En_Ge1',
    308: 'ovl_Obj_Blockstop',
    309: 'ovl_En_Sda',
    310: 'ovl_En_Clear_Tag',
    311: 'ovl_En_Niw_Lady',
    312: 'ovl_En_Gm',
    313: 'ovl_En_Ms',
    314: 'ovl_En_Hs',
    315: 'ovl_Bg_Ingate',
    316: 'ovl_En_Kanban',
    317: 'ovl_En_Heishi3',
    318: 'ovl_En_Syateki_Niw',
    319: 'ovl_En_Attack_Niw',
    320: 'ovl_Bg_Spot01_Idosoko',
    321: 'ovl_En_Sa',
    322: 'ovl_En_Wonder_Talk',
    323: 'ovl_Bg_Gjyo_Bridge',
    324: 'ovl_En_Ds',
    325: 'ovl_En_Mk',
    326: 'ovl_En_Bom_Bowl_Man',
    327: 'ovl_En_Bom_Bowl_Pit',
    328: 'ovl_En_Owl',
    329: 'ovl_En_Ishi',
    330: 'ovl_Obj_Hana',
    331: 'ovl_Obj_Lightswitch',
    332: 'ovl_Obj_Mure2',
    333: 'ovl_En_Go',
    334: 'ovl_En_Fu',
    335: 'ovl_En_Changer',
    336: 'ovl_Bg_Jya_Megami',
    337: 'ovl_Bg_Jya_Lift',
    338: 'ovl_Bg_Jya_Bigmirror',
    339: 'ovl_Bg_Jya_Bombchuiwa',
    340: 'ovl_Bg_Jya_Amishutter',
    341: 'ovl_Bg_Jya_Bombiwa',
    342: 'ovl_Bg_Spot18_Basket',
    343: 'ovl_En_Ganon_Organ',
    344: 'ovl_En_Siofuki',
    345: 'ovl_En_Stream',
    346: 'ovl_En_Mm',
    347: 'ovl_En_Ko',
    348: 'ovl_En_Kz',
    349: 'ovl_En_Weather_Tag',
    350: 'ovl_Bg_Sst_Floor',
    351: 'ovl_En_Ani',
    352: 'ovl_En_Ex_Item',
    353: 'ovl_Bg_Jya_Ironobj',
    354: 'ovl_En_Js',
    355: 'ovl_En_Jsjutan',
    356: 'ovl_En_Cs',
    357: 'ovl_En_Md',
    358: 'ovl_En_Hy',
    359: 'ovl_En_Ganon_Mant',
    360: 'ovl_En_Okarina_Effect',
    361: 'ovl_En_Mag',
    362: 'ovl_Door_Gerudo',
    363: 'ovl_Elf_Msg2',
    364: 'ovl_Demo_Gt',
    365: 'ovl_En_Po_Field',
    366: 'ovl_Efc_Erupc',
    367: 'ovl_Bg_Zg',
    368: 'ovl_En_Heishi4',
    369: 'ovl_En_Zl3',
    370: 'ovl_Boss_Ganon2',
    371: 'ovl_En_Kakasi',
    372: 'ovl_En_Takara_Man',
    373: 'ovl_Obj_Makeoshihiki',
    374: 'ovl_Oceff_Spot',
    375: 'ovl_End_Title',
    376: 'ovl_En_Torch',
    377: 'ovl_Demo_Ec',
    378: 'ovl_Shot_Sun',
    379: 'ovl_En_Dy_Extra',
    380: 'ovl_En_Wonder_Talk2',
    381: 'ovl_En_Ge2',
    382: 'ovl_Obj_Roomtimer',
    383: 'ovl_En_Ssh',
    384: 'ovl_En_Sth',
    385: 'ovl_Oceff_Wipe',
    386: 'ovl_Effect_Ss_Dust',
    387: 'ovl_Effect_Ss_KiraKira',
    388: 'ovl_Effect_Ss_Bomb',
    389: 'ovl_Effect_Ss_Bomb2',
    390: 'ovl_Effect_Ss_Blast',
    391: 'ovl_Effect_Ss_G_Spk',
    392: 'ovl_Effect_Ss_D_Fire',
    393: 'ovl_Effect_Ss_Bubble',
    394: 'ovl_Effect_Ss_G_Ripple',
    395: 'ovl_Effect_Ss_G_Splash',
    396: 'ovl_Effect_Ss_G_Magma',
    397: 'ovl_Effect_Ss_G_Fire',
    398: 'ovl_Effect_Ss_Lightning',
    399: 'ovl_Effect_Ss_Dt_Bubble',
    400: 'ovl_Effect_Ss_Hahen',
    401: 'ovl_Effect_Ss_Stick',
    402: 'ovl_Effect_Ss_Sibuki',
    403: 'ovl_Effect_Ss_Sibuki2',
    404: 'ovl_Effect_Ss_G_Magma2',
    405: 'ovl_Effect_Ss_Stone1',
    406: 'ovl_Effect_Ss_HitMark',
    407: 'ovl_Effect_Ss_Fhg_Flash',
    408: 'ovl_Effect_Ss_K_Fire',
    409: 'ovl_Effect_Ss_Solder_Srch_Ball',
    410: 'ovl_Effect_Ss_Kakera',
    411: 'ovl_Effect_Ss_Ice_Piece',
    412: 'ovl_Effect_Ss_En_Ice',
    413: 'ovl_Effect_Ss_Fire_Tail',
    414: 'ovl_Effect_Ss_En_Fire',
    415: 'ovl_Effect_Ss_Extra',
    416: 'ovl_Effect_Ss_Fcircle',
    417: 'ovl_Effect_Ss_Dead_Db',
    418: 'ovl_Effect_Ss_Dead_Dd',
    419: 'ovl_Effect_Ss_Dead_Ds',
    420: 'ovl_Effect_Ss_Dead_Sound',
    421: 'ovl_Oceff_Storm',
    422: 'ovl_En_Weiyer',
    423: 'ovl_Bg_Spot05_Soko',
    424: 'ovl_Bg_Jya_1flift',
    425: 'ovl_Bg_Jya_Haheniron',
    426: 'ovl_Bg_Spot12_Gate',
    427: 'ovl_Bg_Spot12_Saku',
    428: 'ovl_En_Hintnuts',
    429: 'ovl_En_Nutsball',
    430: 'ovl_Bg_Spot00_Break',
    431: 'ovl_En_Shopnuts',
    432: 'ovl_En_It',
    433: 'ovl_En_GeldB',
    434: 'ovl_Oceff_Wipe2',
    435: 'ovl_Oceff_Wipe3',
    436: 'ovl_En_Niw_Girl',
    437: 'ovl_En_Dog',
    438: 'ovl_En_Si',
    439: 'ovl_Bg_Spot01_Objects2',
    440: 'ovl_Obj_Comb',
    441: 'ovl_Bg_Spot11_Bakudankabe',
    442: 'ovl_Obj_Kibako2',
    443: 'ovl_En_Dnt_Demo',
    444: 'ovl_En_Dnt_Jiji',
    445: 'ovl_En_Dnt_Nomal',
    446: 'ovl_En_Guest',
    447: 'ovl_Bg_Bom_Guard',
    448: 'ovl_En_Hs2',
    449: 'ovl_Demo_Kekkai',
    450: 'ovl_Bg_Spot08_Bakudankabe',
    451: 'ovl_Bg_Spot17_Bakudankabe',
    452: 'ovl_Obj_Mure3',
    453: 'ovl_En_Tg',
    454: 'ovl_En_Mu',
    455: 'ovl_En_Go2',
    456: 'ovl_En_Wf',
    457: 'ovl_En_Skb',
    458: 'ovl_Demo_Gj',
    459: 'ovl_Demo_Geff',
    460: 'ovl_Bg_Gnd_Firemeiro',
    461: 'ovl_Bg_Gnd_Darkmeiro',
    462: 'ovl_Bg_Gnd_Soulmeiro',
    463: 'ovl_Bg_Gnd_Nisekabe',
    464: 'ovl_Bg_Gnd_Iceblock',
    465: 'ovl_Bg_Ydan_Sp',
    466: 'ovl_En_Gb',
    467: 'ovl_En_Gs',
    468: 'ovl_Bg_Mizu_Bwall',
    469: 'ovl_Bg_Mizu_Shutter',
    470: 'ovl_En_Daiku_Kakariko',
    471: 'ovl_Bg_Bowl_Wall',
    472: 'ovl_En_Wall_Tubo',
    473: 'ovl_En_Po_Desert',
    474: 'ovl_En_Crow',
    475: 'ovl_Door_Killer',
    476: 'ovl_Bg_Spot11_Oasis',
    477: 'ovl_Bg_Spot18_Futa',
    478: 'ovl_Bg_Spot18_Shutter',
    479: 'ovl_En_Ma3',
    480: 'ovl_En_Cow',
    481: 'ovl_Bg_Ice_Turara',
    482: 'ovl_Bg_Ice_Shutter',
    483: 'ovl_En_Kakasi2',
    484: 'ovl_En_Kakasi3',
    485: 'ovl_Oceff_Wipe4',
    486: 'ovl_En_Eg',
    487: 'ovl_Bg_Menkuri_Nisekabe',
    488: 'ovl_En_Zo',
    489: 'ovl_Effect_Ss_Ice_Smoke',
    490: 'ovl_Obj_Makekinsuta',
    491: 'ovl_En_Ge3',
    492: 'ovl_Obj_Timeblock',
    493: 'ovl_Obj_Hamishi',
    494: 'ovl_En_Zl4',
    495: 'ovl_En_Mm2',
    496: 'ovl_Bg_Jya_Block',
    497: 'ovl_Obj_Warp2block',
    498: 'gameplay_keep',
    499: 'gameplay_field_keep',
    500: 'gameplay_dangeon_keep',
    501: 'gameplay_object_exchange_static',
    502: 'object_link_boy',
    503: 'object_link_child',
    504: 'object_box',
    505: 'object_human',
    506: 'object_okuta',
    507: 'object_poh',
    508: 'object_wallmaster',
    509: 'object_dy_obj',
    510: 'object_firefly',
    511: 'object_dodongo',
    512: 'object_fire',
    513: 'object_niw',
    514: 'object_tite',
    515: 'object_reeba',
    516: 'object_peehat',
    517: 'object_kingdodongo',
    518: 'object_horse',
    519: 'object_zf',
    520: 'object_goma',
    521: 'object_zl1',
    522: 'object_gol',
    523: 'object_bubble',
    524: 'object_dodojr',
    525: 'object_torch2',
    526: 'object_bl',
    527: 'object_tp',
    528: 'object_oA1',
    529: 'object_st',
    530: 'object_bw',
    531: 'object_ei',
    532: 'object_horse_normal',
    533: 'object_oB1',
    534: 'object_o_anime',
    535: 'object_spot04_objects',
    536: 'object_ddan_objects',
    537: 'object_hidan_objects',
    538: 'object_horse_ganon',
    539: 'object_oA2',
    540: 'object_spot00_objects',
    541: 'object_mb',
    542: 'object_bombf',
    543: 'object_sk2',
    544: 'object_oE1',
    545: 'object_oE_anime',
    546: 'object_oE2',
    547: 'object_ydan_objects',
    548: 'object_gnd',
    549: 'object_am',
    550: 'object_dekubaba',
    551: 'object_oA3',
    552: 'object_oA4',
    553: 'object_oA5',
    554: 'object_oA6',
    555: 'object_oA7',
    556: 'object_jj',
    557: 'object_oA8',
    558: 'object_oA9',
    559: 'object_oB2',
    560: 'object_oB3',
    561: 'object_oB4',
    562: 'object_horse_zelda',
    563: 'object_opening_demo1',
    564: 'object_warp1',
    565: 'object_b_heart',
    566: 'object_dekunuts',
    567: 'object_oE3',
    568: 'object_oE4',
    569: 'object_menkuri_objects',
    570: 'object_oE5',
    571: 'object_oE6',
    572: 'object_oE7',
    573: 'object_oE8',
    574: 'object_oE9',
    575: 'object_oE10',
    576: 'object_oE11',
    577: 'object_oE12',
    578: 'object_vali',
    579: 'object_oA10',
    580: 'object_oA11',
    581: 'object_mizu_objects',
    582: 'object_fhg',
    583: 'object_ossan',
    584: 'object_mori_hineri1',
    585: 'object_Bb',
    586: 'object_toki_objects',
    587: 'object_yukabyun',
    588: 'object_zl2',
    589: 'object_mjin',
    590: 'object_mjin_flash',
    591: 'object_mjin_dark',
    592: 'object_mjin_flame',
    593: 'object_mjin_ice',
    594: 'object_mjin_soul',
    595: 'object_mjin_wind',
    596: 'object_mjin_oka',
    597: 'object_haka_objects',
    598: 'object_spot06_objects',
    599: 'object_ice_objects',
    600: 'object_relay_objects',
    601: 'object_mori_hineri1a',
    602: 'object_mori_hineri2',
    603: 'object_mori_hineri2a',
    604: 'object_mori_objects',
    605: 'object_mori_tex',
    606: 'object_spot08_obj',
    607: 'object_warp2',
    608: 'object_hata',
    609: 'object_bird',
    610: 'object_wood02',
    611: 'object_lightbox',
    612: 'object_pu_box',
    613: 'object_trap',
    614: 'object_vase',
    615: 'object_im',
    616: 'object_ta',
    617: 'object_tk',
    618: 'object_xc',
    619: 'object_vm',
    620: 'object_bv',
    621: 'object_hakach_objects',
    622: 'object_efc_crystal_light',
    623: 'object_efc_fire_ball',
    624: 'object_efc_flash',
    625: 'object_efc_lgt_shower',
    626: 'object_efc_star_field',
    627: 'object_god_lgt',
    628: 'object_light_ring',
    629: 'object_triforce_spot',
    630: 'object_medal',
    631: 'object_bdan_objects',
    632: 'object_sd',
    633: 'object_rd',
    634: 'object_po_sisters',
    635: 'object_heavy_object',
    636: 'object_gndd',
    637: 'object_fd',
    638: 'object_du',
    639: 'object_fw',
    640: 'object_horse_link_child',
    641: 'object_spot02_objects',
    642: 'object_haka',
    643: 'object_ru1',
    644: 'object_syokudai',
    645: 'object_fd2',
    646: 'object_dh',
    647: 'object_rl',
    648: 'object_efc_tw',
    649: 'object_demo_tre_lgt',
    650: 'object_gi_key',
    651: 'object_mir_ray',
    652: 'object_brob',
    653: 'object_gi_jewel',
    654: 'object_spot09_obj',
    655: 'object_spot18_obj',
    656: 'object_bdoor',
    657: 'object_spot17_obj',
    658: 'object_shop_dungen',
    659: 'object_nb',
    660: 'object_mo',
    661: 'object_sb',
    662: 'object_gi_melody',
    663: 'object_gi_heart',
    664: 'object_gi_compass',
    665: 'object_gi_bosskey',
    666: 'object_gi_medal',
    667: 'object_gi_nuts',
    668: 'object_sa',
    669: 'object_gi_hearts',
    670: 'object_gi_arrowcase',
    671: 'object_gi_bombpouch',
    672: 'object_in',
    673: 'object_tr',
    674: 'object_spot16_obj',
    675: 'object_oE1s',
    676: 'object_oE4s',
    677: 'object_os_anime',
    678: 'object_gi_bottle',
    679: 'object_gi_stick',
    680: 'object_gi_map',
    681: 'object_oF1d_map',
    682: 'object_ru2',
    683: 'object_gi_shield_1',
    684: 'object_dekujr',
    685: 'object_gi_magicpot',
    686: 'object_gi_bomb_1',
    687: 'object_oF1s',
    688: 'object_ma2',
    689: 'object_gi_purse',
    690: 'object_hni',
    691: 'object_tw',
    692: 'object_rr',
    693: 'object_bxa',
    694: 'object_anubice',
    695: 'object_gi_gerudo',
    696: 'object_gi_arrow',
    697: 'object_gi_bomb_2',
    698: 'object_gi_egg',
    699: 'object_gi_scale',
    700: 'object_gi_shield_2',
    701: 'object_gi_hookshot',
    702: 'object_gi_ocarina',
    703: 'object_gi_milk',
    704: 'object_ma1',
    705: 'object_ganon',
    706: 'object_sst',
    707: 'object_ny',
    708: 'object_fr',
    709: 'object_gi_pachinko',
    710: 'object_gi_boomerang',
    711: 'object_gi_bow',
    712: 'object_gi_glasses',
    713: 'object_gi_liquid',
    714: 'object_ani',
    715: 'object_demo_6k',
    716: 'object_gi_shield_3',
    717: 'object_gi_letter',
    718: 'object_spot15_obj',
    719: 'object_jya_obj',
    720: 'object_gi_clothes',
    721: 'object_gi_bean',
    722: 'object_gi_fish',
    723: 'object_gi_saw',
    724: 'object_gi_hammer',
    725: 'object_gi_grass',
    726: 'object_gi_longsword',
    727: 'object_spot01_objects',
    728: 'object_md',
    729: 'object_km1',
    730: 'object_kw1',
    731: 'object_zo',
    732: 'object_kz',
    733: 'object_umajump',
    734: 'object_masterkokiri',
    735: 'object_masterkokirihead',
    736: 'object_mastergolon',
    737: 'object_masterzoora',
    738: 'object_aob',
    739: 'object_ik',
    740: 'object_ahg',
    741: 'object_cne',
    742: 'object_gi_niwatori',
    743: 'object_skj',
    744: 'object_gi_bottle_letter',
    745: 'object_bji',
    746: 'object_bba',
    747: 'object_gi_ocarina_0',
    748: 'object_ds',
    749: 'object_ane',
    750: 'object_boj',
    751: 'object_spot03_object',
    752: 'object_spot07_object',
    753: 'object_fz',
    754: 'object_bob',
    755: 'object_ge1',
    756: 'object_yabusame_point',
    757: 'object_gi_boots_2',
    758: 'object_gi_seed',
    759: 'object_gnd_magic',
    760: 'object_d_elevator',
    761: 'object_d_hsblock',
    762: 'object_d_lift',
    763: 'object_mamenoki',
    764: 'object_goroiwa',
    765: 'object_toryo',
    766: 'object_daiku',
    767: 'object_nwc',
    768: 'object_blkobj',
    769: 'object_gm',
    770: 'object_ms',
    771: 'object_hs',
    772: 'object_ingate',
    773: 'object_lightswitch',
    774: 'object_kusa',
    775: 'object_tsubo',
    776: 'object_gi_gloves',
    777: 'object_gi_coin',
    778: 'object_kanban',
    779: 'object_gjyo_objects',
    780: 'object_owl',
    781: 'object_mk',
    782: 'object_fu',
    783: 'object_gi_ki_tan_mask',
    784: 'object_gi_redead_mask',
    785: 'object_gi_skj_mask',
    786: 'object_gi_rabit_mask',
    787: 'object_gi_truth_mask',
    788: 'object_ganon_objects',
    789: 'object_siofuki',
    790: 'object_stream',
    791: 'object_mm',
    792: 'object_fa',
    793: 'object_os',
    794: 'object_gi_eye_lotion',
    795: 'object_gi_powder',
    796: 'object_gi_mushroom',
    797: 'object_gi_ticketstone',
    798: 'object_gi_brokensword',
    799: 'object_js',
    800: 'object_cs',
    801: 'object_gi_prescription',
    802: 'object_gi_bracelet',
    803: 'object_gi_soldout',
    804: 'object_gi_frog',
    805: 'object_mag',
    806: 'object_door_gerudo',
    807: 'object_gt',
    808: 'object_efc_erupc',
    809: 'object_zl2_anime1',
    810: 'object_zl2_anime2',
    811: 'object_gi_golonmask',
    812: 'object_gi_zoramask',
    813: 'object_gi_gerudomask',
    814: 'object_ganon2',
    815: 'object_ka',
    816: 'object_ts',
    817: 'object_zg',
    818: 'object_gi_hoverboots',
    819: 'object_gi_m_arrow',
    820: 'object_ds2',
    821: 'object_ec',
    822: 'object_fish',
    823: 'object_gi_sutaru',
    824: 'object_gi_goddess',
    825: 'object_ssh',
    826: 'object_bigokuta',
    827: 'object_bg',
    828: 'object_spot05_objects',
    829: 'object_spot12_obj',
    830: 'object_bombiwa',
    831: 'object_hintnuts',
    832: 'object_rs',
    833: 'object_spot00_break',
    834: 'object_gla',
    835: 'object_shopnuts',
    836: 'object_geldb',
    837: 'object_gr',
    838: 'object_dog',
    839: 'object_jya_iron',
    840: 'object_jya_door',
    841: 'object_spot01_objects2',
    842: 'object_spot11_obj',
    843: 'object_kibako2',
    844: 'object_dns',
    845: 'object_dnk',
    846: 'object_gi_fire',
    847: 'object_gi_insect',
    848: 'object_gi_butterfly',
    849: 'object_gi_ghost',
    850: 'object_gi_soul',
    851: 'object_bowl',
    852: 'object_po_field',
    853: 'object_demo_kekkai',
    854: 'object_efc_doughnut',
    855: 'object_gi_dekupouch',
    856: 'object_ganon_anime1',
    857: 'object_ganon_anime2',
    858: 'object_ganon_anime3',
    859: 'object_gi_rupy',
    860: 'object_spot01_matoya',
    861: 'object_spot01_matoyab',
    862: 'object_po_composer',
    863: 'object_mu',
    864: 'object_wf',
    865: 'object_skb',
    866: 'object_gj',
    867: 'object_geff',
    868: 'object_haka_door',
    869: 'object_gs',
    870: 'object_ps',
    871: 'object_bwall',
    872: 'object_crow',
    873: 'object_cow',
    874: 'object_cob',
    875: 'object_gi_sword_1',
    876: 'object_door_killer',
    877: 'object_ouke_haka',
    878: 'object_timeblock',
    879: 'object_zl4',
    880: 'g_pn_01',
    881: 'g_pn_02',
    882: 'g_pn_03',
    883: 'g_pn_04',
    884: 'g_pn_05',
    885: 'g_pn_06',
    886: 'g_pn_07',
    887: 'g_pn_08',
    888: 'g_pn_09',
    889: 'g_pn_10',
    890: 'g_pn_11',
    891: 'g_pn_12',
    892: 'g_pn_13',
    893: 'g_pn_14',
    894: 'g_pn_15',
    895: 'g_pn_16',
    896: 'g_pn_17',
    897: 'g_pn_18',
    898: 'g_pn_19',
    899: 'g_pn_20',
    900: 'g_pn_21',
    901: 'g_pn_22',
    902: 'g_pn_23',
    903: 'g_pn_24',
    904: 'g_pn_25',
    905: 'g_pn_26',
    906: 'g_pn_27',
    907: 'g_pn_28',
    908: 'g_pn_29',
    909: 'g_pn_30',
    910: 'g_pn_31',
    911: 'g_pn_32',
    912: 'g_pn_33',
    913: 'g_pn_34',
    914: 'g_pn_35',
    915: 'g_pn_36',
    916: 'g_pn_37',
    917: 'g_pn_38',
    918: 'g_pn_39',
    919: 'g_pn_40',
    920: 'g_pn_41',
    921: 'g_pn_42',
    922: 'g_pn_43',
    923: 'g_pn_44',
    924: 'g_pn_45',
    925: 'g_pn_46',
    926: 'g_pn_47',
    927: 'g_pn_48',
    928: 'g_pn_49',
    929: 'g_pn_50',
    930: 'g_pn_51',
    931: 'g_pn_52',
    932: 'g_pn_53',
    933: 'g_pn_54',
    934: 'g_pn_55',
    935: 'g_pn_56',
    936: 'g_pn_57',
    937: 'z_select_static',
    938: 'nintendo_rogo_static',
    939: 'title_static',
    940: 'parameter_static',
    941: 'vr_fine0_static',
    942: 'vr_fine0_pal_static',
    943: 'vr_fine1_static',
    944: 'vr_fine1_pal_static',
    945: 'vr_fine2_static',
    946: 'vr_fine2_pal_static',
    947: 'vr_fine3_static',
    948: 'vr_fine3_pal_static',
    949: 'vr_cloud0_static',
    950: 'vr_cloud0_pal_static',
    951: 'vr_cloud1_static',
    952: 'vr_cloud1_pal_static',
    953: 'vr_cloud2_static',
    954: 'vr_cloud2_pal_static',
    955: 'vr_cloud3_static',
    956: 'vr_cloud3_pal_static',
    957: 'vr_holy0_static',
    958: 'vr_holy0_pal_static',
    959: 'vr_holy1_static',
    960: 'vr_holy1_pal_static',
    961: 'vr_MDVR_static',
    962: 'vr_MDVR_pal_static',
    963: 'vr_MNVR_static',
    964: 'vr_MNVR_pal_static',
    965: 'vr_RUVR_static',
    966: 'vr_RUVR_pal_static',
    967: 'vr_LHVR_static',
    968: 'vr_LHVR_pal_static',
    969: 'vr_KHVR_static',
    970: 'vr_KHVR_pal_static',
    971: 'vr_K3VR_static',
    972: 'vr_K3VR_pal_static',
    973: 'vr_K4VR_static',
    974: 'vr_K4VR_pal_static',
    975: 'vr_K5VR_static',
    976: 'vr_K5VR_pal_static',
    977: 'vr_SP1a_static',
    978: 'vr_SP1a_pal_static',
    979: 'vr_MLVR_static',
    980: 'vr_MLVR_pal_static',
    981: 'vr_KKRVR_static',
    982: 'vr_KKRVR_pal_static',
    983: 'vr_KR3VR_static',
    984: 'vr_KR3VR_pal_static',
    985: 'vr_IPVR_static',
    986: 'vr_IPVR_pal_static',
    987: 'vr_KSVR_static',
    988: 'vr_KSVR_pal_static',
    989: 'vr_GLVR_static',
    990: 'vr_GLVR_pal_static',
    991: 'vr_ZRVR_static',
    992: 'vr_ZRVR_pal_static',
    993: 'vr_DGVR_static',
    994: 'vr_DGVR_pal_static',
    995: 'vr_ALVR_static',
    996: 'vr_ALVR_pal_static',
    997: 'vr_NSVR_static',
    998: 'vr_NSVR_pal_static',
    999: 'vr_LBVR_static',
    1000: 'vr_LBVR_pal_static',
    1001: 'vr_TTVR_static',
    1002: 'vr_TTVR_pal_static',
    1003: 'vr_FCVR_static',
    1004: 'vr_FCVR_pal_static',
    1005: 'elf_message_field',
    1006: 'elf_message_ydan',
    1007: 'ddan_scene',
    1008: 'ddan_room_0',
    1009: 'ddan_room_1',
    1010: 'ddan_room_2',
    1011: 'ddan_room_3',
    1012: 'ddan_room_4',
    1013: 'ddan_room_5',
    1014: 'ddan_room_6',
    1015: 'ddan_room_7',
    1016: 'ddan_room_8',
    1017: 'ddan_room_9',
    1018: 'ddan_room_10',
    1019: 'ddan_room_11',
    1020: 'ddan_room_12',
    1021: 'ddan_room_13',
    1022: 'ddan_room_14',
    1023: 'ddan_room_15',
    1024: 'ddan_room_16',
    1025: 'spot00_scene',
    1026: 'spot00_room_0',
    1027: 'spot01_scene',
    1028: 'spot01_room_0',
    1029: 'spot02_scene',
    1030: 'spot02_room_0',
    1031: 'spot02_room_1',
    1032: 'spot03_scene',
    1033: 'spot03_room_0',
    1034: 'spot03_room_1',
    1035: 'spot04_scene',
    1036: 'spot04_room_0',
    1037: 'spot04_room_1',
    1038: 'spot04_room_2',
    1039: 'spot05_scene',
    1040: 'spot05_room_0',
    1041: 'spot06_scene',
    1042: 'spot06_room_0',
    1043: 'spot07_scene',
    1044: 'spot07_room_0',
    1045: 'spot07_room_1',
    1046: 'spot08_scene',
    1047: 'spot08_room_0',
    1048: 'spot09_scene',
    1049: 'spot09_room_0',
    1050: 'spot10_scene',
    1051: 'spot10_room_0',
    1052: 'spot10_room_1',
    1053: 'spot10_room_2',
    1054: 'spot10_room_3',
    1055: 'spot10_room_4',
    1056: 'spot10_room_5',
    1057: 'spot10_room_6',
    1058: 'spot10_room_7',
    1059: 'spot10_room_8',
    1060: 'spot10_room_9',
    1061: 'spot11_scene',
    1062: 'spot11_room_0',
    1063: 'spot12_scene',
    1064: 'spot12_room_0',
    1065: 'spot12_room_1',
    1066: 'spot13_scene',
    1067: 'spot13_room_0',
    1068: 'spot13_room_1',
    1069: 'spot15_scene',
    1070: 'spot15_room_0',
    1071: 'spot16_scene',
    1072: 'spot16_room_0',
    1073: 'spot17_scene',
    1074: 'spot17_room_0',
    1075: 'spot17_room_1',
    1076: 'spot18_scene',
    1077: 'spot18_room_0',
    1078: 'spot18_room_1',
    1079: 'spot18_room_2',
    1080: 'spot18_room_3',
    1081: 'market_day_scene',
    1082: 'market_day_room_0',
    1083: 'market_night_scene',
    1084: 'market_night_room_0',
    1085: 'HIDAN_scene',
    1086: 'HIDAN_room_0',
    1087: 'HIDAN_room_1',
    1088: 'HIDAN_room_2',
    1089: 'HIDAN_room_3',
    1090: 'HIDAN_room_4',
    1091: 'HIDAN_room_5',
    1092: 'HIDAN_room_6',
    1093: 'HIDAN_room_7',
    1094: 'HIDAN_room_8',
    1095: 'HIDAN_room_9',
    1096: 'HIDAN_room_10',
    1097: 'HIDAN_room_11',
    1098: 'HIDAN_room_12',
    1099: 'HIDAN_room_13',
    1100: 'HIDAN_room_14',
    1101: 'HIDAN_room_15',
    1102: 'HIDAN_room_16',
    1103: 'HIDAN_room_17',
    1104: 'HIDAN_room_18',
    1105: 'HIDAN_room_19',
    1106: 'HIDAN_room_20',
    1107: 'HIDAN_room_21',
    1108: 'HIDAN_room_22',
    1109: 'HIDAN_room_23',
    1110: 'HIDAN_room_24',
    1111: 'HIDAN_room_25',
    1112: 'HIDAN_room_26',
    1113: 'Bmori1_scene',
    1114: 'Bmori1_room_0',
    1115: 'Bmori1_room_1',
    1116: 'Bmori1_room_2',
    1117: 'Bmori1_room_3',
    1118: 'Bmori1_room_4',
    1119: 'Bmori1_room_5',
    1120: 'Bmori1_room_6',
    1121: 'Bmori1_room_7',
    1122: 'Bmori1_room_8',
    1123: 'Bmori1_room_9',
    1124: 'Bmori1_room_10',
    1125: 'Bmori1_room_11',
    1126: 'Bmori1_room_12',
    1127: 'Bmori1_room_13',
    1128: 'Bmori1_room_14',
    1129: 'Bmori1_room_15',
    1130: 'Bmori1_room_16',
    1131: 'Bmori1_room_17',
    1132: 'Bmori1_room_18',
    1133: 'Bmori1_room_19',
    1134: 'Bmori1_room_20',
    1135: 'Bmori1_room_21',
    1136: 'Bmori1_room_22',
    1137: 'ydan_scene',
    1138: 'ydan_room_0',
    1139: 'ydan_room_1',
    1140: 'ydan_room_2',
    1141: 'ydan_room_3',
    1142: 'ydan_room_4',
    1143: 'ydan_room_5',
    1144: 'ydan_room_6',
    1145: 'ydan_room_7',
    1146: 'ydan_room_8',
    1147: 'ydan_room_9',
    1148: 'ydan_room_10',
    1149: 'ydan_room_11',
    1150: 'kenjyanoma_scene',
    1151: 'kenjyanoma_room_0',
    1152: 'tokinoma_scene',
    1153: 'tokinoma_room_0',
    1154: 'tokinoma_room_1',
    1155: 'link_home_scene',
    1156: 'link_home_room_0',
    1157: 'kokiri_shop_scene',
    1158: 'kokiri_shop_room_0',
    1159: 'MIZUsin_scene',
    1160: 'MIZUsin_room_0',
    1161: 'MIZUsin_room_1',
    1162: 'MIZUsin_room_2',
    1163: 'MIZUsin_room_3',
    1164: 'MIZUsin_room_4',
    1165: 'MIZUsin_room_5',
    1166: 'MIZUsin_room_6',
    1167: 'MIZUsin_room_7',
    1168: 'MIZUsin_room_8',
    1169: 'MIZUsin_room_9',
    1170: 'MIZUsin_room_10',
    1171: 'MIZUsin_room_11',
    1172: 'MIZUsin_room_12',
    1173: 'MIZUsin_room_13',
    1174: 'MIZUsin_room_14',
    1175: 'MIZUsin_room_15',
    1176: 'MIZUsin_room_16',
    1177: 'MIZUsin_room_17',
    1178: 'MIZUsin_room_18',
    1179: 'MIZUsin_room_19',
    1180: 'MIZUsin_room_20',
    1181: 'MIZUsin_room_21',
    1182: 'MIZUsin_room_22',
    1183: 'kokiri_home_scene',
    1184: 'kokiri_home_room_0',
    1185: 'kakusiana_scene',
    1186: 'kakusiana_room_0',
    1187: 'kakusiana_room_1',
    1188: 'kakusiana_room_2',
    1189: 'kakusiana_room_3',
    1190: 'kakusiana_room_4',
    1191: 'kakusiana_room_5',
    1192: 'kakusiana_room_6',
    1193: 'kakusiana_room_7',
    1194: 'kakusiana_room_8',
    1195: 'kakusiana_room_9',
    1196: 'kakusiana_room_10',
    1197: 'kakusiana_room_11',
    1198: 'kakusiana_room_12',
    1199: 'kakusiana_room_13',
    1200: 'entra_scene',
    1201: 'entra_room_0',
    1202: 'bdan_scene',
    1203: 'bdan_room_0',
    1204: 'bdan_room_1',
    1205: 'bdan_room_2',
    1206: 'bdan_room_3',
    1207: 'bdan_room_4',
    1208: 'bdan_room_5',
    1209: 'bdan_room_6',
    1210: 'bdan_room_7',
    1211: 'bdan_room_8',
    1212: 'bdan_room_9',
    1213: 'bdan_room_10',
    1214: 'bdan_room_11',
    1215: 'bdan_room_12',
    1216: 'bdan_room_13',
    1217: 'bdan_room_14',
    1218: 'bdan_room_15',
    1219: 'HAKAdan_scene',
    1220: 'HAKAdan_room_0',
    1221: 'HAKAdan_room_1',
    1222: 'HAKAdan_room_2',
    1223: 'HAKAdan_room_3',
    1224: 'HAKAdan_room_4',
    1225: 'HAKAdan_room_5',
    1226: 'HAKAdan_room_6',
    1227: 'HAKAdan_room_7',
    1228: 'HAKAdan_room_8',
    1229: 'HAKAdan_room_9',
    1230: 'HAKAdan_room_10',
    1231: 'HAKAdan_room_11',
    1232: 'HAKAdan_room_12',
    1233: 'HAKAdan_room_13',
    1234: 'HAKAdan_room_14',
    1235: 'HAKAdan_room_15',
    1236: 'HAKAdan_room_16',
    1237: 'HAKAdan_room_17',
    1238: 'HAKAdan_room_18',
    1239: 'HAKAdan_room_19',
    1240: 'HAKAdan_room_20',
    1241: 'HAKAdan_room_21',
    1242: 'HAKAdan_room_22',
    1243: 'moribossroom_scene',
    1244: 'moribossroom_room_0',
    1245: 'moribossroom_room_1',
    1246: 'syatekijyou_scene',
    1247: 'syatekijyou_room_0',
    1248: 'men_scene',
    1249: 'men_room_0',
    1250: 'men_room_1',
    1251: 'men_room_2',
    1252: 'men_room_3',
    1253: 'men_room_4',
    1254: 'men_room_5',
    1255: 'men_room_6',
    1256: 'men_room_7',
    1257: 'men_room_8',
    1258: 'men_room_9',
    1259: 'men_room_10',
    1260: 'shop1_scene',
    1261: 'shop1_room_0',
    1262: 'hairal_niwa_scene',
    1263: 'hairal_niwa_room_0',
    1264: 'ganon_tou_scene',
    1265: 'ganon_tou_room_0',
    1266: 'market_alley_scene',
    1267: 'market_alley_room_0',
    1268: 'spot20_scene',
    1269: 'spot20_room_0',
    1270: 'market_ruins_scene',
    1271: 'market_ruins_room_0',
    1272: 'entra_n_scene',
    1273: 'entra_n_room_0',
    1274: 'enrui_scene',
    1275: 'enrui_room_0',
    1276: 'market_alley_n_scene',
    1277: 'market_alley_n_room_0',
    1278: 'hiral_demo_scene',
    1279: 'hiral_demo_room_0',
    1280: 'kokiri_home3_scene',
    1281: 'kokiri_home3_room_0',
    1282: 'jyasinzou_scene',
    1283: 'jyasinzou_room_0',
    1284: 'jyasinzou_room_1',
    1285: 'jyasinzou_room_2',
    1286: 'jyasinzou_room_3',
    1287: 'jyasinzou_room_4',
    1288: 'jyasinzou_room_5',
    1289: 'jyasinzou_room_6',
    1290: 'jyasinzou_room_7',
    1291: 'jyasinzou_room_8',
    1292: 'jyasinzou_room_9',
    1293: 'jyasinzou_room_10',
    1294: 'jyasinzou_room_11',
    1295: 'jyasinzou_room_12',
    1296: 'jyasinzou_room_13',
    1297: 'jyasinzou_room_14',
    1298: 'jyasinzou_room_15',
    1299: 'jyasinzou_room_16',
    1300: 'jyasinzou_room_17',
    1301: 'jyasinzou_room_18',
    1302: 'jyasinzou_room_19',
    1303: 'jyasinzou_room_20',
    1304: 'jyasinzou_room_21',
    1305: 'jyasinzou_room_22',
    1306: 'jyasinzou_room_23',
    1307: 'jyasinzou_room_24',
    1308: 'jyasinzou_room_25',
    1309: 'jyasinzou_room_26',
    1310: 'jyasinzou_room_27',
    1311: 'jyasinzou_room_28',
    1312: 'ice_doukutu_scene',
    1313: 'ice_doukutu_room_0',
    1314: 'ice_doukutu_room_1',
    1315: 'ice_doukutu_room_2',
    1316: 'ice_doukutu_room_3',
    1317: 'ice_doukutu_room_4',
    1318: 'ice_doukutu_room_5',
    1319: 'ice_doukutu_room_6',
    1320: 'ice_doukutu_room_7',
    1321: 'ice_doukutu_room_8',
    1322: 'ice_doukutu_room_9',
    1323: 'ice_doukutu_room_10',
    1324: 'ice_doukutu_room_11',
    1325: 'malon_stable_scene',
    1326: 'malon_stable_room_0',
    1327: 'kakariko_scene',
    1328: 'kakariko_room_0',
    1329: 'bdan_boss_scene',
    1330: 'bdan_boss_room_0',
    1331: 'bdan_boss_room_1',
    1332: 'FIRE_bs_scene',
    1333: 'FIRE_bs_room_0',
    1334: 'FIRE_bs_room_1',
    1335: 'hut_scene',
    1336: 'hut_room_0',
    1337: 'daiyousei_izumi_scene',
    1338: 'daiyousei_izumi_room_0',
    1339: 'hakaana_scene',
    1340: 'hakaana_room_0',
    1341: 'yousei_izumi_tate_scene',
    1342: 'yousei_izumi_tate_room_0',
    1343: 'yousei_izumi_yoko_scene',
    1344: 'yousei_izumi_yoko_room_0',
    1345: 'golon_scene',
    1346: 'golon_room_0',
    1347: 'zoora_scene',
    1348: 'zoora_room_0',
    1349: 'drag_scene',
    1350: 'drag_room_0',
    1351: 'alley_shop_scene',
    1352: 'alley_shop_room_0',
    1353: 'night_shop_scene',
    1354: 'night_shop_room_0',
    1355: 'impa_scene',
    1356: 'impa_room_0',
    1357: 'labo_scene',
    1358: 'labo_room_0',
    1359: 'tent_scene',
    1360: 'tent_room_0',
    1361: 'nakaniwa_scene',
    1362: 'nakaniwa_room_0',
    1363: 'ddan_boss_scene',
    1364: 'ddan_boss_room_0',
    1365: 'ddan_boss_room_1',
    1366: 'ydan_boss_scene',
    1367: 'ydan_boss_room_0',
    1368: 'ydan_boss_room_1',
    1369: 'HAKAdan_bs_scene',
    1370: 'HAKAdan_bs_room_0',
    1371: 'HAKAdan_bs_room_1',
    1372: 'MIZUsin_bs_scene',
    1373: 'MIZUsin_bs_room_0',
    1374: 'MIZUsin_bs_room_1',
    1375: 'ganon_scene',
    1376: 'ganon_room_0',
    1377: 'ganon_room_1',
    1378: 'ganon_room_2',
    1379: 'ganon_room_3',
    1380: 'ganon_room_4',
    1381: 'ganon_room_5',
    1382: 'ganon_room_6',
    1383: 'ganon_room_7',
    1384: 'ganon_room_8',
    1385: 'ganon_room_9',
    1386: 'ganon_boss_scene',
    1387: 'ganon_boss_room_0',
    1388: 'jyasinboss_scene',
    1389: 'jyasinboss_room_0',
    1390: 'jyasinboss_room_1',
    1391: 'jyasinboss_room_2',
    1392: 'jyasinboss_room_3',
    1393: 'kokiri_home4_scene',
    1394: 'kokiri_home4_room_0',
    1395: 'kokiri_home5_scene',
    1396: 'kokiri_home5_room_0',
    1397: 'ganon_final_scene',
    1398: 'ganon_final_room_0',
    1399: 'kakariko3_scene',
    1400: 'kakariko3_room_0',
    1401: 'hakasitarelay_scene',
    1402: 'hakasitarelay_room_0',
    1403: 'hakasitarelay_room_1',
    1404: 'hakasitarelay_room_2',
    1405: 'hakasitarelay_room_3',
    1406: 'hakasitarelay_room_4',
    1407: 'hakasitarelay_room_5',
    1408: 'hakasitarelay_room_6',
    1409: 'shrine_scene',
    1410: 'shrine_room_0',
    1411: 'turibori_scene',
    1412: 'turibori_room_0',
    1413: 'shrine_n_scene',
    1414: 'shrine_n_room_0',
    1415: 'shrine_r_scene',
    1416: 'shrine_r_room_0',
    1417: 'ganontika_scene',
    1418: 'ganontika_room_0',
    1419: 'ganontika_room_1',
    1420: 'ganontika_room_2',
    1421: 'ganontika_room_3',
    1422: 'ganontika_room_4',
    1423: 'ganontika_room_5',
    1424: 'ganontika_room_6',
    1425: 'ganontika_room_7',
    1426: 'ganontika_room_8',
    1427: 'ganontika_room_9',
    1428: 'ganontika_room_10',
    1429: 'ganontika_room_11',
    1430: 'ganontika_room_12',
    1431: 'ganontika_room_13',
    1432: 'ganontika_room_14',
    1433: 'ganontika_room_15',
    1434: 'ganontika_room_16',
    1435: 'ganontika_room_17',
    1436: 'ganontika_room_18',
    1437: 'ganontika_room_19',
    1438: 'hakaana2_scene',
    1439: 'hakaana2_room_0',
    1440: 'gerudoway_scene',
    1441: 'gerudoway_room_0',
    1442: 'gerudoway_room_1',
    1443: 'gerudoway_room_2',
    1444: 'gerudoway_room_3',
    1445: 'gerudoway_room_4',
    1446: 'gerudoway_room_5',
    1447: 'HAKAdanCH_scene',
    1448: 'HAKAdanCH_room_0',
    1449: 'HAKAdanCH_room_1',
    1450: 'HAKAdanCH_room_2',
    1451: 'HAKAdanCH_room_3',
    1452: 'HAKAdanCH_room_4',
    1453: 'HAKAdanCH_room_5',
    1454: 'HAKAdanCH_room_6',
    1455: 'hairal_niwa_n_scene',
    1456: 'hairal_niwa_n_room_0',
    1457: 'bowling_scene',
    1458: 'bowling_room_0',
    1459: 'hakaana_ouke_scene',
    1460: 'hakaana_ouke_room_0',
    1461: 'hakaana_ouke_room_1',
    1462: 'hakaana_ouke_room_2',
    1463: 'hylia_labo_scene',
    1464: 'hylia_labo_room_0',
    1465: 'souko_scene',
    1466: 'souko_room_0',
    1467: 'souko_room_1',
    1468: 'souko_room_2',
    1469: 'miharigoya_scene',
    1470: 'miharigoya_room_0',
    1471: 'mahouya_scene',
    1472: 'mahouya_room_0',
    1473: 'takaraya_scene',
    1474: 'takaraya_room_0',
    1475: 'takaraya_room_1',
    1476: 'takaraya_room_2',
    1477: 'takaraya_room_3',
    1478: 'takaraya_room_4',
    1479: 'takaraya_room_5',
    1480: 'takaraya_room_6',
    1481: 'ganon_sonogo_scene',
    1482: 'ganon_sonogo_room_0',
    1483: 'ganon_sonogo_room_1',
    1484: 'ganon_sonogo_room_2',
    1485: 'ganon_sonogo_room_3',
    1486: 'ganon_sonogo_room_4',
    1487: 'ganon_demo_scene',
    1488: 'ganon_demo_room_0',
    1489: 'face_shop_scene',
    1490: 'face_shop_room_0',
    1491: 'kinsuta_scene',
    1492: 'kinsuta_room_0',
    1493: 'ganontikasonogo_scene',
    1494: 'ganontikasonogo_room_0',
    1495: 'ganontikasonogo_room_1',
    1496: 'NEW_payload',
    1497: 'NEW_extended_objects',
    1498: 'NEW_extended_textures',
    1499: 'NEW_bazaar_copy'
}