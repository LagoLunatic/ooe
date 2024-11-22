from __future__ import print_function

from __main__ import currentProgram, state, monitor

from ghidra.program.model.symbol import RefType, SourceType
from ghidra.app.services import DataTypeManagerService
from ghidra.app.util.parser import FunctionSignatureParser
from ghidra.app.cmd.function import ApplyFunctionSignatureCmd

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
