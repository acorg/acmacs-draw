#include "acmacs-draw/draw-points.hh"

// ----------------------------------------------------------------------

void acmacs::draw::Points::draw(drawing_stage stage, surface::Surface& surface) const
{
    switch (stage) {
      case drawing_stage::points:
          break;
      case drawing_stage::labels:
          break;
      case drawing_stage::__first: case drawing_stage::background: case drawing_stage::grid: case drawing_stage::serum_circles: case drawing_stage::procrustes_arrows: case drawing_stage::legend: case drawing_stage::title: case drawing_stage::border: case drawing_stage::__last:
          break;
    }

} // acmacs::draw::Points::draw

// ----------------------------------------------------------------------



// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
