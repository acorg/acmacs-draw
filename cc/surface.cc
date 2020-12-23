#include "acmacs-draw/surface.hh"

using namespace acmacs::surface;

// ----------------------------------------------------------------------

void Surface::grid(Scaled aStep, Color aLineColor, Pixels aLineWidth)
{
    const Viewport& v = viewport();
    for (auto x = v.left_scaled() + aStep; x < v.right_scaled(); x += aStep)
        line({x.value(), v.top_scaled().value()}, {x.value(), v.bottom_scaled().value()}, aLineColor, aLineWidth);
    for (auto y = v.top_scaled() + aStep; y < v.bottom_scaled(); y += aStep)
        line({v.left_scaled().value(), y.value()}, {v.right_scaled().value(), y.value()}, aLineColor, aLineWidth);

} // Surface::grid

// ----------------------------------------------------------------------

void Surface::arrow(const PointCoordinates& a, const PointCoordinates& b, Color aColor, Pixels aLineWidth, Pixels aArrowWidth, arrow_head_at aha)
{
    const bool x_eq = float_equal(b.x(), a.x());
    const double sign2 = x_eq ? (a.y() < b.y() ? 1.0 : -1.0) : (b.x() < a.x() ? 1.0 : -1.0);
    const double angle = x_eq ? -M_PI_2 : std::atan((b.y() - a.y()) / (b.x() - a.x()));
    PointCoordinates la{a}, lb{b};
    if (aha == arrow_head_at::first || aha == arrow_head_at::both)
        la = arrow_head(a, angle, - sign2, aColor, aArrowWidth, true);
    if (aha == arrow_head_at::second || aha == arrow_head_at::both)
        lb = arrow_head(b, angle,   sign2, aColor, aArrowWidth, true);
    line(la, lb, aColor, aLineWidth);

} // Surface::arrow

// ----------------------------------------------------------------------

// void Surface::double_arrow(const PointCoordinates& a, const PointCoordinates& b, Color aColor, Pixels aLineWidth, Pixels aArrowWidth)
// {
//     const bool x_eq = float_equal(b.x(), a.x());
//     const double sign2 = x_eq ? (a.y() < b.y() ? 1.0 : -1.0) : (b.x() < a.x() ? 1.0 : -1.0);
//     const double angle = x_eq ? -M_PI_2 : std::atan((b.y() - a.y()) / (b.x() - a.x()));
//     auto const la = arrow_head(a, angle, - sign2, aColor, aArrowWidth, true);
//     auto const lb = arrow_head(b, angle,   sign2, aColor, aArrowWidth, true);
//     line(la, lb, aColor, aLineWidth);

// } // Surface::double_arrow

// ----------------------------------------------------------------------

acmacs::PointCoordinates Surface::arrow_head(const PointCoordinates& arrow_end, double angle, double sign, Color aColor, Pixels aArrowWidth, bool aFilled)
{
    constexpr double ARROW_WIDTH_TO_LENGTH_RATIO = 2.0;

    const double arrow_width = aArrowWidth.value() / scale();
    const double arrow_length = arrow_width * ARROW_WIDTH_TO_LENGTH_RATIO;
    const PointCoordinates line_attachment_point(arrow_end.x() + sign * arrow_length * std::cos(angle), arrow_end.y() + sign * arrow_length * std::sin(angle));
    const PointCoordinates arrow_base_1(line_attachment_point.x() + sign * arrow_width * std::cos(angle + M_PI_2) * 0.5, line_attachment_point.y() + sign * arrow_width * std::sin(angle + M_PI_2) * 0.5);
    const PointCoordinates arrow_base_2(line_attachment_point.x() + sign * arrow_width * std::cos(angle - M_PI_2) * 0.5, line_attachment_point.y() + sign * arrow_width * std::sin(angle - M_PI_2) * 0.5);

    const std::vector<PointCoordinates> path{arrow_end, arrow_base_1, arrow_base_2};
    if (aFilled)
        path_fill(path.begin(), path.end(), aColor);
    else
        path_outline(path.begin(), path.end(), aColor, Pixels{1}, true, LineCap::Butt);

    return line_attachment_point;

} // Surface::arrow_head

// ----------------------------------------------------------------------

acmacs::PointCoordinates Surface::arrow_head(const PointCoordinates& arrow_end, double angle, double sign, Pixels aArrowWidth, Color outline, Pixels outline_width, Color fill)
{
    constexpr double ARROW_WIDTH_TO_LENGTH_RATIO = 2.0;

    const double arrow_width = aArrowWidth.value() / scale();
    const double arrow_length = arrow_width * ARROW_WIDTH_TO_LENGTH_RATIO;
    const PointCoordinates line_attachment_point(arrow_end.x() + sign * arrow_length * std::cos(angle), arrow_end.y() + sign * arrow_length * std::sin(angle));
    const PointCoordinates arrow_base_1(line_attachment_point.x() + sign * arrow_width * std::cos(angle + M_PI_2) * 0.5, line_attachment_point.y() + sign * arrow_width * std::sin(angle + M_PI_2) * 0.5);
    const PointCoordinates arrow_base_2(line_attachment_point.x() + sign * arrow_width * std::cos(angle - M_PI_2) * 0.5, line_attachment_point.y() + sign * arrow_width * std::sin(angle - M_PI_2) * 0.5);

    const std::vector<PointCoordinates> path{arrow_end, arrow_base_1, arrow_base_2};
    if (fill != TRANSPARENT)
        path_fill(path.begin(), path.end(), fill);
    path_outline(path.begin(), path.end(), outline, outline_width, true, LineCap::Butt);

    return line_attachment_point;

} // Surface::arrow_head

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
