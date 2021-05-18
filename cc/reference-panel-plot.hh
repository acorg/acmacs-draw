#include "acmacs-base/color.hh"
#include "acmacs-base/size-scale.hh"
#include "acmacs-chart-2/reference-panel-plot-data.hh"

// ----------------------------------------------------------------------

namespace acmacs
{
    class PointCoordinates;

    namespace surface
    {
        class Surface;
    }

} // namespace acmacs

namespace acmacs::draw
{
    class ReferencePanelPlot
    {
      public:
        struct Parameters
        {
            std::string title{"*title*"};
            double cell_top_title_height{1.3};
            // double cell_hstep{1};
            // double cell_vstep{1};
            double cell_voffset_base{0.1};
            double cell_voffset_per_level{0};
        };

        void plot(std::string_view output_filename, const acmacs::chart::ReferencePanelPlotData::ASTable& as_table) const;

        Parameters& parameters() { return parameters_; }

      private:
        Parameters parameters_;

        void plot_cell(surface::Surface& cell_surface, const chart::ReferencePanelPlotData::AntigenSerumData& cell, std::string_view antigen_name, std::string_view serum_name, double hstep, double vstep) const;
        void text(surface::Surface& aSurface, const PointCoordinates& aOffset, std::string_view aText, Color aColor, Rotation aRotation, double aFontSize, double aMaxWidth) const;

    };
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
