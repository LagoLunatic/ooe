#Automates setting the parameters taken by some functions.
#@author LagoLunatic
#@category #Decomp
#@keybinding 
#@menupath 
#@toolbar 

from ghidra_utils import *

def set_all_obj_func_signatures():
    for filename in ["enemy_funcs.txt", "dev_funcs.txt", "rune_funcs_RuneArmData.txt", "rune_funcs_RuneBackData.txt", "rune_funcs_RuneLrData.txt"]:
        with open(EXPORTS_DIR + "/" + filename, "r") as f:
            for line in f.readlines():
                for func_name in line.strip().split(" "):
                    func_symbols = st.getGlobalSymbols(func_name)
                    for func_sym in func_symbols:
                        print(func_sym)
                        set_function_signature(func_sym.getAddress(), "void func(Obj_c* obj)")

if __name__ == "__main__":
    set_all_obj_func_signatures()
