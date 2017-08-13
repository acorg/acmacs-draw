#include "surface-cairo.hh"
#include "acmacs-base/float.hh"

// ----------------------------------------------------------------------

class CairoPath
{
 public:
    inline ~CairoPath() { if (mPath) cairo_path_destroy(mPath); }
    inline CairoPath(CairoPath&& aSource) : mPath(aSource.mPath) { aSource.mPath = nullptr; }

 private:
    cairo_path_t* mPath;

    friend class context;
    inline CairoPath(cairo_path_t* aPath) : mPath(aPath) {}
    CairoPath(const CairoPath& aSource) = delete;
    inline operator cairo_path_t*() const { return mPath; }

}; // class CairoPath

// ----------------------------------------------------------------------

class context
{
 public:
    context(SurfaceCairo& aSurface)
        : mSurface(aSurface), mScale(aSurface.scale())
        {
              // std::cerr << "origin_offset: " << aSurface.origin_offset() << "  scale: " << mScale << std::endl;
            cairo_save(cairo_context());
            translate(aSurface.origin_offset());
            scale(mScale);
            translate(- aSurface.viewport().origin);
            if (aSurface.clip()) {
                new_path();
                move_to(aSurface.viewport().origin);
                line_to(aSurface.viewport().top_right());
                line_to(aSurface.viewport().bottom_right());
                line_to(aSurface.viewport().bottom_left());
                close_path();
                clip();
            }
        }
    ~context()
        {
            cairo_restore(cairo_context());
        }

    template <typename S> inline context& set_line_width(S aWidth) { cairo_set_line_width(cairo_context(), convert(aWidth)); return *this; }
    inline context& set_source_rgba(Color aColor) { cairo_set_source_rgba(cairo_context(), aColor.red(), aColor.green(), aColor.blue(), aColor.alpha()); return *this; }

    inline context& set_line_cap(Surface::LineCap aLineCap) { cairo_set_line_cap(cairo_context(), cairo_line_cap(aLineCap)); return *this; }
    inline context& set_line_join(Surface::LineJoin aLineJoin) { cairo_set_line_join(cairo_context(), cairo_line_join(aLineJoin)); return *this; }
    inline context& set_line_dash(Surface::Dash aLineDash)
        {
            double dash_size;
            switch (aLineDash) {
              case Surface::Dash::NoDash:
                  break;
              case Surface::Dash::Dash1:
                  dash_size = convert(Pixels{1});
                  cairo_set_dash(cairo_context(), &dash_size, 1, 0);
                  break;
              case Surface::Dash::Dash2:
                  dash_size = convert(Pixels{5});
                  cairo_set_dash(cairo_context(), &dash_size, 1, 0);
                  break;
            }
            return *this;
        }

    inline context& move_to() { cairo_move_to(cairo_context(), 0.0, 0.0); return *this; }
    inline context& move_to(const Location& a) { cairo_move_to(cairo_context(), a.x, a.y); return *this; }
    template <typename S> inline context& move_to(S x, S y) { cairo_move_to(cairo_context(), convert(x), convert(y)); return *this; }
    inline context& line_to(const Location& a) { cairo_line_to(cairo_context(), a.x, a.y); return *this; }
    template <typename S> inline context& line_to(S x, S y) { cairo_line_to(cairo_context(), convert(x), convert(y)); return *this; }
    inline context& lines_to(std::vector<Location>::const_iterator first, std::vector<Location>::const_iterator last) { for ( ; first != last; ++first) { line_to(*first); } return *this; }
    inline context& rectangle(const Location& a, const Size& s) { cairo_rectangle(cairo_context(), a.x, a.y, s.width, s.height); return *this; }
    template <typename S> inline context& rectangle(S x1, S y1, S x2, S y2) { cairo_rectangle(cairo_context(), convert(x1), convert(y1), convert(x2) - convert(x1), convert(y2) - convert(y1)); return *this; }
      // inline context& arc(const Location& a, double radius, double angle1, double angle2) { cairo_arc(cairo_context(), a.x, a.y, radius, angle1, angle2); return *this; }
    template <typename S> inline context& circle(S radius) { cairo_arc(cairo_context(), 0.0, 0.0, convert(radius), 0.0, 2.0 * M_PI); return *this; }
    template <typename S> inline context& arc(S radius, Rotation start, Rotation end) { cairo_arc(cairo_context(), 0.0, 0.0, convert(radius), start.value(), end.value()); return *this; }
    inline context& circle(const Location& a, double radius) { cairo_arc(cairo_context(), a.x, a.y, radius, 0.0, 2.0 * M_PI); return *this; }
    inline context& stroke() { cairo_stroke(cairo_context()); return *this; }
    inline context& stroke_preserve() { cairo_stroke_preserve(cairo_context()); return *this; }
    inline context& fill() { cairo_fill(cairo_context()); return *this; }
    inline context& fill_preserve() { cairo_fill_preserve(cairo_context()); return *this; }
    inline context& translate(const Size& a) { cairo_translate(cairo_context(), a.width, a.height); return *this; }
    inline context& translate(const Location& a) { cairo_translate(cairo_context(), a.x, a.y); return *this; }
    inline context& rotate(Rotation aAngle) { cairo_rotate(cairo_context(), aAngle.value()); return *this; }
    inline context& scale(double x, double y) { cairo_scale(cairo_context(), x, y); return *this; }
    inline context& scale(double x) { cairo_scale(cairo_context(), x, x); return *this; }
    inline context& aspect(Aspect x) { cairo_scale(cairo_context(), x.value(), 1.0); return *this; }
    inline context& clip() { cairo_clip(cairo_context()); return *this; }
    inline context& new_path() { cairo_new_path(cairo_context()); return *this; }
    inline context& close_path() { cairo_close_path(cairo_context()); return *this; }
    inline context& close_path_if(bool aClose) { if (aClose) cairo_close_path(cairo_context()); return *this; }
    inline context& append_path(CairoPath& aPath) { cairo_append_path(cairo_context(), aPath); return *this; }
    inline CairoPath copy_path() { return std::move(cairo_copy_path(cairo_context())); }

    template <typename S> inline context& prepare_for_text(S aSize, const TextStyle& aTextStyle) { cairo_select_font_face(cairo_context(), aTextStyle.font_family().c_str(), cairo_font_slant(aTextStyle.slant()), cairo_font_weight(aTextStyle.weight())); cairo_set_font_size(cairo_context(), convert(aSize)); return *this; }
    inline context& show_text(std::string aText) { cairo_show_text(cairo_context(), aText.c_str()); return *this; }
    inline context& text_extents(std::string aText, cairo_text_extents_t& extents) { cairo_text_extents(cairo_context(), aText.c_str(), &extents); return *this; }

      // if Location::x is negative - move_to, else - path_to. It assumes origin is {0,0}!!!
    inline context& move_to_line_to(std::vector<Location>::const_iterator first, std::vector<Location>::const_iterator last)
        {
            for ( ; first != last; ++first) {
                if (first->x < 0)
                    move_to({std::abs(first->x), std::abs(first->y)});
                else
                    line_to(*first);
            }
            return *this;
        }

      // the same as above but with raw data
    inline context& move_to_line_to(const double* first, const double* last)
        {
            for ( ; first != last; first += 2) {
                if (*first < 0)
                    move_to({std::abs(*first), std::abs(*(first+1))});
                else
                    line_to({*first, *(first+1)});
            }
            return *this;
        }

    inline context& close_move_to_line_to(const double* first, const double* last)
        {
            for ( ; first != last; first += 2) {
                if (*first < 0) {
                    close_path();
                    move_to({std::abs(*first), std::abs(*(first+1))});
                }
                else
                    line_to({*first, *(first+1)});
            }
            return *this;
        }

 private:
    SurfaceCairo& mSurface;
    double mScale;

    inline cairo_t* cairo_context() { return mSurface.cairo_context(); }

    inline double convert(double aValue) { return aValue; }
    inline double convert(Scaled aValue) { return aValue.value(); }
    inline double convert(Pixels aValue) { return aValue.value() / mScale; }

    inline cairo_line_cap_t cairo_line_cap(Surface::LineCap aLineCap) const
        {
            switch (aLineCap) {
              case Surface::LineCap::Butt:
                  return CAIRO_LINE_CAP_BUTT;
              case Surface::LineCap::Round:
                  return CAIRO_LINE_CAP_ROUND;
              case Surface::LineCap::Square:
                  return CAIRO_LINE_CAP_SQUARE;
            }
            return CAIRO_LINE_CAP_BUTT; // gcc wants return
        }

    inline cairo_line_join_t cairo_line_join(Surface::LineJoin aLineJoin) const
        {
            switch (aLineJoin) {
              case Surface::LineJoin::Miter:
                  return CAIRO_LINE_JOIN_MITER;
              case Surface::LineJoin::Round:
                  return CAIRO_LINE_JOIN_ROUND;
              case Surface::LineJoin::Bevel:
                  return CAIRO_LINE_JOIN_ROUND;
            }
            return CAIRO_LINE_JOIN_MITER; // gcc wants return
        }

    inline cairo_font_slant_t  cairo_font_slant(TextStyle::Slant aSlant) const
        {
            switch (aSlant) {
              case TextStyle::Slant::Normal:
                  return CAIRO_FONT_SLANT_NORMAL;
              case TextStyle::Slant::Italic:
                  return CAIRO_FONT_SLANT_ITALIC;
                    // case TextStyle::Slant::Oblique:
                    //     return CAIRO_FONT_SLANT_OBLIQUE;
            }
            return CAIRO_FONT_SLANT_NORMAL; // gcc wants return
        }

    inline cairo_font_weight_t  cairo_font_weight(TextStyle::Weight aWeight) const
        {
            switch (aWeight) {
              case TextStyle::Weight::Normal:
                  return CAIRO_FONT_WEIGHT_NORMAL;
              case TextStyle::Weight::Bold:
                  return CAIRO_FONT_WEIGHT_BOLD;
            }
            return CAIRO_FONT_WEIGHT_NORMAL; // gcc wants return
        }
};

// ----------------------------------------------------------------------

Surface* SurfaceCairo::make_child(const Location& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport, bool aClip)
{
    return new SurfaceCairoChild(*this, aOriginInParent, aWidthInParent, aViewport, aClip);

} // SurfaceCairo::make_child

// ----------------------------------------------------------------------

// Surface& SurfaceCairo::subsurface(const Location& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport, bool aClip)
// {
//     SurfaceCairoChild* child = new SurfaceCairoChild(*this, aOriginInParent, aWidthInParent, aViewport, aClip);
//     mChildren.emplace_back(child);
//     return *child;

// } // SurfaceCairo::subsurface

// // ----------------------------------------------------------------------

// Surface& SurfaceCairo::subsurface(bool aClip)
// {
//     return subsurface(Location{}, Scaled{}, Viewport{}, aClip);

// } // SurfaceCairo::subsurface

// // ----------------------------------------------------------------------

// Surface& SurfaceCairo::subsurface(const Location& aOriginInParent, Pixels aWidthInParent, const Viewport& aViewport, bool aClip)
// {
//     return subsurface(aOriginInParent / scale(), Scaled{aWidthInParent.value() / scale()}, aViewport, aClip);

// } // SurfaceCairo::subsurface

// ----------------------------------------------------------------------

template <typename S> static inline void s_line(SurfaceCairo& aSurface, const Location& a, const Location& b, Color aColor, S aWidth, PdfCairo::LineCap aLineCap)
{
    context(aSurface)
            .set_line_width(aWidth)
            .set_source_rgba(aColor)
            .set_line_cap(aLineCap)
            .move_to(a)
            .line_to(b)
            .stroke();
}

void SurfaceCairo::line(const Location& a, const Location& b, Color aColor, Pixels aWidth, LineCap aLineCap)
{
    s_line(*this, a, b, aColor, aWidth, aLineCap);

} // SurfaceCairo::line

void SurfaceCairo::line(const Location& a, const Location& b, Color aColor, Scaled aWidth, LineCap aLineCap)
{
    s_line(*this, a, b, aColor, aWidth, aLineCap);

} // SurfaceCairo::line

// ----------------------------------------------------------------------

void SurfaceCairo::rectangle(const Location& a, const Size& s, Color aColor, Pixels aWidth, LineCap aLineCap)
{
    context(*this)
            .set_line_width(aWidth)
            .set_line_cap(aLineCap)
            .rectangle(a, s)
            .set_source_rgba(aColor)
            .stroke();

} // SurfaceCairo::rectangle

// ----------------------------------------------------------------------

void SurfaceCairo::rectangle_filled(const Location& a, const Size& s, Color aOutlineColor, Pixels aWidth, Color aFillColor, LineCap aLineCap)
{
    context(*this)
            .set_line_width(aWidth)
            .set_line_cap(aLineCap)
            .rectangle(a, s)
            .set_source_rgba(aFillColor)
            .fill_preserve()
            .set_source_rgba(aOutlineColor)
            .stroke();

} // SurfaceCairo::rectangle_filled

// ----------------------------------------------------------------------

template <typename S> static inline void s_circle(SurfaceCairo& aSurface, const Location& aCenter, S aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth)
{
    context(aSurface)
            .set_line_width(aOutlineWidth)
            .translate(aCenter)
            .rotate(aAngle)
            .aspect(aAspect)
            .circle(aDiameter / 2)
            .set_source_rgba(aOutlineColor)
            .stroke();
}

void SurfaceCairo::circle(const Location& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth)
{
    s_circle(*this, aCenter, aDiameter, aAspect, aAngle, aOutlineColor, aOutlineWidth);

} // SurfaceCairo::circle

void SurfaceCairo::circle(const Location& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth)
{
    s_circle(*this, aCenter, aDiameter, aAspect, aAngle, aOutlineColor, aOutlineWidth);

} // SurfaceCairo::circle

// ----------------------------------------------------------------------

template <typename S> static inline void s_circle_filled(SurfaceCairo& aSurface, const Location& aCenter, S aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor)
{
    context(aSurface)
            .set_line_width(aOutlineWidth)
            .translate(aCenter)
            .rotate(aAngle)
            .aspect(aAspect)
            .circle(aDiameter / 2)
            .set_source_rgba(aFillColor)
            .fill_preserve()
            .set_source_rgba(aOutlineColor)
            .stroke();
}

void SurfaceCairo::circle_filled(const Location& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor)
{
    s_circle_filled(*this, aCenter, aDiameter, aAspect, aAngle, aOutlineColor, aOutlineWidth, aFillColor);

} // SurfaceCairo::circle_filled

void SurfaceCairo::circle_filled(const Location& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor)
{
    s_circle_filled(*this, aCenter, aDiameter, aAspect, aAngle, aOutlineColor, aOutlineWidth, aFillColor);

} // SurfaceCairo::circle_filled

// ----------------------------------------------------------------------

void SurfaceCairo::sector_filled(const Location& aCenter, Scaled aDiameter, Rotation aStart, Rotation aEnd, Color aOutlineColor, Pixels aOutlineWidth, Color aRadiusColor, Pixels aRadiusWidth, Dash aRadiusDash, Color aFillColor)
{
    context ctx(*this);
    ctx.translate(aCenter);

      // arc
    ctx.arc(aDiameter / 2, aStart, aEnd)
            .set_line_width(aOutlineWidth)
            .set_source_rgba(aOutlineColor);
    CairoPath arc_path(ctx.copy_path());
    ctx.stroke();

      // radius lines
    ctx.rotate(aEnd)
            .move_to(aDiameter / 2, Scaled(0))
            .line_to(Scaled(0), Scaled(0))
            .rotate(aStart - aEnd)
            .line_to(aDiameter / 2, Scaled(0))
            .set_line_width(aRadiusWidth)
            .set_source_rgba(aRadiusColor)
            .set_line_dash(aRadiusDash)
              // .set_line_join(Surface::LineJoin::Miter)
            .stroke_preserve()
            .rotate(-aStart);

      // fill area
    ctx.append_path(arc_path)
            .set_source_rgba(aFillColor)
            .fill();

} // SurfaceCairo::sector_filled

// ----------------------------------------------------------------------

template <typename S> static inline void s_square_filled(SurfaceCairo& aSurface, const Location& aCenter, S aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, PdfCairo::LineCap aLineCap)
{
    context(aSurface)
            .set_line_width(aOutlineWidth)
            .set_line_cap(aLineCap)
            .translate(aCenter)
            .rotate(aAngle)
            .rectangle(- aSide / 2 * aAspect.value(), - aSide / 2, aSide / 2 * aAspect.value(), aSide / 2)
            .set_source_rgba(aFillColor)
            .fill_preserve()
            .set_source_rgba(aOutlineColor)
            .stroke();
}

void SurfaceCairo::square_filled(const Location& aCenter, Pixels aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap)
{
    s_square_filled(*this, aCenter, aSide, aAspect, aAngle, aOutlineColor, aOutlineWidth, aFillColor, aLineCap);

} // SurfaceCairo::square_filled

void SurfaceCairo::square_filled(const Location& aCenter, Scaled aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap)
{
    s_square_filled(*this, aCenter, aSide, aAspect, aAngle, aOutlineColor, aOutlineWidth, aFillColor, aLineCap);

} // SurfaceCairo::square_filled

// ----------------------------------------------------------------------

template <typename S> static inline void s_triangle_filled(SurfaceCairo& aSurface, const Location& aCenter, S aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, PdfCairo::LineCap aLineCap)
{
    const auto cos_pi_6 = std::cos(M_PI / 6.0);
    const auto radius = aSide * cos_pi_6;
    context(aSurface)
            .set_line_width(aOutlineWidth)
            .set_line_cap(aLineCap)
            .translate(aCenter)
            .rotate(aAngle)
            .move_to(S{0}, - radius)
            .line_to(- radius * cos_pi_6 * aAspect.value(), radius * 0.5)
            .line_to(radius * cos_pi_6 * aAspect.value(), radius * 0.5)
            .close_path()
            .set_source_rgba(aFillColor)
            .fill_preserve()
            .set_source_rgba(aOutlineColor)
            .stroke();
}

void SurfaceCairo::triangle_filled(const Location& aCenter, Pixels aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap)
{
    s_triangle_filled(*this, aCenter, aSide, aAspect, aAngle, aOutlineColor, aOutlineWidth, aFillColor, aLineCap);

} // SurfaceCairo::triangle_filled

void SurfaceCairo::triangle_filled(const Location& aCenter, Scaled aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap)
{
    s_triangle_filled(*this, aCenter, aSide, aAspect, aAngle, aOutlineColor, aOutlineWidth, aFillColor, aLineCap);

} // SurfaceCairo::triangle_filled

void SurfaceCairo::triangle_filled(const Location& aCorner1, const Location& aCorner2, const Location& aCorner3, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap)
{
    context(*this)
            .set_line_width(aOutlineWidth)
            .set_line_cap(aLineCap)
            .move_to(aCorner1)
            .line_to(aCorner2)
            .line_to(aCorner3)
            .close_path()
            .set_source_rgba(aFillColor)
            .fill_preserve()
            .set_source_rgba(aOutlineColor)
            .stroke();

} // SurfaceCairo::triangle_filled

// ----------------------------------------------------------------------

void SurfaceCairo::path_outline(std::vector<Location>::const_iterator first, std::vector<Location>::const_iterator last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose, LineCap aLineCap)
{
    context(*this)
            .new_path()
            .set_line_cap(aLineCap)
            .set_line_join(LineJoin::Miter)
            .set_line_width(aOutlineWidth)
            .set_source_rgba(aOutlineColor)
            .move_to_line_to(first, last)
            .close_path_if(aClose)
            .stroke();

} // SurfaceCairo::path_outline

// ----------------------------------------------------------------------

void SurfaceCairo::path_outline(const double* first, const double* last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose, LineCap aLineCap)
{
    context(*this)
            .new_path()
            .set_line_cap(aLineCap)
            .set_line_join(LineJoin::Miter)
            .set_line_width(aOutlineWidth)
            .set_source_rgba(aOutlineColor)
            .move_to_line_to(first, last)
            .close_path_if(aClose)
            .stroke();

} // SurfaceCairo::path_outline

// ----------------------------------------------------------------------

void SurfaceCairo::path_fill(std::vector<Location>::const_iterator first, std::vector<Location>::const_iterator last, Color aFillColor)
{
    context(*this)
            .new_path()
            .set_source_rgba(aFillColor)
            .move_to_line_to(first, last)
            .close_path()
            .fill();

} // SurfaceCairo::path_fill

// ----------------------------------------------------------------------

void SurfaceCairo::path_fill(const double* first, const double* last, Color aFillColor)
{
    context(*this)
            .new_path()
            .set_source_rgba(aFillColor)
            .close_move_to_line_to(first, last)
            .close_path()
            .fill();

} // SurfaceCairo::path_fill

// ----------------------------------------------------------------------

template <typename S> static inline void s_text(SurfaceCairo& aSurface, const Location& a, std::string aText, Color aColor, S aSize, const TextStyle& aTextStyle, Rotation aRotation)
{
    context(aSurface)
            .prepare_for_text(aSize, aTextStyle)
            .move_to(a)
            .rotate(aRotation)
            .set_source_rgba(aColor)
            .show_text(aText)
            .new_path();        // clear text path (bug in cairo?)
}

template <typename S> static inline Size s_text_size(SurfaceCairo& aSurface, std::string aText, S aSize, const TextStyle& aTextStyle, double* x_bearing)
{
    cairo_text_extents_t text_extents;
    context(aSurface)
            .prepare_for_text(aSize, aTextStyle)
            .text_extents(aText, text_extents);
    if (x_bearing != nullptr)
        *x_bearing = text_extents.x_bearing;
    return {text_extents.x_advance, - text_extents.y_bearing};
}

// ----------------------------------------------------------------------

void SurfaceCairo::text(const Location& a, std::string aText, Color aColor, Pixels aSize, const TextStyle& aTextStyle, Rotation aRotation)
{
    s_text(*this, a, aText, aColor, aSize, aTextStyle, aRotation);

} // SurfaceCairo::text

void SurfaceCairo::text(const Location& a, std::string aText, Color aColor, Scaled aSize, const TextStyle& aTextStyle, Rotation aRotation)
{
    s_text(*this, a, aText, aColor, aSize, aTextStyle, aRotation);

} // SurfaceCairo::text

void SurfaceCairo::text_right_aligned(const Location& aEnd, std::string aText, Color aColor, Pixels aSize, const TextStyle& aTextStyle, Rotation aRotation)
{
    s_text(*this, {aEnd.x - s_text_size(*this, aText, aSize, aTextStyle, nullptr).width, aEnd.y}, aText, aColor, aSize, aTextStyle, aRotation);

} // SurfaceCairo::text_right_aligned

void SurfaceCairo::text_right_aligned(const Location& aEnd, std::string aText, Color aColor, Scaled aSize, const TextStyle& aTextStyle, Rotation aRotation)
{
    s_text(*this, {aEnd.x - s_text_size(*this, aText, aSize, aTextStyle, nullptr).width, aEnd.y}, aText, aColor, aSize, aTextStyle, aRotation);

} // SurfaceCairo::text_right_aligned

// ----------------------------------------------------------------------

Size SurfaceCairo::text_size(std::string aText, Pixels aSize, const TextStyle& aTextStyle, double* x_bearing)
{
    return s_text_size(*this, aText, aSize, aTextStyle, x_bearing);

} // SurfaceCairo::text_size

Size SurfaceCairo::text_size(std::string aText, Scaled aSize, const TextStyle& aTextStyle, double* x_bearing)
{
    return s_text_size(*this, aText, aSize, aTextStyle, x_bearing);

} // SurfaceCairo::text_size

// ----------------------------------------------------------------------

PdfCairo::PdfCairo(std::string aFilename, double aWidth, double aHeight, double aViewportWidth)
{
    auto surface = cairo_pdf_surface_create(aFilename.empty() ? nullptr : aFilename.c_str(), aWidth, aHeight);
    mCairoContext = cairo_create(surface);
    cairo_surface_destroy(surface);
    change_width_in_parent(aWidth);
    viewport({Location{0, 0}, Size{aViewportWidth, aHeight * aViewportWidth / aWidth}});
      // std::cerr << *this << std::endl;

} // PdfCairo::PdfCairo

// ----------------------------------------------------------------------

PdfCairo::~PdfCairo()
{
    if (mCairoContext)
        cairo_destroy(mCairoContext);

} // PdfCairo::~PdfCairo

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
