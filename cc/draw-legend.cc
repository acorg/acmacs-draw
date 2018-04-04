#include "acmacs-draw/draw-legend.hh"
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

acmacs::Location acmacs::draw::internal::Window::scaled_origin(surface::Surface& surface) const
{
    acmacs::Location subsurface_origin{surface.convert(Pixels{origin_.x}).value(), surface.convert(Pixels{origin_.y}).value()};
    const acmacs::Size& surface_size = surface.viewport().size;
    if (subsurface_origin.x < 0)
        subsurface_origin.x += surface_size.width - size_.width;
    if (subsurface_origin.y < 0)
        subsurface_origin.y += surface_size.height - size_.height;
    return subsurface_origin;

} // acmacs::draw::internal::Window::scaled_origin

// ----------------------------------------------------------------------

acmacs::Size acmacs::draw::internal::LegendPointLabel::size(surface::Surface& surface) const
{
    const auto txt_sz = TitleLine::size(surface);
    const auto point_lable_sep = surface.text_size("O", text_size(), text_style());
    const auto scaled_point_size = surface.convert(point_size_).value();
    return {txt_sz.width + point_lable_sep.width + scaled_point_size, txt_sz.height};

} // acmacs::draw::internal::LegendPointLabel::size

// ----------------------------------------------------------------------

void acmacs::draw::internal::LegendPointLabel::draw(surface::Surface& surface, const acmacs::Location& origin, double height) const
{
    const auto scaled_point_size = surface.convert(point_size_).value();
    const auto point_x = origin.x + scaled_point_size / 2;
    const auto text_x = origin.x * 2 + scaled_point_size;
    surface.circle_filled({point_x, origin.y + height / 2}, point_size_, AspectNormal, NoRotation, point_outline_, Pixels{1}, point_fill_);
    TitleLine::draw(surface, {text_x, origin.y + height});

} // acmacs::draw::internal::LegendPointLabel::draw

// ----------------------------------------------------------------------

void acmacs::draw::ContinentMap::draw(drawing_stage stage, surface::Surface& surface) const
{
    if (stage == drawing_stage::legend) {
        acmacs::Location origin = origin_;
        if (origin.x < 0)
            origin.x += surface.width_in_pixels() - size_.value();
        if (origin.y < 0)
            origin.y += surface.height_in_pixels() - size_.value() / continent_map_aspect();
        acmacs::surface::Surface& continent_surface = surface.subsurface(origin, size_, continent_map_size(), true);
        continent_map_draw(continent_surface);
    }

} // acmacs::draw::ContinentMap::draw

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
