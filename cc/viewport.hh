#pragma once

#include <iostream>
#include <iomanip>

#include "acmacs-base/size.hh"

// ----------------------------------------------------------------------

namespace acmacs
{
    class Viewport
    {
      public:
        Viewport() : origin{0, 0}, size{0, 0} {}
        Viewport(Location2D a, const Size& s) : origin(a), size(s) {}
        Viewport(const Size& s) : origin{0, 0}, size(s) {}
        Viewport(Location2D a, Location2D b) : origin(a), size(b - a) {}
        Viewport(double aX, double aY, double aSize) : origin{aX, aY}, size{aSize, aSize} {}
        Viewport(double aX, double aY, double aWidth, double aHeight) : origin{aX, aY}, size{aWidth, aHeight} {}

        void set(Location2D a, const Size& s)
        {
            origin = a;
            size = s;
        }
        void set(Location2D a, Location2D b)
        {
            origin = a;
            size = b - a;
        }
        void set(Location2D aOrigin, double aSize)
        {
            origin = aOrigin;
            size.set(aSize, aSize);
        }
        // void set(double aX, double aY, double aSize)
        // {
        //     origin.x(aX);
        //     origin.y(aY);
        //     size.set(aSize, aSize);
        // }
        void set_from_center_size(Location2D aCenter, double aSize)
        {
            origin = aCenter - aSize;
            size.set(aSize, aSize);
        }
        void set_from_center_size(const std::vector<double>& aCenter, double aSize)
        {
            origin = Location2D(aCenter) - aSize / 2;
            size.set(aSize, aSize);
        }
        double aspect() const { return size.aspect(); }
        void zoom(double scale)
        {
            const auto new_size = size * scale;
            origin = center() - new_size * 0.5;
            size = new_size;
        }
        void center(Location2D aCenter) { origin = aCenter - size * 0.5; }
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

        double left() const { return origin.x(); }
        double right() const { return origin.x() + size.width; }
        double top() const { return origin.y(); }
        double bottom() const { return origin.y() + size.height; }
        Location2D top_right() const { return origin + Location2D{size.width, 0}; }
        Location2D bottom_right() const { return origin + size; }
        Location2D bottom_left() const { return origin + Location2D{0, size.height}; }
        Location2D center() const { return origin + size * 0.5; }
        Location2D top_center() const { return origin + Location2D{size.width / 2, 0}; }

        Scaled left_scaled() const { return Scaled{left()}; }
        Scaled right_scaled() const { return Scaled{right()}; }
        Scaled top_scaled() const { return Scaled{top()}; }
        Scaled bottom_scaled() const { return Scaled{bottom()}; }

        bool empty() const { return size.empty(); }

        Location2D origin;
        Size size;

        // std::string to_string() const { return "Viewport(" + origin.to_string() + ", " + size.to_string() + ")"; }

    }; // class Viewport

    // ----------------------------------------------------------------------

    inline std::ostream& operator<<(std::ostream& out, const Viewport& aViewport)
    {
        out << "Viewport[" << std::fixed << std::setw(5) << std::setprecision(2) << aViewport.origin.x() << ", " << aViewport.origin.y() << ", " << aViewport.size.width;
        if (!float_equal(aViewport.size.width, aViewport.size.height))
            out << ", " << aViewport.size.height;
        return out << ']';
    }

} // namespace acmacs


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
