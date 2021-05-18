#include "acmacs-base/enumerate.hh"
#include "acmacs-draw/reference-panel-plot.hh"
#include "acmacs-draw/surface-cairo.hh"

// ----------------------------------------------------------------------

void acmacs::draw::ReferencePanelPlot::plot(std::string_view output_filename, const acmacs::chart::ReferencePanelPlotData::ASTable& as_table) const
{
    const size_t number_of_sera{as_table.sera.size()};
    const double hstep{static_cast<double>(as_table.data[0][0].titers.size())};
    const double vstep{hstep};
    const double title_height = vstep * 0.5;
    acmacs::surface::PdfCairo surface(output_filename, static_cast<double>(number_of_sera) * hstep, static_cast<double>(as_table.antigens.size()) * vstep + title_height,
                                      static_cast<double>(number_of_sera) * hstep);
    const acmacs::Viewport cell_viewport{acmacs::Size{hstep, vstep}};
    text(surface, {title_height, title_height * 0.7}, parameters_.title, BLACK, NoRotation, title_height * 0.8, static_cast<double>(number_of_sera) * hstep - title_height * 2);

    for (const auto [ag_no, row] : enumerate(as_table.data)) {
        for (const auto [sr_no, cell] : enumerate(row)) {
            surface::Surface& cell_surface = surface.subsurface({static_cast<double>(sr_no) * hstep, static_cast<double>(ag_no) * vstep + title_height}, Scaled{hstep}, cell_viewport, true);
            plot_cell(cell_surface, cell, as_table.antigens[ag_no], as_table.sera[sr_no], hstep, vstep);
        }
    }

} // acmacs::draw::ReferencePanelPlot::plot

// ----------------------------------------------------------------------

void acmacs::draw::ReferencePanelPlot::plot_cell(surface::Surface& cell_surface, const chart::ReferencePanelPlotData::AntigenSerumData& cell, std::string_view antigen_name, std::string_view serum_name, double hstep, double vstep) const
{
    cell_surface.rectangle(cell_surface.viewport().origin, cell_surface.viewport().size, BLACK, Pixels{0.4});

      // serum name
    text(cell_surface, {parameters_.cell_top_title_height * 1.2, parameters_.cell_top_title_height}, serum_name, BLACK, NoRotation, parameters_.cell_top_title_height, (hstep - parameters_.cell_top_title_height * 1.5));
      // antigen name
    text(cell_surface, {parameters_.cell_top_title_height, vstep - parameters_.cell_voffset_base}, antigen_name, BLACK, Rotation{-M_PI_2}, parameters_.cell_top_title_height, (vstep - parameters_.cell_voffset_base * 1.5));

} // acmacs::draw::ReferencePanelPlot::plot_cell

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
