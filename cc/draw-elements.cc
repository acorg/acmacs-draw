#include "acmacs-draw/surface-cairo.hh"
#include "acmacs-draw/draw-elements.hh"
#include "acmacs-draw/draw-grid.hh"
#include "acmacs-draw/draw-legend.hh"

// ----------------------------------------------------------------------

void acmacs::draw::DrawElements::draw() const
{
    if (std::string_view(filename_.data() + filename_.size() - 4, 4) == ".pdf") {
        acmacs::surface::PdfCairo main_surface(filename_, size_, size_);
        acmacs::surface::Surface& rescaled_surface = main_surface.subsurface({0, 0}, Scaled{main_surface.viewport().size.width}, viewport_, true);
        draw(rescaled_surface);
    }
    // else if (std::string_view(filename.data() + filename.size() - 5, 5) == ".html") {
    // }
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
    // auto grid = std::make_unique<Grid>(step, line_color, line_width);
    // if (auto found = std::find_if(elements_.begin(), elements_.end(), [](const auto& elt) { return dynamic_cast<Grid*>(elt.get()) != nullptr; }); found != elements_.end())
    //     *found = std::move(grid);
    // else
    //     elements_.push_back(std::move(grid));

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



// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
