# Shared utility code for Ghidra scripts.

from __future__ import print_function

from __main__ import currentProgram, state, monitor, askString

from ghidra.program.model.symbol import RefType, SourceType # pyright: ignore [reportMissingImports]
from ghidra.app.services import DataTypeManagerService # pyright: ignore [reportMissingImports]
from ghidra.app.util.parser import FunctionSignatureParser # pyright: ignore [reportMissingImports]
from ghidra.app.cmd.function import ApplyFunctionSignatureCmd # pyright: ignore [reportMissingImports]

st = currentProgram.getSymbolTable()
rm = currentProgram.getReferenceManager()
dtm = currentProgram.getDataTypeManager()
mem = currentProgram.getMemory()
af = currentProgram.getAddressFactory()
fm = currentProgram.getFunctionManager()

import os

EXPORTS_DIR = os.path.dirname(__file__) + "/exports"
CONFIG_DIR = os.path.dirname(__file__) + "/../config/YR9E00"

def get_reference_at(from_address):
    refs = rm.getReferencesFrom(from_address)
    if len(refs) == 0:
        return None
    assert len(refs) == 1
    return refs[0]

def get_references_at(from_address):
    refs = rm.getReferencesFrom(from_address)
    return refs

def get_symbol_pointed_to_by(from_address):
    ref = get_reference_at(from_address)
    if ref is None:
        return None
    to_symbols = st.getSymbols(ref.getToAddress())
    assert len(to_symbols) == 1
    return to_symbols[0]

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
        addr_space = af.getAddressSpace("overlay_%d" % module)
    return addr_space

def read_key_val_pair_from_words(words, expected_key):
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
    addr_word = read_key_hex_val_pair_from_words(words, expected_key)
    addr = addr_space.getAddress(addr_word)
    return addr

def read_bool_from_words(words, expected_key):
    if expected_key in words:
        return True
    else:
        return False

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
    addr_word = addr.getAddressableWordOffset()
    words = [sym_name, "kind:%s" % sym_kind, "addr:0x%08x" % addr_word]
    if ambiguous:
        words.append("ambiguous")
    if local:
        words.append("local")
    
    return " ".join(words)

def read_relocs_txt_line(line, addr_space):
    words = line.strip().split(" ")
    from_addr = read_key_addr_val_pair_from_words(words, "from", addr_space)
    kind = read_key_val_pair_from_words(words, "kind")
    to_addr = read_key_addr_val_pair_from_words(words, "to", addr_space)
    addend = read_key_hex_val_pair_from_words(words, "add")
    module = read_key_val_pair_from_words(words, "module")
    
    return (from_addr, kind, to_addr, addend, module)
