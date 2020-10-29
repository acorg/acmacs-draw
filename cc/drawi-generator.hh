#pragma once

#include "acmacs-base/color.hh"
#include "acmacs-draw/viewport.hh"

// ----------------------------------------------------------------------

namespace acmacs::drawi::inline v1
{
    class Generator
    {
      public:
        void generate(std::string_view filename) const;

        constexpr Viewport& viewport() { return viewport_; }
        constexpr const Viewport& viewport() const { return viewport_; }

        struct Point
        {
            static inline const std::string_view Circle{"circle"};
            static inline const std::string_view Egg{"egg"};
            static inline const std::string_view UglyEgg{"uglyegg"};
            static inline const std::string_view Box{"box"};
            static inline const std::string_view Triangle{"triangle"};

            Point& coord(const PointCoordinates& new_coord) { coord_ = new_coord; return *this; }
            Point& fill(Color a_fill) { fill_ = a_fill; return *this; }
            Point& outline(Color a_outline) { outline_ = a_outline; return *this; }
            Point& outline_width(Pixels a_outline_width) { outline_width_ = a_outline_width; return *this; }
            Point& size(Pixels a_size) { size_ = a_size; return *this; }
            Point& shape(std::string_view a_shape) { shape_ = a_shape; return *this; }
            Point& label(std::string_view a_label) { label_ = a_label; return *this; }

            PointCoordinates coord_{PointCoordinates::zero2D};
            Color fill_{TRANSPARENT};
            Color outline_{BLACK};
            Pixels outline_width_{1};
            Pixels size_{5};
            std::string shape_{Circle};
            std::string label_{};
        };

        constexpr Point& add_point() { return points_.emplace_back(); }

      private:
        Viewport viewport_{-5, -5, 10};
        std::vector<Point> points_;
    };

} // namespace acmacs::drawi::inline v1

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
