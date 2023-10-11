from enum import Enum
from math import ceil
import sys
from Audiobank import *
from Rom import Rom
from Settings import Settings
from Utils import data_path
import os
import aifc
import numpy as np

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
    ("Adult Link - Attack 1", 0x00),
    ("Adult Link - Attack 2", 0x01),
    ("Adult Link - Attack 3", 0x02),
    ("Adult Link - Attack 4", 0x03),
    ("Adult Link - Strong Attack 1", 0x04),
    ("Adult Link - Strong Attack 2", 0x05),
    ("Adult Link - Dangling Grunt", 0x06),
    ("Adult Link - Climb Edge", 0x07),
    ("Adult Link - Dangling Gasp 1", 0x08),
    ("Adult Link - Hurt 1", 0x09),
    ("Adult Link - Hurt 2", 0x0A),
    ("Adult Link - Hurt 3", 0x0B),
    ("Adult Link - Hurt 4", 0x0C),
    ("Adult Link - Knocked Back", 0x0D),
    ("Adult Link - Hurt 5", 0x0E),
    ("Adult Link - Falling 1", 0x0F),
    ("Adult Link - Falling 2", 0x10),
    ("Adult Link - Gasp 1", 0x11),
    ("Adult Link - Gasp 2", 0x12),
    ("Adult Link - Pant 1", 0x13),
    ("Adult Link - Spur Horse 1", 0x15),
    ("Adult Link - Spur Horse 2", 0x16),
    ("Adult Link - Pant 2", 0x17),
    ("Adult Link - Painful Landing", 0x18),
    ("Adult Link - Dangling Gasp 2", 0x19),
    ("Adult Link - Hup", 0x1A),
    ("Adult Link - Gasp 3", 0x1B),
    ("Adult Link - Glug", 0x37),
    ("Adult Link - Refreshed", 0x38),
    ("Adult Link - Lift", 0x3C),
    ("Adult Link - Cast Spell", 0x3D),
    ("Adult Link - Hurt 6", 0x4D),
    ("Adult Link - Choking", 0x4E),
    ("Adult Link - Gasping", 0x4F),
    ("Adult Link - Small Gasp", 0x50),
    ("Adult Link - Unsettled Moan", 0x51),
    ("Adult Link - Sneeze", 0x52),
    ("Adult Link - Sigh 1", 0x53),
    ("Adult Link - Sigh 2", 0x54),
    ("Adult Link - Sigh 3", 0x55),
    ("Adult Link - Stretch Start", 0x56),
    ("Adult Link - Stretching", 0x57),
    ("Adult Link - Finished Stretching", 0x58),
    ("Adult Link - Dramatic Gasp", 0x86)
]

class VOICE_PACK_AGE(Enum):
    CHILD = 0
    ADULT = 1

def _patch_voice_pack(rom: Rom, age: VOICE_PACK_AGE, voice_pack: str):
    bank0 = rom.audiobanks[0]
    # Build voice pack path
    voice_pack_dir = os.path.join(data_path(), "Voices", "Child" if age == VOICE_PACK_AGE.CHILD else "Adult", voice_pack)

    sfx_list: list[tuple[str,int]] = child_link_sfx if age == VOICE_PACK_AGE.CHILD else adult_link_sfx

    # List all files in the directory
    files : list[str] = os.listdir(voice_pack_dir)

    sfxs = []
    for filename in files:
        if filename.startswith("00-"):
            # Old style, get bank and then sfx id
            substr = filename[0:7]
            split = substr.split("-")
            bank = split[0]
            sfxid = split[1]
            sfxs.append((os.path.join(voice_pack_dir,filename), filename, int(sfxid,16)))
        else:
            split = filename.split('.')
            filename_without_ext = split[0]
            ext = split[1]
            for sfx_name, sfx_id in sfx_list:
                if filename_without_ext == sfx_name and ext == "aifc":
                    sfxs.append((os.path.join(voice_pack_dir,filename), sfx_name, sfx_id))
                    print(filename_without_ext)

    sfx_data_start = len(rom.audiotable)

    # Patch each sfx that we have
    for sfx_to_patch, sfx_name, sfx_id in sfxs:
        # Open the .aifc file
        index = 0
        f = open(sfx_to_patch, 'rb')
        # Read data from the .aifc file
        
        # Read the "FORM" Chunk
        f.read(4) # "FORM"
        size = int.from_bytes(f.read(4), 'big')
        type = str(f.read(4), encoding='utf-8')
        if type != "AIFC":
            raise Exception("Not an AIFC file")

        # Read the rest of the chunks
        done = False
        chunks = {}
        chkID = "FORM"
        while chkID != '':
            chkID = str(f.read(4), encoding='utf-8')
            size = int.from_bytes(f.read(4), 'big')
            data = f.read(size)
            chunk = {
                'size': size,
                'data': data
            }
            chunks[chkID] = chunk
        
        # Process the chunks
        
        # COMM Chunk: Sampling rate, compression type, number of channels
        #define CommonID 'COMM' /* ckID for Common Chunk */
        # typedef struct {
        #   ID ckID; /*  'COMM'  */
        #   long kDataSize;
        #   short numChannels; /* # audio channels */
        #   unsigned long numSampleFrames; /* # sample frames = samples/channel */
        #   short sampleSize; /* # bits/sample */
        #   extended sampleRate; /* sample_frames/sec */
        #   ID compressionType; /* compression type ID code */
        #   pstring compressionName; /* human-readable compression type name */
        # } CommonChunk;

        comm = chunks['COMM']
        data = comm['data']
        numChannels = int.from_bytes(data[0:2], 'big')
        numSampleFrames = int.from_bytes(data[2:6], 'big')
        sampleSize = int.from_bytes(data[6:8], 'big')
        sampleRateBytes = bytearray(data[8:18])
        # Need to process the sample rate. it's an 80-bit extended floating point value stored in 10 bytes which nothing natively supports
        # We can probably use NumPy if we put it in the proper byte order and extend to 128-bit
        dtype = np.dtype(np.longdouble)
        if sys.byteorder == 'little':
            sampleRateBytes.reverse()
        sampleRateBytes = sampleRateBytes + bytearray(16 - len(sampleRateBytes)) # Pad sampleRateBytes to 128 bit
        sampleRate = np.frombuffer(sampleRateBytes, dtype=dtype)[0]
        compressionType = str(data[18:22],encoding='utf-8')
        compressionNameLen = data[22]
        compressionName = str(data[23:23 + compressionNameLen], encoding='utf-8')
        
        # Make sure it's the correct compression type
        if compressionType != "ADP9":
            raise Exception("Unknown compression format. Must be 'ADP9'. Did you use vadpcm_enc?")
    
        # Make sure it's the correct sample size
        if sampleSize != 16:
            raise Exception("Unsupported sample size. Must be 16 bit samples")

        # Compressed sample data
        # SSND Chunk contains the sample data
        ssnd = chunks['SSND']
        data = ssnd['data']
        ssndOffset = int.from_bytes(data[0:4], 'big')
        ssndBlockSize = int.from_bytes(data[4:8], 'big')
        
        if ssndOffset != 0 or ssndBlockSize != 0:
            raise Exception("Unsupported SSND offset/block size")
        # Read the sample data. it's numSampleFrames * 9 / 8 / 2
        dataLen = int(ceil(numSampleFrames * 9 / 8 / 2))
        soundData = data[8:8 + dataLen]
        
        # Calculate the tuning as sampling rate / 32000.
        tuning = sampleRate / 32000

        # Pad the data to 16 bytes
        soundData += bytearray((16 - (len(soundData)%16))%16)


        
        # Sort-of problem. We need to update audiotable in multiple different spots. 
        # So instead of making the new file, maybe just add a new variable to Rom called new_audiotable_data and write it all at the end.
        # Update sample address to point to new data in audiotable.
        sfx: SFX = bank0.SFX[sfx_id]

        # Compare sound data to existing to see if it fits
        if len(soundData) > sfx.sample.size:
            # Put the data in audiotable
            rom.audiotable += soundData
            sfx.sample.addr = sfx_data_start
            sfx_data_start += len(soundData)
        else:
            # Put the data in the existing location. Pad with 0s
            pad_len = sfx.sample.size - len(soundData)
            soundData += bytearray(pad_len)
            rom.audiotable[sfx.sample.addr:sfx.sample.addr + len(soundData)] = soundData

        # Update the sfx tuning
        sfx.sampleTuning = float(tuning)

        # Update loop end as numSampleFrames
        sfx.sample.loop.end = numSampleFrames
        # Update sample data length = length
        sfx.sample.size = dataLen
        sampleBytes = sfx.sample.get_bytes()
        sfxBytes = sfx.get_bytes()
        loopBytes = sfx.sample.loop.get_bytes()

        # Write the new sample into the bank
        bank0.bank_data[sfx.sampleOffset:sfx.sampleOffset+0x10] = sfx.sample.get_bytes()
        bank0.bank_data[sfx.sfx_offset:sfx.sfx_offset+0x08] = sfx.get_bytes()
        bank0.bank_data[sfx.sample.loop_addr:sfx.sample.loop_addr+len(loopBytes)] = loopBytes
        
if __name__ == "__main__":
    rom = Rom("ZOOTDEC.z64")

    _patch_voice_pack(rom, "Mario", VOICE_PACK_AGE.ADULT)

