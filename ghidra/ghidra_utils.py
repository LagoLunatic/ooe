# Shared utility code for Ghidra scripts.

from __future__ import print_function

from __main__ import currentProgram, state, monitor, askString, getDataAt, getState

from ghidra.program.model.symbol import RefType, SourceType # pyright: ignore [reportMissingImports]
from ghidra.app.services import DataTypeManagerService # pyright: ignore [reportMissingImports]
from ghidra.app.util.parser import FunctionSignatureParser # pyright: ignore [reportMissingImports]
from ghidra.app.cmd.function import ApplyFunctionSignatureCmd # pyright: ignore [reportMissingImports]
from ghidra.app.script import GhidraScriptUtil # pyright: ignore [reportMissingImports]
from ghidra.app.services import ConsoleService # pyright: ignore [reportMissingImports]

st = currentProgram.getSymbolTable()
rm = currentProgram.getReferenceManager()
dtm = currentProgram.getDataTypeManager()
mem = currentProgram.getMemory()
af = currentProgram.getAddressFactory()
fm = currentProgram.getFunctionManager()

dt_pointer = dtm.getDataType("/pointer") or dtm.getDataType("/undefined *")

import os
import re

EXPORTS_DIR = os.path.dirname(__file__) + "/exports"
CONFIG_DIR = os.path.dirname(__file__) + "/../config/YR9E00"

NUM_OVERLAYS = 86
ALL_MODULES = ["main", "itcm", "dtcm"] + list(range(NUM_OVERLAYS))
WIFI_OVERLAY_IDX = 21
TITLE_SCREEN_OVERLAY_IDX = 20

NUM_ENEMIES = 121
NUM_GIMMICKS = 141

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

def get_symbol_by_name(name):
    symbols = st.getGlobalSymbols(name)
    assert len(symbols) > 0, "No symbol with name %s" % name
    assert len(symbols) == 1, "Multiple symbols with name %s" % name
    return symbols[0]

def get_symbol_address_by_name(name):
    symbol = get_symbol_by_name(name)
    return symbol.getAddress()

def get_symbol_by_address(address):
    symbols = st.getSymbols(address)
    assert len(symbols) > 0, "No symbol at address %s" % address
    assert len(symbols) == 1, "Multiple symbols at address %s" % address
    return symbols[0]

def get_symbol_or_none_by_address(address):
    symbols = st.getSymbols(address)
    if len(symbols) == 0:
        return None
    assert len(symbols) == 1, "Multiple symbols at address %s" % address
    return symbols[0]

def get_symbol_name_by_address(address):
    symbol = get_symbol_by_address(address)
    return symbol.getName()

def get_reference_at(from_address):
    refs = rm.getReferencesFrom(from_address)
    if len(refs) == 0:
        return None
    assert len(refs) == 1
    return refs[0]

def get_references_at(from_address):
    refs = rm.getReferencesFrom(from_address)
    return refs

def get_references_to(to_address):
    refs = rm.getReferencesTo(to_address)
    return refs

def get_address_pointed_to_by(from_address):
    ref = get_reference_at(from_address)
    if ref is None:
        return None
    return ref.getToAddress()

def get_symbol_pointed_to_by(from_address):
    to_address = get_address_pointed_to_by(from_address)
    if to_address is None:
        return None
    return get_symbol_by_address(to_address)

def get_symbol_name_pointed_to_by(from_address):
    sym = get_symbol_pointed_to_by(from_address)
    if sym is None:
        return None
    return sym.getName()

def get_struct_field_offset(struct, field_name):
    for component in struct.getComponents():
        if component.getFieldName() == field_name:
            return component.getOffset()

def get_struct_field_addr(start_addr, struct, field_name):
    addr_offset = start_addr.getOffset()
    addr_offset += get_struct_field_offset(struct, field_name)
    return start_addr.getNewAddress(addr_offset)

def offset_addr(addr, offset):
    return addr.getNewAddress(addr.getOffset() + offset)

def get_data_value_at_address(addr):
    data = getDataAt(addr)
    assert data is not None
    return data.getValue()

def set_function_signature(func_addr, signature_str):
    # Set the function's signature.
    dtms = state.tool.getService(DataTypeManagerService)
    parser = FunctionSignatureParser(dtm, dtms)
    sig = parser.parse(None, signature_str)
    cmd = ApplyFunctionSignatureCmd(func_addr, sig, SourceType.USER_DEFINED)
    cmd.applyTo(currentProgram, monitor)

def get_addr_space_by_module(module):
    if isinstance(module, str):
        # main ram
        addr_space = af.getAddressSpace("ram")
    elif isinstance(module, int):
        # overlay
        addr_space = af.getAddressSpace("arm9_ov%03d" % module)
    assert addr_space is not None, "Couldn't find address space for module: %s" % repr(module)
    return addr_space

def get_address_in_addr_space(addr_space, addr_offset):
    addr = addr_space.getAddress(addr_offset)
    # addr_space = af.getAddressSpace("overlay_%d" % overlay_index)
    if addr.getAddressSpace() != addr_space:
        if not addr_space.getName().endswith(".bss"):
            # SyncDsd.java creates separate address spaces for each overlay's .bss section.
            bss_addr_space = af.getAddressSpace(addr_space.getName() + ".bss")
            if bss_addr_space is not None:
                addr = bss_addr_space.getAddress(addr_offset)
            if addr.getAddressSpace() != bss_addr_space:
                raise Exception("Failed to resolve address 0x%08x into address space %s" % (addr_offset, addr_space))
    return addr

def read_key_val_pair_from_words(words, expected_key):  # type: (list[str], str) -> str | None
    for word in words:
        key, val = word.split(":", 1)
        if key == expected_key:
            return val
    return None

def read_key_hex_val_pair_from_words(words, expected_key):
    val = read_key_val_pair_from_words(words, expected_key)
    if val is None:
        return None
    hex_word = int(val, 16)
    return hex_word

def read_key_addr_val_pair_from_words(words, expected_key, addr_space):
    addr_offset = read_key_hex_val_pair_from_words(words, expected_key)
    addr = get_address_in_addr_space(addr_space, addr_offset)
    return addr

def read_bool_from_words(words, expected_key):
    if expected_key in words:
        return True
    else:
        return False

def get_symbols_txt_path_by_module(module):
    if isinstance(module, str):
        if module == "main":
            symbols_txt_path = CONFIG_DIR + "/arm9/symbols.txt"
        elif module == "itcm":
            symbols_txt_path = CONFIG_DIR + "/arm9/itcm/symbols.txt"
        elif module == "dtcm":
            symbols_txt_path = CONFIG_DIR + "/arm9/dtcm/symbols.txt"
    elif isinstance(module, int):
        symbols_txt_path = CONFIG_DIR + "/arm9/overlays/ov%03d/symbols.txt" % module
    return symbols_txt_path

def read_symbols_txt_line(line, addr_space):
    words = line.strip().split(" ")
    
    sym_name = words[0]
    words = words[1:]
    
    sym_kind = read_key_val_pair_from_words(words, "kind")
    sym_addr = read_key_addr_val_pair_from_words(words, "addr", addr_space)
    ambiguous = read_bool_from_words(words, "ambiguous")
    local = read_bool_from_words(words, "local")
    
    return (sym_name, sym_kind, sym_addr, ambiguous, local)

def write_symbols_txt_line(sym_name, sym_kind, addr, ambiguous, local):
    addr_offset = addr.getAddressableWordOffset()
    words = [sym_name, "kind:%s" % sym_kind, "addr:0x%08x" % addr_offset]
    if ambiguous:
        words.append("ambiguous")
    if local:
        words.append("local")
    
    return " ".join(words)

def get_relocs_txt_path_by_module(module):
    if isinstance(module, str):
        if module == "main":
            relocs_txt_path = CONFIG_DIR + "/arm9/relocs.txt"
        elif module == "itcm":
            relocs_txt_path = CONFIG_DIR + "/arm9/itcm/relocs.txt"
        elif module == "dtcm":
            relocs_txt_path = CONFIG_DIR + "/arm9/dtcm/relocs.txt"
    elif isinstance(module, int):
        relocs_txt_path = CONFIG_DIR + "/arm9/overlays/ov%03d/relocs.txt" % module
    return relocs_txt_path

def read_relocs_txt_line(line, addr_space):
    words = line.strip().split(" ")
    from_addr = read_key_addr_val_pair_from_words(words, "from", addr_space)
    kind = read_key_val_pair_from_words(words, "kind")
    to_addr_offset = read_key_hex_val_pair_from_words(words, "to")
    addend = read_key_hex_val_pair_from_words(words, "add")
    to_module = read_key_val_pair_from_words(words, "module")
    
    if to_module is not None:
        single_overlay_module_match = re.search(r"^overlay\((\d+)\)", to_module)
        multi_overlay_module_match = re.search(r"^overlays\(((?:\d+,)+\d+)\)", to_module)
        if single_overlay_module_match:
            to_module = int(single_overlay_module_match.group(1))
        elif multi_overlay_module_match:
            to_module = [
                int(overlay)
                for overlay in multi_overlay_module_match.group(1).split(",")
            ]
    
    return (from_addr, kind, to_addr_offset, addend, to_module)

def write_relocs_txt_line(from_addr, kind, to_addr_offset, addend, to_module):
    from_addr_offset = from_addr.getAddressableWordOffset()
    words = [
        "from:0x%08x" % from_addr_offset,
        "kind:%s" % kind,
        "to:0x%08x" % to_addr_offset,
    ]
    if addend is not None:
        words.append("add:0x%x" % addend)
    if isinstance(to_module, list) and len(to_module) == 1:
        to_module = to_module[0]
    if isinstance(to_module, int):
        words.append("module:overlay(%d)" % to_module)
    elif isinstance(to_module, list):
        words.append("module:overlays(%s)" % ",".join(str(overlay) for overlay in to_module))
    else:
        words.append("module:%s" % to_module)
    
    return " ".join(words)

def run_sync_dsd_script():
    script = GhidraScriptUtil.findScriptByName("SyncDsd.java")
    provider = GhidraScriptUtil.getProvider(script)
    state = getState()
    console = state.getTool().getService(ConsoleService)
    inst = provider.getScriptInstance(script, console.getStdOut())
    inst.execute(state, monitor, console.getStdOut())
