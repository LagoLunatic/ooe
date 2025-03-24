#Exports function names referenced in tables to text files.
#@author LagoLunatic
#@category #Decomp
#@keybinding 
#@menupath 
#@toolbar 

from __future__ import print_function

import os
from ghidra_utils import *

def export_enemy_funcs():
    dt_EnemyParam_s = dtm.getDataType("/_Custom/EnemyParam_s")
    
    start_symbols = st.getGlobalSymbols("EnemyParam")
    assert len(start_symbols) == 1
    enemy_param_addr = start_symbols[0].getAddress()
    
    with open(EXPORTS_DIR + "/enemy_funcs.txt", "w") as f:
        for i in range(NUM_ENEMIES):
            init_func_name = get_symbol_name_pointed_to_by(get_struct_field_addr(enemy_param_addr, dt_EnemyParam_s, "mInitFunc"))
            main_func_name = get_symbol_name_pointed_to_by(get_struct_field_addr(enemy_param_addr, dt_EnemyParam_s, "mMainFunc"))
            f.write("%s %s\n" % (init_func_name, main_func_name))
            enemy_param_addr = offset_addr(enemy_param_addr, dt_EnemyParam_s.getLength())

def export_dev_funcs():
    dt_pointer = dtm.getDataType("/pointer")
    
    init_start_symbols = st.getGlobalSymbols("dev_i_func")
    main_start_symbols = st.getGlobalSymbols("dev_func")
    assert len(init_start_symbols) == 1
    assert len(main_start_symbols) == 1
    init_func_ptr_addr = init_start_symbols[0].getAddress()
    main_func_ptr_addr = main_start_symbols[0].getAddress()
    
    with open(EXPORTS_DIR + "/dev_funcs.txt", "w") as f:
        for i in range(NUM_GIMMICKS):
            init_func_name = get_symbol_name_pointed_to_by(init_func_ptr_addr)
            main_func_name = get_symbol_name_pointed_to_by(main_func_ptr_addr)
            f.write("%s %s\n" % (init_func_name, main_func_name))
            init_func_ptr_addr = offset_addr(init_func_ptr_addr, dt_pointer.getLength())
            main_func_ptr_addr = offset_addr(main_func_ptr_addr, dt_pointer.getLength())

def export_rune_funcs():
    rune_lists = [
        ("RuneArmData", 0x37),
        ("RuneBackData", 0x19),
        ("RuneLrData", 0x1F),
    ]
    for rune_list_name, rune_count in rune_lists:
        rune_data_struct = dtm.getDataType("/_Custom/" + rune_list_name + "_s")
        
        func_start_symbols = st.getGlobalSymbols(rune_list_name)
        assert len(func_start_symbols) == 1
        rune_data_addr = func_start_symbols[0].getAddress()
        
        with open(EXPORTS_DIR + "/rune_funcs_" + rune_list_name + ".txt", "w") as f:
            for i in range(rune_count):
                func_name = get_symbol_name_pointed_to_by(get_struct_field_addr(rune_data_addr, rune_data_struct, "mFunc"))
                f.write("%s\n" % (func_name))
                rune_data_addr = offset_addr(rune_data_addr, rune_data_struct.getLength())

if __name__ == "__main__":
    if not os.path.exists(EXPORTS_DIR):
        os.makedirs(EXPORTS_DIR)
    
    export_enemy_funcs()
    export_dev_funcs()
    export_rune_funcs()
