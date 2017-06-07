#include "acmacs-base/pybind11.hh"
#include "surface-cairo.hh"

// ----------------------------------------------------------------------

PYBIND11_MODULE(acmacs_draw_backend, m)
{
    m.doc() = "Acmacs draw plugin";

      // ----------------------------------------------------------------------
      // Color
      // ----------------------------------------------------------------------

    py::class_<Color>(m, "Color")
            .def(py::init<std::string>(), py::arg("color") = "black")
            .def("__str__", &Color::to_string)
            .def("to_string", &Color::to_string)
            .def("to_hex_string", &Color::to_string)
            .def("light", &Color::light)
            ;

    m.def("distinct_colors", &Color::distinct_colors);

    py::class_<Surface>(m, "Surface")
            ;

    py::class_<SurfaceCairo, Surface>(m, "SurfaceCairo")
            ;

    py::class_<PdfCairo, SurfaceCairo>(m, "PdfCairo")
            .def(py::init<std::string, double, double, double>(), py::arg("filename"), py::arg("width"), py::arg("height"), py::arg("viewport_width") = 1000.0)
            ;
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
