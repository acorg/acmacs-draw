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
        acmacs::Location from_;
        acmacs::Location to_;
        Color line_color_;
        Pixels line_width_;

    }; // class Line

// ----------------------------------------------------------------------

    class Arrow : public Line
    {
     public:
        Arrow(const acmacs::Location& from, const acmacs::Location& to, Color line_color, Pixels line_width, Color arrow_head_color, bool arrow_head_filled, Pixels arrow_width)
            : Line(from, to, line_color, line_width), arrow_head_color_(arrow_head_color), arrow_head_filled_(arrow_head_filled), arrow_width_(arrow_width) {}

        void draw(drawing_stage stage, surface::Surface& surface) const override;

     private:
        Color arrow_head_color_;
        bool arrow_head_filled_;
        Pixels arrow_width_;

    }; // class Arrow

} // namespace acmacs::draw

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
