import sys
import argparse
import hashlib
import binascii
import zipfile

from Audiobank import *
from Rom import *
from Cosmetics import *
from Music import bgm_sequence_ids as seq_map, fanfare_sequence_ids, process_sequences
from Utils import *

bgm_sequence_ids = [
    0x02, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2C, 0x2D,
    0x2E, 0x2F, 0x30, 0x38, 0x3A, 0x3C, 0x3E, 0x3F, 0x40, 0x42, 0x4A, 0x4B, 0x4C, 0x4E, 0x4F, 0x50,
    0x55, 0x56, 0x58, 0x5A, 0x5B, 0x5C, 0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x6B, 0x6C
]

def read_sequence_names_from_rom(rom: Rom, cosmetic_context_addr, patch_set):
    song_name_addr = cosmetic_context_addr + patch_set["symbols"]['CFG_SONG_NAMES']
    # Read the song names
    # Each is 50 characters
    names = {}
    bgm_map = {bgm[1]: bgm[0] for bgm in seq_map}
    for i in range(0, len(bgm_sequence_ids)):
        names[bgm_map[bgm_sequence_ids[i]]] = (bgm_sequence_ids[i], rom.read_bytes(song_name_addr + 50 * i, 50).decode())
    return names

def read_seqbank_table_from_rom(rom: Rom, seqbank_base_addr: int):
    seqbanks = [0]*0x6E
    for i in range(0x6E):
        seqbanks[i] = rom.read_byte(seqbank_base_addr + 2*i)
    return seqbanks

def read_seq_table(rom: Rom, sequence_index_addr: int) -> list[AudioSequence_TableEntry]:
    num_entries = rom.read_int16(sequence_index_addr)
    sequence_table: list[AudioSequence_TableEntry] = []
    for i in range(0, num_entries):
        seq_entry_bytes = rom.read_bytes(sequence_index_addr + (i + 1)*0x10, 0x10)
        sequence_table.append(AudioSequence_TableEntry(seq_entry_bytes))
    return sequence_table

if __name__ == '__main__':
    parser = argparse.ArgumentParser("audiobank")
    parser.add_argument("input")
    args = parser.parse_args()
    rom_file = args.input
    rom = Rom(rom_file, verify_crc=False)
    rom_dir, rom_name = os.path.split(rom_file)

    #rom.dump_file_system(os.path.join(rom_dir, rom_name + "_dump"))

    # Extract the audiobin from the decompressed ROM
    Audiobank_dma_entry = DMAEntry(rom, 0x03)
    Audioseq_dma_entry = DMAEntry(rom, 0x04)
    Audiotable_dma_entry = DMAEntry(rom, 0x05)
    Audiobank = Audiobank_dma_entry.file_bytes()
    Audioseq = Audioseq_dma_entry.file_bytes()
    Audiotable = Audiotable_dma_entry.file_bytes()
    
    # try to detect the cosmetic patch data format
    cosmetic_version = None
    versioned_patch_set = None
    cosmetic_context = rom.read_int32(rom.sym('RANDO_CONTEXT') + 4)
    if 0x80000000 <= cosmetic_context <= 0x80F7FFFC:
        cosmetic_context = (cosmetic_context - 0x80400000) + 0x3480000 # convert from RAM to ROM address
        cosmetic_version = rom.read_int32(cosmetic_context)
        versioned_patch_set = patch_sets.get(cosmetic_version)
    else:
        # If cosmetic_context is not a valid pointer, then try to
        # search over all possible legacy header locations.
        for header in legacy_cosmetic_data_headers:
            cosmetic_context = header
            cosmetic_version = rom.read_int32(cosmetic_context)
            if cosmetic_version in patch_sets:
                versioned_patch_set = patch_sets[cosmetic_version]
                break

    audiobank_table_addr = rom.read_int32(cosmetic_context + versioned_patch_set["symbols"]['CFG_AUDIOBANK_TABLE_EXTENDED_ADDR'])
    audiobank_table_addr = (audiobank_table_addr - 0x80400000) + 0x3480000
    
    song_names = read_sequence_names_from_rom(rom, cosmetic_context, versioned_patch_set)

    # Read the audiobank index table
    # Read the first 16 bits to get the number of entries
    num_audiobanks = rom.read_int16(audiobank_table_addr)
    audiobank_table = rom.read_bytes(audiobank_table_addr, (num_audiobanks + 1) * 0x10)
    
    # Read the audiotable index
    AUDIOTABLE_INDEX_ADDR = 0xB8A1C0
    audiotable_index = rom.read_bytes(AUDIOTABLE_INDEX_ADDR, 0x80) # Read audiotable index into bytearray

    # Read the sequence-bank table
    seq_bank_base = 0xB89911 + 0xDD
    seqbank = read_seqbank_table_from_rom(rom, seq_bank_base)

    # Read the sequence table
    SEQUENCE_TABLE_ADDR = 0x00B89AD0
    rom_sequence_table = read_seq_table(rom, SEQUENCE_TABLE_ADDR)
    i = 0

    # Read the sequence data
    for sequence in rom_sequence_table:
        sequence.data = Audioseq[sequence.addr:sequence.addr + sequence.size]
        # Hash the data to compare against our music directory to find unknown sequences
        sequence.hash = hashlib.sha1(sequence.data).digest()
    
    # Import custom music sequences

    bgm_ids = {bgm[0]: bgm for bgm in seq_map}
    ff_ids = {bgm[0]: bgm for bgm in fanfare_sequence_ids}
    fanfare_seqids = [ff[1] for ff in fanfare_sequence_ids]
    bgm_sequences, bgm_target_sequences, bgm_groups = process_sequences(rom, bgm_ids.values(), 'bgm', disabled_source_sequences=None, disabled_target_sequences=None, include_custom_audiobanks=True, log=None)
    ff_sequences, ff_target_sequences, ff_groups = process_sequences(rom, ff_ids.values(), 'fanfare', disabled_source_sequences=None, disabled_target_sequences=None, include_custom_audiobanks=True, log=None)

    # Filter custom sequences
    bgm_sequences = {seq: bgm_sequences[seq] for seq in bgm_sequences if bgm_sequences[seq].vanilla_id == -1}
    ff_sequences = {seq: ff_sequences[seq] for seq in ff_sequences if ff_sequences[seq].vanilla_id == -1}
    
    bgm_sequences.update(ff_sequences)
    # Attach data to the custom sequences
    for seq in bgm_sequences.values():
        if seq.name.endswith('.zseq'):
            with open(seq.name, 'rb') as stream:
                seq.data = bytearray(stream.read())
                seq.size = len(seq.data)
        else:
            with zipfile.ZipFile(seq.name) as zip:
                with zip.open(seq.seq_file, 'r') as stream:
                    seq.data = bytearray(stream.read())
                    seq.size = len(seq.data)        
        # Align sequences to 0x10
        if seq.size % 0x10 != 0:
            seq.data.extend(bytearray(0x10 - (seq.size % 0x10)))
            seq.size += 0x10 - (seq.size % 0x10)
        seq.hash = hashlib.sha1(seq.data).digest()

    # Cross reference ROM sequence data with custom music
    i = 0
    bgm_map = {bgm[1]: bgm[0] for bgm in seq_map}
    ff_map = {ff[1]: ff[0] for ff in fanfare_sequence_ids}
    seq_spoiler = {}
    for rom_sequence in rom_sequence_table:
        rom_sequence.name = None
        for seq in bgm_sequences.values():
            if rom_sequence.hash == seq.hash:
                rom_sequence.name = seq.cosmetic_name
                break
        if rom_sequence.name == None:
            if i in bgm_sequence_ids:
                print(f"No match found for BGM sequence {i}. Should have been: {song_names[bgm_map[i]]}")
            elif i in fanfare_seqids:
                print(f"No match found for fanfare sequence {i}.")
            else:
                print(f"No match found for sequence {i}")
        else:
            if i in bgm_sequence_ids:
                print(f"Match found for BGM sequence {i} {bgm_map[i]}: {rom_sequence.name}")
                seq_spoiler[bgm_map[i]] = rom_sequence.name
            elif i in fanfare_seqids:
                print(f"Match found for FF sequence {i} {ff_map[i]}: {rom_sequence.name}")
                seq_spoiler[ff_map[i]] = rom_sequence.name
            else:
                print(f"Match found for sequence {i}: {rom_sequence.name}")
        i += 1

    print(json.dumps(seq_spoiler,indent=4))

    # Base file addresses
    print(f"Audiobank: {hex(Audiobank_dma_entry.start)}")
    print(f"Audioseq: {hex(Audioseq_dma_entry.start)}")
    print(f"Audiotable: {hex(Audiotable_dma_entry.start)}")

    audiobin = Audiobin(Audiobank, audiobank_table, Audiotable, audiotable_index)
    i = 0
    for bank in audiobin.audiobanks:
        print(f"--- Bank {i} - {hex(Audiobank_dma_entry.start + bank.bank_offset)} ---")
        for sample in bank.get_all_samples():
            parent = sample.parent
            this_type = None
            this_index = -1
            if type(parent) == Drum:
                this_type = "Drum"
                this_index = parent.drum_id
            elif type(parent) == SFX:
                this_type = "SFX"
                this_index = parent.sfx_id
            elif type(parent) == Instrument:
                this_type = "INST"
                this_index = parent.inst_id
            print(f"{this_type} {this_index} - {hex(sample.audiotable_addr)}")
        i += 1
    print("Hi")