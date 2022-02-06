def calculate_drop_flag(flag):
    lower = flag & 0x003F
    upper = flag & 0x0FC0
    dropFlag = ((upper >> 5) & 0xFE) + 1

    print("Lower: " + hex(lower))
    print("Upper: " + hex(upper))
    print("Drop Flag: " + hex(dropFlag))

    rebuilt = ((dropFlag & 0xFE) << 5) + lower
    print("Rebuilt: " + hex(rebuilt))

calculate_drop_flag(0x50)