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
    if isinstance(module, str):
        if module == "main":
            relocs_txt_path = CONFIG_DIR + "/arm9/relocs.txt"
        elif module == "itcm":
            relocs_txt_path = CONFIG_DIR + "/arm9/itcm/relocs.txt"
        elif module == "dtcm":
            relocs_txt_path = CONFIG_DIR + "/arm9/dtcm/relocs.txt"
    elif isinstance(module, int):
        relocs_txt_path = CONFIG_DIR + "/arm9/overlays/ov%03d/relocs.txt" % module
    addr_space = get_addr_space_by_module(module)
    
    with open(relocs_txt_path, "r") as f:
        for line in f.readlines():
            # print(repr(line))
            from_addr, kind, to_addr, addend, module = read_relocs_txt_line(line, addr_space)
            refs = get_references_at(from_addr)
            if len(refs) != 1:
                # TODO: how to handle multiple relocations at the same from address?
                # ghidra's analysis is probably wrong here
                # for ref in refs:
                #     print(ref)
                continue
            ref = refs[0]
            single_overlay_module_match = re.search(r"^overlay\((\d+)\)", module)
            if not single_overlay_module_match:
                continue
            to_overlay_index = int(single_overlay_module_match.group(1))
            if ref.getToAddress().getAddressSpace().getName() != "ram":
                # Already done previously.
                continue
            
            # Example: from overlay_75::022c3ad0
            
            assert ref is not None
            assert from_addr == ref.getFromAddress()
            to_addr_space = get_addr_space_by_module(to_overlay_index)
            to_addr = to_addr_space.getAddress(ref.getToAddress().getAddressableWordOffset())
            print(from_addr, to_addr, ref.getReferenceType(), SourceType.USER_DEFINED, ref.getOperandIndex())
            rm.delete(ref)
            rm.addMemoryReference(from_addr, to_addr, ref.getReferenceType(), SourceType.USER_DEFINED, ref.getOperandIndex())

def import_overlay_relocs():
    for module in ["main", "itcm", "dtcm"]:
        import_overlay_relocs_for_src_module(module)
    for overlay_index in range(86):
        import_overlay_relocs_for_src_module(overlay_index)

if __name__ == "__main__":
    import_overlay_relocs()
