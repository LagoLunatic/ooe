#Dumps struct name and fields in C format to the clipboard, with offset comments for each field.
#@Pheenoh / Taka / LagoLunatic
#@category #Decomp
#@keybinding 
#@menupath 
#@toolbar 

from __future__ import print_function

from ghidra.program.model.data import Pointer, FunctionDefinition # pyright: ignore [reportMissingImports]

from ghidra_utils import *

DATATYPE_REMAPS = {
    'byte': 'u8',
    'uchar': 'u8',
    'sbyte': 's8',
    'short': 's16',
    'ushort': 'u16',
    'undefined1': 'u8',
    'undefined2': 'u16',
    'undefined4': 'u32',
    'undefined1 *': 'u8*',
    'undefined2 *': 'u16*',
    'undefined4 *': 'u32*',
    'long': 's32',
    'unsigned int': 'uint',
    'ulong': 'u32',
    'float': 'f32',
    'pointer': 'void*',
    'undefined*': 'void*',
}

UNNAMED_FIELD_PREFIX = "m"

def get_member_string_for_undefined_region(undefined_member_start_str, hex_offset_string):
    member_name = UNNAMED_FIELD_PREFIX+undefined_member_start_str
    member_name += "[0x"+hex_offset_string+" - 0x"+undefined_member_start_str+"]"
    undefined_member_string = "    /* 0x" +undefined_member_start_str+" */ u8 "+member_name+";"
    return undefined_member_string

def get_data_type(struct_name_or_path):
    struct = dtm.getDataType("/" + struct_name_or_path)
    if struct is None:
        struct = dtm.getDataType("/_Custom/" + struct_name_or_path)
    if struct is None:
        struct = dtm.getDataType("/Demangler/" + struct_name_or_path)
    return struct

def export_struct_to_clipboard(struct_data_type_path):
    struct = get_data_type(struct_data_type_path)
    struct_name = struct.getName()
    struct_length = struct.getLength()
    if struct_name == "DrObjBase":
        # For consistency with DrObj
        struct_length = get_data_type("DrObj").getLength()
    offset_pad_size = len("{:X}".format(struct_length))

    out_lines = []
    # out_lines.append("typedef struct %s %s;" % (struct_name, struct_name))
    # out_lines.append("")
    out_lines.append("struct %s {" % (struct_name))

    in_undefined_region = False
    undefined_member_start_str = ""
    for component in struct.getComponents():
        data_type = str(component.getDataType().getName())
        offset = component.getOffset()
        hex_offset_string = str("%0*X" % (offset_pad_size, offset))
        
        if component.getFieldName() is not None:
            member_name = str(component.getFieldName())
        else:
            member_name = UNNAMED_FIELD_PREFIX + hex_offset_string
        
        if data_type == 'undefined':
            if not in_undefined_region:
                in_undefined_region = True
                undefined_member_start_str = hex_offset_string
            continue
        elif in_undefined_region:
            # Found a field immediately following an undefined region, so commit the undefined region.
            in_undefined_region = False
            undefined_member_string = get_member_string_for_undefined_region(undefined_member_start_str, hex_offset_string)
            out_lines.append(undefined_member_string)
        
        # print(data_type)
        data_type = str(data_type)
        
        # Remove the star from function pointers.
        if isinstance(component.getDataType(), Pointer):
            if isinstance(component.getDataType().getDataType(), FunctionDefinition):
                assert data_type.endswith("*")
                data_type = data_type[:-1]
        
        if data_type.endswith(" *"):
            data_type = data_type[:-2] + "*"

        if data_type in DATATYPE_REMAPS:
            data_type = DATATYPE_REMAPS[data_type]
        if data_type[-1] == "*" and data_type[:-1] in DATATYPE_REMAPS:
            data_type = DATATYPE_REMAPS[data_type[:-1]] + "*"
        
        if "[" in data_type:
            # move array to member name
            array_start_idx = data_type.find("[")
            array = data_type[array_start_idx:]
            data_type = data_type[0:array_start_idx]
            if data_type in DATATYPE_REMAPS:
                data_type = DATATYPE_REMAPS[data_type]
            member_name = member_name+array
        elif data_type == "char":
            data_type = "s8"
        
        if data_type[0] == "_":
            data_type = data_type[1:]
        
        # if data_type == "PTMF": # Pointer to member function
        #     match = re.search(r"^mCurr?(\S+Func)$", member_name)
        #     if match:
        #       data_type = match.group(1)
        #     else:
        #       # Just guess at the return type and parameters
        #       data_type = "int"
        #       member_name = "("+type_name+"::*"+member_name+")()"
        
        member_string = "    /* 0x" +hex_offset_string+" */ "+str(data_type).replace(" ","")
        if member_name != "base": # anonymous struct
            member_string += " "+member_name
        member_string += ";"
        out_lines.append(member_string)

    hex_end_offset_string = str("%0*X" % (offset_pad_size, struct.getLength()))

    if in_undefined_region:
        # An undefined region lasted to the end of the struct, so commit the undefined region.
        undefined_member_string = get_member_string_for_undefined_region(undefined_member_start_str, hex_end_offset_string)
        out_lines.append(undefined_member_string)

    out_lines.append("};  // Size: 0x%s" % hex_end_offset_string)

    out_str = "\n".join(out_lines)
    print(out_str)

    # Copy to clipboard
    from docking.dnd import GClipboard # pyright: ignore [reportMissingImports]
    from java.awt.datatransfer import Clipboard, StringSelection # pyright: ignore [reportMissingImports]
    clipboard = GClipboard.getSystemClipboard()
    data = StringSelection(out_str)
    clipboard.setContents(data, None)

if __name__ == "__main__":
    struct_data_type_path = askString("Generate Struct Members", "Enter a data type name: ")
    export_struct_to_clipboard(struct_data_type_path)
