#Exports symbol names from Ghidra to symbols.txt files for use with dsd.
#@author LagoLunatic
#@category #Decomp
#@keybinding 
#@menupath 
#@toolbar 

from __future__ import print_function

import re

from ghidra_utils import *

def export_ghidra_symbols_for_overlay(overlay_index):
    if overlay_index is None:
        # main ram
        symbols_txt_path = CONFIG_DIR + "/arm9/symbols.txt"
        dsd_auto_sym_name_prefix = ""
    else:
        symbols_txt_path = CONFIG_DIR + "/arm9/overlays/ov%03d/symbols.txt" % overlay_index
        dsd_auto_sym_name_prefix = "ov%03d_" % overlay_index
    addr_space = get_addr_space_by_overlay_index(overlay_index)
    
    updated_symbols = []
    with open(symbols_txt_path, "r") as f:
        for line in f.readlines():
            sym_name, sym_kind, addr, ambiguous = read_symbols_txt_line(line, addr_space)
            
            syms = st.getSymbols(addr)
            if len(syms) > 0:
                assert len(syms) == 1, addr
                sym = syms[0]
                
                ghidra_sym_name = sym.getName()
                if sym.getSource() != SourceType.DEFAULT:
                    print(sym_name, sym, sym.getSource())
                    if sym_name != ghidra_sym_name and sym_name != "Entry":
                        # assert sym_name.startswith("func_" + dsd_auto_sym_name_prefix) or sym_name.startswith("data_" + dsd_auto_sym_name_prefix), sym_name
                        sym_name = ghidra_sym_name
            
            updated_symbols.append((sym_name, sym_kind, addr, ambiguous))
    
    print(len(updated_symbols))
    
    with open(symbols_txt_path, "w") as f:
        for sym_name, sym_kind, addr, ambiguous in updated_symbols:
            line = write_symbols_txt_line(sym_name, sym_kind, addr, ambiguous)
            f.write(line + "\n")

def export_ghidra_symbols_to_symbols_txt():
    export_ghidra_symbols_for_overlay(None)
    for overlay_index in range(86):
        export_ghidra_symbols_for_overlay(overlay_index)

if __name__ == "__main__":
    export_ghidra_symbols_to_symbols_txt()
