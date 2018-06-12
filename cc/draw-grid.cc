#include "acmacs-draw/surface.hh"
#include "acmacs-draw/surface-js-static.hh"
#include "acmacs-draw/surface-js-dynamic.hh"
#include "acmacs-draw/draw-grid.hh"

// ======================================================================

void acmacs::draw::Grid::draw(drawing_stage stage, surface::Surface& surface) const
{
    if (stage == drawing_stage::grid) {
        const Viewport& v = surface.viewport();
        for (auto x = v.left_scaled() + step_; x < v.right_scaled(); x += step_)
            surface.line({x.value(), v.top_scaled().value()}, {x.value(), v.bottom_scaled().value()}, line_color_, line_width_);
        for (auto y = v.top_scaled() + step_; y < v.bottom_scaled(); y += step_)
            surface.line({v.left_scaled().value(), y.value()}, {v.right_scaled().value(), y.value()}, line_color_, line_width_);
    }

} // acmacs::draw::Grid::draw

// ----------------------------------------------------------------------

void acmacs::draw::Grid::draw(drawing_stage stage, surface::JsStatic& surface) const
{
    if (stage == drawing_stage::grid)
        surface.func("grid", "__context", "viewport", step_, line_color_, line_width_);

} // acmacs::draw::Grid::draw

// ----------------------------------------------------------------------

void acmacs::draw::Grid::draw(drawing_stage stage, surface::JsDynamic& surface) const
{
    if (stage == drawing_stage::grid)
        surface.add_field("grid", rjson::object{{{"step", surface.convert(step_)}, {"line_color", surface.convert(line_color_)}, {"line_width", surface.convert(line_width_)}}});
    //     surface.func("grid", "__context", "viewport", step_, line_color_, line_width_);

} // acmacs::draw::Grid::draw

// ======================================================================

void acmacs::draw::Background::draw(drawing_stage stage, surface::Surface& surface) const
{
    if (stage == drawing_stage::background) {
        const Viewport& v = surface.viewport();
        surface.rectangle_filled(v.origin, v.size, color_, Pixels{0}, color_);
    }

} // acmacs::draw::Background::draw

// ----------------------------------------------------------------------

void acmacs::draw::Background::draw(drawing_stage stage, surface::JsDynamic& surface) const
{
    if (stage == drawing_stage::background)
        surface.add_field("background", rjson::object{{{"color", surface.convert(color_)}}});

} // acmacs::draw::Background::draw

// ----------------------------------------------------------------------

void acmacs::draw::Background::draw(drawing_stage stage, surface::JsStatic& surface) const
{
    if (stage == drawing_stage::background) {
        surface.context_assign("fillStyle", color_);
        surface.context_func("fillRect", "viewport[0]", "viewport[1]", "viewport[2]", "viewport[3]");
    }

} // acmacs::draw::Background::draw

// ======================================================================

void acmacs::draw::Border::draw(drawing_stage stage, surface::Surface& surface) const
{
    if (stage == drawing_stage::border) {
        const Viewport& v = surface.viewport();
        surface.rectangle(v.origin, v.size, line_color_, line_width_ * 2);
    }

} // acmacs::draw::Border::draw

// ----------------------------------------------------------------------

void acmacs::draw::Border::draw(drawing_stage stage, surface::JsDynamic& surface) const
{
    if (stage == drawing_stage::border)
        surface.add_field("border", rjson::object{{{"line_color", surface.convert(line_color_)}, {"line_width", surface.convert(line_width_)}}});

} // acmacs::draw::Border::draw

// ----------------------------------------------------------------------

void acmacs::draw::Border::draw(drawing_stage /*stage*/, surface::JsStatic& /*surface*/) const
{

} // acmacs::draw::Border::draw

// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
