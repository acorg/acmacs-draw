#include "acmacs-base/layout.hh"
#include "acmacs-base/range.hh"
#include "acmacs-base/range-v3.hh"
#include "acmacs-base/regex.hh"
#include "acmacs-draw/surface.hh"
#include "acmacs-draw/draw-points.hh"
#include "acmacs-draw/point-style-data.hh"

// ----------------------------------------------------------------------

acmacs::draw::PointLabel& acmacs::draw::PointLabel::operator=(const PointLabel& rhs)
{
    show_ = rhs.show_;
    if (!std::isnan(rhs.offset_.x()))
        offset_ = rhs.offset_;
    if (rhs.display_name_.has_value())
        display_name_ = rhs.display_name_;
    text_color_.add(rhs.text_color_);
    if (*rhs.text_size_ > 0)
        text_size_ = rhs.text_size_;
    text_style_ = rhs.text_style_;
    return *this;

} // acmacs::draw::PointLabel::operator=

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

acmacs::draw::PointRefs::PointRefs(Points& a_points)
    : std::vector<size_t>(a_points.layout_->number_of_points()),
      points_{a_points}
{
    ranges::copy(ranges::views::ints(0ul, size()), begin());
}

// ----------------------------------------------------------------------

void acmacs::draw::PointRefs::filter_by_name(std::string_view pattern)
{
    if (pattern.size() > 1 && pattern[0] == '~') {
        const std::regex re{std::next(std::begin(pattern), 1), std::end(pattern), acmacs::regex::icase};
        erase(std::remove_if(begin(), end(),
                             [&re, this](size_t point_no) {
                                 const auto name = points().add_label(point_no).display_name();
                                 return !std::regex_search(std::begin(name), std::end(name), re);
                             }),
              end());
    }
    else {
        erase(std::remove_if(begin(), end(), [pattern, this](size_t point_no) { return points().add_label(point_no).display_name() != pattern; }), end());
    }

} // acmacs::draw::PointRefs::filter_by_name

// ----------------------------------------------------------------------

acmacs::draw::Points::Points()
    : layout_{std::make_shared<acmacs::Layout>()},
      styles_{std::make_shared<PointStylesData>()},
      labels_here_{std::make_unique<PointLabels>()},
      labels_{labels_here_.get()}
{
} // acmacs::draw::Points::Points

// ----------------------------------------------------------------------

acmacs::draw::Points::Points(std::shared_ptr<acmacs::Layout> layout, const acmacs::Transformation& transformation)
    : layout_{layout},
      transformation_{transformation},
      drawing_order_{acmacs::filled_with_indexes(layout->number_of_points())}
{
} // acmacs::draw::Points::Points

// ----------------------------------------------------------------------

std::pair<size_t, acmacs::PointStyle*> acmacs::draw::Points::add(const PointCoordinates& coord)
{
    const auto point_no = layout_->append_point();
    layout_->update(point_no, coord);
    drawing_order_.push_back(point_no);
    auto* styles = dynamic_cast<PointStylesData*>(styles_.get());
    if (!styles)
        throw std::runtime_error {"acmacs::draw::Points::add: invalid styles value"};
    return {point_no, &styles->add(default_style_)};

} // acmacs::draw::Points::add

// ----------------------------------------------------------------------

void acmacs::draw::Points::draw_points(surface::Surface& surface) const
{
    std::shared_ptr<Layout> layout{layout_->transform(transformation_)};

    for (auto point_no : drawing_order_) {
        if (layout->point_has_coordinates(point_no)) {
            if (const auto& styl = style(point_no); styl.shown()) {
                // AD_DEBUG("diameter {} size {}", styl.diameter(), styl.size());
                if (styl.diameter() > Scaled{0.0}) {
                    switch (styl.shape()) {
                        case PointShape::Circle:
                            surface.circle_filled(layout->at(point_no), styl.diameter(), styl.aspect(), styl.rotation(), styl.outline(), styl.outline_width(), surface::Dash::NoDash, styl.fill());
                            break;
                        case PointShape::Egg:
                            surface.egg_filled(layout->at(point_no), styl.diameter(), styl.aspect(), styl.rotation(), styl.outline(), styl.outline_width(), surface::Dash::NoDash, styl.fill());
                            break;
                        case PointShape::UglyEgg:
                            surface.ugly_egg_filled(layout->at(point_no), styl.diameter(), styl.aspect(), styl.rotation(), styl.outline(), styl.outline_width(), surface::Dash::NoDash, styl.fill());
                            break;
                        case PointShape::Box:
                            surface.square_filled(layout->at(point_no), styl.diameter(), styl.aspect(), styl.rotation(), styl.outline(), styl.outline_width(), styl.fill());
                            break;
                        case PointShape::Triangle:
                            surface.triangle_filled(layout->at(point_no), styl.diameter(), styl.aspect(), styl.rotation(), styl.outline(), styl.outline_width(), styl.fill());
                            break;
                    }
                }
                else {
                    switch (styl.shape()) {
                        case PointShape::Circle:
                            surface.circle_filled(layout->at(point_no), styl.size(), styl.aspect(), styl.rotation(), styl.outline(), styl.outline_width(), surface::Dash::NoDash, styl.fill());
                            break;
                        case PointShape::Egg:
                            surface.egg_filled(layout->at(point_no), styl.size(), styl.aspect(), styl.rotation(), styl.outline(), styl.outline_width(), surface::Dash::NoDash, styl.fill());
                            break;
                        case PointShape::UglyEgg:
                            surface.ugly_egg_filled(layout->at(point_no), styl.size(), styl.aspect(), styl.rotation(), styl.outline(), styl.outline_width(), surface::Dash::NoDash, styl.fill());
                            break;
                        case PointShape::Box:
                            surface.square_filled(layout->at(point_no), styl.size(), styl.aspect(), styl.rotation(), styl.outline(), styl.outline_width(), styl.fill());
                            break;
                        case PointShape::Triangle:
                            surface.triangle_filled(layout->at(point_no), styl.size(), styl.aspect(), styl.rotation(), styl.outline(), styl.outline_width(), styl.fill());
                            break;
                    }
                }
            }
        }
    }

} // acmacs::draw::Points::draw_points

// ----------------------------------------------------------------------

// void acmacs::draw::Points::draw_points(surface::JsStatic& surface) const
// {
//     const auto cos_pi_12 = std::cos(M_PI / 12.0);
//     std::unique_ptr<Layout> layout{layout_->transform(transformation_)};

//     surface.context_assign("lineCap", "\"butt\"");
//     for (auto point_no : drawing_order_) {
//         if (layout->point_has_coordinates(point_no)) {
//             if (const auto& styl = style(point_no); styl.shown) {
//                 surface::JsStatic::ContextSave save(surface);
//                 const auto coord = layout->at(point_no);
//                 const auto size = surface.convert(styl.size);
//                 const auto aspect = styl.aspect->value();
//                 surface.context_assign("lineWidth", styl.outline_width);
//                 surface.context_assign("fillStyle", styl.fill);
//                 surface.context_assign("strokeStyle", styl.outline);
//                 surface.context_func("translate", coord[0], coord[1]);
//                 if (styl.rotation != NoRotation)
//                     surface.context_func("rotate", styl.rotation);
//                 switch (styl.shape) {
//                     case acmacs::PointShape::Circle:
//                         if (styl.aspect != AspectNormal)
//                             surface.context_func("scale", aspect, 1);
//                         surface.context_func("beginPath");
//                         surface.context_func("arc", 0, 0, size / 2, 0, "2*Math.PI");
//                         surface.context_func("fill");
//                         surface.context_func("stroke");
//                         break;
//                     case acmacs::PointShape::Box:
//                         surface.context_func("fillRect", -size * aspect, -size, size * aspect, size);
//                         surface.context_func("strokeRect", -size * aspect, -size, size * aspect, size);
//                         break;
//                     case acmacs::PointShape::Triangle:
//                         surface.context_func("beginPath");
//                         surface.context_func("move_to", 0, -size / 2);
//                         surface.context_func("line_to", -size * cos_pi_12 * aspect, size / 4);
//                         surface.context_func("line_to", size * cos_pi_12 * aspect, size / 4);
//                         surface.context_func("closePath");
//                         surface.context_func("fill");
//                         surface.context_func("stroke");
//                         break;
//                 }
//             }
//         }
//     }

// } // acmacs::draw::Points::draw_points

// ----------------------------------------------------------------------

// void acmacs::draw::Points::draw_points(surface::JsDynamic& surface) const
// {
//     surface.add_field("drawing_order", surface.convert(drawing_order_));
//     surface.add_field("layout", surface.convert(*layout_));
//     surface.add_field("transformation", surface.convert(transformation_));
//     const auto styles = styles_->compacted();
//     surface.add_field("style_index", surface.convert(styles.index));
//     rjson::v1::array& target_styles = surface.add_array("styles");
//     for (const auto& styl : styles.styles) {
//         rjson::v1::object target_style;
//         auto set_if_not_default = [&](const char* name, const auto& field) { if (field.not_default()) target_style.set_field(name, surface.convert(*field)); };
//         set_if_not_default("shown", styl.shown);
//         set_if_not_default("fill", styl.fill);
//         set_if_not_default("outline", styl.outline);
//         set_if_not_default("outline_width", styl.outline_width);
//         set_if_not_default("size", styl.size);
//         set_if_not_default("rotation", styl.rotation);
//         set_if_not_default("aspect", styl.aspect);
//         target_style.set_field("shape", rjson::v1::string{styl.shape});
//         target_styles.insert(std::move(target_style));
//     }

// } // acmacs::draw::Points::draw_points

// // ======================================================================

// void acmacs::draw::Points::draw_labels(surface::JsDynamic& surface) const
// {
//     if (labels_) {
//         auto& target_labels = surface.add_array("labels");
//         target_labels.resize(layout_->number_of_points());
//         for (const auto& label : *labels_)
//             target_labels[label.index()] = rjson::v1::string(label.display_name());
//     }

// } // acmacs::draw::Points::draw_labels

// ----------------------------------------------------------------------

void acmacs::draw::Points::draw_labels(surface::Surface& surface) const
{
    if (labels_) {
        std::shared_ptr<Layout> layout{layout_->transform(transformation_)};
        for (const auto& label : *labels_) {
            if (label.show()) {
                const auto index = label.index();
                if (const auto styl = style(index); styl.shown()) {
                    if (auto text_origin = layout->at(index).copy(); text_origin.exists()) { // point is not disconnected (coordinates exist)
                        const double scaled_point_size = surface.convert(styl.size()).value();
                        const acmacs::Size ts = surface.text_size(label.display_name(), label.text_size(), label.text_style());
                        text_origin[number_of_dimensions_t{0}] += label.text_offset(label.offset().x(), scaled_point_size, ts.width, false);
                        text_origin[number_of_dimensions_t{1}] += label.text_offset(label.offset().y(), scaled_point_size, ts.height, true);
                        surface.text(text_origin, label.display_name(), label.text_color(), label.text_size(), label.text_style());
                    }
                }
            }
        }
    }

} // acmacs::draw::Points::draw_labels

// ----------------------------------------------------------------------

// void acmacs::draw::Points::draw_labels(surface::JsStatic& /*surface*/) const
// {
//     std::cerr << "WARNING: acmacs::draw::Points::draw_labels() not implemented\n";

// } // acmacs::draw::Points::draw_labels

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
