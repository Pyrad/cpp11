#ifndef _WINMINGWRUN_HPP
#define _WINMINGWRUN_HPP

extern "C" {
    __declspec(dllexport) int __stdcall run_normal();
    __declspec(dllexport) int __stdcall run_boosttest();
    __declspec(dllexport) int __stdcall run_lvalue_rvalue();
    __declspec(dllexport) int __stdcall run_effective_modern_cpp();
}

#endif // _WINMINGWRUN_HPP
