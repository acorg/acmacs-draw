#pragma once

#error Obsolete

#include <memory>
#include <fstream>

#include "acmacs-draw/surface.hh"

// ----------------------------------------------------------------------

namespace acmacs::surface
{
    namespace internal
    {
        class JavascriptChild;

        class Javascript : public acmacs::surface::Surface
        {
          public:
            virtual std::ostream& output() = 0;

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
            Javascript() : Surface{} {}
            Javascript(const Location& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport) : Surface{aOriginInParent, aWidthInParent, aViewport} {}

            Surface* make_child(const Location& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport, bool aClip) override;

          private:
              // friend class context;

        }; // class Javascript

        // ----------------------------------------------------------------------

        class JavascriptChild : public acmacs::surface::SurfaceChild<Javascript>
        {
          public:
            std::ostream& output() override { return dynamic_cast<Javascript&>(root()).output(); }

            Javascript& parent() override { return mParent; }
            const Javascript& parent() const override { return mParent; }

          protected:
            JavascriptChild(Javascript& aParent, const Location& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport, bool aClip)
                : acmacs::surface::SurfaceChild<Javascript>{aOriginInParent, aWidthInParent, aViewport, aClip}, mParent{aParent}
            {
            }

          private:
            Javascript& mParent;

            friend class Javascript;

        }; // class JavascriptChild

    } // namespace internal

    // ----------------------------------------------------------------------

    class Html : public internal::Javascript
    {
      public:
        Html(std::string aFilename, double aWidth, double aHeight, double aViewportWidth = default_canvas_width);
        ~Html() override;

        std::ostream& output() override { return output_; }
        void new_page() override { }

     private:
        std::ofstream output_;

    }; // class Html

} // namespace acmacs::surface

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
