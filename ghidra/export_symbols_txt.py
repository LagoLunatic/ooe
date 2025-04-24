#Exports symbol names from Ghidra to symbols.txt files for use with dsd.
#@author LagoLunatic
#@category #Decomp
#@keybinding 
#@menupath 
#@toolbar 

from __future__ import print_function

from ghidra_utils import *

from import_objdiff_mappings import import_objdiff_mappings

def export_ghidra_symbols_for_module(module):
    symbols_txt_path = get_symbols_txt_path_by_module(module)
    addr_space = get_addr_space_by_module(module)
    
    updated_symbols = []
    with open(symbols_txt_path, "r") as f:
        for line in f.readlines():
            sym_name, sym_kind, addr, ambiguous, local = read_symbols_txt_line(line, addr_space)
            
            syms = st.getSymbols(addr)
            if len(syms) > 0 and (len(syms) == 1 or not syms[0].name.startswith("caseD_")):
                assert len(syms) == 1, addr
                sym = syms[0]
                
                ghidra_sym_name = sym.getName()
                if sym.getSource() != SourceType.DEFAULT:
                    if sym_name != ghidra_sym_name and sym_name != "Entry":
                        print("Exporting:", sym_name, sym, sym.getSource())
                        sym_name = ghidra_sym_name
            
            updated_symbols.append((sym_name, sym_kind, addr, ambiguous, local))
    
    # print(len(updated_symbols))
    
    with open(symbols_txt_path, "w") as f:
        for sym_name, sym_kind, addr, ambiguous, local in updated_symbols:
            line = write_symbols_txt_line(sym_name, sym_kind, addr, ambiguous, local)
            f.write(line + "\n")

def export_ghidra_symbols_to_symbols_txt():
    print("Exporting symbol names to symbols.txt...")
    for module in ALL_MODULES:
        export_ghidra_symbols_for_module(module)

if __name__ == "__main__":
    import_objdiff_mappings()
    export_ghidra_symbols_to_symbols_txt()
