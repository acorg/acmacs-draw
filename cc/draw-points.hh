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
        Points& styles(const UnpackedStyles& styles) { unpacked_styles_ = styles; return *this; }

        void draw(drawing_stage stage, surface::Surface& surface) const override;

     private:
        std::shared_ptr<acmacs::LayoutInterface> layout_;
        acmacs::Transformation transformation_;
        std::optional<DrawingOrder> drawing_order_;
        PointStyle default_style_;
        std::optional<UnpackedStyles> unpacked_styles_;

        void draw_points(surface::Surface& surface) const;
        const PointStyle& style(size_t point_no) const;

    }; // class Points

} // namespace acmacs::draw

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
