#Imports names for functions referenced in tables from text files.
#@author LagoLunatic
#@category #Decomp
#@keybinding 
#@menupath 
#@toolbar 

from __future__ import print_function

from ghidra_utils import *

ENEMY_IDX_TO_OVERLAY_IDX = {
    0x16: 38, # the creature
    0x3C: 38, # owl
    0x41: 38, # owl knight
    0x4E: 38, # draculina
    0x5E: 38, # spectral sword
    # 0x66: 36, # final knight
    0x67: 29, # jiang shi
    0x6C: 24, # arthroverta
    0x6D: 30, # brachyura
    0x6E: 26, # maneater
    0x6F: 27, # rusalka
    0x70: 32, # goliath
    0x71: 33, # gravedorcus
    0x72: 36, # albus
    0x73: 37, # barlowe
    0x74: 28, # wallman
    0x75: 35, # blackmore
    0x76: 31, # eligor
    0x77: 25, # death
    0x78: 34, # dracula
}

DEV_IDX_TO_OVERLAY_IDX = {
  0x18: 71,
  0x19: 57,
  0x1F: 68,
  0x23: 43,
  0x25: 62,
  0x27: 51,
  0x28: 53,
  0x2C: 41,
  0x2D: 40,
  0x2F: 53,
  0x38: 46,
  0x3A: 46,
  0x3B: 55,
  0x3D: 69,
  0x40: 78,
  0x43: 65,
  0x44: 68,
  0x45: 55,
  0x47: 57,
  0x48: 57,
  0x49: 57,
  0x4A: 57,
  0x4C: 52,
  0x4D: 42,
  0x4E: 42,
  0x4F: 63,
  0x50: 76,
  0x51: 77,
  0x53: 72,
  0x54: 60,
  0x57: 72,
  0x58: 41,
  0x59: 66,
  0x5A: 46,
  0x5B: 46,
  0x5D: 84,
#   0x60: 42,
  0x61: 42,
  0x62: 42,
  0x63: 42,
  0x64: 42,
  0x65: 42,
  0x66: 78,
  0x67: 41,
  0x68: 41,
  0x69: 51,
  0x6A: 42,
  0x6B: 53,
  0x6C: 64,
  0x6D: 64,
  0x6E: 41,
  0x6F: 59,
  0x70: 42,
  0x71: 62,
  0x72: 62,
  0x73: 42,
  0x74: 60,
  0x75: 60,
  0x76: 60,
  0x77: 42,
  0x78: 42,
  0x79: 60,
  0x7A: 42,
  0x7E: 67,
  0x7F: 67,
  0x85: 75,
  0x86: 75,
}

def check_if_address_is_valid_in_space(addr_space, addr):
    # Hack because I can't find the proper way to check if an address is valid in an address space.
    # print(addr, "%08X" % addr.getAddressableWordOffset())
    addr_in_space = addr_space.getAddress(addr.getAddressableWordOffset())
    addr_in_space_str = addr_in_space.toString(True)
    if addr_space.getName() == "ram":
        return addr.getAddressableWordOffset() < 0x021DD280
    else: # Overlay
        return addr_in_space_str.startswith(addr_space.getName() + "::")

def replace_mem_ref_to_main_ram_with_ref_to_overlay(ref, overlay_indexes_to_try):
    if ref.getToAddress().getAddressSpace().getName() != "ram":
        # Already did this on a previous run of the script.
        assert ref.getSource() == SourceType.USER_DEFINED
        return
    
    # print("Attempting to find overlay for address:", ref.getToAddress())
    # The reference destination is a default generated by ghidra because it doesn't know the proper overlay to use.
    # We have to add a new reference pointing to the overlay and then delete the original reference.
    # assert ref.getSource() == SourceType.DEFAULT
    from_addr = ref.getFromAddress()
    valid_addr_spaces = []
    for overlay_index in overlay_indexes_to_try:
        if overlay_index is None:
            # main ram
            addr_space = af.getAddressSpace("ram")
        else:
            addr_space = af.getAddressSpace("overlay_%d" % overlay_index)
        # print(addr_space)
        # print(overlay_indexes_to_try, addr_space, ref.getToAddress(), check_if_address_is_valid_in_space(addr_space, ref.getToAddress()))
        if check_if_address_is_valid_in_space(addr_space, ref.getToAddress()):
            valid_addr_spaces.append(addr_space)
    # print(overlay_indexes_to_try)
    assert len(valid_addr_spaces) > 0, "No valid address spaces found for to address: %s (Checked: %s)" % (ref.getToAddress(), overlay_indexes_to_try)
    assert len(valid_addr_spaces) < 2, "Multiple valid address spaces found for to address %s: %s (Checked: %s)" % (ref.getToAddress(), repr(valid_addr_spaces), overlay_indexes_to_try)
    to_address = valid_addr_spaces[0].getAddress(ref.getToAddress().getAddressableWordOffset())
    # print(to_address)
    # print("Adding memory reference:", from_addr, to_address)
    rm.delete(ref)
    rm.addMemoryReference(from_addr, to_address, ref.getReferenceType(), SourceType.USER_DEFINED, ref.getOperandIndex())

def fix_up_reference_and_symbol(from_addr, new_dest_symbol_name, overlay_indexes_to_try):
    if overlay_indexes_to_try is not None:
        init_func_ref = get_reference_at(from_addr)
        assert init_func_ref, from_addr
        replace_mem_ref_to_main_ram_with_ref_to_overlay(init_func_ref, overlay_indexes_to_try)
    else:
        # If overlay_indexes_to_try is None, we assume that it's *supposed* to be in main RAM,
        # and don't change anything about the reference itself.
        pass
    
    func_sym = get_symbol_pointed_to_by(from_addr)
    assert func_sym, from_addr
    func_sym.setName(new_dest_symbol_name, SourceType.USER_DEFINED)
    
    func_addr = func_sym.getAddress()
    func = fm.getFunctionContaining(func_addr)
    # print(fm.getFunctionContaining(func_addr).getBody())
    if func is None:
        print(func_addr, func_sym.getSymbolType())
        # Can't seem to create a function via python scripting... I'll just do it manually.
        # disassemble(func_addr)
        # clearListing(func_addr)
        # func_addr_set_view = af.getAddressSet(from_addr, from_addr)
        # fm.createFunction(new_dest_symbol_name, from_addr, func_addr_set_view, SourceType.USER_DEFINED)
    # else:
    #     # Set the function's signature.
    #     signature = "void func(DrObj* obj)"
    #     dtms = state.tool.getService(DataTypeManagerService)
    #     parser = FunctionSignatureParser(dtm, dtms)
    #     sig = parser.parse(None, signature)
    #     cmd = ApplyFunctionSignatureCmd(func_addr, sig, SourceType.USER_DEFINED)
    #     cmd.applyTo(currentProgram, monitor)


def import_enemy_funcs():
    dt_EnemyParam_s = dtm.getDataType("/_Custom/EnemyParam_s")
    
    start_symbols = st.getGlobalSymbols("EnemyParam")
    assert len(start_symbols) == 1
    enemy_param_addr = start_symbols[0].getAddress()
    
    with open(EXPORTS_DIR + "/enemy_funcs.txt", "r") as f:
        for enemy_idx in range(0x78+1):
            # print("Enemy index: 0x%02X" % enemy_idx)
            
            line = f.readline()
            assert line != ""
            init_func_name, main_func_name = line.strip().split(" ")
            
            init_func_ptr_addr = get_struct_field_addr(enemy_param_addr, dt_EnemyParam_s, "mInitFunc")
            main_func_ptr_addr = get_struct_field_addr(enemy_param_addr, dt_EnemyParam_s, "mMainFunc")
            
            if enemy_idx in ENEMY_IDX_TO_OVERLAY_IDX:
                overlay_indexes_to_try = [ENEMY_IDX_TO_OVERLAY_IDX[enemy_idx]]
            else:
                overlay_indexes_to_try = [19, 22]
            
            fix_up_reference_and_symbol(init_func_ptr_addr, init_func_name, overlay_indexes_to_try)
            fix_up_reference_and_symbol(main_func_ptr_addr, main_func_name, overlay_indexes_to_try)
            
            enemy_param_addr = offset_addr(enemy_param_addr, dt_EnemyParam_s.getLength())
        assert f.readline() == ""

def import_dev_funcs():
    dt_pointer = dtm.getDataType("/pointer")
    
    init_start_symbols = st.getGlobalSymbols("dev_i_func")
    main_start_symbols = st.getGlobalSymbols("dev_func")
    # print(init_start_symbols)
    assert len(init_start_symbols) == 1
    assert len(main_start_symbols) == 1
    init_func_ptr_addr = init_start_symbols[0].getAddress()
    main_func_ptr_addr = main_start_symbols[0].getAddress()
    
    with open(EXPORTS_DIR + "/dev_funcs.txt", "r") as f:
        for dev_idx in range(0x8C+1):
            # print("Dev: 0x%02X" % dev_idx)
            
            line = f.readline()
            assert line != ""
            init_func_name, main_func_name = line.strip().split(" ")
            
            if dev_idx in DEV_IDX_TO_OVERLAY_IDX:
                overlay_indexes_to_try = [DEV_IDX_TO_OVERLAY_IDX[dev_idx]]
            else:
                overlay_indexes_to_try = [None, 19, 22]
            
            fix_up_reference_and_symbol(init_func_ptr_addr, init_func_name, overlay_indexes_to_try)
            fix_up_reference_and_symbol(main_func_ptr_addr, main_func_name, overlay_indexes_to_try)
            
            init_func_ptr_addr = offset_addr(init_func_ptr_addr, dt_pointer.getLength())
            main_func_ptr_addr = offset_addr(main_func_ptr_addr, dt_pointer.getLength())
        assert f.readline() == ""

def import_rune_funcs():
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
        
        with open(EXPORTS_DIR + "/rune_funcs_" + rune_list_name + ".txt", "r") as f:
            for i in range(rune_count):
                # print("Rune %s: 0x%02X" % (rune_list_name, i))
                # print(rune_data_addr)
                
                line = f.readline()
                assert line != ""
                func_name = line.strip()
                
                # print(get_reference_at(rune_data_addr))
                func_ptr_addr = get_struct_field_addr(rune_data_addr, rune_data_struct, "mFunc")
                if get_symbol_pointed_to_by(func_ptr_addr) is None:
                    # First entry of each list is null.
                    assert i == 0, func_ptr_addr
                else:
                    fix_up_reference_and_symbol(func_ptr_addr, func_name, None)
                
                rune_data_addr = offset_addr(rune_data_addr, rune_data_struct.getLength())
            assert f.readline() == ""

if __name__ == "__main__":
    import_enemy_funcs()
    import_dev_funcs()
    import_rune_funcs()
