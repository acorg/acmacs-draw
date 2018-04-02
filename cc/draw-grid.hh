#include "acmacs-draw/draw-elements.hh"

// ----------------------------------------------------------------------

namespace acmacs::draw
{
    class Grid : public Element
    {
     public:
        Grid(Scaled step, Color line_color, Pixels line_width) : step_(step), line_color_(line_color), line_width_(line_width) {}

        void draw(drawing_stage stage, surface::Surface& surface) const override;

     private:
        Scaled step_;
        Color line_color_;
        Pixels line_width_;

    }; // class Grid

} // namespace acmacs::draw

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
