#pragma once

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

} // namespace acmacs::draw

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
