codes = [0xFC176C6035D8ED76, 0xFC11FFFFFFFFF638, 0xF590000007014050, 0xF588080000014050, 0xF3000000071FF200, 0xF20000000007C07C ]

def number_of_trailing_zeros(n):
    # Convert the integer to binary representation as a string
    binary_n = bin(n)
    
    # Count the number of trailing zeros in the binary string
    count = 0
    for i in range(len(binary_n) - 1, 1, -1):
        if binary_n[i] == '0':
            count += 1
        else:
            break
    return count

opcode_fields = {
    0xF5: {
        "name": "gsDPSetTile", 
        "fields": [
            ("fmt",     0x00E0000000000000),
            ("siz",     0x0018000000000000),
            ("line",    0x0003FE0000000000),
            ("tmem",    0x000001FF00000000),
            ("tile",    0x0000000007000000),
            ("palette", 0x0000000000F00000),
            ("cmT",     0x00000000000C0000),
            ("maskT",   0x000000000003C000),
            ("shiftT",  0x0000000000003C00),
            ("cmS",     0x0000000000000300),
            ("maskS",   0x00000000000000F0),
            ("shiftS",  0x000000000000000F)
        ]
    },
    0xF3: {
        "name": "gsDPLoadBlock",
        "fields": [ 
            ("uls",     0x00FFF00000000000),
            ("ult",     0x00000FFF00000000),
            ("tile",    0x000000000F000000),
            ("texels",  0x0000000000FFF000),
            ("dxt",     0x0000000000000FFF)
        ]
    },
    0xF2: {
        "name": "gsDPSetTileSize",
        "fields": [
            ("uls",     0x00FFF00000000000),
            ("ult",     0x00000FFF00000000),
            ("tile",    0x000000000F000000),
            ("lrs",     0x0000000000FFF000),
            ("lrt",     0x0000000000000FFF)
        ]
    },
    0xFC: {
        "name": "gsDPSetCombineLERP",
        "fields" : [
            ("a0",      0x00F0000000000000),
            ("b0",      0x00000000F0000000),
            ("c0",      0x000F800000000000),
            ("d0",      0x0000000000038000),
            ("Aa0",     0x0000700000000000),
            ("Ab0",     0x0000000000007000),
            ("Ac0",     0x00000E0000000000),
            ("Ad0",     0x0000000000000E00),
            ("a1",      0x000001E000000000),
            ("b1",      0x000000000F000000),
            ("c1",      0x0000001F00000000),
            ("d1",      0x00000000000001C0),
            ("Aa1",     0x0000000000E00000),
            ("Ab1",     0x0000000000000038),
            ("Ac1",     0x00000000001C0000),
            ("Ad1",     0x0000000000000007)
        ]
    }
}

for code in codes:
    print(hex(code))
    opcode = (code & 0xFF00000000000000) >> 56
    print(opcode_fields[opcode]["name"])
    for field, mask in opcode_fields[opcode]["fields"]:
        print(field + ": " + str((code & mask) >> number_of_trailing_zeros(mask)))