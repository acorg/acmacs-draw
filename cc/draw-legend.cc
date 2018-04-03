#include "acmacs-draw/draw-legend.hh"

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

// void acmacs::draw::Title::draw(drawing_stage stage, surface::Surface& surface) const
// {
//     if (stage == drawing_stage::title) {
//         set_size(surface);
//         Window::draw(stage, surface);
//     }

// } // acmacs::draw::Title::draw

// ----------------------------------------------------------------------

// void acmacs::draw::Title::draw_content(surface::Surface& surface) const
// {
//     const double padding = surface.convert(padding_).value();
//     const double text_x = padding;
//     double y = padding + line_height_;
//     for (const auto& line: lines_) {
//         surface.text({text_x, y}, line, text_color_, text_size_, text_style_);
//         y += line_height_ * interline_;
//     }

// } // acmacs::draw::Window::draw_content

// ----------------------------------------------------------------------

// void acmacs::draw::Title::set_size(surface::Surface& surface) const
// {
//     double width = 0;
//     for (const auto& line : lines_) {
//         const acmacs::Size line_size = surface.text_size(line, text_size_, text_style_);
//         if (line_size.width > width)
//             width = line_size.width;
//         if (line_size.height > line_height_)
//             line_height_ = line_size.height;
//     }
//     const double padding = surface.convert(padding_).value();
//     size({width + padding * 2, line_height_ * (lines_.size() - 1) * interline_ + line_height_ + padding * 2});

// } // acmacs::draw::Title::set_size

// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
