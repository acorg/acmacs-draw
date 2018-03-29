#include "acmacs-draw/draw-elements-pdf.hh"
#include "acmacs-draw/surface-cairo.hh"

// ----------------------------------------------------------------------

template <> void acmacs::draw::DrawElementsOn<acmacs::surface::PdfCairo>::grid(double step)
{

} // acmacs::draw::DrawElementsOn<>::grid

// ----------------------------------------------------------------------

template class acmacs::draw::DrawElementsOn<acmacs::surface::PdfCairo>;

// void acmacs::draw::DrawElementsPdf::grid(double step)
// {

// } // acmacs::draw::DrawElementsPdf::grid

// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
