#pragma once

#include "acmacs-base/layout.hh"
#include "acmacs-base/line.hh"
#include "acmacs-draw/surface.hh"
#include "acmacs-draw/draw-elements.hh"

// ----------------------------------------------------------------------

namespace acmacs::draw
{
    class Line : public Element
    {
     public:
        Line(const PointCoordinates& from, const PointCoordinates& to, Color line_color, Pixels line_width, bool apply_transformation)
            : from_(from), to_(to), line_color_(line_color), line_width_(line_width), apply_transformation_{apply_transformation} {}

        void transform(const Transformation& transformation) const override;
        void draw(drawing_stage stage, surface::Surface& surface) const override;

        PointCoordinates from() const { return from_; }
        PointCoordinates to() const { return to_; }
        Color line_color() const { return line_color_; }
        Pixels line_width() const { return line_width_; }
        constexpr bool apply_transformation() const { return apply_transformation_; }

     private:
        mutable PointCoordinates from_, to_;
        const Color line_color_;
        const Pixels line_width_;
        bool apply_transformation_;

    }; // class Line

    class LineDefinedByEquation : public Element
    {
     public:
        LineDefinedByEquation(const acmacs::LineDefinedByEquation& line, Color line_color, Pixels line_width, bool apply_transformation)
            : line_(line), line_color_(line_color), line_width_(line_width), apply_transformation_{apply_transformation} {}

        void transform(const Transformation& transformation) const override;
        void draw(drawing_stage stage, surface::Surface& surface) const override;

        Color line_color() const { return line_color_; }
        Pixels line_width() const { return line_width_; }
        constexpr bool apply_transformation() const { return apply_transformation_; }

     private:
        mutable acmacs::LineDefinedByEquation line_;
        const Color line_color_;
        const Pixels line_width_;
        bool apply_transformation_;

    }; // class LineDefinedByEquation

// ----------------------------------------------------------------------

    class Path : public Element
    {
     public:
        Path(const std::vector<PointCoordinates>& path, Color line_color, Pixels line_width, std::optional<Color> close_and_fill)
            : path_(path), line_color_{line_color}, line_width_{line_width}, close_and_fill_{close_and_fill} {}

        void draw(drawing_stage stage, surface::Surface& surface) const override;

     private:
        const std::vector<PointCoordinates> path_;
        const Color line_color_;
        const Pixels line_width_;
        std::optional<Color> close_and_fill_;

    }; // class Path

// ----------------------------------------------------------------------

    class Arrow : public Line
    {
     public:
        Arrow(const PointCoordinates& from, const PointCoordinates& to, Color line_color, Pixels line_width, Color arrow_head_color, bool arrow_head_filled, Pixels arrow_width, bool apply_transformation)
            : Line(from, to, line_color, line_width, apply_transformation), arrow_head_color_(arrow_head_color), arrow_head_filled_(arrow_head_filled), arrow_width_(arrow_width) {}

        void draw(drawing_stage stage, surface::Surface& surface) const override;

     private:
        const Color arrow_head_color_;
        const bool arrow_head_filled_;
        const Pixels arrow_width_;

    }; // class Arrow

// ----------------------------------------------------------------------

    class Rectangle : public Element
    {
     public:
        Rectangle(const PointCoordinates& corner1, const PointCoordinates& corner2, Color color, bool filled, Pixels line_width)
            : corner1_(corner1), corner2_(corner2), color_(color), filled_(filled), line_width_(line_width) {}

        void draw(drawing_stage stage, surface::Surface& surface) const override;

     private:
        const PointCoordinates corner1_, corner2_;
        const Color color_;
        const bool filled_;
        const Pixels line_width_;

    }; // class Rectangle

// ----------------------------------------------------------------------

    template <typename ScaledOrPixels> class Circle : public Element
    {
     public:
        Circle(const PointCoordinates& center, ScaledOrPixels size, Color fill_color, Color outline_color, Pixels outline_width, Aspect aspect, Rotation rotation, std::string label={})
            : center_(center), size_(size), fill_color_(fill_color), outline_color_(outline_color), outline_width_(outline_width), aspect_(aspect), rotation_(rotation), label_{label} {}

        void draw(drawing_stage stage, surface::Surface& surface) const override
            {
                if (stage == drawing_stage::procrustes_arrows) {
                    surface.circle_filled(center_, size_, aspect_, rotation_, outline_color_, outline_width_, fill_color_);
                    if (!label_.empty()) {
                        const Pixels size{10};
                        const auto tsize = surface.text_size(label_, size);
                        surface.text({center_[0] - tsize.width / 2, center_[1] + surface.convert(size_).value() / 2 + tsize.height}, label_, BLACK, size);
                    }
                }
            }

     private:
        const PointCoordinates center_;
        const ScaledOrPixels size_;
        const Color fill_color_, outline_color_;
        const Pixels outline_width_;
        const Aspect aspect_;
        const Rotation rotation_;
        const std::string label_;

    }; // class Circle

// ----------------------------------------------------------------------

    class Sector : public Element
    {
     public:
        Sector(const PointCoordinates& center, Scaled size, Color fill_color, Color outline_color, Pixels outline_width, acmacs::surface::Dash outline_dash, Color radius_color, Pixels radius_width, acmacs::surface::Dash radius_dash, Rotation start, Rotation end)
            : center_(center), size_(size), fill_color_(fill_color), outline_color_(outline_color), outline_width_(outline_width), outline_dash_{outline_dash},
              radius_color_(radius_color), radius_width_(radius_width), radius_dash_(radius_dash), start_(start), end_(end) {}

        void draw(drawing_stage stage, surface::Surface& surface) const override;

        void center(const PointCoordinates& center) const { center_ = center; }
        void stage(drawing_stage stage) { stage_ = stage; }

     private:
        drawing_stage stage_{drawing_stage::procrustes_arrows};
        mutable PointCoordinates center_;
        const Scaled size_;
        const Color fill_color_, outline_color_;
        const Pixels outline_width_;
        const acmacs::surface::Dash outline_dash_;
        const Color radius_color_;
        const Pixels radius_width_;
        const acmacs::surface::Dash radius_dash_;
        const Rotation start_;
        const Rotation end_;

    }; // class Circle

// ----------------------------------------------------------------------

    class SerumCircle : public Sector
    {
     public:
        SerumCircle(const PointCoordinates& coordinates, const Transformation& transformation, Scaled size, Color fill_color, Color outline_color, Pixels outline_width, acmacs::surface::Dash outline_dash, Color radius_color, Pixels radius_width, acmacs::surface::Dash radius_dash, Rotation start, Rotation end)
            : Sector(PointCoordinates(0.0, 0.0), size, fill_color, outline_color, outline_width, outline_dash, radius_color, radius_width, radius_dash, start, end), coordinates_(coordinates), transformation_(transformation)
            { stage(drawing_stage::serum_circles); }

        void draw(drawing_stage stage, surface::Surface& surface) const override;

     private:
        const PointCoordinates coordinates_;
        const Transformation transformation_;

    }; // class SerumCircle

} // namespace acmacs::draw

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
