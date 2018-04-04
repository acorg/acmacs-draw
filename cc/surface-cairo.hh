#pragma once

#include <memory>

#include "acmacs-draw/surface.hh"
#include "acmacs-draw/cairo.hh"

// ----------------------------------------------------------------------

namespace acmacs::surface
{
    namespace internal_1
    {
        class CairoChild;

        class Cairo : public acmacs::surface::Surface
        {
          public:
            virtual cairo_t* cairo_context() = 0;

            void line(const Location& a, const Location& b, Color aColor, Pixels aWidth, LineCap aLineCap = LineCap::Butt) override;
            void line(const Location& a, const Location& b, Color aColor, Scaled aWidth, LineCap aLineCap = LineCap::Butt) override;
            void rectangle(const Location& a, const Size& s, Color aColor, Pixels aWidth, LineCap aLineCap = LineCap::Butt) override;
            void rectangle_filled(const Location& a, const Size& s, Color aOutlineColor, Pixels aWidth, Color aFillColor, LineCap aLineCap = LineCap::Butt) override;

            void circle(const Location& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth) override;
            void circle(const Location& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth) override;
            void circle_filled(const Location& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor) override;
            void circle_filled(const Location& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor) override;
            void sector_filled(const Location& aCenter, Scaled aDiameter, Rotation aStart, Rotation aEnd, Color aOutlineColor, Pixels aOutlineWidth, Color aRadiusColor, Pixels aRadiusWidth,
                               Dash aRadiusDash, Color aFillColor) override;
            void square_filled(const Location& aCenter, Pixels aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor,
                               LineCap aLineCap = LineCap::Butt) override;
            void square_filled(const Location& aCenter, Scaled aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor,
                               LineCap aLineCap = LineCap::Butt) override;
            void triangle_filled(const Location& aCenter, Pixels aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor,
                                 LineCap aLineCap = LineCap::Butt) override;
            void triangle_filled(const Location& aCenter, Scaled aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor,
                                 LineCap aLineCap = LineCap::Butt) override;
            void triangle_filled(const Location& aCorner1, const Location& aCorner2, const Location& aCorner3, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor,
                                 LineCap aLineCap = LineCap::Butt) override;

            // if x is negative, move_to is used, otherwise line_to
            void path_outline_negative_move(std::vector<Location>::const_iterator first, std::vector<Location>::const_iterator last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose = false,
                                            LineCap aLineCap = LineCap::Butt) override;
            void path_outline_negative_move(const double* first, const double* last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose = false, LineCap aLineCap = LineCap::Butt) override;
            void path_fill_negative_move(std::vector<Location>::const_iterator first, std::vector<Location>::const_iterator last, Color aFillColor) override;
            void path_fill_negative_move(const double* first, const double* last, Color aFillColor) override;

            // all parts are drawn
            void path_outline(std::vector<Location>::const_iterator first, std::vector<Location>::const_iterator last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose = false,
                              LineCap aLineCap = LineCap::Butt) override;
            void path_outline(const double* first, const double* last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose = false, LineCap aLineCap = LineCap::Butt) override;
            void path_fill(std::vector<Location>::const_iterator first, std::vector<Location>::const_iterator last, Color aFillColor) override;
            void path_fill(const double* first, const double* last, Color aFillColor) override;

            void text(const Location& a, std::string aText, Color aColor, Pixels aSize, const TextStyle& aTextStyle = TextStyle(), Rotation aRotation = Rotation{0}) override;
            void text(const Location& a, std::string aText, Color aColor, Scaled aSize, const TextStyle& aTextStyle = TextStyle(), Rotation aRotation = Rotation{0}) override;
            void text_right_aligned(const Location& aEnd, std::string aText, Color aColor, Pixels aSize, const TextStyle& aTextStyle = TextStyle(), Rotation aRotation = Rotation{0}) override;
            void text_right_aligned(const Location& aEnd, std::string aText, Color aColor, Scaled aSize, const TextStyle& aTextStyle = TextStyle(), Rotation aRotation = Rotation{0}) override;
            Size text_size(std::string aText, Pixels aSize, const TextStyle& aTextStyle = TextStyle(), double* x_bearing = nullptr) override;
            Size text_size(std::string aText, Scaled aSize, const TextStyle& aTextStyle = TextStyle(), double* x_bearing = nullptr) override;

          protected:
            Cairo() : Surface{} {}
            Cairo(const Location& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport) : Surface{aOriginInParent, aWidthInParent, aViewport} {}

            Surface* make_child(const Location& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport, bool aClip) override;

          private:
            friend class context;

        }; // class Cairo

        // ----------------------------------------------------------------------

        class CairoChild : public acmacs::surface::SurfaceChild<Cairo>
        {
          public:
            cairo_t* cairo_context() override { return dynamic_cast<Cairo&>(root()).cairo_context(); }

            Cairo& parent() override { return mParent; }
            const Cairo& parent() const override { return mParent; }

          protected:
            CairoChild(Cairo& aParent, const Location& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport, bool aClip)
                : acmacs::surface::SurfaceChild<Cairo>(aOriginInParent, aWidthInParent, aViewport, aClip), mParent(aParent)
            {
            }

          private:
            Cairo& mParent;

            friend class Cairo;

        }; // class CairoChild

    } // namespace internal_1

    // ----------------------------------------------------------------------

    class PdfCairo : public internal_1::Cairo
    {
      public:
        PdfCairo(std::string aFilename, double aWidth, double aHeight, double aViewportWidth = default_canvas_width);
        ~PdfCairo() override;

        cairo_t* cairo_context() override { return mCairoContext; }
        // virtual Cairo* parent() { return nullptr; }
        // virtual const Cairo* parent() const { return nullptr; }
        void new_page() override { cairo_show_page(cairo_context()); }

      private:
        cairo_t* mCairoContext;

    }; // class PdfCairo

} // namespace acmacs::surface

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
