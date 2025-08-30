# Tool for handling symbol tables from objdump

class Symbol:
    def __init__(self, section: str, symbol_name: str, address: int):
        self.section = section
        self.symbol_name = symbol_name
        self.address = address
    
    def __str__(self):
        return f"{self.symbol_name} @ {hex(self.address)}"

class SymbolTable:
    def __init__(self):
        self.sections: dict[str, list[Symbol]] = {}
    
    def __getitem__(self, key):
        for section in self.sections:
            for sym in self.sections[section]:
                if sym.symbol_name == key:
                    return sym
        return None

    # Builds a new symbol table with address = target - self
    # for every symbol that exists in both
    def diff_tables(self, target: 'SymbolTable'):
        diff_table: SymbolTable = SymbolTable()
        for section in self.sections:
            
            # Make sure this section exists in the target table
            for sym in self.sections[section]:
                target_sym = target[sym.symbol_name]
                # See if this symbol exists in the target table
                if target_sym:
                    # Make a new symbol with the same name, addr = target address - self address
                    diff_sym = Symbol(sym.section, sym.symbol_name, target_sym.address - sym.address)
                    # Add it to the new table
                    if diff_sym.section not in diff_table.sections:
                        diff_table.sections[diff_sym.section] = []
                    diff_table.sections[diff_sym.section].append(diff_sym)
        return diff_table

    def read_symbol_table(file: str):
        table: SymbolTable = SymbolTable()
        with open(file) as f:
            for line in f:
                parts = line.split()
                if len(parts) != 6:
                    continue

                addr_str, _1, _2, section, len_str, sym_name = parts
                if section not in table.sections.keys():
                    table.sections[section] = []
                sym: Symbol = Symbol(section, sym_name, int(addr_str, 16))
                table.sections[section].append(sym)
        return table

ntsc1_0_table: SymbolTable = SymbolTable.read_symbol_table("bin/tools/object_fish_ntsc1.0.sym")
karp_table: SymbolTable = SymbolTable.read_symbol_table("bin/tools/object_fish_karp.sym")

karp_diff = ntsc1_0_table.diff_tables(karp_table)

for section in karp_diff.sections:
    print(section)
    for symbol in karp_diff.sections[section]:
        print(f'"{symbol.symbol_name}": {hex(symbol.address)},')
print(karp_diff)