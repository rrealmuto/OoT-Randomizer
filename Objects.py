from Rom import Rom

class ObjectTableEntry:
    def __init__(self, vrom_start: int, vrom_end: int):
        self.vrom_start: int = vrom_start
        self.vrom_end: int = vrom_end
    
    @property
    def size(self):
        return self.vrom_end - self.vrom_start
    
    def decode(bytes: bytearray):
        vrom_start = int.from_bytes(bytes[0:4], 'big')
        vrom_end = int.from_bytes(bytes[4:8], 'big')
        return ObjectTableEntry(vrom_start, vrom_end)

class ObjectTable:
    def __init__(self, rom: Rom, start_address: int):
        self.numObjects = rom.read_int32(start_address)
        start_address += 4
        self.objects: list[ObjectTableEntry] = []
        for i in range(0, self.numObjects):
            entry_bytes = rom.read_bytes(start_address + 8*i, 8)
            self.objects.append(ObjectTableEntry.decode(entry_bytes))
