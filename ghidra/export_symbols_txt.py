#Exports symbol names from Ghidra to symbols.txt files for use with dsd.
#@author LagoLunatic
#@category #Decomp
#@keybinding 
#@menupath 
#@toolbar 

from __future__ import print_function

import re

from ghidra_utils import *

def read_symbols_txt_line(line, addr_space):
    words = line.strip().split(" ")
    if len(words) == 4:
        sym_name, sym_kind, sym_addr, ambiguous = words
        assert ambiguous == "ambiguous"
        ambiguous = True
    elif len(words) == 3:
        sym_name, sym_kind, sym_addr = words
        ambiguous = False
    
    addr_key, addr_val = sym_addr.split(":", 1)
    assert addr_key == "addr"
    addr_word = int(addr_val, 16)
    addr = addr_space.getAddress(addr_word)
    
    return (sym_name, sym_kind, addr, ambiguous)

def write_symbols_txt_line(sym_name, sym_kind, addr, ambiguous):
    addr_word = addr.getAddressableWordOffset()
    sym_addr = "addr:0x%x" % addr_word
    
    words = [sym_name, sym_kind, sym_addr]
    if ambiguous:
        words.append("ambiguous")
    
    return " ".join(words)

def export_ghidra_symbols_for_overlay(overlay_index):
    if overlay_index is None:
        # main ram
        symbols_txt_path = CONFIG_DIR + "/arm9/symbols.txt"
        addr_space = af.getAddressSpace("ram")
        dsd_auto_sym_name_prefix = ""
    else:
        symbols_txt_path = CONFIG_DIR + "/arm9/overlays/ov%03d/symbols.txt" % overlay_index
        addr_space = af.getAddressSpace("overlay_%d" % overlay_index)
        dsd_auto_sym_name_prefix = "ov%03d_" % overlay_index
    
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
                        assert sym_name.startswith("func_" + dsd_auto_sym_name_prefix) or sym_name.startswith("data_" + dsd_auto_sym_name_prefix), sym_name
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
