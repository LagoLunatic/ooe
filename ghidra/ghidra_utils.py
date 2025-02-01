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

def read_key_val_pair_from_word(word, expected_key):
    key, val = word.split(":", 1)
    assert key == expected_key
    return val

def read_key_hex_val_pair_from_word(word, expected_key):
    val = read_key_val_pair_from_word(word, expected_key)
    hex_word = int(val, 16)
    return hex_word

def read_key_addr_val_pair_from_word(word, expected_key, addr_space):
    addr_word = read_key_hex_val_pair_from_word(word, expected_key)
    addr = addr_space.getAddress(addr_word)
    return addr

def read_symbols_txt_line(line, addr_space):
    words = line.strip().split(" ")
    if len(words) == 4:
        assert words[3] == "ambiguous"
        ambiguous = True
    elif len(words) == 3:
        ambiguous = False
    else:
        raise Exception("Invalid symbols.txt line:", repr(line))
    
    sym_name = words[0]
    sym_kind = read_key_val_pair_from_word(words[1], "kind")
    sym_addr = read_key_addr_val_pair_from_word(words[2], "addr", addr_space)
    
    return (sym_name, sym_kind, sym_addr, ambiguous)

def write_symbols_txt_line(sym_name, sym_kind, addr, ambiguous):
    addr_word = addr.getAddressableWordOffset()
    sym_addr = "addr:0x%08x" % addr_word
    
    words = [sym_name, "kind:%s" % sym_kind, sym_addr]
    if ambiguous:
        words.append("ambiguous")
    
    return " ".join(words)

def read_relocs_txt_line(line, addr_space):
    words = line.strip().split(" ")
    if len(words) == 4:
        addend = 0
    elif len(words) == 5:
        addend = read_key_hex_val_pair_from_word(words[3], "add")
    else:
        raise Exception("Invalid symbols.txt line:", repr(line))
    
    from_addr = read_key_addr_val_pair_from_word(words[0], "from", addr_space)
    kind = read_key_val_pair_from_word(words[1], "kind")
    to_addr = read_key_addr_val_pair_from_word(words[2], "to", addr_space)
    module = read_key_val_pair_from_word(words[len(words)-1], "module")
    
    return (from_addr, kind, to_addr, module)
