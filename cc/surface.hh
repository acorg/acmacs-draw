#pragma once

#include <vector>
#include <memory>

#include "acmacs-draw/text-style.hh"
#include "acmacs-draw/viewport.hh"
#include "acmacs-draw/color.hh"

// ----------------------------------------------------------------------

namespace _acmacs_draw_internal
{
    template <char Tag> class SizeScale
    {
     public:
        inline SizeScale() : mValue(0) {}
        inline explicit SizeScale(double aValue) : mValue(aValue) {}
        inline SizeScale(const SizeScale& a) = default;
        inline SizeScale& operator = (double aValue) { mValue = aValue; return *this; }
        inline double value() const { return mValue; }
        inline SizeScale operator / (double a) const { return SizeScale{mValue / a}; }
        inline SizeScale operator * (double a) const { return SizeScale{mValue * a}; }
        inline SizeScale& operator *= (double a) { mValue *= a; return *this; }
        inline SizeScale operator - () const { return SizeScale{- mValue}; }

     private:
        double mValue;
    };
}

using Pixels = _acmacs_draw_internal::SizeScale<'P'>; // size in pixels, indepenent from the surface internal coordinate system
using Scaled = _acmacs_draw_internal::SizeScale<'S'>; // size in the surface internal coordinate system

// ----------------------------------------------------------------------

class Surface
{
 public:
    enum class LineCap { Butt, Round, Square };
    enum class LineJoin { Miter, Round, Bevel };

    [[noreturn]] inline Surface(const Surface&) { throw std::runtime_error("Surface copying forbidden!"); } // cannto make it private due to using vector<SurfaceCairoChild>
    virtual ~Surface() {}

    constexpr static const double default_canvas_width = 1000.0;

    virtual const Viewport& viewport() const = 0;
    virtual const Location& origin_in_parent() const = 0;
    virtual double width_in_parent() const = 0;

    virtual void viewport(const Viewport& aViewport) = 0;
    virtual void move(const Location& aOriginInParent) = 0;
    virtual void move_resize(const Location& aOriginInParent, double aWidthInParent) = 0;
    inline void move_resize_viewport(const Location& aOriginInParent, double aWidthInParent, const Viewport& aViewport) { move_resize(aOriginInParent, aWidthInParent); viewport(aViewport); }

    inline double aspect() const { return viewport().aspect(); }

    virtual Surface& subsurface(const Location& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport, bool aClip) = 0;
    virtual Surface& subsurface(bool aClip) = 0;

    virtual void line(const Location& a, const Location& b, Color aColor, Pixels aWidth, LineCap aLineCap = LineCap::Butt) = 0;
    virtual void line(const Location& a, const Location& b, Color aColor, Scaled aWidth, LineCap aLineCap = LineCap::Butt) = 0;
    virtual void rectangle(const Location& a, const Size& s, Color aColor, Pixels aWidth, LineCap aLineCap = LineCap::Butt) = 0;
    virtual void rectangle_filled(const Location& a, const Size& s, Color aOutlineColor, Pixels aWidth, Color aFillColor, LineCap aLineCap = LineCap::Butt) = 0;

    virtual void circle(const Location& aCenter, Pixels aDiameter, double aAspect, double aAngle, Color aOutlineColor, Pixels aOutlineWidth) = 0;
    virtual void circle(const Location& aCenter, Scaled aDiameter, double aAspect, double aAngle, Color aOutlineColor, Pixels aOutlineWidth) = 0;
    virtual void circle_filled(const Location& aCenter, Pixels aDiameter, double aAspect, double aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor) = 0;
    virtual void circle_filled(const Location& aCenter, Scaled aDiameter, double aAspect, double aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor) = 0;
    virtual void square_filled(const Location& aCenter, Pixels aSide, double aAspect, double aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap = LineCap::Butt) = 0;
    virtual void square_filled(const Location& aCenter, Scaled aSide, double aAspect, double aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap = LineCap::Butt) = 0;
    virtual void triangle_filled(const Location& aCenter, Pixels aSide, double aAspect, double aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap = LineCap::Butt) = 0;
    virtual void triangle_filled(const Location& aCenter, Scaled aSide, double aAspect, double aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap = LineCap::Butt) = 0;

    virtual void path_outline(std::vector<Location>::const_iterator first, std::vector<Location>::const_iterator last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose = false, LineCap aLineCap = LineCap::Butt) = 0;
    virtual void path_outline(const double* first, const double* last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose = false, LineCap aLineCap = LineCap::Butt) = 0;
    virtual void path_fill(std::vector<Location>::const_iterator first, std::vector<Location>::const_iterator last, Color aFillColor) = 0;
    virtual void path_fill(const double* first, const double* last, Color aFillColor) = 0;

    virtual void double_arrow(const Location& a, const Location& b, Color aColor, Pixels aLineWidth, Pixels aArrowWidth) = 0;
    virtual void grid(Scaled aStep, Color aLineColor, Pixels aLineWidth) = 0;
    virtual void border(Color aLineColor, Pixels aLineWidth) = 0;
    virtual void background(Color aColor) = 0;

    virtual void text(const Location& a, std::string aText, Color aColor, Pixels aSize, const TextStyle& aTextStyle = TextStyle(), double aRotation = 0) = 0;
    virtual void text(const Location& a, std::string aText, Color aColor, Scaled aSize, const TextStyle& aTextStyle = TextStyle(), double aRotation = 0) = 0;
    virtual Size text_size(std::string aText, Pixels aSize, const TextStyle& aTextStyle = TextStyle(), double* x_bearing = nullptr) = 0;
    virtual Size text_size(std::string aText, Scaled aSize, const TextStyle& aTextStyle = TextStyle(), double* x_bearing = nullptr) = 0;

    inline Scaled convert(Pixels a) const { return Scaled{a.value() / scale()}; }

 protected:
    inline Surface() = default;

    virtual Location arrow_head(const Location& a, double angle, double sign, Color aColor, Pixels aArrowWidth) = 0;
    virtual inline double scale() const = 0;

}; // class Surface

inline std::ostream& operator << (std::ostream& out, const Surface& aSurface)
{
    return out << "Surface(" << aSurface.viewport() << ", Origin" << aSurface.origin_in_parent() << ", width=" << aSurface.width_in_parent() << ')';
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
