#Fixes reference destination addresses to use the correct overlay, as read from on relocs.txt.
#@author LagoLunatic
#@category #Decomp
#@keybinding 
#@menupath 
#@toolbar 

from __future__ import print_function

from ghidra_utils import *

import re

def import_overlay_relocs_for_src_module(module):
    relocs_txt_path = get_relocs_txt_path_by_module(module)
    addr_space = get_addr_space_by_module(module)
    
    with open(relocs_txt_path, "r") as f:
        for line in f.readlines():
            # print(repr(line))
            from_addr, kind, to_addr_offset, addend, to_module = read_relocs_txt_line(line, addr_space)
            refs = get_references_at(from_addr)
            if len(refs) != 1:
                # TODO: how to handle multiple relocations at the same from address?
                # ghidra's analysis is probably wrong here
                # for ref in refs:
                #     print(ref)
                continue
            ref = refs[0]
            if not isinstance(to_module, int):
                continue
            to_overlay_index = to_module
            if ref.getToAddress().getAddressSpace().getName() != "ram":
                # Already done previously.
                continue
            
            # Example: from overlay_75::022c3ad0
            
            assert ref is not None
            assert from_addr == ref.getFromAddress()
            to_addr_space = get_addr_space_by_module(to_overlay_index)
            to_addr = to_addr_space.getAddress(to_addr_offset)
            print(from_addr, to_addr, ref.getReferenceType(), SourceType.USER_DEFINED, ref.getOperandIndex())
            rm.delete(ref)
            rm.addMemoryReference(from_addr, to_addr, ref.getReferenceType(), SourceType.USER_DEFINED, ref.getOperandIndex())

def import_overlay_relocs():
    for module in ALL_MODULES:
        import_overlay_relocs_for_src_module(module)

if __name__ == "__main__":
    import_overlay_relocs()
