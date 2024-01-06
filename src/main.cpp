#include "main.hpp"
#include "./normal/template.hpp"
#include <memory>

#include "projectconfig.h"

// ------------------------------------------------------
// The following header files are included conditionally
// based on the macros defined in CMakeLists.txt
// ------------------------------------------------------
#ifdef ENB_PYRUN_CPP
#include "./pyrun/pyrun.hpp"
#endif // ENB_PYRUN_CPP

int main(int argc, char* argv[]) {

#ifdef ENB_PYRUN_CPP
    std::cout << "Running before ENB_PYRUN_CPP\n";
    std::cout << "Running ENB_PYRUN_CPP\n";
    pyrun::pyrun();
    std::cout << "Running after ENB_PYRUN_CPP\n";
#else
    fprintf(stdout, "Warning: macro %s is not defined, code in function %s not "
                    "compiled\n", "ENB_PYRUN_CPP", __FUNCTION__);
#endif // ENB_PYRUN_CPP

    std::cout << "\n\n---end of function main---\n" << std::endl;

    return 0;
}
