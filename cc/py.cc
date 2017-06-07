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
            .def("new_page", &Surface::new_page)
            .def("line_p", [](Surface& aSurface, double x1, double y1, double x2, double y2, std::string color, double width) { aSurface.line({x1, y1}, {x2, y2}, color, Pixels{width}); }, py::arg("x1"), py::arg("y1"), py::arg("x2"), py::arg("y2"), py::arg("color"), py::arg("width"))
            .def("line_s", [](Surface& aSurface, double x1, double y1, double x2, double y2, std::string color, double width) { aSurface.line({x1, y1}, {x2, y2}, color, Scaled{width}); }, py::arg("x1"), py::arg("y1"), py::arg("x2"), py::arg("y2"), py::arg("color"), py::arg("width"))
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
