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

            void line(const PointCoordinates& a, const PointCoordinates& b, Color aColor, Pixels aWidth, LineCap aLineCap = LineCap::Butt) override;
            void line(const PointCoordinates& a, const PointCoordinates& b, Color aColor, Pixels aWidth, Dash aDash, LineCap aLineCap = LineCap::Butt) override;
            void line(const PointCoordinates& a, const PointCoordinates& b, Color aColor, Scaled aWidth, LineCap aLineCap = LineCap::Butt) override;
            void rectangle(const PointCoordinates& a, const Size& s, Color aColor, Pixels aWidth, LineCap aLineCap = LineCap::Butt) override;
            void rectangle_filled(const PointCoordinates& a, const Size& s, Color aOutlineColor, Pixels aWidth, Color aFillColor, LineCap aLineCap = LineCap::Butt) override;

            void circle(const PointCoordinates& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth) override;
            void circle(const PointCoordinates& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth) override;
            void circle_filled(const PointCoordinates& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Dash aOutlineDash, Color aFillColor) override;
            void circle_filled(const PointCoordinates& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Dash aOutlineDash, Color aFillColor) override;
            void egg(const PointCoordinates& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth) override;
            void egg(const PointCoordinates& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth) override;
            void egg_filled(const PointCoordinates& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Dash aOutlineDash, Color aFillColor) override;
            void egg_filled(const PointCoordinates& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Dash aOutlineDash, Color aFillColor) override;
            void ugly_egg(const PointCoordinates& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth) override;
            void ugly_egg(const PointCoordinates& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth) override;
            void ugly_egg_filled(const PointCoordinates& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Dash aOutlineDash, Color aFillColor) override;
            void ugly_egg_filled(const PointCoordinates& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Dash aOutlineDash, Color aFillColor) override;
            void sector_filled(const PointCoordinates& aCenter, Scaled aDiameter, Rotation aStart, Rotation aEnd, Color aOutlineColor, Pixels aOutlineWidth, Color aRadiusColor, Pixels aRadiusWidth,
                               Dash aRadiusDash, Color aFillColor) override;
            void square_filled(const PointCoordinates& aCenter, Pixels aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor,
                               LineCap aLineCap = LineCap::Butt) override;
            void square_filled(const PointCoordinates& aCenter, Scaled aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor,
                               LineCap aLineCap = LineCap::Butt) override;
            void triangle_filled(const PointCoordinates& aCenter, Pixels aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor,
                                 LineCap aLineCap = LineCap::Butt) override;
            void triangle_filled(const PointCoordinates& aCenter, Scaled aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor,
                                 LineCap aLineCap = LineCap::Butt) override;
            void triangle_filled(const PointCoordinates& aCorner1, const PointCoordinates& aCorner2, const PointCoordinates& aCorner3, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor,
                                 LineCap aLineCap = LineCap::Butt) override;

            // if x is negative, move_to is used, otherwise line_to
            void path_outline_negative_move(std::vector<PointCoordinates>::const_iterator first, std::vector<PointCoordinates>::const_iterator last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose = false,
                                            LineCap aLineCap = LineCap::Butt) override;
            void path_outline_negative_move(const double* first, const double* last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose = false, LineCap aLineCap = LineCap::Butt) override;
            void path_fill_negative_move(std::vector<PointCoordinates>::const_iterator first, std::vector<PointCoordinates>::const_iterator last, Color aFillColor) override;
            void path_fill_negative_move(const double* first, const double* last, Color aFillColor) override;

            // all parts are drawn
            void path_outline(std::vector<PointCoordinates>::const_iterator first, std::vector<PointCoordinates>::const_iterator last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose = false,
                              LineCap aLineCap = LineCap::Butt) override;
            void path_outline(const double* first, const double* last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose = false, LineCap aLineCap = LineCap::Butt) override;
            void path_fill(std::vector<PointCoordinates>::const_iterator first, std::vector<PointCoordinates>::const_iterator last, Color aFillColor) override;
            void path_fill(const double* first, const double* last, Color aFillColor) override;

            void text(const PointCoordinates& a, std::string_view aText, Color aColor, Pixels aSize, const TextStyle& aTextStyle = TextStyle(), Rotation aRotation = Rotation{0}) override;
            void text(const PointCoordinates& a, std::string_view aText, Color aColor, Scaled aSize, const TextStyle& aTextStyle = TextStyle(), Rotation aRotation = Rotation{0}) override;
            void text_right_aligned(const PointCoordinates& aEnd, std::string_view aText, Color aColor, Pixels aSize, const TextStyle& aTextStyle = TextStyle(), Rotation aRotation = Rotation{0}) override;
            void text_right_aligned(const PointCoordinates& aEnd, std::string_view aText, Color aColor, Scaled aSize, const TextStyle& aTextStyle = TextStyle(), Rotation aRotation = Rotation{0}) override;
            Size text_size(std::string_view aText, Pixels aSize, const TextStyle& aTextStyle = TextStyle(), double* x_bearing = nullptr) override;
            Size text_size(std::string_view aText, Scaled aSize, const TextStyle& aTextStyle = TextStyle(), double* x_bearing = nullptr) override;

          protected:
            Cairo() : Surface{} {}
            Cairo(const PointCoordinates& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport) : Surface(aOriginInParent, aWidthInParent, aViewport) {}

            Surface* make_child(const PointCoordinates& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport, bool aClip) override;

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
            CairoChild(Cairo& aParent, const PointCoordinates& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport, bool aClip)
                : acmacs::surface::SurfaceChild<Cairo>(aOriginInParent, aWidthInParent, aViewport, aClip), mParent(aParent)
            {
            }

          private:
            Cairo& mParent;

            friend class Cairo;

        }; // class CairoChild

        // ----------------------------------------------------------------------

        class CairoCommon : public Cairo
        {
          public:
            CairoCommon() {}
            ~CairoCommon() override { flush(); }

            cairo_t* cairo_context() override { return mCairoContext; }
            void new_page() override { cairo_show_page(cairo_context()); }

            void flush()        // not virtual, called from destructor
                {
                    if (surface_) {
                        cairo_surface_destroy(surface_);
                        surface_ = nullptr;
                    }
                    if (mCairoContext) {
                        cairo_destroy(mCairoContext);
                        mCairoContext = nullptr;
                    }
                }

          protected:
            void setup(cairo_surface_t* surface, double width, double height, double viewport_width)
            {
                surface_ = surface;
                mCairoContext = cairo_create(surface);
                change_width_in_parent(width);
                viewport({acmacs::PointCoordinates(0.0, 0.0), Size{viewport_width, height * viewport_width / width}});
            }

            auto surface() { return surface_; }

          private:
            cairo_t* mCairoContext;
            cairo_surface_t* surface_; // keep it for PngCairo
        };

    } // namespace internal_1

    // ----------------------------------------------------------------------

    class PdfCairo : public internal_1::CairoCommon
    {
      public:
        PdfCairo(std::string_view aFilename, double aWidth, double aHeight, double aViewportWidth = default_canvas_width)
        {
            setup(cairo_pdf_surface_create(aFilename.empty() ? nullptr : aFilename.data(), aWidth, aHeight), aWidth, aHeight, aViewportWidth);
        }

    }; // class PdfCairo

    // ----------------------------------------------------------------------

    class PdfBufferCairo : public internal_1::CairoCommon
    {
      public:
        PdfBufferCairo(double aWidth, double aHeight, double aViewportWidth = default_canvas_width)
        {
            setup(cairo_pdf_surface_create_for_stream(&writer, this, aWidth, aHeight), aWidth, aHeight, aViewportWidth);
        }

        const std::string& data() const { return data_; }

      private:
        std::string data_;

        static cairo_status_t writer(void *closure, const unsigned char *data, unsigned int length);

    }; // class PdfCairo

    // ----------------------------------------------------------------------

    class PngCairo : public internal_1::CairoCommon
    {
      public:
        PngCairo(std::string_view aFilename, double aWidth, double aHeight, double aViewportWidth = default_canvas_width) : filename_{aFilename}
        {
            setup(cairo_image_surface_create(CAIRO_FORMAT_ARGB32, static_cast<int>(aWidth), static_cast<int>(aHeight)), aWidth, aHeight, aViewportWidth);
        }
        ~PngCairo() override { flush(); }

        void flush() // not virtual, called from destructor
        {
            if (surface()) {
                if (const auto err = cairo_surface_write_to_png(surface(), filename_.c_str()); err != CAIRO_STATUS_SUCCESS)
                    throw std::runtime_error{AD_FORMAT("writing png to {} failed: {}", filename_, err)};
                internal_1::CairoCommon::flush();
            }
        }

      private:
        std::string filename_;

    }; // class PngCairo

} // namespace acmacs::surface

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
