#include "acmacs-draw/draw-elements.hh"

// ----------------------------------------------------------------------

extern template class acmacs::draw::DrawElementsOn<acmacs::surface::PdfCairo>;

// ----------------------------------------------------------------------

std::shared_ptr<acmacs::draw::DrawElements> acmacs::draw::factory(std::string filename, double size)
{
    if (std::string_view(filename.data() + filename.size() - 4, 4) == ".pdf") {
        return std::make_shared<DrawElementsOn<acmacs::surface::PdfCairo>>(filename, size);
    }
    // else if (std::string_view(filename.data() + filename.size() - 5, 5) == ".html") {
    // }
    else {
        throw std::runtime_error("Unrecognized filename suffix: " + filename);
    }

} // acmacs::draw::factory

// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
