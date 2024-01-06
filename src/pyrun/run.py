import os

# In Windows, use path format as below
os.add_dll_directory("D:/Gitee/cpp11/build/boosttest")
os.add_dll_directory("D:/Gitee/cpp11/build/cppfeatures")
os.add_dll_directory("D:/Gitee/cpp11/build/EffectiveModernCpp")
os.add_dll_directory("D:/Gitee/cpp11/build/normal")
os.add_dll_directory("D:/procs/python38")

# Load dynamic library
import ctypes
dl = ctypes.windll.LoadLibrary
lib = dl('./pyrun/winmingwrun.dll')

# Run C function "run_pytest()"
lib.run_pytest()

