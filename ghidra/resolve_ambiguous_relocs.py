#Attempts to automatically resolve some ambiguous overlay relocations, and removes ambiguous symbols that are no longer referenced anywhere.
#@author LagoLunatic
#@category #Decomp
#@keybinding 
#@menupath 
#@toolbar 

from __future__ import print_function

from ghidra_utils import *

def get_addr_offset_into_symbol_range(addr, symbol_name, symbol_length):
    start_addr = get_symbol_address_by_name(symbol_name)
    end_addr = offset_addr(start_addr, symbol_length)
    offset_from_start = addr.subtract(start_addr)
    offset_from_end = addr.subtract(end_addr)
    if offset_from_start >= 0 and offset_from_end < 0:
        return offset_from_start
    else:
        return None

def try_resolve_module_for_enemy(from_addr, to_module):
    dt_EnemyParam_s = dtm.getDataType("/_Custom/EnemyParam_s")
    EnemyParam_length = NUM_ENEMIES * dt_EnemyParam_s.getLength()
    offset_into_EnemyParam = get_addr_offset_into_symbol_range(from_addr, "EnemyParam", EnemyParam_length)
    
    if offset_into_EnemyParam is not None:
        offset_in_struct = offset_into_EnemyParam % dt_EnemyParam_s.getLength()
        assert offset_in_struct in [0, 4], "Relocation is not init func or main func"
        enemy_idx = offset_into_EnemyParam // dt_EnemyParam_s.getLength()
        # print(from_addr, module, enemy_idx)
        if enemy_idx in ENEMY_IDX_TO_OVERLAY_IDX:
            overlay_idx = ENEMY_IDX_TO_OVERLAY_IDX[enemy_idx]
            assert overlay_idx in to_module
            to_module = overlay_idx
        elif WIFI_OVERLAY_IDX in to_module:
            to_module.remove(WIFI_OVERLAY_IDX)
        elif TITLE_SCREEN_OVERLAY_IDX in to_module:
            to_module.remove(TITLE_SCREEN_OVERLAY_IDX)
    
    return to_module

def try_resolve_module_for_gimmick(from_addr, to_module):
    dev_func_length = NUM_GIMMICKS * dt_pointer.getLength()
    offset_into_dev_i_func = get_addr_offset_into_symbol_range(from_addr, "dev_i_func", dev_func_length)
    offset_into_dev_func = get_addr_offset_into_symbol_range(from_addr, "dev_func", dev_func_length)
    
    if offset_into_dev_i_func is not None:
        gimmick_idx = offset_into_dev_i_func // dt_pointer.getLength()
        if gimmick_idx in DEV_IDX_TO_OVERLAY_IDX:
            overlay_idx = DEV_IDX_TO_OVERLAY_IDX[gimmick_idx]
            assert overlay_idx in to_module
            to_module = overlay_idx
        elif WIFI_OVERLAY_IDX in to_module:
            to_module.remove(WIFI_OVERLAY_IDX)
        elif TITLE_SCREEN_OVERLAY_IDX in to_module:
            to_module.remove(TITLE_SCREEN_OVERLAY_IDX)
    elif offset_into_dev_func is not None:
        gimmick_idx = offset_into_dev_func // dt_pointer.getLength()
        if gimmick_idx in DEV_IDX_TO_OVERLAY_IDX:
            overlay_idx = DEV_IDX_TO_OVERLAY_IDX[gimmick_idx]
            assert overlay_idx in to_module
            assert overlay_idx in to_module
            to_module = overlay_idx
        elif WIFI_OVERLAY_IDX in to_module:
            to_module.remove(WIFI_OVERLAY_IDX)
        elif TITLE_SCREEN_OVERLAY_IDX in to_module:
            to_module.remove(TITLE_SCREEN_OVERLAY_IDX)
    
    return to_module

def resolve_ambiguous_relocs_in_tables():
    relocs_txt_path = get_relocs_txt_path_by_module("main")
    addr_space = get_addr_space_by_module("main")
    
    updated_relocs = []
    with open(relocs_txt_path, "r") as f:
        for line in f.readlines():
            # print(repr(line))
            from_addr, kind, to_addr_offset, addend, to_module = read_relocs_txt_line(line, addr_space)
            if isinstance(to_module, list):
                # Ambiguous relocation. Try to resolve it if it's in a table.
                to_module = try_resolve_module_for_enemy(from_addr, to_module)
                to_module = try_resolve_module_for_gimmick(from_addr, to_module)
            
            updated_relocs.append((from_addr, kind, to_addr_offset, addend, to_module))
    
    with open(relocs_txt_path, "w") as f:
        for from_addr, kind, to_addr_offset, addend, to_module in updated_relocs:
            line = write_relocs_txt_line(from_addr, kind, to_addr_offset, addend, to_module)
            f.write(line + "\n")

def remove_ambiguous_symbols_for_module(module, all_relocation_dests):
    symbols_txt_path = get_symbols_txt_path_by_module(module)
    addr_space = get_addr_space_by_module(module)
    
    updated_symbols = []
    with open(symbols_txt_path, "r") as f:
        for line in f.readlines():
            sym_name, sym_kind, addr, ambiguous, local = read_symbols_txt_line(line, addr_space)
            
            # print("A", len(list(get_references_to(addr))))
            if ambiguous and addr not in all_relocation_dests:
                # This is an ambiguous data symbol that no longer has any possible relocations pointing to it.
                # Remove it from symbols.txt.
                print("Removing ambiguous symbol at %s" % addr)
                assert sym_name.startswith("data_")
                assert sym_kind in ["data(any)", "bss"]
                # Also delete the corresponding symbol from Ghidra, if it exists.
                sym = get_symbol_or_none_by_address(addr)
                print(addr, sym)
                if sym is not None:
                    assert sym.getName() == sym_name
                    assert sym.getSource() == SourceType.USER_DEFINED
                    refs = get_references_to(addr)
                    assert len(list(refs)) == 0, "Symbol %s should be unused, but it has references to it?" % sym_name
                    sym.delete()
                continue
            
            updated_symbols.append((sym_name, sym_kind, addr, ambiguous, local))
    
    with open(symbols_txt_path, "w") as f:
        for sym_name, sym_kind, addr, ambiguous, local in updated_symbols:
            line = write_symbols_txt_line(sym_name, sym_kind, addr, ambiguous, local)
            f.write(line + "\n")

def get_all_relocation_dests_for_module(module):
    relocs_txt_path = get_relocs_txt_path_by_module(module)
    addr_space = get_addr_space_by_module(module)
    # print(module, addr_space)
    
    all_relocation_dests = set()
    with open(relocs_txt_path, "r") as f:
        for line in f.readlines():
            from_addr, kind, to_addr_offset, addend, to_module = read_relocs_txt_line(line, addr_space)
            if isinstance(to_module, list):
                for to_module_possibility in to_module:
                    to_addr_space = get_addr_space_by_module(to_module_possibility)
                    to_addr = get_address_in_addr_space(to_addr_space, to_addr_offset)
                    all_relocation_dests.add(to_addr)
            else:
                # print(to_addr)
                to_addr_space = get_addr_space_by_module(to_module)
                to_addr = get_address_in_addr_space(to_addr_space, to_addr_offset)
                all_relocation_dests.add(to_addr)
    
    return all_relocation_dests

def remove_ambiguous_symbols():
    # Removes any symbols in symbols.txt that are marked "ambiguous", and are not pointed to by any relocs.
    # Should be run *after* SyncDsd.java.
    all_relocation_dests = set()
    for module in ALL_MODULES:
        all_relocation_dests |= get_all_relocation_dests_for_module(module)
    for module in ALL_MODULES:
        remove_ambiguous_symbols_for_module(module, all_relocation_dests)

if __name__ == "__main__":
    resolve_ambiguous_relocs_in_tables()
    run_sync_dsd_script()
    remove_ambiguous_symbols()
