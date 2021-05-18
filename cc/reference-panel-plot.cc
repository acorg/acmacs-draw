#include "acmacs-base/enumerate.hh"
#include "acmacs-draw/reference-panel-plot.hh"
#include "acmacs-draw/surface-cairo.hh"

// ----------------------------------------------------------------------

void acmacs::draw::ReferencePanelPlot::plot(std::string_view output_filename, const acmacs::chart::ReferencePanelPlotData::ASTable& data) const
{
    const size_t number_of_sera{data[0].size()};
    const double hstep{static_cast<double>(data[0][0].titers.size())};
    const double vstep{hstep};
    const double title_height = vstep * 0.5;
    acmacs::surface::PdfCairo surface(output_filename, static_cast<double>(number_of_sera) * hstep, static_cast<double>(data.size()) * vstep + title_height,
                                      static_cast<double>(number_of_sera) * hstep);
    const acmacs::Viewport cell_viewport{acmacs::Size{hstep, vstep}};
    text(surface, {title_height, title_height * 0.7}, parameters_.title, BLACK, NoRotation, title_height * 0.8, static_cast<double>(number_of_sera) * hstep - title_height * 2);

    for (const auto [ag_no, row] : enumerate(data)) {
        for (const auto [sr_no, cell] : enumerate(row)) {
            surface::Surface& cell_surface = surface.subsurface({static_cast<double>(sr_no) * hstep, static_cast<double>(ag_no) * vstep + title_height}, Scaled{hstep}, cell_viewport, true);
            cell_surface.rectangle(cell_surface.viewport().origin, cell_surface.viewport().size, BLACK, Pixels{0.4});
        }
    }

} // acmacs::draw::ReferencePanelPlot::plot

// ----------------------------------------------------------------------

void acmacs::draw::ReferencePanelPlot::text(surface::Surface& aSurface, const PointCoordinates& aOffset, std::string_view aText, Color aColor, Rotation aRotation, double aFontSize,
                                            double aMaxWidth) const
{
    const auto size = aSurface.text_size(aText, Scaled{aFontSize});
    if (size.width > aMaxWidth)
        aFontSize *= aMaxWidth / size.width;
    aSurface.text(aOffset, aText, aColor, Scaled{aFontSize}, acmacs::TextStyle(), aRotation);

} // acmacs::draw::ReferencePanelPlot::text

// ----------------------------------------------------------------------



// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
