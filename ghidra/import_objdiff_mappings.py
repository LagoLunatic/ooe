#Import symbol names from objdiff.json's mappings to Ghidra.
#@author LagoLunatic
#@category #Decomp
#@keybinding 
#@menupath 
#@toolbar 

from __future__ import print_function

from ghidra_utils import *

import json

def import_objdiff_mappings():
    print("Importing symbol mappings from objdiff.json...")
    with open(OBJDIFF_CONFIG_PATH, "r") as f:
        objdiff_config = json.load(f)
    
    for unit in objdiff_config["units"]:
        if "symbol_mappings" not in unit:
            continue
        for old_name, new_name in unit["symbol_mappings"].items():
            sym = get_symbol_or_none_by_address(old_name)
            if sym is None:
                continue
            ghidra_sym_name = sym.getName()
            if ghidra_sym_name == new_name:
                # TODO: remove from objdiff.json?
                continue
            print("Changing name:", old_name, new_name)
            sym.setName(new_name, SourceType.USER_DEFINED)

if __name__ == "__main__":
    import_objdiff_mappings()
