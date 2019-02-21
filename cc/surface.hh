#pragma once

#include <vector>
#include <memory>
#include <cassert>

#include "acmacs-base/log.hh"
#include "acmacs-base/color.hh"
#include "acmacs-base/size-scale.hh"
#include "acmacs-base/text-style.hh"
#include "acmacs-draw/viewport.hh"
#include "acmacs-draw/surface-line.hh"

// ----------------------------------------------------------------------

namespace acmacs::surface
{
    class Surface
    {
      public:
        using Location = acmacs::Location2D;
        using Size = acmacs::Size;
        using Viewport = acmacs::Viewport;
        using TextStyle = acmacs::TextStyle;

        [[noreturn]] Surface(const Surface&) { throw std::runtime_error("Surface copying forbidden!"); } // cannto make it private due to using vector<SurfaceCairoChild>
        virtual ~Surface()
        {
        }

        constexpr static const double default_canvas_width = 1000.0;

        const Viewport& viewport() const { return mViewport; }
        void viewport(const Viewport& aViewport) { mViewport = aViewport; }

        const Location& origin_in_parent() const { return mOriginInParent; }
        double width_in_parent() const { return mWidthInParent; }

        double width_in_pixels() const { return viewport().size.width * scale(); }
        double height_in_pixels() const { return viewport().size.height * scale(); }

        virtual void move(const Location& /*aOriginInParent*/) { throw std::logic_error("cannot move this surface"); }
        virtual void move_resize(const Location& /*aOriginInParent*/, double /*aWidthInParent*/) { throw std::logic_error("cannot move/resize this surface"); }
        void move_resize_viewport(const Location& aOriginInParent, double aWidthInParent, const Viewport& aViewport)
        {
            move_resize(aOriginInParent, aWidthInParent);
            viewport(aViewport);
        }

        double aspect() const { return viewport().aspect(); }

        Surface& subsurface(const Location& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport, bool aClip)
        {
            auto* child = make_child(aOriginInParent, aWidthInParent, aViewport, aClip);
            mChildren.emplace_back(child);
            return *child;
        }
        Surface& subsurface(bool aClip) { return subsurface(Location{}, Scaled{}, Viewport{}, aClip); }
        Surface& subsurface(const Location& aOriginInParent, Pixels aWidthInParent, const Viewport& aViewport, bool aClip) // origin is in pixels
        {
            return subsurface(aOriginInParent / scale(), Scaled{aWidthInParent.value() / scale()}, aViewport, aClip);
        }

        virtual void line(const Location& a, const Location& b, Color aColor, Pixels aWidth, LineCap aLineCap = LineCap::Butt) = 0;
        virtual void line(const Location& a, const Location& b, Color aColor, Pixels aWidth, Dash aDash, LineCap aLineCap = LineCap::Butt) = 0;
        virtual void line(const Location& a, const Location& b, Color aColor, Scaled aWidth, LineCap aLineCap = LineCap::Butt) = 0;
        virtual void rectangle(const Location& a, const Size& s, Color aColor, Pixels aWidth, LineCap aLineCap = LineCap::Butt) = 0;
        virtual void rectangle_filled(const Location& a, const Size& s, Color aOutlineColor, Pixels aWidth, Color aFillColor, LineCap aLineCap = LineCap::Butt) = 0;

        virtual void circle(const Location& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth) = 0;
        virtual void circle(const Location& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth) = 0;
        void circle(const Location& aCenter, Pixels aDiameter, Color aOutlineColor, Pixels aOutlineWidth) { circle(aCenter, aDiameter, Aspect{1}, Rotation{0}, aOutlineColor, aOutlineWidth); }
        void circle(const Location& aCenter, Scaled aDiameter, Color aOutlineColor, Pixels aOutlineWidth) { circle(aCenter, aDiameter, Aspect{1}, Rotation{0}, aOutlineColor, aOutlineWidth); }
        virtual void circle_filled(const Location& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor) = 0;
        virtual void circle_filled(const Location& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor) = 0;
        virtual void sector_filled(const Location& aCenter, Scaled aDiameter, Rotation aStart, Rotation aEnd, Color aOutlineColor, Pixels aOutlineWidth, Color aRadiusColor, Pixels aRadiusWidth,
                           Dash aRadiusDash, Color aFillColor) = 0;
        virtual void square_filled(const Location& aCenter, Pixels aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor,
                                   LineCap aLineCap = LineCap::Butt) = 0;
        virtual void square_filled(const Location& aCenter, Scaled aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor,
                                   LineCap aLineCap = LineCap::Butt) = 0;
        virtual void triangle_filled(const Location& aCenter, Pixels aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor,
                                     LineCap aLineCap = LineCap::Butt) = 0;
        virtual void triangle_filled(const Location& aCenter, Scaled aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor,
                                     LineCap aLineCap = LineCap::Butt) = 0;
        virtual void triangle_filled(const Location& aCorner1, const Location& aCorner2, const Location& aCorner3, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor,
                                     LineCap aLineCap = LineCap::Butt) = 0;

        // if x is negative, move_to is used, otherwise line_to
        virtual void path_outline_negative_move(std::vector<Location>::const_iterator first, std::vector<Location>::const_iterator last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose = false, LineCap aLineCap = LineCap::Butt) = 0;
        virtual void path_outline_negative_move(const double* first, const double* last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose = false, LineCap aLineCap = LineCap::Butt) = 0;
        virtual void path_fill_negative_move(std::vector<Location>::const_iterator first, std::vector<Location>::const_iterator last, Color aFillColor) = 0;
        virtual void path_fill_negative_move(const double* first, const double* last, Color aFillColor) = 0;

        // all parts are drawn
        virtual void path_outline(std::vector<Location>::const_iterator first, std::vector<Location>::const_iterator last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose = false, LineCap aLineCap = LineCap::Butt) = 0;
        virtual void path_outline(const double* first, const double* last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose = false, LineCap aLineCap = LineCap::Butt) = 0;
        virtual void path_fill(std::vector<Location>::const_iterator first, std::vector<Location>::const_iterator last, Color aFillColor) = 0;
        virtual void path_fill(const double* first, const double* last, Color aFillColor) = 0;

        void double_arrow(const Location& a, const Location& b, Color aColor, Pixels aLineWidth, Pixels aArrowWidth);
        Location arrow_head(const Location& a, double angle, double sign, Color aColor, Pixels aArrowWidth, bool aFilled);
        void grid(Scaled aStep, Color aLineColor, Pixels aLineWidth);

        virtual void text(const Location& a, std::string aText, Color aColor, Pixels aSize, const TextStyle& aTextStyle = TextStyle(), Rotation aRotation = Rotation{0}) = 0;
        virtual void text(const Location& a, std::string aText, Color aColor, Scaled aSize, const TextStyle& aTextStyle = TextStyle(), Rotation aRotation = Rotation{0}) = 0;
        virtual void text_right_aligned(const Location& aEnd, std::string aText, Color aColor, Pixels aSize, const TextStyle& aTextStyle = TextStyle(), Rotation aRotation = Rotation{0}) = 0;
        virtual void text_right_aligned(const Location& aEnd, std::string aText, Color aColor, Scaled aSize, const TextStyle& aTextStyle = TextStyle(), Rotation aRotation = Rotation{0}) = 0;
        virtual Size text_size(std::string aText, Pixels aSize, const TextStyle& aTextStyle = TextStyle(), double* x_bearing = nullptr) = 0;
        virtual Size text_size(std::string aText, Scaled aSize, const TextStyle& aTextStyle = TextStyle(), double* x_bearing = nullptr) = 0;

        Scaled convert(Pixels a) const { return Scaled{a.value() / scale()}; }
        Scaled convert(Scaled a) const { return a; }

        virtual double scale() const { return width_in_parent() / viewport().size.width; }
        virtual Location origin_offset() const { return origin_in_parent(); }

        virtual void new_page() = 0;

        virtual Surface& root() { return *this; }
        virtual const Surface& root() const { return *this; }

        virtual bool clip() const { return false; }

      protected:
        Surface() : mOriginInParent{0, 0}, mWidthInParent{viewport().size.width} {}
        Surface(const Location& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport) : mViewport{aViewport}, mOriginInParent{aOriginInParent}, mWidthInParent{aWidthInParent.value()} {}

        virtual Surface* make_child(const Location& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport, bool aClip) = 0;

        void change_origin(const Location& aOriginInParent) { mOriginInParent = aOriginInParent; }
        void change_width_in_parent(double aWidthInParent) { mWidthInParent = aWidthInParent; }

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
        using Location = typename Parent::Location;
        using Size = typename Parent::Size;
        using Viewport = typename Parent::Viewport;

        Surface& root() override { return parent().root(); }
        const Surface& root() const override { return parent().root(); }

        virtual Surface& parent() = 0;
        virtual const Surface& parent() const = 0;

        void new_page() override { root().new_page(); }
        void move(const Location& aOriginInParent) override { this->change_origin(aOriginInParent); }
        void move_resize(const Location& aOriginInParent, double aWidthInParent) override
        {
            assert(aWidthInParent >= 0);
            this->change_origin(aOriginInParent);
            this->change_width_in_parent(aWidthInParent);
        }

        double scale() const override { return parent().scale() * (this->width_in_parent() / this->viewport().size.width); }
        Location origin_offset() const override { return parent().origin_offset() + this->origin_in_parent() * parent().scale(); }

        bool clip() const override { return mClip; }

      protected:
        SurfaceChild(const Location& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport, bool aClip) : Parent{aOriginInParent, aWidthInParent, aViewport}, mClip{aClip} {}

      private:
        bool mClip; // force surface area clipping

    }; // class SurfaceChild<Parent>

    // ----------------------------------------------------------------------

    inline std::ostream& operator<<(std::ostream& out, const Surface& aSurface)
    {
        return out << "Surface(" << aSurface.viewport() << ", Origin" << aSurface.origin_in_parent() << ", width=" << aSurface.width_in_parent() << ')';
    }

} // namespace acmacs::surface

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
