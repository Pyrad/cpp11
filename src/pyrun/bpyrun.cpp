#include <boost/python.hpp>

#include "bpyrun.hpp"

char const* greet() {
    return "hello, world";
}

BOOST_PYTHON_MODULE(bpyrun) {
    namespace bpy = boost::python;
    bpy::def("greet", greet);
}

