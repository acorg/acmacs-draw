#include "acmacs-base/pybind11.hh"
#include "surface-cairo.hh"

// ----------------------------------------------------------------------

PYBIND11_MODULE(acmacs_draw_backend, m)
{
    using Size = acmacs::Size;
    using TextStyle = acmacs::TextStyle;
    using namespace acmacs::surface;

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

    m.def("distinct_colors", &Color::distinct_s);

    py::class_<acmacs::surface::Surface>(m, "Surface")
            .def("subsurface_s", [](Surface& aSurface, double x, double y, double width, double sub_width, double sub_height, bool clip) -> Surface& { return aSurface.subsurface({x, y}, Scaled{width}, Size{sub_width, sub_height}, clip); }, py::arg("origin_x"), py::arg("origin_y"), py::arg("width_in_parent"), py::arg("viewport_width"), py::arg("viewport_height"), py::arg("clip"), py::return_value_policy::reference)
            .def("subsurface_p", [](Surface& aSurface, double x, double y, double width, double sub_width, double sub_height, bool clip) -> Surface& { return aSurface.subsurface({x, y}, Pixels{width}, Size{sub_width, sub_height}, clip); }, py::arg("origin_x_pixels"), py::arg("origin_y_pixels"), py::arg("width_in_parent"), py::arg("viewport_width"), py::arg("viewport_height"), py::arg("clip"), py::return_value_policy::reference)
            .def("new_page", &Surface::new_page)
            .def("line_p", [](Surface& aSurface, double x1, double y1, double x2, double y2, std::string color, double width) { aSurface.line({x1, y1}, {x2, y2}, Color(color), Pixels{width}); }, py::arg("x1"), py::arg("y1"), py::arg("x2"), py::arg("y2"), py::arg("color"), py::arg("width"))
            .def("line_s", [](Surface& aSurface, double x1, double y1, double x2, double y2, std::string color, double width) { aSurface.line({x1, y1}, {x2, y2}, Color(color), Scaled{width}); }, py::arg("x1"), py::arg("y1"), py::arg("x2"), py::arg("y2"), py::arg("color"), py::arg("width"))
            .def("rectangle", [](Surface& aSurface, double x1, double y1, double width, double height, std::string color, double outline_width) { aSurface.rectangle({x1, y1}, {width, height}, Color(color), Pixels{outline_width}); }, py::arg("x"), py::arg("y"), py::arg("width"), py::arg("height"), py::arg("color"), py::arg("outline_width"))
            .def("rectangle_filled", [](Surface& aSurface, double x1, double y1, double width, double height, std::string outline_color, double outline_width, std::string fill_color) { aSurface.rectangle_filled({x1, y1}, {width, height}, Color(outline_color), Pixels{outline_width}, Color(fill_color)); }, py::arg("x"), py::arg("y"), py::arg("width"), py::arg("height"), py::arg("outline_color"), py::arg("outline_width"), py::arg("fill_color"))
            .def("circle_p", [](Surface& aSurface, double x, double y, double diameter, std::string outline_color, double width, double aspect, double rotation) { aSurface.circle({x, y}, Pixels{diameter}, Aspect{aspect}, Rotation{rotation}, Color(outline_color), Pixels{width}); }, py::arg("x"), py::arg("y"), py::arg("diameter"), py::arg("outline_color"), py::arg("width"), py::arg("aspect") = 1.0, py::arg("rotation") = 0.0)
            .def("circle_s", [](Surface& aSurface, double x, double y, double diameter, std::string outline_color, double width, double aspect, double rotation) { aSurface.circle({x, y}, Scaled{diameter}, Aspect{aspect}, Rotation{rotation}, Color(outline_color), Pixels{width}); }, py::arg("x"), py::arg("y"), py::arg("diameter"), py::arg("outline_color"), py::arg("width"), py::arg("aspect") = 1.0, py::arg("rotation") = 0.0)
            .def("circle_filled_p", [](Surface& aSurface, double x, double y, double diameter, std::string outline_color, double width, std::string fill_color, double aspect, double rotation) { aSurface.circle_filled({x, y}, Pixels{diameter}, Aspect{aspect}, Rotation{rotation}, Color(outline_color), Pixels{width}, Color(fill_color)); }, py::arg("x"), py::arg("y"), py::arg("diameter"), py::arg("outline_color"), py::arg("width"), py::arg("fill_color"), py::arg("aspect") = 1.0, py::arg("rotation") = 0.0)
            .def("circle_filled_s", [](Surface& aSurface, double x, double y, double diameter, std::string outline_color, double width, std::string fill_color, double aspect, double rotation) { aSurface.circle_filled({x, y}, Scaled{diameter}, Aspect{aspect}, Rotation{rotation}, Color(outline_color), Pixels{width}, Color(fill_color)); }, py::arg("x"), py::arg("y"), py::arg("diameter"), py::arg("outline_color"), py::arg("width"), py::arg("fill_color"), py::arg("aspect") = 1.0, py::arg("rotation") = 0.0)
            .def("square_filled_p", [](Surface& aSurface, double x, double y, double side, std::string outline_color, double width, std::string fill_color, double aspect, double rotation) { aSurface.square_filled({x, y}, Pixels{side}, Aspect{aspect}, Rotation{rotation}, Color(outline_color), Pixels{width}, Color(fill_color)); }, py::arg("x"), py::arg("y"), py::arg("side"), py::arg("outline_color"), py::arg("width"), py::arg("fill_color"), py::arg("aspect") = 1.0, py::arg("rotation") = 0.0)
            .def("square_filled_s", [](Surface& aSurface, double x, double y, double side, std::string outline_color, double width, std::string fill_color, double aspect, double rotation) { aSurface.square_filled({x, y}, Scaled{side}, Aspect{aspect}, Rotation{rotation}, Color(outline_color), Pixels{width}, Color(fill_color)); }, py::arg("x"), py::arg("y"), py::arg("side"), py::arg("outline_color"), py::arg("width"), py::arg("fill_color"), py::arg("aspect") = 1.0, py::arg("rotation") = 0.0)
            .def("path_outline", [](Surface& aSurface, const std::vector<double>& coordinates, std::string outline_color, double outline_width, bool close) { aSurface.path_outline(&*coordinates.begin(), &*coordinates.end(), Color(outline_color), Pixels{outline_width}, close); }, py::arg("path"), py::arg("outline_color"), py::arg("outline_width"), py::arg("close") = false)
            .def("path_fill", [](Surface& aSurface, const std::vector<double>& coordinates, std::string fill_color) { aSurface.path_fill(&*coordinates.begin(), &*coordinates.end(), Color(fill_color)); }, py::arg("path"), py::arg("fill_color"))
            .def("path_outline_negative_move", [](Surface& aSurface, const std::vector<double>& coordinates, std::string outline_color, double outline_width, bool close) { aSurface.path_outline_negative_move(&*coordinates.begin(), &*coordinates.end(), Color(outline_color), Pixels{outline_width}, close); }, py::arg("path"), py::arg("outline_color"), py::arg("outline_width"), py::arg("close") = false)
            .def("path_fill_negative_move", [](Surface& aSurface, const std::vector<double>& coordinates, std::string fill_color) { aSurface.path_fill_negative_move(&*coordinates.begin(), &*coordinates.end(), Color(fill_color)); }, py::arg("path"), py::arg("fill_color"))
            .def("border", [](Surface& aSurface, std::string color, double width) { const auto& v = aSurface.viewport(); aSurface.rectangle(v.origin, v.size, Color(color), Pixels{width * 2}); }, py::arg("color"), py::arg("width"))
            .def("background", [](Surface& aSurface, std::string color) { const auto& v = aSurface.viewport(); aSurface.rectangle_filled(v.origin, v.size, Color(color), Pixels{0}, Color(color)); }, py::arg("color"))
            .def("text_p", [](Surface& aSurface, double x, double y, std::string text, std::string color, double size, double rotation) { aSurface.text({x, y}, text, Color(color), Pixels{size}, TextStyle(), Rotation(rotation)); }, py::arg("x"), py::arg("y"), py::arg("text"), py::arg("color"), py::arg("size"), py::arg("rotation") = 0)
            .def("text_s", [](Surface& aSurface, double x, double y, std::string text, std::string color, double size, double rotation) { aSurface.text({x, y}, text, Color(color), Scaled{size}, TextStyle(), Rotation(rotation)); }, py::arg("x"), py::arg("y"), py::arg("text"), py::arg("color"), py::arg("size"), py::arg("rotation") = 0)
            .def("text_right_aligned_p", [](Surface& aSurface, double x, double y, std::string text, std::string color, double size, double rotation) { aSurface.text_right_aligned({x, y}, text, Color(color), Pixels{size}, TextStyle(), Rotation(rotation)); }, py::arg("x"), py::arg("y"), py::arg("text"), py::arg("color"), py::arg("size"), py::arg("rotation") = 0)
            .def("text_right_aligned_s", [](Surface& aSurface, double x, double y, std::string text, std::string color, double size, double rotation) { aSurface.text_right_aligned({x, y}, text, Color(color), Scaled{size}, TextStyle(), Rotation(rotation)); }, py::arg("x"), py::arg("y"), py::arg("text"), py::arg("color"), py::arg("size"), py::arg("rotation") = 0)
            ;

    py::class_<acmacs::surface::internal::Cairo, acmacs::surface::Surface>(m, "SurfaceCairo")
            ;

    py::class_<acmacs::surface::PdfCairo, acmacs::surface::internal::Cairo>(m, "PdfCairo")
            .def(py::init<std::string, double, double, double>(), py::arg("filename"), py::arg("width"), py::arg("height"), py::arg("viewport_width") = 1000.0)
            ;
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
