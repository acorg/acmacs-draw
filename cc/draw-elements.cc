#include "acmacs-draw/surface-cairo.hh"
#include "acmacs-draw/draw-elements.hh"
#include "acmacs-draw/draw-grid.hh"

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

void acmacs::draw::DrawElements::grid(Scaled step, Color line_color, Pixels line_width)
{
    auto grid = std::make_unique<Grid>(step, line_color, line_width);
    if (auto found = std::find_if(elements_.begin(), elements_.end(), [](const auto& elt) { return dynamic_cast<Grid*>(elt.get()) != nullptr; }); found != elements_.end())
        *found = std::move(grid);
    else
        elements_.push_back(std::move(grid));

} // acmacs::draw::DrawElements::grid

// ----------------------------------------------------------------------



// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
