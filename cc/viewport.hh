#pragma once

#include <iostream>
#include <iomanip>

#include "acmacs-draw/size.hh"

// ----------------------------------------------------------------------

class Viewport
{
 public:
    inline Viewport() : origin{0, 0}, size{0, 0} {}
    inline Viewport(const Location& a, const Size& s) : origin(a), size(s) {}
    inline Viewport(const Size& s) : origin{0, 0}, size(s) {}
    inline Viewport(const Location& a, const Location& b) : origin(a), size(b - a) {}
    inline Viewport(double aX, double aY, double aSize) : origin{aX, aY}, size{aSize, aSize} {}
    inline Viewport(double aX, double aY, double aWidth, double aHeight) : origin{aX, aY}, size{aWidth, aHeight} {}

    inline void set(const Location& a, const Size& s) { origin = a; size = s; }
    inline void set(const Location& a, const Location& b) { origin = a; size = b - a; }
    inline void set(double aX, double aY, double aSize) { origin.set(aX, aY); size.set(aSize, aSize); }
    inline void set_from_center_size(const Location& aCenter, double aSize) { origin = aCenter - Size(aSize / 2, aSize / 2); size.set(aSize, aSize); }
    inline void set_from_center_size(const std::vector<double>& aCenter, double aSize) { origin.from_vector(aCenter); origin -= aSize / 2; size.set(aSize, aSize); }
    inline double aspect() const { return size.aspect(); }
    inline void zoom(double scale) { const Size new_size = size * scale; origin = center() - new_size * 0.5; size = new_size; }
    inline void center(const Location& aCenter) { origin = aCenter - size * 0.5; }
    inline Size offset() const { return {-origin.x, -origin.y}; }

      // make viewport a square by extending the smaller side from center
    inline void square()
        {
            if (size.width < size.height) {
                origin.x -= (size.height - size.width) / 2;
                size.width = size.height;
            }
            else {
                origin.y -= (size.width - size.height) / 2;
                size.height = size.width;
            }
        }

      // zoom out viewport to make width a whole number)
    inline void whole_width() { zoom(std::ceil(size.width) / size.width); }

    inline double left() const { return origin.x; }
    inline double right() const { return origin.x + size.width; }
    inline double top() const { return origin.y; }
    inline double bottom() const { return origin.y + size.height; }
    inline Location top_right() const { return origin + Size(size.width, 0); }
    inline Location bottom_right() const { return origin + size; }
    inline Location bottom_left() const { return origin + Size(0, size.height); }
    inline Location center() const { return origin + size * 0.5; }
    inline Location top_center() const { return origin + Size(size.width / 2, 0); }

    inline bool empty() const { return size.empty(); }

    Location origin;
    Size size;

    // inline std::string to_string() const { return "Viewport(" + origin.to_string() + ", " + size.to_string() + ")"; }

}; // class Viewport

// ----------------------------------------------------------------------

inline std::ostream& operator << (std::ostream& out, const Viewport& aViewport)
{
    out << "Viewport[" << std::fixed << std::setw(5) << std::setprecision(2) << aViewport.origin.x << ", " << aViewport.origin.y << ", " << aViewport.size.width;
    if (!float_equal(aViewport.size.width, aViewport.size.height))
        out << ", " << aViewport.size.height;
    return out << ']';
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
