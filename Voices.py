from Audiobank import *
from Rom import Rom

if __name__ == "__main__":
    rom = Rom("ZOOTDEC.z64")

    AUDIOBANK_INDEX_ADDR = 0x00B896A0
    AUDIOBANK_FILE_ADDR = 0xD390
    AUDIOBANK_FILE_LENGTH = 0x1CA50
    AUDIOTABLE_FILE_ADDR = 0x79470
    AUDIOTABLE_FILE_LENGTH = 0x460AD0
    AUDIOTABLE_INDEX_ADDR = 0x00B8A1C0

    
    bank_index_header: bytearray = rom.read_bytes(AUDIOBANK_INDEX_ADDR, 0x10)
    bank_index_length = int.from_bytes(bank_index_header[0:2], 'big')

    # Read Audiobank file
    audiobank = rom.read_bytes(AUDIOBANK_FILE_ADDR, AUDIOBANK_FILE_LENGTH)    

    # Read Audiotable file
    audiotable = rom.read_bytes(AUDIOTABLE_FILE_ADDR, AUDIOTABLE_FILE_LENGTH)

    # Read Audiotable index
    audiotable_index_header: bytearray = rom.read_bytes(AUDIOTABLE_INDEX_ADDR, 0x10)
    audiotable_index_length = int.from_bytes(audiotable_index_header[0:2], 'big')
    audiotable_index = rom.read_bytes(AUDIOTABLE_INDEX_ADDR, 0x10*audiotable_index_length + 0x10)


    # Read bank entry 0 from audiobank pointer table
    banks = []
    for i in range(0, bank_index_length):
        bank_entry = rom.read_bytes(AUDIOBANK_INDEX_ADDR + 0x10 + i*0x10, 0x10)    
        bank = AudioBank(bank_entry, audiobank, audiotable, audiotable_index)
        banks.append(bank)

    print("hi")