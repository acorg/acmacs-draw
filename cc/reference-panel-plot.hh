#include "acmacs-chart-2/reference-panel-plot-data.hh"

// ----------------------------------------------------------------------

namespace acmacs::draw
{
    class ReferencePanelPlot
    {
      public:
        void plot(std::string_view output_filename, const acmacs::chart::ReferencePanelPlotData& data);

      private:
        struct Parameters
        {
        };

        Parameters parameters_;
    };
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
