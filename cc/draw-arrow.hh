#pragma once

#include "acmacs-base/layout.hh"
#include "acmacs-draw/surface.hh"
#include "acmacs-draw/draw-elements.hh"

// ----------------------------------------------------------------------

namespace acmacs::draw
{
    class Line : public Element
    {
     public:
        Line(const acmacs::Location& from, const acmacs::Location& to, Color line_color, Pixels line_width)
            : from_(from), to_(to), line_color_(line_color), line_width_(line_width) {}

        void draw(drawing_stage stage, surface::Surface& surface) const override;

        const acmacs::Location& from() const { return from_; }
        const acmacs::Location& to() const { return to_; }
        Color line_color() const { return line_color_; }
        Pixels line_width() const { return line_width_; }

     private:
        const acmacs::Location from_, to_;
        const Color line_color_;
        const Pixels line_width_;

    }; // class Line

// ----------------------------------------------------------------------

    class Arrow : public Line
    {
     public:
        Arrow(const acmacs::Location& from, const acmacs::Location& to, Color line_color, Pixels line_width, Color arrow_head_color, bool arrow_head_filled, Pixels arrow_width)
            : Line(from, to, line_color, line_width), arrow_head_color_(arrow_head_color), arrow_head_filled_(arrow_head_filled), arrow_width_(arrow_width) {}

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
        Rectangle(const acmacs::Location& corner1, const acmacs::Location& corner2, Color color, bool filled, Pixels line_width)
            : corner1_(corner1), corner2_(corner2), color_(color), filled_(filled), line_width_(line_width) {}

        void draw(drawing_stage stage, surface::Surface& surface) const override;

     private:
        const acmacs::Location corner1_, corner2_;
        const Color color_;
        const bool filled_;
        const Pixels line_width_;

    }; // class Rectangle

// ----------------------------------------------------------------------

    template <typename ScaledOrPixels> class Circle : public Element
    {
     public:
        Circle(const acmacs::Location& center, ScaledOrPixels size, Color fill_color, Color outline_color, Pixels outline_width, Aspect aspect, Rotation rotation)
            : center_(center), size_(size), fill_color_(fill_color), outline_color_(outline_color), outline_width_(outline_width), aspect_(aspect), rotation_(rotation) {}

        void draw(drawing_stage stage, surface::Surface& surface) const override
            {
                if (stage == drawing_stage::procrustes_arrows)
                    surface.circle_filled(center_, size_, aspect_, rotation_, outline_color_, outline_width_, fill_color_);
            }

     private:
        const acmacs::Location center_;
        const ScaledOrPixels size_;
        const Color fill_color_, outline_color_;
        const Pixels outline_width_;
        const Aspect aspect_;
        const Rotation rotation_;

    }; // class Circle

// ----------------------------------------------------------------------

    class Sector : public Element
    {
     public:
        Sector(const acmacs::Location& center, Scaled size, Color fill_color, Color outline_color, Pixels outline_width, Color radius_color, Pixels radius_width, acmacs::surface::Dash radius_dash, Rotation start, Rotation end)
            : center_(center), size_(size), fill_color_(fill_color), outline_color_(outline_color), outline_width_(outline_width),
              radius_color_(radius_color), radius_width_(radius_width), radius_dash_(radius_dash), start_(start), end_(end) {}

        void draw(drawing_stage stage, surface::Surface& surface) const override;

        void center(const acmacs::Location& center) const { center_ = center; }
        void stage(drawing_stage stage) { stage_ = stage; }

     private:
        drawing_stage stage_{drawing_stage::procrustes_arrows};
        mutable acmacs::Location center_;
        const Scaled size_;
        const Color fill_color_, outline_color_;
        const Pixels outline_width_;
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
        SerumCircle(const Coordinates& coordinates, const acmacs::Transformation& transformation, Scaled size, Color fill_color, Color outline_color, Pixels outline_width, Color radius_color, Pixels radius_width, acmacs::surface::Dash radius_dash, Rotation start, Rotation end)
            : Sector(acmacs::Location{}, size, fill_color, outline_color, outline_width, radius_color, radius_width, radius_dash, start, end), coordinates_(coordinates), transformation_(transformation)
            { stage(drawing_stage::serum_circles); }

        void draw(drawing_stage stage, surface::Surface& surface) const override;

     private:
        const acmacs::Coordinates coordinates_;
        const acmacs::Transformation transformation_;

    }; // class SerumCircle

} // namespace acmacs::draw

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
