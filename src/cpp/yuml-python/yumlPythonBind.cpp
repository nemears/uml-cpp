#include <pybind11/pybind11.h>
#include "uml/element.h"

namespace py = pybind11;

PYBIND11_MODULE(yuml_python, m) {
    m.def("isValidUUID4", UML::isValidUUID4);
}