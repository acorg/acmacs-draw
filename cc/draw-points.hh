#pragma once

#include <optional>
#include <memory>
#include <vector>

#include "acmacs-base/transformation.hh"
#include "acmacs-base/point-style.hh"
#include "acmacs-base/text-style.hh"
#include "acmacs-draw/draw-elements.hh"

// ----------------------------------------------------------------------

namespace acmacs
{
    class LayoutInterface;
}

namespace acmacs::draw
{
// ----------------------------------------------------------------------

    class PointLabel
    {
     public:
        PointLabel(size_t index) : index_(index), offset_{0, 1}, text_color_{BLACK}, text_size_{12} {}

        PointLabel& offset(double x, double y) { offset_.set(x, y); return *this; }
        PointLabel& display_name(std::string_view display_name) { display_name_ = display_name; return *this; }
        PointLabel& color(Color color) { text_color_ = color; return *this; }
        PointLabel& size(double size) { text_size_ = size; return *this; }
        PointLabel& weight(std::string weight) { text_style_.weight = weight; return *this; }
        PointLabel& slant(std::string slant) { text_style_.slant = slant; return *this; }
        PointLabel& font_family(std::string family) { text_style_.font_family = family; return *this; }

        constexpr size_t index() const { return index_; }
        const std::string& display_name() const { return display_name_; }
        const acmacs::Location& offset() const { return offset_; }
        Color text_color() const { return text_color_; }
        Pixels text_size() const { return text_size_; }
        const acmacs::TextStyle text_style() const { return text_style_; }

        static double text_offset(double offset_hint, double point_size, double text_size, bool text_origin_at_opposite);

     private:
        size_t index_;
        acmacs::Location offset_;
        std::string display_name_;
        Color text_color_;
        Pixels text_size_;
        acmacs::TextStyle text_style_;

    }; // class PointLabel

    class PointLabels : public std::vector<PointLabel>
    {
     public:
        PointLabels() = default;

        PointLabel& add(size_t index) { if (auto found = std::find_if(begin(), end(), [=](const auto& label) { return label.index() == index; }); found == end()) return emplace_back(index); else return *found; }
        void remove(size_t index) { erase(std::remove_if(begin(), end(), [index](const auto& label) { return label.index() == index; }), end()); }

    }; // class PointLabels

// ----------------------------------------------------------------------

    class Points : public Element
    {
     public:
        using DrawingOrder = std::vector<size_t>;
        using UnpackedStyles = std::vector<PointStyle>;

        Points(std::shared_ptr<acmacs::LayoutInterface> layout, const acmacs::Transformation& transformation);

        Points& drawing_order(const DrawingOrder& drawing_order) { drawing_order_ = drawing_order; return *this; }
        Points& styles(std::shared_ptr<PointStyles> styles) { styles_ = styles; return *this; }

        void draw(drawing_stage stage, surface::Surface& surface) const override;

     private:
        std::shared_ptr<acmacs::LayoutInterface> layout_;
        acmacs::Transformation transformation_;
        DrawingOrder drawing_order_;
        PointStyle default_style_;
        std::shared_ptr<PointStyles> styles_;

        void draw_points(surface::Surface& surface) const;

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
