#include "acmacs-base/layout.hh"
#include "acmacs-base/range.hh"
#include "acmacs-draw/draw-points.hh"

// ----------------------------------------------------------------------

double acmacs::draw::PointLabel::text_offset(double offset_hint, double point_size, double text_size, bool text_origin_at_opposite)
{
    double offset = 0;
    if (offset_hint < -1) {
        offset += point_size * (offset_hint + 0.5) - (text_origin_at_opposite ? 0 : text_size);
    }
    else if (offset_hint < 1) {
        offset += point_size * offset_hint / 2 + (text_origin_at_opposite ? (text_size * (offset_hint + 1) / 2) : (text_size * (offset_hint - 1) / 2));
    }
    else {
        offset += point_size * (offset_hint - 0.5) + (text_origin_at_opposite ? text_size : 0);
    }
    return offset;

} // acmacs::draw::PointLabel::text_offset

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
          draw_labels(surface);
          break;
      case drawing_stage::__first: case drawing_stage::background: case drawing_stage::grid: case drawing_stage::serum_circles: case drawing_stage::procrustes_arrows: case drawing_stage::legend: case drawing_stage::title: case drawing_stage::border: case drawing_stage::__last:
          break;
    }

} // acmacs::draw::Points::draw

// ----------------------------------------------------------------------

void acmacs::draw::Points::draw_points(surface::Surface& surface) const
{
    std::unique_ptr<LayoutInterface> layout{layout_->transform(transformation_)};

    for (auto point_no : drawing_order_) {
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

void acmacs::draw::Points::draw_labels(surface::Surface& surface) const
{
    if (labels_) {
        std::unique_ptr<LayoutInterface> layout{layout_->transform(transformation_)};
        for (const auto& label : *labels_) {
            const auto index = label.index();
            if (const auto styl = style(index); *styl.shown) {
                if (auto text_origin = layout->get(index); !text_origin.empty()) { // point is not disconnected
                    const double scaled_point_size = surface.convert(Pixels{*styl.size}).value();
                    const acmacs::Size ts = surface.text_size(label.display_name(), label.text_size(), label.text_style());
                    text_origin[0] += label.text_offset(label.offset().x, scaled_point_size, ts.width, false);
                    text_origin[1] += label.text_offset(label.offset().y, scaled_point_size, ts.height, true);
                    surface.text(text_origin, label.display_name(), label.text_color(), label.text_size(), label.text_style());
                }
            }
        }
    }

} // acmacs::draw::Points::draw_labels

// ----------------------------------------------------------------------



// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
