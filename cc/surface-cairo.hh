#pragma once

#include <memory>

#include "acmacs-draw/surface.hh"
#include "acmacs-draw/cairo.hh"

// ----------------------------------------------------------------------

class SurfaceCairoChild;

class SurfaceCairo : public Surface
{
 public:
    // virtual SurfaceCairo* parent() = 0;
    // virtual const SurfaceCairo* parent() const = 0;
    virtual cairo_t* cairo_context() = 0;

    void line(const Location& a, const Location& b, Color aColor, Pixels aWidth, LineCap aLineCap = LineCap::Butt) override;
    void line(const Location& a, const Location& b, Color aColor, Scaled aWidth, LineCap aLineCap = LineCap::Butt) override;
    void rectangle(const Location& a, const Size& s, Color aColor, Pixels aWidth, LineCap aLineCap = LineCap::Butt) override;
    void rectangle_filled(const Location& a, const Size& s, Color aOutlineColor, Pixels aWidth, Color aFillColor, LineCap aLineCap = LineCap::Butt) override;

    void circle(const Location& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth) override;
    void circle(const Location& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth) override;
    void circle_filled(const Location& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor) override;
    void circle_filled(const Location& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor) override;
    void sector_filled(const Location& aCenter, Scaled aDiameter, Rotation aStart, Rotation aEnd, Color aOutlineColor, Pixels aOutlineWidth, Color aRadiusColor, Pixels aRadiusWidth, Dash aRadiusDash, Color aFillColor) override;
    void square_filled(const Location& aCenter, Pixels aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap = LineCap::Butt) override;
    void square_filled(const Location& aCenter, Scaled aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap = LineCap::Butt) override;
    void triangle_filled(const Location& aCenter, Pixels aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap = LineCap::Butt) override;
    void triangle_filled(const Location& aCenter, Scaled aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap = LineCap::Butt) override;
    void triangle_filled(const Location& aCorner1, const Location& aCorner2, const Location& aCorner3, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap = LineCap::Butt) override;

    void path_outline(std::vector<Location>::const_iterator first, std::vector<Location>::const_iterator last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose = false, LineCap aLineCap = LineCap::Butt) override;
    void path_outline(const double* first, const double* last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose = false, LineCap aLineCap = LineCap::Butt) override;
    void path_fill(std::vector<Location>::const_iterator first, std::vector<Location>::const_iterator last, Color aFillColor) override;
    void path_fill(const double* first, const double* last, Color aFillColor) override;

    void double_arrow(const Location& a, const Location& b, Color aColor, Pixels aLineWidth, Pixels aArrowWidth) override;
    void grid(Scaled aStep, Color aLineColor, Pixels aLineWidth) override;
    void border(Color aLineColor, Pixels aLineWidth) override;
    void background(Color aColor) override;

    void text(const Location& a, std::string aText, Color aColor, Pixels aSize, const TextStyle& aTextStyle = TextStyle(), Rotation aRotation = Rotation{0}) override;
    void text(const Location& a, std::string aText, Color aColor, Scaled aSize, const TextStyle& aTextStyle = TextStyle(), Rotation aRotation = Rotation{0}) override;
    void text_right_aligned(const Location& aEnd, std::string aText, Color aColor, Pixels aSize, const TextStyle& aTextStyle = TextStyle(), Rotation aRotation = Rotation{0}) override;
    void text_right_aligned(const Location& aEnd, std::string aText, Color aColor, Scaled aSize, const TextStyle& aTextStyle = TextStyle(), Rotation aRotation = Rotation{0}) override;
    Size text_size(std::string aText, Pixels aSize, const TextStyle& aTextStyle = TextStyle(), double* x_bearing = nullptr) override;
    Size text_size(std::string aText, Scaled aSize, const TextStyle& aTextStyle = TextStyle(), double* x_bearing = nullptr) override;

 protected:
    inline SurfaceCairo() : Surface{} {}
    inline SurfaceCairo(const Location& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport)
        : Surface{aOriginInParent, aWidthInParent, aViewport} {}

    Location arrow_head(const Location& a, double angle, double sign, Color aColor, Pixels aArrowWidth) override;
    Surface* make_child(const Location& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport, bool aClip) override;

 private:
    // std::vector<std::shared_ptr<SurfaceCairoChild>> mChildren;

    friend class context;

}; // class SurfaceCairo

// ----------------------------------------------------------------------

class SurfaceCairoChild : public SurfaceCairo
{
 public:
    inline cairo_t* cairo_context() override { return dynamic_cast<SurfaceCairo&>(root()).cairo_context(); }

    inline Surface& root() override { return mParent.root(); }
    inline const Surface& root() const override { return mParent.root(); }
 //    virtual inline SurfaceCairo* parent() { return &mParent; }
 //    virtual inline const SurfaceCairo* parent() const { return &mParent; }

    inline void new_page() override { root().new_page(); }
    inline void move(const Location& aOriginInParent) override { change_origin(aOriginInParent); }
    inline void move_resize(const Location& aOriginInParent, double aWidthInParent) override { change_origin(aOriginInParent); change_width_in_parent(aWidthInParent); }

    inline double scale() const override { return mParent.scale() * (width_in_parent() / viewport().size.width); }
    inline Location origin_offset() const override { return mParent.origin_offset() + origin_in_parent() * mParent.scale(); }

 protected:
    inline bool clip() const override { return mClip; }

 private:
    SurfaceCairo& mParent;
    bool mClip;                 // force surface area clipping

    inline SurfaceCairoChild(SurfaceCairo& aParent, const Location& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport, bool aClip)
        : SurfaceCairo{aOriginInParent, aWidthInParent, aViewport}, mParent{aParent}, mClip{aClip} {}
    // inline SurfaceCairoChild(SurfaceCairo& aParent, const Size& aOffset, const Size& aSize, double aScale, bool aClip)
    //     : mParent(aParent), mOffset(aOffset), mSize(aSize), mScale(aScale), mClip(aClip) {}

    friend class SurfaceCairo;

}; // class SurfaceCairoChild

// ----------------------------------------------------------------------

class PdfCairo : public SurfaceCairo
{
 public:
    PdfCairo(std::string aFilename, double aWidth, double aHeight, double aViewportWidth = default_canvas_width);
    virtual ~PdfCairo();

    inline cairo_t* cairo_context() override { return mCairoContext; }
    // virtual inline SurfaceCairo* parent() { return nullptr; }
    // virtual inline const SurfaceCairo* parent() const { return nullptr; }
    inline void new_page() override { cairo_show_page(cairo_context()); }

    inline void move(const Location&) override { throw std::logic_error("cannot move PdfCairo surface"); }
    inline void move_resize(const Location&, double) override { throw std::logic_error("cannot move/resize PdfCairo surface"); }

 private:
    cairo_t* mCairoContext;

}; // class PdfCairo


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
