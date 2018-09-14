#include "acmacs-draw/draw-legend.hh"
// #include "acmacs-draw/surface-js-static.hh"
// #include "acmacs-draw/surface-js-dynamic.hh"
#include "acmacs-draw/continent-map.hh"

// ----------------------------------------------------------------------

void acmacs::draw::internal::Window::draw_window(surface::Surface& surface) const
{
    const auto& v = surface.viewport();
    surface.rectangle_filled(v.origin, v.size, background_, Pixels{0}, background_);
    surface.rectangle(v.origin, v.size, border_color_, border_width_);

} // acmacs::draw::internal::Window::draw_window

// ----------------------------------------------------------------------

void acmacs::draw::internal::Window::draw(drawing_stage, surface::Surface& surface) const
{
    acmacs::surface::Surface& window_surface = surface.subsurface(scaled_origin(surface), Scaled{size_.width}, size_, false);
    draw_window(window_surface);
    draw_content(window_surface);

} // acmacs::draw::internal::Window::draw

// ----------------------------------------------------------------------

// void acmacs::draw::internal::Window::draw_window(surface::JsStatic& surface) const
// {
//     surface.context_assign("fillStyle", background_);
//     surface.context_func("fillRect", "viewport[0]", "viewport[1]", "viewport[2]", "viewport[3]");

// } // acmacs::draw::internal::Window::draw_window

// ----------------------------------------------------------------------

// void acmacs::draw::internal::Window::draw(drawing_stage /*stage*/, surface::JsStatic& /*surface*/) const
// {
//     // surface::JsStatic::WithSubsurface subs(surface, scaled_origin(surface), Scaled{size_.width}, size_, false);
//     // draw_window(surface);
//       //draw_content(surface);

// } // acmacs::draw::internal::Window::draw

// ----------------------------------------------------------------------

acmacs::Location2D acmacs::draw::internal::Window::scaled_origin(surface::Surface& surface) const
{
    acmacs::Location2D subsurface_origin{surface.convert(Pixels{origin_.x()}).value(), surface.convert(Pixels{origin_.y()}).value()};
    scaled_origin_adjust(subsurface_origin, surface.viewport().size);
    return subsurface_origin;

} // acmacs::draw::internal::Window::scaled_origin

// ----------------------------------------------------------------------

// acmacs::Location2D acmacs::draw::internal::Window::scaled_origin(surface::JsStatic& surface) const
// {
//     acmacs::Location2D subsurface_origin{surface.convert(Pixels{origin_.x()}), surface.convert(Pixels{origin_.y()})};
//     scaled_origin_adjust(subsurface_origin, surface.viewport().size);
//     return subsurface_origin;

// } // acmacs::draw::internal::Window::scaled_origin

// ----------------------------------------------------------------------

void acmacs::draw::internal::Window::scaled_origin_adjust(acmacs::Location2D& origin, const acmacs::Size& surface_size) const
{
    if (origin.x() < 0)
        origin.x(origin.x() + surface_size.width - size_.width);
    if (origin.y() < 0)
        origin.y(origin.y() + surface_size.height - size_.height);

} // acmacs::draw::internal::Window::scaled_origin_adjust

// ----------------------------------------------------------------------

acmacs::Size acmacs::draw::internal::LegendPointLabel::size(surface::Surface& surface) const
{
    const auto txt_sz = TitleLine::size(surface);
    const auto point_lable_sep = surface.text_size("O", text_size(), text_style());
    const auto scaled_point_size = surface.convert(point_size_).value();
    return {txt_sz.width + point_lable_sep.width + scaled_point_size, txt_sz.height};

} // acmacs::draw::internal::LegendPointLabel::size

// ----------------------------------------------------------------------

void acmacs::draw::internal::LegendPointLabel::draw(surface::Surface& surface, acmacs::Location2D origin, double height) const
{
    const auto scaled_point_size = surface.convert(point_size_).value();
    const auto point_x = origin.x() + scaled_point_size / 2;
    const auto text_x = origin.x() * 2 + scaled_point_size;
    surface.circle_filled({point_x, origin.y() + height / 2}, point_size_, AspectNormal, NoRotation, point_outline_, Pixels{1}, point_fill_);
    TitleLine::draw(surface, {text_x, origin.y() + height});

} // acmacs::draw::internal::LegendPointLabel::draw

// ----------------------------------------------------------------------

// void acmacs::draw::internal::TitleLine::draw(surface::JsDynamic& surface, const char* field) const
// {
//     auto& target = surface.add_array(field);
//     target.insert(rjson::v1::object{{{"text", rjson::v1::string{text_}}, {"color", surface.convert(text_color_)}, {"size", surface.convert(text_size_)}}});

// } // acmacs::draw::internal::TitleLine::draw

// ----------------------------------------------------------------------

void acmacs::draw::ContinentMap::draw(drawing_stage stage, surface::Surface& surface) const
{
    if (stage == drawing_stage::legend) {
        acmacs::Location2D origin = origin_;
        if (origin.x() < 0)
            origin.x(origin.x() + surface.width_in_pixels() - size_.value());
        if (origin.y() < 0)
            origin.y(origin.y() + surface.height_in_pixels() - size_.value() / continent_map_aspect());
        acmacs::surface::Surface& continent_surface = surface.subsurface(origin, size_, continent_map_size(), true);
        continent_map_draw(continent_surface);
    }

} // acmacs::draw::ContinentMap::draw

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
