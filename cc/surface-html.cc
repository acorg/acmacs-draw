#include "acmacs-draw/surface-html.hh"

static void write_header(std::ostream& output);
static void write_html_body(std::ostream& output);

// ----------------------------------------------------------------------

acmacs::surface::Html::Html(std::string aFilename, double aWidth, double aHeight, double aViewportWidth)
    : output_(aFilename)
{
    write_header(output_);
    // auto surface = cairo_pdf_surface_create(aFilename.empty() ? nullptr : aFilename.c_str(), aWidth, aHeight);
    // mCairoContext = cairo_create(surface);
    change_width_in_parent(aWidth);
    viewport({Location{0, 0}, Size{aViewportWidth, aHeight * aViewportWidth / aWidth}});
      // std::cerr << *this << std::endl;

} // acmacs::surface::Html::Html

// ----------------------------------------------------------------------

acmacs::surface::Html::~Html()
{
    write_html_body(output_);

} // acmacs::surface::Html::~Html

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::line(const Location& a, const Location& b, Color aColor, Pixels aWidth, LineCap aLineCap)
{

} // acmacs::surface::internal::Javascript::line

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::line(const Location& a, const Location& b, Color aColor, Scaled aWidth, LineCap aLineCap)
{

} // acmacs::surface::internal::Javascript::line

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::rectangle(const Location& a, const Size& s, Color aColor, Pixels aWidth, LineCap aLineCap)
{

} // acmacs::surface::internal::Javascript::rectangle

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::rectangle_filled(const Location& a, const Size& s, Color aOutlineColor, Pixels aWidth, Color aFillColor, LineCap aLineCap)
{

} // acmacs::surface::internal::Javascript::rectangle_filled

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::circle(const Location& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth)
{

} // acmacs::surface::internal::Javascript::circle

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::circle(const Location& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth)
{

} // acmacs::surface::internal::Javascript::circle

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::circle_filled(const Location& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor)
{

} // acmacs::surface::internal::Javascript::circle_filled

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::circle_filled(const Location& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor)
{

} // acmacs::surface::internal::Javascript::circle_filled

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::sector_filled(const Location& aCenter, Scaled aDiameter, Rotation aStart, Rotation aEnd, Color aOutlineColor, Pixels aOutlineWidth, Color aRadiusColor, Pixels aRadiusWidth, Dash aRadiusDash, Color aFillColor)
{

} // acmacs::surface::internal::Javascript::sector_filled

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::square_filled(const Location& aCenter, Pixels aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap)
{

} // acmacs::surface::internal::Javascript::square_filled

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::square_filled(const Location& aCenter, Scaled aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap)
{

} // acmacs::surface::internal::Javascript::square_filled

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::triangle_filled(const Location& aCenter, Pixels aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap)
{

} // acmacs::surface::internal::Javascript::triangle_filled

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::triangle_filled(const Location& aCenter, Scaled aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap)
{

} // acmacs::surface::internal::Javascript::triangle_filled

// ----------------------------------------------------------------------

void acmacs::surface::internal::Javascript::triangle_filled(const Location& aCorner1, const Location& aCorner2, const Location& aCorner3, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap)
{

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

} // acmacs::surface::internal::Javascript::text_size

// ----------------------------------------------------------------------

acmacs::Size acmacs::surface::internal::Javascript::text_size(std::string aText, Scaled aSize, const TextStyle& aTextStyle, double* x_bearing)
{

} // acmacs::surface::internal::Javascript::text_size

// ----------------------------------------------------------------------

acmacs::surface::Surface* acmacs::surface::internal::Javascript::make_child(const Location& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport, bool aClip)
{
    return new acmacs::surface::internal::JavascriptChild(*this, aOriginInParent, aWidthInParent, aViewport, aClip);

} // acmacs::surface::internal::Javascript::make_child

// ----------------------------------------------------------------------

void write_header(std::ostream& output)
{
    output << "<html>\n<head>\n";

} // write_header

// ----------------------------------------------------------------------

void write_html_body(std::ostream& output)
{
    output << "</head>\n<body></body>\n</html>\n";

} // write_html_body

// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
