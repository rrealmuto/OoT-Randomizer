import binascii

class RelocParser:
    def __init__(self, ovl_bytes, vram_addr):
        self.luiaddiu_last: int = 0xFFFFFFFF

        self.ovl_bytes = ovl_bytes
        self.vram_addr = vram_addr

        # Read table offset
        table_offset = int.from_bytes(ovl_bytes[-4:], 'big')
        print(table_offset)

        # Read table

        table = ovl_bytes[-1*table_offset : -1*table_offset + 20]
        text_len = int.from_bytes(table[0:4], 'big')
        data_len = int.from_bytes(table[4:8], 'big')
        rodata_len = int.from_bytes(table[8:12], 'big')
        bss_len = int.from_bytes(table[12:16], 'big')
        self.reloc_count = int.from_bytes(table[16:20], 'big')

        self.sections_offsets = {
            1: 0, # .text
            2: text_len, # .data
            3: text_len + data_len, # .rodata
            4: text_len + data_len + rodata_len, # .bss
        }

        self.sections = {
            1: (vram_addr, text_len), # .text
            2: (vram_addr + text_len, data_len), # .data
            3: (vram_addr + text_len + data_len, rodata_len), # .rodata
            4: (vram_addr + text_len + data_len + rodata_len, bss_len), # .bss
        }

        self.relocs = {
            1: [],
            2: [],
            3: [],
            4: []
        }

        self.reloc_start = len(ovl_bytes) - table_offset + 20


        print(f"Text: {text_len}\nData: {data_len}\nrodata: {rodata_len}\nbss: {bss_len}\nreloc_count: {self.reloc_count}\n")
        print(f"Reloc table start: {self.reloc_start}")


    # 32-bit pointer
    def process_reloc_type_R_MIPS_32(self, ovl_bytes: bytearray, section_id: int, reloc_type: int, offset: int):
        section_offset = self.sections_offsets[section_id] + offset
        target = int.from_bytes(ovl_bytes[section_offset:section_offset+4], 'big')
        print(hex(target))
        self.relocs[section_id].append((reloc_type, offset, target))

    # jump target
    def process_reloc_type_R_MIPS_26(self, ovl_bytes: bytearray, section_id: int, reloc_type: int, offset: int):
        section_offset = self.sections_offsets[section_id] + offset
        
        # Read the 26 bit value at the offset
        target = int.from_bytes(ovl_bytes[section_offset:section_offset+4], 'big')
        target = 0x80000000 | ((target & ((1 << 26) - 1)) << 2)

        self.relocs[section_id].append((reloc_type, offset, target))
        print(hex(target))

    # lui pair
    def process_reloc_type_luiaddiu(self, ovl_bytes: bytearray, section_id: int, reloc_type: int, offset: int):
        section_offset = self.sections_offsets[section_id] + offset
        
        if reloc_type == 5: # Hi part of the hi/lo pair
            # Read the high 2 bytes from offset
            self.luiaddiu_last = int.from_bytes(ovl_bytes[section_offset + 2:section_offset+4], 'big') << 16
        elif reloc_type == 6: # Low part of the hi/lo pair
            if self.luiaddiu_last == 0xFFFFFFFF:
                raise Exception("LO reloc without HI")
            # Read the low 2 bytes
            self.luiaddiu_last |= int.from_bytes(ovl_bytes[section_offset+2:section_offset+4], 'big')
            self.relocs[section_id].append((reloc_type, offset, self.luiaddiu_last))
            print(hex(self.luiaddiu_last))
            self.luiaddiu_last = 0xFFFFFFFF

    reloc_process_map = {
        2: process_reloc_type_R_MIPS_32,
        4: process_reloc_type_R_MIPS_26,
        5: process_reloc_type_luiaddiu,
        6: process_reloc_type_luiaddiu
    }

    def process_reloc(self, section_id, reloc_type, offset):
        self.reloc_process_map[reloc_type](self, self.ovl_bytes, section_id, reloc_type, offset)
    
    def process_relocs(self):
        self.relocs = {
            1: [],
            2: [],
            3: [],
            4: []
        }
        # Read the reloc table
        for i in range(0, self.reloc_count):
            entry = self.ovl_bytes[self.reloc_start + 4 * i: self.reloc_start + 4 * i + 4]
            entry = int.from_bytes(entry, 'big')
            section_id = (entry & 0xC0000000) >> 30
            reloc_type = (entry & 0x3F000000) >> 24
            offset = (entry & 0x00FFFFFF)
            print(f"{i}: {hex(entry)} section_id={section_id}, reloc_type={reloc_type}, offset={hex(offset)}")
            self.process_reloc(section_id, reloc_type, offset)
        
        print(self.relocs)
        # For each reloc we want to:
        # 1) Create a symbol at the relocated address in the ASM
        # 2) Replace the ASM at the reloc offset to reference the symbol instead of the hardcoded address

f = open("ASM/disasm/en_skjneedle/en_skjneedle.o", 'rb')
vram_addr = 0x80A703D0
ovl_bytes = f.read()


parser = RelocParser(ovl_bytes, vram_addr)
parser.process_relocs()