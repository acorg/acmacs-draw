#pragma once

#include <vector>
#include <memory>

#include "acmacs-base/throw.hh"
#include "acmacs-base/color.hh"
#include "acmacs-base/size-scale.hh"
#include "acmacs-draw/text-style.hh"
#include "acmacs-draw/viewport.hh"

// ----------------------------------------------------------------------

class Surface
{
 public:
    enum class LineCap { Butt, Round, Square };
    enum class LineJoin { Miter, Round, Bevel };
    enum class Dash {NoDash, Dash1, Dash2};

#ifdef ACMACS_TARGET_OS
    [[noreturn]]
#endif
        inline Surface(const Surface&) { THROW_OR_CERR(std::runtime_error("Surface copying forbidden!")); } // cannto make it private due to using vector<SurfaceCairoChild>
    virtual ~Surface() {}

    constexpr static const double default_canvas_width = 1000.0;

    const Viewport& viewport() const { return mViewport; }
    inline void viewport(const Viewport& aViewport) { mViewport = aViewport; }

    inline const Location& origin_in_parent() const { return mOriginInParent; }
    inline double width_in_parent() const { return mWidthInParent; }

    inline double width_in_pixels() const { return viewport().size.width * scale(); }
    inline double height_in_pixels() const { return viewport().size.height * scale(); }

    virtual void move(const Location& aOriginInParent) = 0;
    virtual void move_resize(const Location& aOriginInParent, double aWidthInParent) = 0;
    inline void move_resize_viewport(const Location& aOriginInParent, double aWidthInParent, const Viewport& aViewport) { move_resize(aOriginInParent, aWidthInParent); viewport(aViewport); }

    inline double aspect() const { return viewport().aspect(); }

    inline Surface& subsurface(const Location& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport, bool aClip)
        {
            auto* child = make_child(aOriginInParent, aWidthInParent, aViewport, aClip);
            mChildren.emplace_back(child);
            return *child;
        }
    inline Surface& subsurface(bool aClip) { return subsurface(Location{}, Scaled{}, Viewport{}, aClip); }
    inline Surface& subsurface(const Location& aOriginInParent, Pixels aWidthInParent, const Viewport& aViewport, bool aClip) // origin is in pixels
        { return subsurface(aOriginInParent / scale(), Scaled{aWidthInParent.value() / scale()}, aViewport, aClip); }

    virtual void line(const Location& a, const Location& b, Color aColor, Pixels aWidth, LineCap aLineCap = LineCap::Butt) = 0;
    virtual void line(const Location& a, const Location& b, Color aColor, Scaled aWidth, LineCap aLineCap = LineCap::Butt) = 0;
    virtual void rectangle(const Location& a, const Size& s, Color aColor, Pixels aWidth, LineCap aLineCap = LineCap::Butt) = 0;
    virtual void rectangle_filled(const Location& a, const Size& s, Color aOutlineColor, Pixels aWidth, Color aFillColor, LineCap aLineCap = LineCap::Butt) = 0;

    virtual void circle(const Location& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth) = 0;
    virtual void circle(const Location& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth) = 0;
    inline void circle(const Location& aCenter, Pixels aDiameter, Color aOutlineColor, Pixels aOutlineWidth) { circle(aCenter, aDiameter, Aspect{1}, Rotation{0}, aOutlineColor, aOutlineWidth); }
    inline void circle(const Location& aCenter, Scaled aDiameter, Color aOutlineColor, Pixels aOutlineWidth) { circle(aCenter, aDiameter, Aspect{1}, Rotation{0}, aOutlineColor, aOutlineWidth); }
    virtual void circle_filled(const Location& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor) = 0;
    virtual void circle_filled(const Location& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor) = 0;
    virtual void sector_filled(const Location& aCenter, Scaled aDiameter, Rotation aStart, Rotation aEnd, Color aOutlineColor, Pixels aOutlineWidth, Color aRadiusColor, Pixels aRadiusWidth, Dash aRadiusDash, Color aFillColor) = 0;
    virtual void square_filled(const Location& aCenter, Pixels aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap = LineCap::Butt) = 0;
    virtual void square_filled(const Location& aCenter, Scaled aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap = LineCap::Butt) = 0;
    virtual void triangle_filled(const Location& aCenter, Pixels aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap = LineCap::Butt) = 0;
    virtual void triangle_filled(const Location& aCenter, Scaled aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap = LineCap::Butt) = 0;
    virtual void triangle_filled(const Location& aCorner1, const Location& aCorner2, const Location& aCorner3, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap = LineCap::Butt) = 0;

    virtual void path_outline(std::vector<Location>::const_iterator first, std::vector<Location>::const_iterator last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose = false, LineCap aLineCap = LineCap::Butt) = 0;
    virtual void path_outline(const double* first, const double* last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose = false, LineCap aLineCap = LineCap::Butt) = 0;
    virtual void path_fill(std::vector<Location>::const_iterator first, std::vector<Location>::const_iterator last, Color aFillColor) = 0;
    virtual void path_fill(const double* first, const double* last, Color aFillColor) = 0;

    void double_arrow(const Location& a, const Location& b, Color aColor, Pixels aLineWidth, Pixels aArrowWidth);
    virtual Location arrow_head(const Location& a, double angle, double sign, Color aColor, Pixels aArrowWidth) = 0;
    void grid(Scaled aStep, Color aLineColor, Pixels aLineWidth);
    inline void border(Color aLineColor, Pixels aLineWidth) { const Viewport& v = viewport(); rectangle(v.origin, v.size, aLineColor, aLineWidth * 2); }
    inline void background(Color aColor) { const Viewport& v = viewport(); rectangle_filled(v.origin, v.size, aColor, Pixels{0}, aColor); }

    virtual void text(const Location& a, std::string aText, Color aColor, Pixels aSize, const TextStyle& aTextStyle = TextStyle(), Rotation aRotation = Rotation{0}) = 0;
    virtual void text(const Location& a, std::string aText, Color aColor, Scaled aSize, const TextStyle& aTextStyle = TextStyle(), Rotation aRotation = Rotation{0}) = 0;
    virtual void text_right_aligned(const Location& aEnd, std::string aText, Color aColor, Pixels aSize, const TextStyle& aTextStyle = TextStyle(), Rotation aRotation = Rotation{0}) = 0;
    virtual void text_right_aligned(const Location& aEnd, std::string aText, Color aColor, Scaled aSize, const TextStyle& aTextStyle = TextStyle(), Rotation aRotation = Rotation{0}) = 0;
    virtual Size text_size(std::string aText, Pixels aSize, const TextStyle& aTextStyle = TextStyle(), double* x_bearing = nullptr) = 0;
    virtual Size text_size(std::string aText, Scaled aSize, const TextStyle& aTextStyle = TextStyle(), double* x_bearing = nullptr) = 0;

    inline Scaled convert(Pixels a) const { return Scaled{a.value() / scale()}; }

    virtual inline double scale() const { return width_in_parent() / viewport().size.width; }
    virtual inline Location origin_offset() const { return origin_in_parent(); }

    virtual void new_page() = 0;

    virtual inline Surface& root() { return *this; }
    virtual inline const Surface& root() const { return *this; }

 protected:
    inline Surface() : mOriginInParent{0, 0}, mWidthInParent{viewport().size.width} {}
    inline Surface(const Location& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport)
        : mViewport{aViewport}, mOriginInParent{aOriginInParent}, mWidthInParent{aWidthInParent.value()} {}

    virtual Surface* make_child(const Location& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport, bool aClip) = 0;

    virtual inline bool clip() const { return false; }
    inline void change_origin(const Location& aOriginInParent) { mOriginInParent = aOriginInParent; }
    inline void change_width_in_parent(double aWidthInParent) { mWidthInParent = aWidthInParent; }

 private:
    Viewport mViewport;
    Location mOriginInParent;
    double mWidthInParent;
    std::vector<std::shared_ptr<Surface>> mChildren;

}; // class Surface

// ----------------------------------------------------------------------

template <typename Parent> class SurfaceChild : public Parent
{
 public:
    inline Surface& root() override { return parent().root(); }
    inline const Surface& root() const override { return parent().root(); }

    virtual Surface& parent() = 0;
    virtual const Surface& parent() const = 0;

    inline void new_page() override { root().new_page(); }
    inline void move(const Location& aOriginInParent) override { this->change_origin(aOriginInParent); }
    inline void move_resize(const Location& aOriginInParent, double aWidthInParent) override { this->change_origin(aOriginInParent); this->change_width_in_parent(aWidthInParent); }

    inline double scale() const override { return parent().scale() * (this->width_in_parent() / this->viewport().size.width); }
    inline Location origin_offset() const override { return parent().origin_offset() + this->origin_in_parent() * parent().scale(); }

 protected:
    inline SurfaceChild(const Location& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport, bool aClip)
        : Parent{aOriginInParent, aWidthInParent, aViewport}, mClip{aClip} {}

    inline bool clip() const override { return mClip; }

 private:
    bool mClip;                 // force surface area clipping

}; // class SurfaceChild<Parent>

// ----------------------------------------------------------------------

#ifdef ACMACS_TARGET_OS
inline std::ostream& operator << (std::ostream& out, const Surface& aSurface)
{
    return out << "Surface(" << aSurface.viewport() << ", Origin" << aSurface.origin_in_parent() << ", width=" << aSurface.width_in_parent() << ')';
}
#endif

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
