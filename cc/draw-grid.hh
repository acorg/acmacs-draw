#include "acmacs-draw/draw-elements.hh"

// ----------------------------------------------------------------------

namespace acmacs::draw
{
    class Background : public Element
    {
     public:
        Background(Color color) : color_(color) {}

        void draw(drawing_stage stage, surface::Surface& surface) const override;
        void draw(drawing_stage stage, surface::JsStatic& surface) const override;
        void draw(drawing_stage stage, surface::JsDynamic& surface) const override;

     private:
        Color color_;

    }; // class Background

    class Grid : public Element
    {
     public:
        Grid(Scaled step, Color line_color, Pixels line_width) : step_(step), line_color_(line_color), line_width_(line_width) {}

        void draw(drawing_stage stage, surface::Surface& surface) const override;
        void draw(drawing_stage stage, surface::JsStatic& surface) const override;
        void draw(drawing_stage stage, surface::JsDynamic& surface) const override;

     private:
        Scaled step_;
        Color line_color_;
        Pixels line_width_;

    }; // class Grid

    class Border : public Element
    {
     public:
        Border(Color line_color, Pixels line_width) : line_color_(line_color), line_width_(line_width) {}

        void draw(drawing_stage stage, surface::Surface& surface) const override;
        void draw(drawing_stage stage, surface::JsStatic& surface) const override;
        void draw(drawing_stage stage, surface::JsDynamic& surface) const override;

     private:
        Color line_color_;
        Pixels line_width_;

    }; // class Border

} // namespace acmacs::draw

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
