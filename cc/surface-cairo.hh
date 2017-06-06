#pragma once

#include <memory>

#include "acmacs-draw/surface.hh"
#include "acmacs-draw/cairo.hh"

// ----------------------------------------------------------------------

class SurfaceCairoChild;

class SurfaceCairo : public Surface
{
 public:
    virtual SurfaceCairo* parent() = 0;
    virtual const SurfaceCairo* parent() const = 0;
    virtual cairo_t* cairo_context() = 0;

    virtual Surface& subsurface(const Location& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport, bool aClip);
    virtual Surface& subsurface(bool aClip);
    virtual Surface& subsurface(const Location& aOriginInParent, Pixels aWidthInParent, const Viewport& aViewport, bool aClip);

    virtual inline const Viewport& viewport() const { return mViewport; }
    virtual inline const Location& origin_in_parent() const { return mOriginInParent; }
    virtual inline double width_in_parent() const { return mWidthInParent; }

    virtual inline void viewport(const Viewport& aViewport) { mViewport = aViewport; }

    virtual void line(const Location& a, const Location& b, Color aColor, Pixels aWidth, LineCap aLineCap = LineCap::Butt);
    virtual void line(const Location& a, const Location& b, Color aColor, Scaled aWidth, LineCap aLineCap = LineCap::Butt);
    virtual void rectangle(const Location& a, const Size& s, Color aColor, Pixels aWidth, LineCap aLineCap = LineCap::Butt);
    virtual void rectangle_filled(const Location& a, const Size& s, Color aOutlineColor, Pixels aWidth, Color aFillColor, LineCap aLineCap = LineCap::Butt);

    virtual void circle(const Location& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth);
    virtual void circle(const Location& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth);
    virtual void circle_filled(const Location& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor);
    virtual void circle_filled(const Location& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor);
    virtual void sector_filled(const Location& aCenter, Scaled aDiameter, Rotation aStart, Rotation aEnd, Color aOutlineColor, Pixels aOutlineWidth, Color aRadiusColor, Pixels aRadiusWidth, Dash aRadiusDash, Color aFillColor);
    virtual void square_filled(const Location& aCenter, Pixels aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap = LineCap::Butt);
    virtual void square_filled(const Location& aCenter, Scaled aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap = LineCap::Butt);
    virtual void triangle_filled(const Location& aCenter, Pixels aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap = LineCap::Butt);
    virtual void triangle_filled(const Location& aCenter, Scaled aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap = LineCap::Butt);
    virtual void triangle_filled(const Location& aCorner1, const Location& aCorner2, const Location& aCorner3, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap = LineCap::Butt);

    virtual void path_outline(std::vector<Location>::const_iterator first, std::vector<Location>::const_iterator last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose = false, LineCap aLineCap = LineCap::Butt);
    virtual void path_outline(const double* first, const double* last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose = false, LineCap aLineCap = LineCap::Butt);
    virtual void path_fill(std::vector<Location>::const_iterator first, std::vector<Location>::const_iterator last, Color aFillColor);
    virtual void path_fill(const double* first, const double* last, Color aFillColor);

    virtual void double_arrow(const Location& a, const Location& b, Color aColor, Pixels aLineWidth, Pixels aArrowWidth);
    virtual void grid(Scaled aStep, Color aLineColor, Pixels aLineWidth);
    virtual void border(Color aLineColor, Pixels aLineWidth);
    virtual void background(Color aColor);

    virtual void text(const Location& a, std::string aText, Color aColor, Pixels aSize, const TextStyle& aTextStyle = TextStyle(), Rotation aRotation = Rotation{0});
    virtual void text(const Location& a, std::string aText, Color aColor, Scaled aSize, const TextStyle& aTextStyle = TextStyle(), Rotation aRotation = Rotation{0});
    virtual void text_right_aligned(const Location& aEnd, std::string aText, Color aColor, Pixels aSize, const TextStyle& aTextStyle = TextStyle(), Rotation aRotation = Rotation{0});
    virtual void text_right_aligned(const Location& aEnd, std::string aText, Color aColor, Scaled aSize, const TextStyle& aTextStyle = TextStyle(), Rotation aRotation = Rotation{0});
    virtual Size text_size(std::string aText, Pixels aSize, const TextStyle& aTextStyle = TextStyle(), double* x_bearing = nullptr);
    virtual Size text_size(std::string aText, Scaled aSize, const TextStyle& aTextStyle = TextStyle(), double* x_bearing = nullptr);

    virtual inline double scale() const
        {
            return (parent() ? parent()->scale() : 1.0) * (width_in_parent() / viewport().size.width);
        }

    virtual inline Location origin_offset() const
        {
            return parent() ? (parent()->origin_offset() + origin_in_parent() * parent()->scale()) : origin_in_parent();
        }

 protected:
    inline SurfaceCairo() : mViewport{}, mOriginInParent{0, 0}, mWidthInParent{mViewport.size.width} {}
    inline SurfaceCairo(const Location& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport)
        : mViewport(aViewport), mOriginInParent(aOriginInParent), mWidthInParent(aWidthInParent.value()) {}

    virtual Location arrow_head(const Location& a, double angle, double sign, Color aColor, Pixels aArrowWidth);

    inline void change_origin(const Location& aOriginInParent) { mOriginInParent = aOriginInParent; }
    inline void change_width_in_parent(double aWidthInParent) { mWidthInParent = aWidthInParent; }

 private:
    Viewport mViewport;
    Location mOriginInParent;
    double mWidthInParent;
    std::vector<std::shared_ptr<SurfaceCairoChild>> mChildren;

    friend class context;
    virtual inline bool clip() const { return false; }

}; // class SurfaceCairo

// ----------------------------------------------------------------------

class SurfaceCairoChild : public SurfaceCairo
{
 public:
    inline SurfaceCairoChild(SurfaceCairo& aParent, const Location& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport, bool aClip)
        : SurfaceCairo{aOriginInParent, aWidthInParent, aViewport}, mParent(aParent), mClip(aClip) {}
    // inline SurfaceCairoChild(SurfaceCairo& aParent, const Size& aOffset, const Size& aSize, double aScale, bool aClip)
    //     : mParent(aParent), mOffset(aOffset), mSize(aSize), mScale(aScale), mClip(aClip) {}

    virtual inline SurfaceCairo* parent() { return &mParent; }
    virtual inline const SurfaceCairo* parent() const { return &mParent; }
    virtual inline cairo_t* cairo_context() { return mParent.cairo_context(); }
    virtual inline void new_page() { mParent.new_page(); }

    virtual inline void move(const Location& aOriginInParent) { change_origin(aOriginInParent); }
    virtual inline void move_resize(const Location& aOriginInParent, double aWidthInParent) { change_origin(aOriginInParent); change_width_in_parent(aWidthInParent); }

 private:
    SurfaceCairo& mParent;
    bool mClip;                 // force surface area clipping

      // inline SurfaceCairoChild() : SurfaceCairo(), mParent(nullptr) {}
    virtual inline bool clip() const { return mClip; }

}; // class SurfaceCairoChild

// ----------------------------------------------------------------------

class PdfCairo : public SurfaceCairo
{
 public:
    PdfCairo(std::string aFilename, double aWidth, double aHeight, double aViewportWidth = default_canvas_width);
    virtual ~PdfCairo();

    virtual inline cairo_t* cairo_context() { return mCairoContext; }
    virtual inline SurfaceCairo* parent() { return nullptr; }
    virtual inline const SurfaceCairo* parent() const { return nullptr; }
    virtual inline void new_page() { cairo_show_page(cairo_context()); }

    virtual inline void move(const Location&) { throw std::logic_error("cannot move PdfCairo surface"); }
    virtual inline void move_resize(const Location&, double) { throw std::logic_error("cannot move/resize PdfCairo surface"); }

 private:
    cairo_t* mCairoContext;

}; // class PdfCairo

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
