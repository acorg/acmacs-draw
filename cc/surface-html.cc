#include "acmacs-draw/surface-html.hh"

static void write_header(std::ostream& output, double aWidth, double aHeight);
static void write_html_body(std::ostream& output);

// ----------------------------------------------------------------------

acmacs::surface::Html::Html(std::string aFilename, double aWidth, double aHeight, double aViewportWidth)
    : output_(aFilename)
{
    change_width_in_parent(aWidth);
    viewport({Location{0, 0}, Size{aViewportWidth, aHeight * aViewportWidth / aWidth}});
    write_header(output_, aWidth, aHeight);

} // acmacs::surface::Html::Html

// ----------------------------------------------------------------------

acmacs::surface::Html::~Html()
{
    write_html_body(output_);

} // acmacs::surface::Html::~Html

// ----------------------------------------------------------------------

class context
{
 public:
    context(acmacs::surface::internal::Javascript& surface)
        : output_(surface.output()), scale_(surface.scale()), indent_("        ")
        {
            const auto& viewport = surface.viewport();
            const auto origin = surface.origin_offset();
            output_ << indent_ << "__context.save()\n";
            translate(-origin.x, -origin.y);
            scale(scale_);
            translate(-viewport.origin.x, -viewport.origin.y);
            // if (surface.clip()) {
            //     new_path();
            //     move_to(viewport.origin);
            //     line_to(viewport.top_right());
            //     line_to(viewport.bottom_right());
            //     line_to(viewport.bottom_left());
            //     close_path();
            //     clip();
            // }
        }

    ~context()
        {
            output_ << indent_ << "__context.restore()\n";
        }

    double to_str(double v) { return v; }

    template <typename S> context& set_line_width(S aWidth) { output_ << indent_ << "__context.lineWidth = " << to_str(convert(aWidth)) << ";\n"; return *this; }
    context& set_stroke(Color aColor) { output_ << indent_ << "__context.strokeStyle = \"" << aColor.to_hex_string() << "\";\n"; return *this; }
    context& set_fill(Color aColor) { output_ << indent_ << "__context.fillStyle = \"" << aColor.to_hex_string() << "\";\n"; return *this; }
    context& set_line_cap(acmacs::surface::Surface::LineCap aLineCap) { output_ << indent_ << "__context.lineCap = \"" << canvas_line_cap(aLineCap) << "\";\n"; return *this; }
    context& set_line_join(acmacs::surface::Surface::LineJoin aLineJoin) { output_ << indent_ << "__context.lineJoin = \"" << canvas_line_join(aLineJoin) << "\";\n"; return *this; }

    context& begin_path() { output_ << indent_ << "__context.beginPath();\n"; return *this; }
    context& close_path() { output_ << indent_ << "__context.closePath();\n"; return *this; }
    context& move_to(double x, double y) { output_ << indent_ << "__context.moveTo(" << to_str(x) << ',' << to_str(y) << ");\n"; return *this; }
    context& move_to() { return move_to(0, 0); }
    context& move_to(const acmacs::Location& a) { return move_to(a.x, a.y); }
    template <typename S> context& move_to(S x, S y) { return move_to(convert(x), convert(y)); }
    context& line_to(double x, double y) { output_ << indent_ << "__context.lineTo(" << to_str(x) << ',' << to_str(y) << ");\n"; return *this; }
    context& line_to(const acmacs::Location& a) { return line_to(a.x, a.y); }
    template <typename S> context& line_to(S x, S y) { return line_to(convert(x), convert(y)); }
    context& rectangle_wh(double x, double y, double w, double h) { output_ << indent_ << "__context.rect(" << to_str(x) << ',' << to_str(y) << ',' << to_str(w) << ',' << to_str(h) << ");\n"; return *this; }
    template <typename S> context& rectangle(S x1, S y1, S x2, S y2) { return rectangle_wh(convert(x1), convert(y1), convert(x2) - convert(x1), convert(y2) - convert(y1)); }
    context& rectangle(const acmacs::Location& a, const acmacs::Size& s) { return rectangle_wh(a.x, a.y, s.width, s.height); }
    template <typename S> context& circle(S radius) { output_ << indent_ << "__context.arc(0,0," << to_str(convert(radius)) << ",0,2*Math.PI);\n"; return *this; }
    template <typename S> context& arc(S radius, Rotation start, Rotation end) { output_ << indent_ << "__context.arc(0,0," << to_str(convert(radius)) << ',' << to_str(start.value()) << ',' << to_str(end.value()) << ");\n"; return *this; }
    context& circle(const acmacs::Location& a, double radius) { output_ << indent_ << "__context.arc(" << to_str(a.x) << ',' << to_str(a.y) << ',' << to_str(convert(radius)) << ",0,2*Math.PI);\n"; return *this; }

    context& stroke() { output_ << indent_ << "__context.stroke();\n"; return *this; }
    context& fill() { output_ << indent_ << "__context.fill();\n"; return *this; }
    // context& stroke_preserve() { cairo_stroke_preserve(cairo_context()); return *this; }

    context& translate(double x, double y) { output_ << indent_ << "__context.translate(" << to_str(x) << ',' << to_str(y) << ");\n"; return *this; }
    context& translate(const acmacs::Size& a) { return translate(a.width, a.height); }
    context& translate(const acmacs::Location& a) { return translate(a.x, a.y); }
    context& rotate(Rotation aAngle) { output_ << indent_ << "__context.rotate(" << to_str(aAngle.value()) << ");\n"; return *this; }
    context& scale(double x, double y) { output_ << indent_ << "__context.scale(" << to_str(x) << ',' << to_str(y) << ");\n"; return *this; }
    context& scale(double x) { return scale(x, x); }
    context& aspect(Aspect x) { return scale(x.value(), 1.0); }

 private:
    std::ostream& output_;
    const double scale_;
    const char* const indent_;

    double convert(double aValue) { return aValue; }
    double convert(Scaled aValue) { return aValue.value(); }
    double convert(Pixels aValue) { return aValue.value() / scale_; }

    const char* canvas_line_cap(acmacs::surface::Surface::LineCap aLineCap) const
        {
            switch (aLineCap) {
              case acmacs::surface::Surface::LineCap::Butt:
                  return "butt";
              case acmacs::surface::Surface::LineCap::Round:
                  return "round";
              case acmacs::surface::Surface::LineCap::Square:
                  return "square";
            }
            return "butt"; // gcc wants return
        }

    const char* canvas_line_join(acmacs::surface::Surface::LineJoin aLineJoin) const
        {
            switch (aLineJoin) {
              case acmacs::surface::Surface::LineJoin::Miter:
                  return "miter";
              case acmacs::surface::Surface::LineJoin::Round:
                  return "round";
              case acmacs::surface::Surface::LineJoin::Bevel:
                  return "bevel";
            }
            return "miter"; // gcc wants return
        }

}; // class context

// ----------------------------------------------------------------------

template <typename S> static inline void s_line(acmacs::surface::internal::Javascript& surface, const acmacs::Location& a, const acmacs::Location& b, Color aColor, S aWidth, acmacs::surface::Surface::LineCap aLineCap)
{
    context(surface)
            .begin_path()
            .set_line_width(aWidth)
            .set_stroke(aColor)
            .set_line_cap(aLineCap)
            .move_to(a)
            .line_to(b)
            .stroke();
}

void acmacs::surface::internal::Javascript::line(const Location& a, const Location& b, Color aColor, Pixels aWidth, LineCap aLineCap)
{
    s_line(*this, a, b, aColor, aWidth, aLineCap);

} // acmacs::surface::internal::Javascript::line

void acmacs::surface::internal::Javascript::line(const Location& a, const Location& b, Color aColor, Scaled aWidth, LineCap aLineCap)
{
    s_line(*this, a, b, aColor, aWidth, aLineCap);

} // acmacs::surface::internal::Javascript::line

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::rectangle(const Location& a, const Size& s, Color aColor, Pixels aWidth, LineCap aLineCap)
{
    context(*this)
            .begin_path()
            .set_line_width(aWidth)
            .set_line_cap(aLineCap)
            .rectangle(a, s)
            .set_stroke(aColor)
            .stroke();

} // acmacs::surface::internal::Javascript::rectangle

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::rectangle_filled(const Location& a, const Size& s, Color aOutlineColor, Pixels aWidth, Color aFillColor, LineCap aLineCap)
{
    context(*this)
            .begin_path()
            .set_line_width(aWidth)
            .set_line_cap(aLineCap)
            .rectangle(a, s)
            .set_fill(aFillColor)
            .set_stroke(aOutlineColor)
            .fill()
            .stroke();

} // acmacs::surface::internal::Javascript::rectangle_filled

// ----------------------------------------------------------------------

template <typename S> static inline void s_circle(acmacs::surface::internal::Javascript& aSurface, const acmacs::Location& aCenter, S aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth)
{
    context(aSurface)
            .begin_path()
            .set_line_width(aOutlineWidth)
            .translate(aCenter)
            .rotate(aAngle)
            .aspect(aAspect)
            .circle(aDiameter / 2)
            .set_stroke(aOutlineColor)
            .stroke();
}

void acmacs::surface::internal::Javascript::circle(const Location& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth)
{
    s_circle(*this, aCenter, aDiameter, aAspect, aAngle, aOutlineColor, aOutlineWidth);

} // acmacs::surface::internal::Javascript::circle

void acmacs::surface::internal::Javascript::circle(const Location& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth)
{
    s_circle(*this, aCenter, aDiameter, aAspect, aAngle, aOutlineColor, aOutlineWidth);

} // acmacs::surface::internal::Javascript::circle

// ----------------------------------------------------------------------

template <typename S> static inline void s_circle_filled(acmacs::surface::internal::Javascript& aSurface, const acmacs::Location& aCenter, S aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor)
{
    context(aSurface)
            .begin_path()
            .set_line_width(aOutlineWidth)
            .translate(aCenter)
            .rotate(aAngle)
            .aspect(aAspect)
            .circle(aDiameter / 2)
            .set_fill(aFillColor)
            .set_stroke(aOutlineColor)
            .fill()
            .stroke();
}

void acmacs::surface::internal::Javascript::circle_filled(const Location& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor)
{
    s_circle_filled(*this, aCenter, aDiameter, aAspect, aAngle, aOutlineColor, aOutlineWidth, aFillColor);

} // acmacs::surface::internal::Javascript::circle_filled

void acmacs::surface::internal::Javascript::circle_filled(const Location& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor)
{
    s_circle_filled(*this, aCenter, aDiameter, aAspect, aAngle, aOutlineColor, aOutlineWidth, aFillColor);

} // acmacs::surface::internal::Javascript::circle_filled

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::sector_filled(const Location& aCenter, Scaled aDiameter, Rotation aStart, Rotation aEnd, Color aOutlineColor, Pixels aOutlineWidth, Color aRadiusColor, Pixels aRadiusWidth, Dash aRadiusDash, Color aFillColor)
{

} // acmacs::surface::internal::Javascript::sector_filled

// ----------------------------------------------------------------------

template <typename S> static inline void s_square_filled(acmacs::surface::internal::Javascript& aSurface, const acmacs::Location& aCenter, S aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, acmacs::surface::Surface::LineCap aLineCap)
{
    context(aSurface)
            .begin_path()
            .set_line_width(aOutlineWidth)
            .set_line_cap(aLineCap)
            .translate(aCenter)
            .rotate(aAngle)
            .rectangle(- aSide / 2 * aAspect.value(), - aSide / 2, aSide / 2 * aAspect.value(), aSide / 2)
            .set_fill(aFillColor)
            .set_stroke(aOutlineColor)
            .fill()
            .stroke();
}

void acmacs::surface::internal::Javascript::square_filled(const Location& aCenter, Pixels aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap)
{
    s_square_filled(*this, aCenter, aSide, aAspect, aAngle, aOutlineColor, aOutlineWidth, aFillColor, aLineCap);

} // acmacs::surface::internal::Javascript::square_filled

void acmacs::surface::internal::Javascript::square_filled(const Location& aCenter, Scaled aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap)
{
    s_square_filled(*this, aCenter, aSide, aAspect, aAngle, aOutlineColor, aOutlineWidth, aFillColor, aLineCap);

} // acmacs::surface::internal::Javascript::square_filled

// ----------------------------------------------------------------------

template <typename S> static inline void s_triangle_filled(acmacs::surface::internal::Javascript& aSurface, const acmacs::Location& aCenter, S aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, acmacs::surface::Surface::LineCap aLineCap)
{
    const auto cos_pi_6 = std::cos(M_PI / 6.0);
    const auto radius = aSide * cos_pi_6;
    context(aSurface)
            .begin_path()
            .set_line_width(aOutlineWidth)
            .set_line_cap(aLineCap)
            .translate(aCenter)
            .rotate(aAngle)
            .move_to(S{0}, - radius)
            .line_to(- radius * cos_pi_6 * aAspect.value(), radius * 0.5)
            .line_to(radius * cos_pi_6 * aAspect.value(), radius * 0.5)
            .close_path()
            .set_fill(aFillColor)
            .set_stroke(aOutlineColor)
            .fill()
            .stroke();
}

void acmacs::surface::internal::Javascript::triangle_filled(const Location& aCenter, Pixels aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap)
{
    s_triangle_filled(*this, aCenter, aSide, aAspect, aAngle, aOutlineColor, aOutlineWidth, aFillColor, aLineCap);

} // acmacs::surface::internal::Javascript::triangle_filled

void acmacs::surface::internal::Javascript::triangle_filled(const Location& aCenter, Scaled aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap)
{
    s_triangle_filled(*this, aCenter, aSide, aAspect, aAngle, aOutlineColor, aOutlineWidth, aFillColor, aLineCap);

} // acmacs::surface::internal::Javascript::triangle_filled

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::triangle_filled(const Location& aCorner1, const Location& aCorner2, const Location& aCorner3, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap)
{
    context(*this)
            .begin_path()
            .set_line_width(aOutlineWidth)
            .set_line_cap(aLineCap)
            .move_to(aCorner1)
            .line_to(aCorner2)
            .line_to(aCorner3)
            .close_path()
            .set_fill(aFillColor)
            .fill()
            .set_stroke(aOutlineColor)
            .stroke();

} // acmacs::surface::internal::Javascript::triangle_filled

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::path_outline_negative_move(std::vector<Location>::const_iterator first, std::vector<Location>::const_iterator last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose, LineCap aLineCap)
{

} // acmacs::surface::internal::Javascript::path_outline_negative_move

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::path_outline_negative_move(const double* first, const double* last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose, LineCap aLineCap)
{

} // acmacs::surface::internal::Javascript::path_outline_negative_move

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::path_fill_negative_move(std::vector<Location>::const_iterator first, std::vector<Location>::const_iterator last, Color aFillColor)
{

} // acmacs::surface::internal::Javascript::path_fill_negative_move

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::path_fill_negative_move(const double* first, const double* last, Color aFillColor)
{

} // acmacs::surface::internal::Javascript::path_fill_negative_move

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::path_outline(std::vector<Location>::const_iterator first, std::vector<Location>::const_iterator last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose, LineCap aLineCap)
{
    // context(*this)
    //         .begin_path()
    //         .set_line_cap(aLineCap)
    //         .set_line_join(LineJoin::Miter)
    //         .set_line_width(aOutlineWidth)
    //         .set_stroke(aOutlineColor)
    //         .move_to_first_line_to_rest(first, last)
    //         .close_path_if(aClose)
    //         .stroke();

} // acmacs::surface::internal::Javascript::path_outline

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::path_outline(const double* first, const double* last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose, LineCap aLineCap)
{

} // acmacs::surface::internal::Javascript::path_outline

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::path_fill(std::vector<Location>::const_iterator first, std::vector<Location>::const_iterator last, Color aFillColor)
{

} // acmacs::surface::internal::Javascript::path_fill

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::path_fill(const double* first, const double* last, Color aFillColor)
{

} // acmacs::surface::internal::Javascript::path_fill

// ----------------------------------------------------------------------

// template <typename S> static inline void s_text(acmacs::surface::internal::Javascript& aSurface, const acmacs::Location& a, std::string aText, Color aColor, S aSize, const acmacs::TextStyle& aTextStyle, Rotation aRotation)
// {
//     context(aSurface)
//             .begin_path()
//             .prepare_for_text(aSize, aTextStyle)
//             .move_to(a)
//             .rotate(aRotation)
//             .set_source_rgba(aColor)
//             .show_text(aText)
//             ;        // clear text path (bug in cairo?)
// }

// template <typename S> static inline acmacs::Size s_text_size(acmacs::surface::internal::Javascript& aSurface, std::string aText, S aSize, const acmacs::TextStyle& aTextStyle, double* x_bearing)
// {
//     cairo_text_extents_t text_extents;
//     context(aSurface)
//             .prepare_for_text(aSize, aTextStyle)
//             .text_extents(aText, text_extents);
//     if (x_bearing != nullptr)
//         *x_bearing = text_extents.x_bearing;
//       // std::cerr << "s_text_size [" << aText << "] " << text_extents.x_advance << ' ' << text_extents.y_bearing << ' ' << text_extents.x_bearing << '\n';
//     return {text_extents.x_advance < 0 ? 0.0 : text_extents.x_advance, - text_extents.y_bearing};
// }

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::text(const Location& a, std::string aText, Color aColor, Pixels aSize, const TextStyle& aTextStyle, Rotation aRotation)
{

} // acmacs::surface::internal::Javascript::text

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::text(const Location& a, std::string aText, Color aColor, Scaled aSize, const TextStyle& aTextStyle, Rotation aRotation)
{

} // acmacs::surface::internal::Javascript::text

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::text_right_aligned(const Location& aEnd, std::string aText, Color aColor, Pixels aSize, const TextStyle& aTextStyle, Rotation aRotation)
{

} // acmacs::surface::internal::Javascript::text_right_aligned

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::text_right_aligned(const Location& aEnd, std::string aText, Color aColor, Scaled aSize, const TextStyle& aTextStyle, Rotation aRotation)
{

} // acmacs::surface::internal::Javascript::text_right_aligned

// ----------------------------------------------------------------------

acmacs::Size acmacs::surface::internal::Javascript::text_size(std::string aText, Pixels aSize, const TextStyle& aTextStyle, double* x_bearing)
{
    return {1, 1};

} // acmacs::surface::internal::Javascript::text_size

// ----------------------------------------------------------------------

acmacs::Size acmacs::surface::internal::Javascript::text_size(std::string aText, Scaled aSize, const TextStyle& aTextStyle, double* x_bearing)
{
    return {1, 1};

} // acmacs::surface::internal::Javascript::text_size

// ----------------------------------------------------------------------

acmacs::surface::Surface* acmacs::surface::internal::Javascript::make_child(const Location& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport, bool aClip)
{
    return new acmacs::surface::internal::JavascriptChild(*this, aOriginInParent, aWidthInParent, aViewport, aClip);

} // acmacs::surface::internal::Javascript::make_child

// ----------------------------------------------------------------------

void write_header(std::ostream& output, double aWidth, double aHeight)
{
    output << R"(<html>
  <head>
    <meta charset="utf-8" />
    <style>
      #canvas {
        margin: 1em;
        border: 1px solid black;
      }
    </style>
    <title>acmacs-draw</title>
    <script>
      function main() {
        var canvas = document.getElementById('canvas');
        canvas.width = )" << aWidth << R"(;
        canvas.height = )" << aHeight << R"(;
        var ctx = canvas.getContext('2d');
        var start = new Date();
        draw(ctx);
        var elapsed = new Date() - start;
        console.log("drawing time: " + elapsed + "ms -> " + (1000 / elapsed).toFixed(1) + "fps");
      }
      function draw(__context) {
)";

} // write_header

// ----------------------------------------------------------------------

void write_html_body(std::ostream& output)
{
    output << R"(          }
      document.addEventListener('DOMContentLoaded', main, false);
    </script>
  </head>
  <body>
    <canvas id="canvas"></canvas>
  </body>
</html>
)";

} // write_html_body

// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
