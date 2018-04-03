#include "acmacs-draw/surface.hh"
#include "acmacs-draw/draw-arrow.hh"

// ----------------------------------------------------------------------

void acmacs::draw::Line::draw(drawing_stage stage, surface::Surface& surface) const
{
    if (stage == drawing_stage::procrustes_arrows)
        surface.line(from_, to_, line_color_, line_width_);

} // acmacs::draw::Line::draw

// ----------------------------------------------------------------------

void acmacs::draw::Arrow::draw(drawing_stage stage, surface::Surface& surface) const
{
    if (stage == drawing_stage::procrustes_arrows) {
        const bool x_eq = float_equal(to().x, from().x);
        const double sign2 = x_eq ? (from().y < to().y ? 1.0 : -1.0) : (to().x < from().x ? 1.0 : -1.0);
        const double angle = x_eq ? -M_PI_2 : std::atan((to().y - from().y) / (to().x - from().x));
        const auto end = surface.arrow_head(to(), angle, sign2, arrow_head_color_, arrow_width_, arrow_head_filled_);
        surface.line(from(), end, line_color(), line_width());
    }

} // acmacs::draw::Arrow::draw

// ----------------------------------------------------------------------

void acmacs::draw::Rectangle::draw(drawing_stage stage, surface::Surface& surface) const
{
    if (stage == drawing_stage::procrustes_arrows) {
        const std::vector<acmacs::Location> path{corner1_, {corner1_.x, corner2_.y}, corner2_, {corner2_.x, corner1_.y}};
        if (filled_)
            surface.path_fill(path.begin(), path.end(), color_);
        else
            surface.path_outline(path.begin(), path.end(), color_, line_width_, true);
    }

} // acmacs::draw::Rectangle::draw

// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
