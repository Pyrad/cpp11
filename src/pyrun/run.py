import os

def run_on_Asus_Win7_MSYS2():
    """
    To run commands on Asus_Win7_MSYS2
    """
    # On Windows, use path format as below
    os.add_dll_directory("D:/Gitee/cpp11/build/boosttest")
    os.add_dll_directory("D:/Gitee/cpp11/build/cppfeatures")
    os.add_dll_directory("D:/Gitee/cpp11/build/EffectiveModernCpp")
    os.add_dll_directory("D:/Gitee/cpp11/build/normal")
    os.add_dll_directory("D:/Gitee/cpp11/build/pyrun")
    os.add_dll_directory("D:/procs/python38")

    # Load dynamic library
    import ctypes
    dl = ctypes.windll.LoadLibrary
    lib = dl('./pyrun/libwinmingwrun.dll')

    # Run C functions
    lib.run_normal();
    lib.run_boosttest();
    lib.run_lvalue_rvalue();
    lib.run_effective_modern_cpp();

def run_on_Asus_Win7_VBox_Ubuntu_20_04_LTS():
    """
    To run commands on Asus_Win7_VBox_Ubuntu_20_04_LTS
    """
    pass

def run_main():
    # Get env dict
    edict = os.environ

    # Need env var DEV_NICKNAME to decide paths to boost, python and others
    dname = "DEV_NICKNAME"
    if dname not in edict:
        return False

    if edict[dname] == "Asus_Win7_MSYS2":
        run_on_Asus_Win7_MSYS2()
        print("Finished running Python function: run_on_Asus_Win7_MSYS2")
    elif edict[dname] == "Asus_Win7_VBox_Ubuntu_20_04_LTS":
        run_on_Asus_Win7_VBox_Ubuntu_20_04_LTS()
        print("Finished running Python function: run_on_Asus_Win7_VBox_Ubuntu_20_04_LTS")
    else:
        print("Not a supported platform to run")

if __name__ == "__main__":
    run_main()

