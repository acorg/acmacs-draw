#pragma once

#include <iostream>
#include <iomanip>
#include <cassert>

#include "acmacs-base/size.hh"

// ----------------------------------------------------------------------

namespace acmacs
{
    class Viewport
    {
      public:
        Viewport() : origin(PointCoordinates::zero2D), size{0, 0} {}
        Viewport(const Viewport& src) : origin(src.origin), size(src.size) {}
        Viewport(const PointCoordinates& a, const Size& s) : origin(a), size(s) {}
        Viewport(const Size& s) : origin(PointCoordinates::zero2D), size(s) {}
        Viewport(const PointCoordinates& a, const PointCoordinates& b) : origin(a), size(b - a) {}
        Viewport(double aX, double aY, double aSize) : origin(aX, aY), size{aSize, aSize} {}
        Viewport(double aX, double aY, double aWidth, double aHeight) : origin(aX, aY), size{aWidth, aHeight} {}
        Viewport& operator=(const Viewport& src) = default;

        void set(const PointCoordinates& a, const Size& s)
        {
            origin = a;
            size = s;
            assert(size.width > 0 && size.height > 0);
        }
        void set(const PointCoordinates& a, const PointCoordinates& b)
        {
            origin = a;
            size = b - a;
            assert(size.width > 0 && size.height > 0);
        }
        void set(const PointCoordinates& aOrigin, double aSize)
        {
            assert(aSize > 0);
            origin = aOrigin;
            size.set(aSize, aSize);
        }
        void set_from_center_size(const PointCoordinates& aCenter, double aSize)
        {
            assert(aSize > 0);
            origin = aCenter - aSize / 2;
            size.set(aSize, aSize);
        }

        double aspect() const { return size.aspect(); }
        void zoom(double scale)
        {
            const auto new_size = size * scale;
            origin = center() - new_size * 0.5;
            size = new_size;
            assert(size.width > 0 && size.height > 0);
        }
        void center(const PointCoordinates& aCenter) { origin = aCenter - size * 0.5; }
        Size offset() const { return - origin; }

        // make viewport a square by extending the smaller side from center
        void square()
        {
            if (size.width < size.height) {
                origin.x(origin.x() - (size.height - size.width) / 2);
                size.width = size.height;
            }
            else {
                origin.y(origin.y() - (size.width - size.height) / 2);
                size.height = size.width;
            }
        }

        // zoom out viewport to make width a whole number)
        void whole_width() { zoom(std::ceil(size.width) / size.width); }

        constexpr double left() const { return origin.x(); }
        constexpr double right() const { return origin.x() + size.width; }
        constexpr double top() const { return origin.y(); }
        constexpr double bottom() const { return origin.y() + size.height; }
        PointCoordinates top_right() const { return origin + PointCoordinates(size.width, 0); }
        PointCoordinates bottom_right() const { return origin + size; }
        PointCoordinates bottom_left() const { return origin + PointCoordinates(0, size.height); }
        PointCoordinates center() const { return origin + size * 0.5; }
        PointCoordinates top_center() const { return origin + PointCoordinates(size.width / 2, 0); }
        PointCoordinates bottom_center() const { return origin + PointCoordinates(size.width / 2, size.height); }
        PointCoordinates left_center() const { return origin + PointCoordinates(0, size.height / 2); }
        PointCoordinates right_center() const { return origin + PointCoordinates(size.width, size.height / 2); }

        constexpr Scaled left_scaled() const { return Scaled{left()}; }
        constexpr Scaled right_scaled() const { return Scaled{right()}; }
        constexpr Scaled top_scaled() const { return Scaled{top()}; }
        constexpr Scaled bottom_scaled() const { return Scaled{bottom()}; }

        bool empty() const { return size.empty(); }

        bool is_overlap(const Viewport& rhs) const
        {
            // https://stackoverflow.com/questions/20925818/algorithm-to-check-if-two-boxes-overlap
            return left() < rhs.right() && rhs.left() < right() && top() < rhs.bottom() && rhs.top() < bottom();
        }

        PointCoordinates origin;
        Size size;

        // std::string to_string() const { return "Viewport(" + origin.to_string() + ", " + size.to_string() + ")"; }

    }; // class Viewport

} // namespace acmacs

// ----------------------------------------------------------------------

// format for Viewport is format for double of each value (left, top, width, height), e.g. :.8f

template <> struct fmt::formatter<acmacs::Viewport> : public fmt::formatter<acmacs::fmt_helper::float_formatter>
{
    template <typename FormatContext> auto format(const acmacs::Viewport& viewport, FormatContext& ctx) const
    {
        format_to(ctx.out(), fmt::runtime("Viewport[{}, {}, {}"), format_val(viewport.left()), format_val(viewport.top()), format_val(viewport.size.width));
        if (!float_equal(viewport.size.width, viewport.size.height))
            format_to(ctx.out(), ", {}", format_val(viewport.size.height));
        return format_to(ctx.out(), "]");
    }
};

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
