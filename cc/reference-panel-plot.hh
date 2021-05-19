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

            mutable double hstep{1};
            mutable double vstep{1};

            double cell_label_scale{2.0};
            double cell_padding_scale{2.0};
            double cell_voffset_base{0.1};
            double cell_voffset_per_level{0};

            std::vector<std::string> titer_levels{"5", "10", "20", "40", "80", "160", "320", "640", "1280", "2560", "5120", "10240", "20480", "40960"};

            Color color_median{0x00CD00};
            Color color_next_to_median{0xCDCD00};
            Color color_other{RED};
        };

        void plot(std::string_view output_filename, const chart::ReferencePanelPlotData::ASTable& as_table) const;

        Parameters& parameters() { return parameters_; }

      private:
        Parameters parameters_;

        void plot_cell(surface::Surface& cell_surface, const chart::ReferencePanelPlotData::AntigenSerumData& cell, std::string_view antigen_name, std::string_view serum_name) const;
        void text(surface::Surface& aSurface, const PointCoordinates& aOffset, std::string_view aText, Color aColor, Rotation aRotation, double aFontSize, double aMaxWidth) const;
        Color titer_color(double titer_logged, double median_titer_logged) const;
    };
} // namespace acmacs::draw

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
