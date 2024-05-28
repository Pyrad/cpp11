#ifndef _WINMINGWRUN_HPP
#define _WINMINGWRUN_HPP

#if _WIN32
  #define MY_LIB_EXPORT __declspec(dllexport)
  #define MY_STD_CALL_CONVENTION __stdcall
#else
  #define MY_LIB_EXPORT
  #define MY_STD_CALL_CONVENTION
#endif

extern "C" {
    MY_LIB_EXPORT int MY_STD_CALL_CONVENTION run_normal();
    MY_LIB_EXPORT int MY_STD_CALL_CONVENTION run_boosttest();
    MY_LIB_EXPORT int MY_STD_CALL_CONVENTION run_lvalue_rvalue();
    MY_LIB_EXPORT int MY_STD_CALL_CONVENTION run_effective_modern_cpp();
}

extern "C" {
    MY_LIB_EXPORT bool MY_STD_CALL_CONVENTION run_pytest();
}

extern "C" {
    MY_LIB_EXPORT bool MY_STD_CALL_CONVENTION show_env_info();
    MY_LIB_EXPORT bool MY_STD_CALL_CONVENTION run_python_script();
}


#endif // _WINMINGWRUN_HPP
