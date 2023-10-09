from enum import Enum
from Audiobank import *
from Rom import Rom
from Utils import data_path
import os


AUDIOBANK_INDEX_ADDR = 0x00B896A0
AUDIOBANK_FILE_ADDR = 0xD390
AUDIOBANK_FILE_LENGTH = 0x1CA50
AUDIOTABLE_FILE_ADDR = 0x79470
AUDIOTABLE_FILE_LENGTH = 0x460AD0
AUDIOTABLE_INDEX_ADDR = 0x00B8A1C0

child_link_sfx = [
    ("Child Link - Dying Gasp", 20),
    ("Child Link - Attack 1", 28),
    ("Child Link - Attack 2", 29),
    ("Child Link - Attack 3", 30),
    ("Child Link - Attack 4", 31),
    ("Child Link - Strong Attack 1", 32),
    ("Child Link - Strong Attack 2", 33),
    ("Child Link - Dangling Gasp 1", 34),
    ("Child Link - Sigh 1", 35),
    ("Child Link - Sigh 2", 36),
    ("Child Link - Hurt 1", 37),
    ("Child Link - Hurt 2", 38),
    ("Child Link - Hurt 3", 39),
    ("Child Link - Hurt 4", 40),
    ("Child Link - Knocked Back", 41),
    ("Child Link - Hurt 5", 42),
    ("Child Link - Falling 1", 43),
    ("Child Link - Falling 2", 44),
    ("Child Link - Gasp 1", 45),
    ("Child Link - Gasp 2", 46),
    ("Child Link - Wheeze", 47),
    ("Child Link - Exhausted Panting", 48),
    ("Child Link - Painful Landing", 49),
    ("Child Link - Danging Gasp 2", 50),
    ("Child Link - Glug", 51),
    ("Child Link - Refreshed", 52),
    ("Child Link - Hup 1", 53),
    ("Child Link - Hup 2", 54),
    ("Child Link - Charge Up", 62),
    ("Child Link - Cast Spell", 63),
    ("Child Link - Dying Gasp", 64),
    ("Child Link - Strangled", 65),
    ("Child Link - Gasp 3", 66),
    ("Child Link - Shiver", 67),
    ("Child Link - Sneeze", 69),
    ("Child Link - Grunt", 70),
    ("Child Link - Moans From Heat", 71),
    ("Child Link - Awakening Grunt", 72),
    ("Child Link - Wiping Off Sweat", 73),
    ("Child Link - Yawn Starts", 74),
    ("Child Link - Yawn and Stretch", 75),
    ("Child Link - Stretch Refreshingly", 76),
    ("Child Link - Dramatic Gasp", 135)
]

adult_link_sfx = [
    ("Adult Link - Attack 1", 0),
    ("Adult Link - Attack 2", 1),
    ("Adult Link - Attack 3", 2),
    ("Adult Link - Attack 4", 3),
    ("Adult Link - Strong Attack 1", 4),
    ("Adult Link - Strong Attack 2", 5),
    ("Adult Link - Dangling Grunt", 6),
    ("Adult Link - Climb Edge", 7),
    ("Adult Link - Dangling Gasp 1", 8),
    ("Adult Link - Hurt 1", 9),
    ("Adult Link - Hurt 2", 10),
    ("Adult Link - Hurt 3", 11),
    ("Adult Link - Hurt 4", 12),
    ("Adult Link - Knocked Back", 13),
    ("Adult Link - Hurt 5", 14),
    ("Adult Link - Falling 1", 15),
    ("Adult Link - Falling 2", 16),
    ("Adult Link - Gasp 1", 17),
    ("Adult Link - Gasp 2", 18),
    ("Adult Link - Pant 1", 19),
    ("Adult Link - Spur Horse 1", 21),
    ("Adult Link - Spur Horse 2", 22),
    ("Adult Link - Pant 2", 23),
    ("Adult Link - Painful Landing", 24),
    ("Adult Link - Dangling Gasp 2", 25),
    ("Adult Link - Hup", 26),
    ("Adult Link - Gasp 3", 27),
    ("Adult Link - Glug", 55),
    ("Adult Link - Refreshed", 56),
    ("Adult Link - Lift", 60),
    ("Adult Link - Cast Spell", 61),
    ("Adult Link - Hurt 6", 77),
    ("Adult Link - Choking", 78),
    ("Adult Link - Gasping", 79),
    ("Adult Link - Small Gasp", 80),
    ("Adult Link - Unsettled Moan", 81),
    ("Adult Link - Sneeze", 82),
    ("Adult Link - Sigh 1", 83),
    ("Adult Link - Sigh 2", 84),
    ("Adult Link - Sigh 3", 85),
    ("Adult Link - Stretch Start", 86),
    ("Adult Link - Stretching", 87),
    ("Adult Link - Finished Stretching", 88),
    ("Adult Link - Dramatic Gasp", 134)
]

class VOICE_PACK_AGE(Enum):
    CHILD = 0
    ADULT = 1

def patch_voice_pack(rom: Rom, voice_pack: str, age: VOICE_PACK_AGE):
    # Build voice pack path
    voice_pack_dir = os.path.join(data_path(), "Voices", "Child" if age == VOICE_PACK_AGE.CHILD else "Adult", voice_pack)

    sfx_list: list[tuple[str,int]] = child_link_sfx if age == VOICE_PACK_AGE.CHILD else adult_link_sfx

    # List all files in the directory
    files : list[str] = os.listdir(voice_pack_dir)
    for file in files:
        filename = os.path.basename(file)
        filename_without_ext = filename.split('.')[0]
        for sfx_name, sfx_id in sfx_list:
            if filename_without_ext == sfx_name:
                print(filename_without_ext)

if __name__ == "__main__":
    rom = Rom("ZOOTDEC.z64")

    patch_voice_pack(rom, "Test", VOICE_PACK_AGE.ADULT)

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
