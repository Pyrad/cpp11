#ifndef _WINMINGWRUN_HPP
#define _WINMINGWRUN_HPP

extern "C" {
    __declspec(dllexport) int __stdcall run_normal();
    __declspec(dllexport) int __stdcall run_boosttest();
    __declspec(dllexport) int __stdcall run_lvalue_rvalue();
    __declspec(dllexport) int __stdcall run_effective_modern_cpp();
}

extern "C" {
    __declspec(dllexport) bool __stdcall run_pytest();
}

#endif // _WINMINGWRUN_HPP
