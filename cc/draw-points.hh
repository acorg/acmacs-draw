#include <optional>
#include <memory>
#include <vector>

#include "acmacs-base/transformation.hh"
#include "acmacs-base/point-style.hh"
#include "acmacs-draw/draw-elements.hh"

// ----------------------------------------------------------------------

namespace acmacs
{
    class LayoutInterface;
}

namespace acmacs::draw
{
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
