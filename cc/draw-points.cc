#include "acmacs-base/layout.hh"
#include "acmacs-base/range.hh"
#include "acmacs-draw/surface.hh"
#include "acmacs-draw/draw-points.hh"

// ----------------------------------------------------------------------

acmacs::draw::Points::Points(std::shared_ptr<acmacs::LayoutInterface> layout, const acmacs::Transformation& transformation)
    : layout_(layout), transformation_(transformation), drawing_order_(acmacs::filled_with_indexes(layout->number_of_points()))
{

} // acmacs::draw::Points::Points

// ----------------------------------------------------------------------

void acmacs::draw::Points::draw(drawing_stage stage, surface::Surface& surface) const
{
    switch (stage) {
      case drawing_stage::points:
          draw_points(surface);
          break;
      case drawing_stage::labels:
          break;
      case drawing_stage::__first: case drawing_stage::background: case drawing_stage::grid: case drawing_stage::serum_circles: case drawing_stage::procrustes_arrows: case drawing_stage::legend: case drawing_stage::title: case drawing_stage::border: case drawing_stage::__last:
          break;
    }

} // acmacs::draw::Points::draw

// ----------------------------------------------------------------------

void acmacs::draw::Points::draw_points(surface::Surface& surface) const
{
    std::unique_ptr<LayoutInterface> layout{layout_->transform(transformation_)};

    for (auto point_no : *drawing_order_) {
        if (layout->point_has_coordinates(point_no)) {
            if (const auto& styl = style(point_no); *styl.shown) {
                switch (*styl.shape) {
                    case acmacs::PointShape::Circle:
                        surface.circle_filled(layout->get(point_no), Pixels{*styl.size}, *styl.aspect, *styl.rotation, *styl.outline, *styl.outline_width, *styl.fill);
                        break;
                    case acmacs::PointShape::Box:
                        surface.square_filled(layout->get(point_no), Pixels{*styl.size}, *styl.aspect, *styl.rotation, *styl.outline, *styl.outline_width, *styl.fill);
                        break;
                    case acmacs::PointShape::Triangle:
                        surface.triangle_filled(layout->get(point_no), Pixels{*styl.size}, *styl.aspect, *styl.rotation, *styl.outline, *styl.outline_width, *styl.fill);
                        break;
                }
            }
        }
    }

} // acmacs::draw::Points::draw_points

// ----------------------------------------------------------------------

const acmacs::PointStyle& acmacs::draw::Points::style(size_t point_no) const
{
    if (unpacked_styles_)
        return (*unpacked_styles_)[point_no];
    return default_style_;

} // acmacs::draw::Points::style

// ----------------------------------------------------------------------



// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
