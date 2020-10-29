#pragma once

#include <optional>
#include <memory>
#include <vector>

#include "acmacs-base/sfinae.hh"
#include "acmacs-base/transformation.hh"
#include "acmacs-base/point-style.hh"
#include "acmacs-base/text-style.hh"
#include "acmacs-draw/draw-elements.hh"

// ----------------------------------------------------------------------

namespace acmacs
{
    class Layout;
}

namespace acmacs::draw
{
// ----------------------------------------------------------------------

    class PointLabel
    {
     public:
        PointLabel(size_t index) : index_(index) {}

        constexpr PointLabel& show(bool show) { show_ = show; return *this; }
        PointLabel& offset(const PointCoordinates& offs) { offset_ = offs; return *this; }
        PointLabel& display_name(std::string_view display_name) { display_name_ = display_name; return *this; }
        PointLabel& color(const acmacs::color::Modifier& color) { text_color_.add(color); return *this; }
        PointLabel& size(Pixels size) { text_size_ = size; return *this; }
        // template <typename S, typename = std::enable_if_t<acmacs::sfinae::is_string_v<S>>> PointLabel& weight(S weight) { text_style_.weight = weight; return *this; }
        // template <typename S, typename = std::enable_if_t<acmacs::sfinae::is_string_v<S>>> PointLabel& slant(S slant) { text_style_.slant = slant; return *this; }
        // template <typename S, typename = std::enable_if_t<acmacs::sfinae::is_string_v<S>>> PointLabel& font_family(S family) { text_style_.font_family = family; return *this; }
        PointLabel& weight(std::string_view weight) { text_style_.weight = std::string(weight); return *this; }
        PointLabel& slant(std::string_view slant) { text_style_.slant = std::string(slant); return *this; }
        PointLabel& font_family(std::string_view family) { text_style_.font_family = std::string(family); return *this; }

        constexpr size_t index() const { return index_; }
        constexpr bool show() const { return show_; }
        const std::string& display_name() const { return display_name_; }
        PointCoordinates offset() const { return offset_; }
        Color text_color() const { return text_color_; }
        Pixels text_size() const { return text_size_; }
        const TextStyle text_style() const { return text_style_; }

        static double text_offset(double offset_hint, double point_size, double text_size, bool text_origin_at_opposite);

      private:
        bool show_{true};
        size_t index_;
        PointCoordinates offset_{0, 1};
        std::string display_name_;
        acmacs::color::Modifier text_color_{BLACK};
        Pixels text_size_{12};
        TextStyle text_style_;

    }; // class PointLabel

    class PointLabels : public std::vector<PointLabel>
    {
     public:
        PointLabels() = default;

        void reset() { clear(); }
        PointLabel& add(size_t index) { if (auto found = std::find_if(begin(), end(), [=](const auto& label) { return label.index() == index; }); found == end()) return emplace_back(index); else return *found; }
        bool exists(size_t index) const { return std::find_if(begin(), end(), [=](const auto& label) { return label.index() == index; }) != end(); }
        void remove(size_t index) { erase(std::remove_if(begin(), end(), [index](const auto& label) { return label.index() == index; }), end()); }

    }; // class PointLabels

// ----------------------------------------------------------------------

    class Points : public Element
    {
     private:
       template <typename S> void draw_forward(drawing_stage stage, S&& surface) const
       {
           switch (stage) {
               case drawing_stage::points:
                   draw_points(std::forward<S>(surface));
                   break;
               case drawing_stage::labels:
                   draw_labels(std::forward<S>(surface));
                   break;
               case drawing_stage::__first: case drawing_stage::background: case drawing_stage::grid: case drawing_stage::serum_circles:
               case drawing_stage::procrustes_arrows: case drawing_stage::legend: case drawing_stage::title: case drawing_stage::border: case drawing_stage::__last:
                   break;
           }
       }

     public:
        using DrawingOrder = std::vector<size_t>;
        using UnpackedStyles = std::vector<PointStyle>;

        Points();
        Points(std::shared_ptr<acmacs::Layout> layout, const acmacs::Transformation& transformation);

        Points& drawing_order(const DrawingOrder& drawing_order) { drawing_order_ = drawing_order; return *this; }
        Points& styles(std::shared_ptr<PointStyles> styles) { styles_ = styles; return *this; }
        Points& labels(const PointLabels& labels) { labels_ = &labels; return *this; }

        void draw(drawing_stage stage, surface::Surface& surface) const override { draw_forward(stage, surface); }
        // void draw(drawing_stage stage, surface::JsDynamic& surface) const override { draw_forward(stage, surface); }
        // void draw(drawing_stage stage, surface::JsStatic& surface) const override { draw_forward(stage, surface); }

        const auto& transformation() const { return transformation_; }

        // drawi
        std::pair<size_t, PointStyle*> add(const PointCoordinates& coord);
        PointLabel& add_label(size_t point_no) { return labels_here_->add(point_no); }

      private:
        std::shared_ptr<acmacs::Layout> layout_;
        acmacs::Transformation transformation_;
        DrawingOrder drawing_order_;
        PointStyle default_style_;
        std::shared_ptr<PointStyles> styles_;
        std::unique_ptr<PointLabels> labels_here_;
        const PointLabels* labels_ = nullptr;

        void draw_points(surface::Surface& surface) const;
        void draw_labels(surface::Surface& surface) const;
        // void draw_points(surface::JsDynamic& surface) const;
        // void draw_labels(surface::JsDynamic& surface) const;
        // void draw_points(surface::JsStatic& surface) const;
        // void draw_labels(surface::JsStatic& surface) const;

        PointStyle style(size_t point_no) const
            {
                if (styles_)
                    return styles_->style(point_no);
                else
                    return default_style_;
            }

    }; // class Points

} // namespace acmacs::draw

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
