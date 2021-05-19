#include "acmacs-base/enumerate.hh"
#include "acmacs-draw/reference-panel-plot.hh"
#include "acmacs-draw/surface-cairo.hh"

// ----------------------------------------------------------------------

void acmacs::draw::ReferencePanelPlot::plot(std::string_view output_filename, const acmacs::chart::ReferencePanelPlotData::ASTable& as_table) const
{
    const size_t number_of_sera{as_table.sera.size()};
    parameters_.hstep = static_cast<double>(as_table.data[0][0].titers.size()) + 2.0 * parameters_.cell_padding_scale;
    parameters_.vstep = parameters_.hstep;
    const double title_height = parameters_.vstep * 0.5;
    acmacs::surface::PdfCairo surface(output_filename, static_cast<double>(number_of_sera) * parameters_.hstep, static_cast<double>(as_table.antigens.size()) * parameters_.vstep + title_height,
                                      static_cast<double>(number_of_sera) * parameters_.hstep);
    const acmacs::Viewport cell_viewport{acmacs::Size{parameters_.hstep, parameters_.vstep}};
    text(surface, {title_height, title_height * 0.7}, parameters_.title, BLACK, NoRotation, title_height * 0.8, static_cast<double>(number_of_sera) * parameters_.hstep - title_height * 2);

    for (const auto [ag_no, row] : enumerate(as_table.data)) {
        for (const auto [sr_no, cell] : enumerate(row)) {
            surface::Surface& cell_surface = surface.subsurface({static_cast<double>(sr_no) * parameters_.hstep, static_cast<double>(ag_no) * parameters_.vstep + title_height}, Scaled{parameters_.hstep}, cell_viewport, true);
            plot_cell(cell_surface, cell, as_table.antigens[ag_no], as_table.sera[sr_no]);
        }
    }

} // acmacs::draw::ReferencePanelPlot::plot

// ----------------------------------------------------------------------

void acmacs::draw::ReferencePanelPlot::plot_cell(surface::Surface& cell_surface, const chart::ReferencePanelPlotData::AntigenSerumData& cell, std::string_view antigen_name,
                                                 std::string_view serum_name) const
{
    cell_surface.rectangle(cell_surface.viewport().origin, cell_surface.viewport().size, BLACK, Pixels{0.4});

    // serum name
    text(cell_surface, {parameters_.cell_label_scale + parameters_.cell_padding_scale, parameters_.cell_label_scale}, serum_name, BLACK, NoRotation, parameters_.cell_label_scale,
         (parameters_.hstep - parameters_.cell_label_scale - parameters_.cell_padding_scale * 2.0));
    // antigen name
    text(cell_surface, {parameters_.cell_label_scale, parameters_.vstep - parameters_.cell_padding_scale}, antigen_name, BLACK, Rotation{-M_PI_2}, parameters_.cell_label_scale,
         (parameters_.vstep - parameters_.cell_padding_scale * 2.0));

    const double logged_titer_step = (parameters_.vstep - parameters_.cell_padding_scale * 2 - parameters_.cell_label_scale) / static_cast<double>(parameters_.titer_levels.size());
    const double median_titer_logged = cell.median_titer.logged_with_thresholded();

    double table_no{2};
    for (const auto& titer : cell.titers) {
        if (!titer.is_dont_care()) {
            const double symbol_left{table_no - 0.5 + parameters_.cell_padding_scale};
            const double titer_logged = titer.logged_with_thresholded();
            const double symbol_top = parameters_.vstep - parameters_.cell_padding_scale - (titer_logged + 2.0) * logged_titer_step;
            // AD_DEBUG("{} {} top:{}", titer, titer_logged, symbol_top);
            const double symbol_bottom = symbol_top + logged_titer_step;
            const Color symbol_color = titer_color(titer_logged, median_titer_logged);
            if (titer.is_less_than()) {
                cell_surface.triangle_filled({symbol_left, symbol_top}, {symbol_left + 1.0, symbol_top}, {symbol_left + 0.5, symbol_bottom}, TRANSPARENT, Pixels{0}, symbol_color);
            }
            else if (titer.is_more_than()) {
                cell_surface.triangle_filled({symbol_left, symbol_bottom}, {symbol_left + 1.0, symbol_bottom}, {symbol_left + 0.5, symbol_top}, TRANSPARENT, Pixels{0}, symbol_color);
            }
            else {
                cell_surface.rectangle_filled({symbol_left, symbol_top}, {1.0, logged_titer_step}, TRANSPARENT, Pixels{0}, symbol_color);
            }
        }
        ++table_no;
    }

    // titer value marks
    for (const auto [titer_label_vpos, titer_label] : enumerate(parameters_.titer_levels)) {
        cell_surface.text_right_aligned(
            {parameters_.hstep - parameters_.cell_label_scale * 0.2,
             parameters_.vstep - parameters_.cell_padding_scale - static_cast<double>(titer_label_vpos) * logged_titer_step - logged_titer_step * 0.5 + parameters_.cell_titer_scale * 0.3},
            titer_label, BLACK, Scaled{parameters_.cell_titer_scale});
    }

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

Color acmacs::draw::ReferencePanelPlot::titer_color(double titer_logged, double median_titer_logged) const
{
    const auto dist = std::abs(titer_logged - median_titer_logged);
    if (dist < 0.5)
        return parameters_.color_median;
    else if (dist < 1.5)
        return parameters_.color_next_to_median;
    else
        return parameters_.color_other;

} // acmacs::draw::ReferencePanelPlot::titer_color

// ----------------------------------------------------------------------



// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
