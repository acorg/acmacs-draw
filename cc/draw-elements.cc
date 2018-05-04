#include "acmacs-draw/surface-cairo.hh"
#include "acmacs-draw/surface-js-static.hh"
#include "acmacs-draw/surface-js-dynamic.hh"
#include "acmacs-draw/draw-elements.hh"
#include "acmacs-draw/draw-grid.hh"
#include "acmacs-draw/draw-legend.hh"
#include "acmacs-draw/draw-points.hh"
#include "acmacs-draw/draw-arrow.hh"

// Tree
// Tree time series
// Clade regions

// ----------------------------------------------------------------------

void acmacs::draw::DrawElements::draw() const
{
    if (std::string_view(filename_.data() + filename_.size() - 4, 4) == ".pdf") {
        acmacs::surface::PdfCairo main_surface(filename_, size_, size_);
        acmacs::surface::Surface& rescaled_surface = main_surface.subsurface({0, 0}, Scaled{main_surface.viewport().size.width}, viewport_, true);
        draw(rescaled_surface);
    }
    else if (std::string_view(filename_.data() + filename_.size() - 5, 5) == ".html") {
        acmacs::surface::JsStatic surface(filename_, {size_, size_}, viewport_);
        draw(surface);
    }
    else if (is_json_buffer()) {
        acmacs::surface::JsDynamic surface({}, {0, 0}, viewport_);
        draw(surface);
        output_ = surface.write_to_string();
    }
    else if (is_json()) {
        acmacs::surface::JsDynamic surface(filename_, {size_, size_}, viewport_);
        draw(surface);
    }
    else {
        throw std::runtime_error("Unrecognized filename suffix: " + filename_);
    }

} // acmacs::draw::DrawElements::draw

// ----------------------------------------------------------------------

template <typename E> inline E& replace_or_add(std::unique_ptr<E> element, std::vector<std::unique_ptr<acmacs::draw::Element>>& elements)
{
    auto& result = *element;
    if (auto found = std::find_if(elements.begin(), elements.end(), [](const auto& elt) { return dynamic_cast<E*>(elt.get()) != nullptr; }); found != elements.end())
        *found = std::move(element);
    else
        elements.push_back(std::move(element));
    return result;
}

// ----------------------------------------------------------------------

void acmacs::draw::DrawElements::grid(Scaled step, Color line_color, Pixels line_width)
{
    replace_or_add(std::make_unique<Grid>(step, line_color, line_width), elements_);

} // acmacs::draw::DrawElements::grid

// ----------------------------------------------------------------------

void acmacs::draw::DrawElements::background(Color color)
{
    replace_or_add(std::make_unique<Background>(color), elements_);

} // acmacs::draw::DrawElements::background

// ----------------------------------------------------------------------

void acmacs::draw::DrawElements::border(Color line_color, Pixels line_width)
{
    replace_or_add(std::make_unique<Border>(line_color, line_width), elements_);

} // acmacs::draw::DrawElements::border

// ----------------------------------------------------------------------

acmacs::draw::Title& acmacs::draw::DrawElements::title(const std::vector<std::string>& lines)
{
    return replace_or_add(std::make_unique<Title>(lines), elements_);

} // acmacs::draw::DrawElements::title

// ----------------------------------------------------------------------

acmacs::draw::Legend& acmacs::draw::DrawElements::legend()
{
    return replace_or_add(std::make_unique<Legend>(), elements_);

} // acmacs::draw::DrawElements::legend

// ----------------------------------------------------------------------

acmacs::draw::Points& acmacs::draw::DrawElements::points(std::shared_ptr<acmacs::LayoutInterface> layout, const acmacs::Transformation& transformation)
{
    return replace_or_add(std::make_unique<Points>(layout, transformation), elements_);

} // acmacs::draw::DrawElements::points

// ----------------------------------------------------------------------

void acmacs::draw::DrawElements::line(const acmacs::Location& from, const acmacs::Location& to, Color line_color, Pixels line_width)
{
    elements_.push_back(std::make_unique<Line>(from ,to, line_color, line_width));

} // acmacs::draw::DrawElements::line

// ----------------------------------------------------------------------

void acmacs::draw::DrawElements::arrow(const acmacs::Location& from, const acmacs::Location& to, Color line_color, Pixels line_width, Color arrow_head_color, bool arrow_head_filled, Pixels arrow_width)
{
    elements_.push_back(std::make_unique<Arrow>(from ,to, line_color, line_width, arrow_head_color, arrow_head_filled, arrow_width));

} // acmacs::draw::DrawElements::arrow

// ----------------------------------------------------------------------

void acmacs::draw::DrawElements::rectangle(const acmacs::Location& corner1, const acmacs::Location& corner2, Color color, bool filled, Pixels line_width)
{
    elements_.push_back(std::make_unique<Rectangle>(corner1, corner2, color, filled, line_width));

} // acmacs::draw::DrawElements::rectangle

// ----------------------------------------------------------------------

void acmacs::draw::DrawElements::circle(const acmacs::Location& center, Scaled size, Color fill_color, Color outline_color, Pixels outline_width, Aspect aspect, Rotation rotation)
{
    elements_.push_back(std::make_unique<Circle<Scaled>>(center, size, fill_color, outline_color, outline_width, aspect, rotation));

} // acmacs::draw::DrawElements::circle

// ----------------------------------------------------------------------

void acmacs::draw::DrawElements::point(const acmacs::Location& center, Pixels size, Color fill_color, Color outline_color, Pixels outline_width, Aspect aspect, Rotation rotation)
{
    elements_.push_back(std::make_unique<Circle<Pixels>>(center, size, fill_color, outline_color, outline_width, aspect, rotation));

} // acmacs::draw::DrawElements::point

// ----------------------------------------------------------------------

void acmacs::draw::DrawElements::sector(const acmacs::Location& center, Scaled size, Color fill_color, Color outline_color, Pixels outline_width, Color radius_color, Pixels radius_width, acmacs::surface::Dash radius_dash, Rotation start, Rotation end)
{
    elements_.push_back(std::make_unique<Sector>(center, size, fill_color, outline_color, outline_width, radius_color, radius_width, radius_dash, start, end));

} // acmacs::draw::DrawElements::sector

// ----------------------------------------------------------------------

void acmacs::draw::DrawElements::serum_circle(const Coordinates& coordinates, const acmacs::Transformation& transformation, Scaled size, Color fill_color, Color outline_color, Pixels outline_width, Color radius_color, Pixels radius_width, acmacs::surface::Dash radius_dash, Rotation start, Rotation end)
{
    elements_.push_back(std::make_unique<SerumCircle>(coordinates, transformation, size, fill_color, outline_color, outline_width, radius_color, radius_width, radius_dash, start, end));

} // acmacs::draw::DrawElements::serum_circle

// ----------------------------------------------------------------------

void acmacs::draw::DrawElements::continent_map(const acmacs::Location& origin, Pixels size)
{
    replace_or_add(std::make_unique<ContinentMap>(origin, size), elements_);

} // acmacs::draw::DrawElements::continent_map

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
