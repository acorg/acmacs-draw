#include "acmacs-draw/surface-cairo.hh"
#include "acmacs-draw/draw-grid.hh"

// ----------------------------------------------------------------------

void acmacs::draw::Grid::draw(drawing_stage stage, surface::Surface& surface) const
{
    if (stage == drawing_stage::grid) {
        const Viewport& v = surface.viewport();
        for (auto x = v.left_scaled() + step_; x < v.right_scaled(); x += step_)
            surface.line({x, v.top_scaled()}, {x, v.bottom_scaled()}, line_color_, line_width_);
        for (auto y = v.top_scaled() + step_; y < v.bottom_scaled(); y += step_)
            surface.line({v.left_scaled(), y}, {v.right_scaled(), y}, line_color_, line_width_);
    }

} // acmacs::draw::Grid::draw

// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
