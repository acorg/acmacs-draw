#include "acmacs-draw/surface.hh"
#include "acmacs-draw/draw-legend.hh"

// ----------------------------------------------------------------------

void acmacs::draw::Window::draw_window(surface::Surface& surface) const
{
    const auto& v = surface.viewport();
    surface.rectangle_filled(v.origin, v.size, background_, Pixels{0}, background_);
    surface.rectangle(v.origin, v.size, border_color_, border_width_);

} // acmacs::draw::Window::draw_window

// ----------------------------------------------------------------------

void acmacs::draw::Window::draw(drawing_stage, surface::Surface& surface) const
{
    acmacs::surface::Surface& window_surface = surface.subsurface(origin_, Scaled{size_.width}, size_, false);
    draw_window(window_surface);
    draw_content(window_surface);

} // acmacs::draw::Window::draw

// ----------------------------------------------------------------------

void acmacs::draw::Title::draw(drawing_stage stage, surface::Surface& surface) const
{
    if (stage == drawing_stage::title) {
        set_size(surface);
        Window::draw(stage, surface);
    }

} // acmacs::draw::Title::draw

// ----------------------------------------------------------------------

void acmacs::draw::Title::draw_content(surface::Surface& surface) const
{
    const double padding = surface.convert(padding_).value();
    const double text_x = padding;
    double y = padding + height();
    for (const auto& line: lines_) {
        surface.text({text_x, y}, line, text_color_, text_size_, text_style_);
        y += height() * interline_;
    }
    std::cerr << "WARNING: acmacs::draw::Title::draw_content not imlemented\n";

} // acmacs::draw::Window::draw_content

// ----------------------------------------------------------------------

void acmacs::draw::Title::set_size(surface::Surface& surface) const
{
    double width = 0, height = 0;
    for (const auto& line : lines_) {
        const acmacs::Size line_size = surface.text_size(line, text_size_, text_style_);
        if (line_size.width > width)
            width = line_size.width;
        if (line_size.height > height)
            height = line_size.height;
    }
    const double padding = surface.convert(padding_).value();
    size({width + padding * 2, height * (lines_.size() - 1) * interline_ + height + padding * 2});

} // acmacs::draw::Title::set_size

// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
