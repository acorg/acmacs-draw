#include "acmacs-base/float.hh"
#include "acmacs-draw/surface-cairo.hh"

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
    using Size = acmacs::Size;
    using Viewport = acmacs::Viewport;
    using TextStyle = acmacs::TextStyle;

    context(acmacs::surface::internal_1::Cairo& aSurface)
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

    template <typename S> context& set_line_width(S aWidth) { cairo_set_line_width(cairo_context(), convert(aWidth)); return *this; }
    context& set_source_rgba(Color aColor) { cairo_set_source_rgba(cairo_context(), aColor.red(), aColor.green(), aColor.blue(), aColor.alpha()); return *this; }

    context& set_line_cap(acmacs::surface::LineCap aLineCap) { cairo_set_line_cap(cairo_context(), cairo_line_cap(aLineCap)); return *this; }
    context& set_line_join(acmacs::surface::LineJoin aLineJoin) { cairo_set_line_join(cairo_context(), cairo_line_join(aLineJoin)); return *this; }
    context& set_line_dash(acmacs::surface::Dash aLineDash)
        {
            double dash_size;
            switch (aLineDash) {
              case acmacs::surface::Dash::NoDash:
                  break;
              case acmacs::surface::Dash::Dash1:
                  dash_size = convert(Pixels{1});
                  cairo_set_dash(cairo_context(), &dash_size, 1, 0);
                  break;
              case acmacs::surface::Dash::Dash2:
                  dash_size = convert(Pixels{5});
                  cairo_set_dash(cairo_context(), &dash_size, 1, 0);
                  break;
              case acmacs::surface::Dash::Dash3:
              {
                  const double dashes[] = {convert(Pixels{1}), convert(Pixels{4})};
                  cairo_set_dash(cairo_context(), dashes, 2, 0);
              }
              break;
            }
            return *this;
        }

    context& move_to() { cairo_move_to(cairo_context(), 0.0, 0.0); return *this; }
    context& move_to(const acmacs::PointCoordinates& a) { cairo_move_to(cairo_context(), a.x(), a.y()); return *this; }
    template <typename S> context& move_to(S x, S y) { cairo_move_to(cairo_context(), convert(x), convert(y)); return *this; }
    context& line_to(const acmacs::PointCoordinates& a) { cairo_line_to(cairo_context(), a.x(), a.y()); return *this; }
    template <typename S> context& line_to(S x, S y) { cairo_line_to(cairo_context(), convert(x), convert(y)); return *this; }
    context& lines_to(std::vector<acmacs::PointCoordinates>::const_iterator first, std::vector<acmacs::PointCoordinates>::const_iterator last) { for ( ; first != last; ++first) { line_to(*first); } return *this; }
    context& rectangle(const acmacs::PointCoordinates& a, const Size& s) { cairo_rectangle(cairo_context(), a.x(), a.y(), s.width, s.height); return *this; }
    template <typename S> context& rectangle(S x1, S y1, S x2, S y2) { cairo_rectangle(cairo_context(), convert(x1), convert(y1), convert(x2) - convert(x1), convert(y2) - convert(y1)); return *this; }
      // context& arc(const acmacs::PointCoordinates& a, double radius, double angle1, double angle2) { cairo_arc(cairo_context(), a.x(), a.y(), radius, angle1, angle2); return *this; }
    template <typename S> context& circle(S radius) { cairo_arc(cairo_context(), 0.0, 0.0, convert(radius), 0.0, 2.0 * M_PI); return *this; }
    template <typename S> context& arc(S radius, Rotation start, Rotation end) { cairo_arc(cairo_context(), 0.0, 0.0, convert(radius), start.value(), end.value()); return *this; }
    context& circle(const acmacs::PointCoordinates& a, double radius) { cairo_arc(cairo_context(), a.x(), a.y(), radius, 0.0, 2.0 * M_PI); return *this; }
    context& stroke() { cairo_stroke(cairo_context()); return *this; }
    context& stroke_preserve() { cairo_stroke_preserve(cairo_context()); return *this; }
    context& fill() { cairo_fill(cairo_context()); return *this; }
    context& fill_preserve() { cairo_fill_preserve(cairo_context()); return *this; }
    context& translate(const Size& a) { cairo_translate(cairo_context(), a.width, a.height); return *this; }
    context& translate(const acmacs::PointCoordinates& a) { cairo_translate(cairo_context(), a.x(), a.y()); return *this; }
    context& rotate(Rotation aAngle) { cairo_rotate(cairo_context(), aAngle.value()); return *this; }
    context& scale(double x, double y) { cairo_scale(cairo_context(), x, y); return *this; }
    context& scale(double x) { cairo_scale(cairo_context(), x, x); return *this; }
    context& aspect(Aspect x) { cairo_scale(cairo_context(), x.value(), 1.0); return *this; }
    context& clip() { cairo_clip(cairo_context()); return *this; }
    context& new_path() { cairo_new_path(cairo_context()); return *this; }
    context& close_path() { cairo_close_path(cairo_context()); return *this; }
    context& close_path_if(bool aClose) { if (aClose) cairo_close_path(cairo_context()); return *this; }
    context& append_path(CairoPath& aPath) { cairo_append_path(cairo_context(), aPath); return *this; }
    CairoPath copy_path() { return std::move(cairo_copy_path(cairo_context())); }

    // // http://www.eurion.net/python-snippets/snippet/Cairo%20Easter%20Eggs.html
    // template <typename S> context& egg(S s_half_height)
    // {
    //     constexpr auto whratio = 1.0; // 2.0 / 3.0;
    //     const double half_height = convert(s_half_height), half_width = half_height * whratio;
    //     cairo_move_to(cairo_context(), 0, -half_height);
    //     cairo_curve_to(cairo_context(),
    //         half_width / 2, -half_height,
    //         half_width, - half_height / 3,
    //         -half_width + 2.0 * half_width, -half_height + 4 * half_height / 3
    //                    );
    //     cairo_curve_to(cairo_context(),
    //         -half_width + 2.0 * half_width, -half_height + 5 * half_height / 3,
    //         -half_width + 3 * half_width / 2, half_height,
    //         0.0, half_height
    //                    );
    //     cairo_curve_to(cairo_context(),
    //         -half_width / 2, half_height,
    //         -half_width, -half_height + 5 * half_height / 3,
    //         -half_width, -half_height + 4 * half_height / 3
    //                    );
    //     cairo_curve_to(cairo_context(),
    //         -half_width, -half_height + 2 * half_height / 3,
    //         -half_width / 2, -half_height,
    //         0.0, -half_height
    //                    );

    //     return *this;
    // }

    // https://books.google.de/books?id=StdwgT34RCwC&pg=PA107
    template <typename S> context& egg(S s_half_height)
    {
        const auto half_height = convert(s_half_height);
        cairo_move_to(cairo_context(), 0, half_height);
        cairo_curve_to(cairo_context(),
                       half_height * 1.4, half_height * 0.95,
                       half_height * 0.8, - half_height * 0.98,
                       0, - half_height);
        cairo_curve_to(cairo_context(),
                       - half_height * 0.8, - half_height * 0.98,
                       - half_height * 1.4, half_height * 0.95,
                       0, half_height);
        cairo_close_path(cairo_context());
        return *this;
    }

    template <typename S> context& ugly_egg(S s_half_height)
    {
        const auto half_height = convert(s_half_height);
        // const auto c1x = half_height * 1.4, c1y = half_height * 0.95, c2x = half_height * 0.8, c2y = - half_height * 0.98;
        const auto c1x = half_height * 1.0, c1y = half_height * 0.6, c2x = half_height * 0.8, c2y = - half_height * 0.6;
        cairo_move_to(cairo_context(), 0, half_height);
        cairo_line_to(cairo_context(), c1x, c1y);
        cairo_line_to(cairo_context(), c2x, c2y);
        cairo_line_to(cairo_context(), 0, - half_height);
        cairo_line_to(cairo_context(), - c2x, c2y);
        cairo_line_to(cairo_context(), - c1x, c1y);
        cairo_close_path(cairo_context());
        return *this;
    }

    template <typename S> context& prepare_for_text(S aSize, const TextStyle& aTextStyle) { cairo_select_font_face(cairo_context(), (aTextStyle.font_family).c_str(), cairo_font_slant(aTextStyle.slant), cairo_font_weight(aTextStyle.weight)); cairo_set_font_size(cairo_context(), convert(aSize)); return *this; }
    context& show_text(std::string_view aText) { cairo_show_text(cairo_context(), aText.data()); return *this; }
    context& text_extents(std::string_view aText, cairo_text_extents_t& extents) { cairo_text_extents(cairo_context(), aText.data(), &extents); return *this; }

      // if acmacs::PointCoordinates::x is negative - move_to, else - path_to. It assumes origin is {0,0}!!!
    context& move_to_negative_line_to_positive(std::vector<acmacs::PointCoordinates>::const_iterator first, std::vector<acmacs::PointCoordinates>::const_iterator last)
        {
            for ( ; first != last; ++first) {
                if (first->x() < 0)
                    move_to({std::abs(first->x()), std::abs(first->y())});
                else
                    line_to(*first);
            }
            return *this;
        }

      // the same as above but with raw data
    context& move_to_negative_line_to_positive(const double* first, const double* last)
        {
            for ( ; first != last; first += 2) {
                if (*first < 0)
                    move_to({std::abs(*first), std::abs(*(first+1))});
                else
                    line_to({*first, *(first+1)});
            }
            return *this;
        }

    context& move_to_first_line_to_rest(std::vector<acmacs::PointCoordinates>::const_iterator first, std::vector<acmacs::PointCoordinates>::const_iterator last)
        {
            move_to(*first);
            for (++first; first != last; ++first)
                line_to(*first);
            return *this;
        }

    context& move_to_first_line_to_rest(const double* first, const double* last)
        {
            move_to({*first, *(first+1)});
            for (first += 2; first != last; first += 2)
                line_to({*first, *(first+1)});
            return *this;
        }

    context& close_move_to_line_to(const double* first, const double* last)
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
    acmacs::surface::internal_1::Cairo& mSurface;
    const double mScale;

    cairo_t* cairo_context() { return mSurface.cairo_context(); }

    double convert(double aValue) { return aValue; }
    double convert(Scaled aValue) { return aValue.value(); }
    double convert(Pixels aValue) { return aValue.value() / mScale; }

    cairo_line_cap_t cairo_line_cap(acmacs::surface::LineCap aLineCap) const
        {
            switch (aLineCap) {
              case acmacs::surface::LineCap::Butt:
                  return CAIRO_LINE_CAP_BUTT;
              case acmacs::surface::LineCap::Round:
                  return CAIRO_LINE_CAP_ROUND;
              case acmacs::surface::LineCap::Square:
                  return CAIRO_LINE_CAP_SQUARE;
            }
            return CAIRO_LINE_CAP_BUTT; // gcc wants return
        }

    cairo_line_join_t cairo_line_join(acmacs::surface::LineJoin aLineJoin) const
        {
            switch (aLineJoin) {
              case acmacs::surface::LineJoin::Miter:
                  return CAIRO_LINE_JOIN_MITER;
              case acmacs::surface::LineJoin::Round:
                  return CAIRO_LINE_JOIN_ROUND;
              case acmacs::surface::LineJoin::Bevel:
                  return CAIRO_LINE_JOIN_BEVEL;
            }
            return CAIRO_LINE_JOIN_MITER; // gcc wants return
        }

    cairo_font_slant_t cairo_font_slant(acmacs::FontSlant::Value aSlant) const
        {
            switch (aSlant) {
              case acmacs::FontSlant::Normal:
                  return CAIRO_FONT_SLANT_NORMAL;
              case acmacs::FontSlant::Italic:
                  return CAIRO_FONT_SLANT_ITALIC;
                    // case TextStyle::Slant::Oblique:
                    //     return CAIRO_FONT_SLANT_OBLIQUE;
            }
            return CAIRO_FONT_SLANT_NORMAL; // gcc wants return
        }

    cairo_font_weight_t  cairo_font_weight(acmacs::FontWeight::Value aWeight) const
        {
            switch (aWeight) {
              case acmacs::FontWeight::Normal:
                  return CAIRO_FONT_WEIGHT_NORMAL;
              case acmacs::FontWeight::Bold:
                  return CAIRO_FONT_WEIGHT_BOLD;
            }
            return CAIRO_FONT_WEIGHT_NORMAL; // gcc wants return
        }
};

// ----------------------------------------------------------------------

acmacs::surface::Surface* acmacs::surface::internal_1::Cairo::make_child(const acmacs::PointCoordinates& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport, bool aClip)
{
    // std::cerr << "DEBUG: Cairo::make_child " << aOriginInParent << ' ' << aWidthInParent << ' ' << aViewport << '\n';
    return new acmacs::surface::internal_1::CairoChild(*this, aOriginInParent, aWidthInParent, aViewport, aClip);

} // acmacs::surface::internal_1::Cairo::make_child

// ----------------------------------------------------------------------

// acmacs::surface::Surface& acmacs::surface::internal_1::Cairo::subsurface(const acmacs::PointCoordinates& aOriginInParent, Scaled aWidthInParent, const Viewport& aViewport, bool aClip)
// {
//     acmacs::surface::internal_1::CairoChild* child = new acmacs::surface::internal_1::CairoChild(*this, aOriginInParent, aWidthInParent, aViewport, aClip);
//     mChildren.emplace_back(child);
//     return *child;

// } // acmacs::surface::internal_1::Cairo::subsurface

// // ----------------------------------------------------------------------

// acmacs::surface::Surface& acmacs::surface::internal_1::Cairo::subsurface(bool aClip)
// {
//     return subsurface(acmacs::PointCoordinates{}, Scaled{}, Viewport{}, aClip);

// } // acmacs::surface::internal_1::Cairo::subsurface

// // ----------------------------------------------------------------------

// acmacs::surface::Surface& acmacs::surface::internal_1::Cairo::subsurface(const acmacs::PointCoordinates& aOriginInParent, Pixels aWidthInParent, const Viewport& aViewport, bool aClip)
// {
//     return subsurface(aOriginInParent / scale(), Scaled{aWidthInParent.value() / scale()}, aViewport, aClip);

// } // acmacs::surface::internal_1::Cairo::subsurface

// ----------------------------------------------------------------------

template <typename S> static inline void s_line(acmacs::surface::internal_1::Cairo& aSurface, const acmacs::PointCoordinates& a, const acmacs::PointCoordinates& b, Color aColor, S aWidth, acmacs::surface::LineCap aLineCap)
{
    context(aSurface)
            .set_line_width(aWidth)
            .set_source_rgba(aColor)
            .set_line_cap(aLineCap)
            .move_to(a)
            .line_to(b)
            .stroke();
}

void acmacs::surface::internal_1::Cairo::line(const acmacs::PointCoordinates& a, const acmacs::PointCoordinates& b, Color aColor, Pixels aWidth, LineCap aLineCap)
{
    s_line(*this, a, b, aColor, aWidth, aLineCap);

} // acmacs::surface::internal_1::Cairo::line

void acmacs::surface::internal_1::Cairo::line(const acmacs::PointCoordinates& a, const acmacs::PointCoordinates& b, Color aColor, Scaled aWidth, LineCap aLineCap)
{
    s_line(*this, a, b, aColor, aWidth, aLineCap);

} // acmacs::surface::internal_1::Cairo::line


void acmacs::surface::internal_1::Cairo::line(const acmacs::PointCoordinates& a, const acmacs::PointCoordinates& b, Color aColor, Pixels aWidth, Dash aDash, LineCap aLineCap)
{
    context(*this)
            .set_line_width(aWidth)
            .set_source_rgba(aColor)
            .set_line_cap(aLineCap)
            .set_line_dash(aDash)
            .move_to(a)
            .line_to(b)
            .stroke();

} // acmacs::surface::internal_1::Cairo::line


// ----------------------------------------------------------------------

void acmacs::surface::internal_1::Cairo::rectangle(const acmacs::PointCoordinates& a, const Size& s, Color aColor, Pixels aWidth, LineCap aLineCap)
{
    context(*this)
            .set_line_width(aWidth)
            .set_line_cap(aLineCap)
            .rectangle(a, s)
            .set_source_rgba(aColor)
            .stroke();

} // acmacs::surface::internal_1::Cairo::rectangle

// ----------------------------------------------------------------------

void acmacs::surface::internal_1::Cairo::rectangle_filled(const acmacs::PointCoordinates& a, const Size& s, Color aOutlineColor, Pixels aWidth, Color aFillColor, LineCap aLineCap)
{
    context(*this)
            .set_line_width(aWidth)
            .set_line_cap(aLineCap)
            .rectangle(a, s)
            .set_source_rgba(aFillColor)
            .fill_preserve()
            .set_source_rgba(aOutlineColor)
            .stroke();

} // acmacs::surface::internal_1::Cairo::rectangle_filled

// ----------------------------------------------------------------------

template <typename S> static inline void s_circle(acmacs::surface::internal_1::Cairo& aSurface, const acmacs::PointCoordinates& aCenter, S aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth)
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

void acmacs::surface::internal_1::Cairo::circle(const acmacs::PointCoordinates& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth)
{
    s_circle(*this, aCenter, aDiameter, aAspect, aAngle, aOutlineColor, aOutlineWidth);

} // acmacs::surface::internal_1::Cairo::circle

void acmacs::surface::internal_1::Cairo::circle(const acmacs::PointCoordinates& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth)
{
    s_circle(*this, aCenter, aDiameter, aAspect, aAngle, aOutlineColor, aOutlineWidth);

} // acmacs::surface::internal_1::Cairo::circle

// ----------------------------------------------------------------------

template <typename S> static inline void s_circle_filled(acmacs::surface::internal_1::Cairo& aSurface, const acmacs::PointCoordinates& aCenter, S aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, acmacs::surface::Dash aOutlineDash, Color aFillColor)
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
            .set_line_dash(aOutlineDash)
            .stroke();
}

void acmacs::surface::internal_1::Cairo::circle_filled(const acmacs::PointCoordinates& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Dash aOutlineDash, Color aFillColor)
{
    s_circle_filled(*this, aCenter, aDiameter, aAspect, aAngle, aOutlineColor, aOutlineWidth, aOutlineDash, aFillColor);

} // acmacs::surface::internal_1::Cairo::circle_filled

void acmacs::surface::internal_1::Cairo::circle_filled(const acmacs::PointCoordinates& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Dash aOutlineDash, Color aFillColor)
{
    s_circle_filled(*this, aCenter, aDiameter, aAspect, aAngle, aOutlineColor, aOutlineWidth, aOutlineDash, aFillColor);

} // acmacs::surface::internal_1::Cairo::circle_filled

// ----------------------------------------------------------------------

template <typename S> static inline void s_egg(acmacs::surface::internal_1::Cairo& aSurface, const acmacs::PointCoordinates& aCenter, S aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth)
{
    context(aSurface)
            .set_line_width(aOutlineWidth)
            .translate(aCenter)
            .rotate(aAngle)
            .aspect(aAspect)
            .egg(aDiameter / 2)
            .set_source_rgba(aOutlineColor)
            .stroke();
}

void acmacs::surface::internal_1::Cairo::egg(const acmacs::PointCoordinates& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth)
{
    s_egg(*this, aCenter, aDiameter, aAspect, aAngle, aOutlineColor, aOutlineWidth);

} // acmacs::surface::internal_1::Cairo::egg

void acmacs::surface::internal_1::Cairo::egg(const acmacs::PointCoordinates& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth)
{
    s_egg(*this, aCenter, aDiameter, aAspect, aAngle, aOutlineColor, aOutlineWidth);

} // acmacs::surface::internal_1::Cairo::egg

// ----------------------------------------------------------------------

template <typename S> static inline void s_egg_filled(acmacs::surface::internal_1::Cairo& aSurface, const acmacs::PointCoordinates& aCenter, S aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, acmacs::surface::Dash aOutlineDash, Color aFillColor)
{
    context(aSurface)
            .set_line_width(aOutlineWidth)
            .translate(aCenter)
            .rotate(aAngle)
            .aspect(aAspect)
            .egg(aDiameter / 2)
            .set_source_rgba(aFillColor)
            .fill_preserve()
            .set_source_rgba(aOutlineColor)
            .set_line_dash(aOutlineDash)
            .stroke();
}

void acmacs::surface::internal_1::Cairo::egg_filled(const acmacs::PointCoordinates& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Dash aOutlineDash, Color aFillColor)
{
    s_egg_filled(*this, aCenter, aDiameter, aAspect, aAngle, aOutlineColor, aOutlineWidth, aOutlineDash, aFillColor);

} // acmacs::surface::internal_1::Cairo::egg_filled

void acmacs::surface::internal_1::Cairo::egg_filled(const acmacs::PointCoordinates& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Dash aOutlineDash, Color aFillColor)
{
    s_egg_filled(*this, aCenter, aDiameter, aAspect, aAngle, aOutlineColor, aOutlineWidth, aOutlineDash, aFillColor);

} // acmacs::surface::internal_1::Cairo::egg_filled

// ----------------------------------------------------------------------

template <typename S> static inline void s_ugly_egg(acmacs::surface::internal_1::Cairo& aSurface, const acmacs::PointCoordinates& aCenter, S aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth)
{
    context(aSurface)
            .set_line_width(aOutlineWidth)
            .translate(aCenter)
            .rotate(aAngle)
            .aspect(aAspect)
            .ugly_egg(aDiameter / 2)
            .set_source_rgba(aOutlineColor)
            .stroke();
}

void acmacs::surface::internal_1::Cairo::ugly_egg(const acmacs::PointCoordinates& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth)
{
    s_ugly_egg(*this, aCenter, aDiameter, aAspect, aAngle, aOutlineColor, aOutlineWidth);

} // acmacs::surface::internal_1::Cairo::ugly_egg

void acmacs::surface::internal_1::Cairo::ugly_egg(const acmacs::PointCoordinates& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth)
{
    s_ugly_egg(*this, aCenter, aDiameter, aAspect, aAngle, aOutlineColor, aOutlineWidth);

} // acmacs::surface::internal_1::Cairo::ugly_egg

// ----------------------------------------------------------------------

template <typename S> static inline void s_ugly_egg_filled(acmacs::surface::internal_1::Cairo& aSurface, const acmacs::PointCoordinates& aCenter, S aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, acmacs::surface::Dash aOutlineDash, Color aFillColor)
{
    context(aSurface)
            .set_line_width(aOutlineWidth)
            .translate(aCenter)
            .rotate(aAngle)
            .aspect(aAspect)
            .ugly_egg(aDiameter / 2)
            .set_source_rgba(aFillColor)
            .fill_preserve()
            .set_source_rgba(aOutlineColor)
            .set_line_dash(aOutlineDash)
            .stroke();
}

void acmacs::surface::internal_1::Cairo::ugly_egg_filled(const acmacs::PointCoordinates& aCenter, Pixels aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Dash aOutlineDash, Color aFillColor)
{
    s_ugly_egg_filled(*this, aCenter, aDiameter, aAspect, aAngle, aOutlineColor, aOutlineWidth, aOutlineDash, aFillColor);

} // acmacs::surface::internal_1::Cairo::ugly_egg_filled

void acmacs::surface::internal_1::Cairo::ugly_egg_filled(const acmacs::PointCoordinates& aCenter, Scaled aDiameter, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Dash aOutlineDash, Color aFillColor)
{
    s_ugly_egg_filled(*this, aCenter, aDiameter, aAspect, aAngle, aOutlineColor, aOutlineWidth, aOutlineDash, aFillColor);

} // acmacs::surface::internal_1::Cairo::ugly_egg_filled

// ----------------------------------------------------------------------

void acmacs::surface::internal_1::Cairo::sector_filled(const acmacs::PointCoordinates& aCenter, Scaled aDiameter, Rotation aStart, Rotation aEnd, Color aOutlineColor, Pixels aOutlineWidth, Color aRadiusColor, Pixels aRadiusWidth, Dash aRadiusDash, Color aFillColor)
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

} // acmacs::surface::internal_1::Cairo::sector_filled

// ----------------------------------------------------------------------

template <typename S> static inline void s_square_filled(acmacs::surface::internal_1::Cairo& aSurface, const acmacs::PointCoordinates& aCenter, S aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, acmacs::surface::LineCap aLineCap)
{
    context(aSurface)
            .set_line_width(aOutlineWidth)
            .set_line_cap(aLineCap)
            .translate(aCenter)
            .rotate(aAngle)
            .rectangle(- aSide / 2 * aAspect, - aSide / 2, aSide / 2 * aAspect, aSide / 2)
            .set_source_rgba(aFillColor)
            .fill_preserve()
            .set_source_rgba(aOutlineColor)
            .stroke();
}

void acmacs::surface::internal_1::Cairo::square_filled(const acmacs::PointCoordinates& aCenter, Pixels aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap)
{
    s_square_filled(*this, aCenter, aSide, aAspect, aAngle, aOutlineColor, aOutlineWidth, aFillColor, aLineCap);

} // acmacs::surface::internal_1::Cairo::square_filled

void acmacs::surface::internal_1::Cairo::square_filled(const acmacs::PointCoordinates& aCenter, Scaled aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap)
{
    s_square_filled(*this, aCenter, aSide, aAspect, aAngle, aOutlineColor, aOutlineWidth, aFillColor, aLineCap);

} // acmacs::surface::internal_1::Cairo::square_filled

// ----------------------------------------------------------------------

template <typename S> static inline void s_triangle_filled(acmacs::surface::internal_1::Cairo& aSurface, const acmacs::PointCoordinates& aCenter, S aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, acmacs::surface::LineCap aLineCap)
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

void acmacs::surface::internal_1::Cairo::triangle_filled(const acmacs::PointCoordinates& aCenter, Pixels aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap)
{
    s_triangle_filled(*this, aCenter, aSide, aAspect, aAngle, aOutlineColor, aOutlineWidth, aFillColor, aLineCap);

} // acmacs::surface::internal_1::Cairo::triangle_filled

void acmacs::surface::internal_1::Cairo::triangle_filled(const acmacs::PointCoordinates& aCenter, Scaled aSide, Aspect aAspect, Rotation aAngle, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap)
{
    s_triangle_filled(*this, aCenter, aSide, aAspect, aAngle, aOutlineColor, aOutlineWidth, aFillColor, aLineCap);

} // acmacs::surface::internal_1::Cairo::triangle_filled

void acmacs::surface::internal_1::Cairo::triangle_filled(const acmacs::PointCoordinates& aCorner1, const acmacs::PointCoordinates& aCorner2, const acmacs::PointCoordinates& aCorner3, Color aOutlineColor, Pixels aOutlineWidth, Color aFillColor, LineCap aLineCap)
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

} // acmacs::surface::internal_1::Cairo::triangle_filled

// ----------------------------------------------------------------------

void acmacs::surface::internal_1::Cairo::path_outline(std::vector<acmacs::PointCoordinates>::const_iterator first, std::vector<acmacs::PointCoordinates>::const_iterator last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose, LineCap aLineCap)
{
    context(*this)
            .new_path()
            .set_line_cap(aLineCap)
            .set_line_join(LineJoin::Miter)
            .set_line_width(aOutlineWidth)
            .set_source_rgba(aOutlineColor)
            .move_to_first_line_to_rest(first, last)
            .close_path_if(aClose)
            .stroke();

} // acmacs::surface::internal_1::Cairo::path_outline

// ----------------------------------------------------------------------

void acmacs::surface::internal_1::Cairo::path_outline(const double* first, const double* last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose, LineCap aLineCap)
{
    context(*this)
            .new_path()
            .set_line_cap(aLineCap)
            .set_line_join(LineJoin::Miter)
            .set_line_width(aOutlineWidth)
            .set_source_rgba(aOutlineColor)
            .move_to_first_line_to_rest(first, last)
            .close_path_if(aClose)
            .stroke();

} // acmacs::surface::internal_1::Cairo::path_outline

// ----------------------------------------------------------------------

void acmacs::surface::internal_1::Cairo::path_fill(std::vector<acmacs::PointCoordinates>::const_iterator first, std::vector<acmacs::PointCoordinates>::const_iterator last, Color aFillColor)
{
    context(*this)
            .new_path()
            .set_source_rgba(aFillColor)
            .move_to_first_line_to_rest(first, last)
            .close_path()
            .fill();

} // acmacs::surface::internal_1::Cairo::path_fill

// ----------------------------------------------------------------------

void acmacs::surface::internal_1::Cairo::path_fill(const double* first, const double* last, Color aFillColor)
{
    context(*this)
            .new_path()
            .set_source_rgba(aFillColor)
            .move_to_first_line_to_rest(first, last)
            .close_path()
            .fill();

} // acmacs::surface::internal_1::Cairo::path_fill

// ----------------------------------------------------------------------

void acmacs::surface::internal_1::Cairo::path_outline_negative_move(std::vector<acmacs::PointCoordinates>::const_iterator first, std::vector<acmacs::PointCoordinates>::const_iterator last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose, LineCap aLineCap)
{
    context(*this)
            .new_path()
            .set_line_cap(aLineCap)
            .set_line_join(LineJoin::Miter)
            .set_line_width(aOutlineWidth)
            .set_source_rgba(aOutlineColor)
            .move_to_negative_line_to_positive(first, last)
            .close_path_if(aClose)
            .stroke();

} // acmacs::surface::internal_1::Cairo::path_outline_negative_move

// ----------------------------------------------------------------------

void acmacs::surface::internal_1::Cairo::path_outline_negative_move(const double* first, const double* last, Color aOutlineColor, Pixels aOutlineWidth, bool aClose, LineCap aLineCap)
{
    context(*this)
            .new_path()
            .set_line_cap(aLineCap)
            .set_line_join(LineJoin::Miter)
            .set_line_width(aOutlineWidth)
            .set_source_rgba(aOutlineColor)
            .move_to_negative_line_to_positive(first, last)
            .close_path_if(aClose)
            .stroke();

} // acmacs::surface::internal_1::Cairo::path_outline_negative_move

// ----------------------------------------------------------------------

void acmacs::surface::internal_1::Cairo::path_fill_negative_move(std::vector<acmacs::PointCoordinates>::const_iterator first, std::vector<acmacs::PointCoordinates>::const_iterator last, Color aFillColor)
{
    context(*this)
            .new_path()
            .set_source_rgba(aFillColor)
            .move_to_negative_line_to_positive(first, last)
            .close_path()
            .fill();

} // acmacs::surface::internal_1::Cairo::path_fill_negative_move

// ----------------------------------------------------------------------

void acmacs::surface::internal_1::Cairo::path_fill_negative_move(const double* first, const double* last, Color aFillColor)
{
    context(*this)
            .new_path()
            .set_source_rgba(aFillColor)
            .close_move_to_line_to(first, last)
            .close_path()
            .fill();

} // acmacs::surface::internal_1::Cairo::path_fill_negative_move

// ----------------------------------------------------------------------

template <typename S> static inline void s_text(acmacs::surface::internal_1::Cairo& aSurface, const acmacs::PointCoordinates& a, std::string_view aText, Color aColor, S aSize, const acmacs::TextStyle& aTextStyle, Rotation aRotation)
{
    context(aSurface)
            .prepare_for_text(aSize, aTextStyle)
            .move_to(a)
            .rotate(aRotation)
            .set_source_rgba(aColor)
            .show_text(aText)
            .new_path();        // clear text path (bug in cairo?)
}

template <typename S> static inline acmacs::Size s_text_size(acmacs::surface::internal_1::Cairo& aSurface, std::string_view aText, S aSize, const acmacs::TextStyle& aTextStyle, double* x_bearing)
{
    cairo_text_extents_t text_extents;
    context(aSurface)
            .prepare_for_text(aSize, aTextStyle)
            .text_extents(aText, text_extents);
    if (x_bearing != nullptr)
        *x_bearing = text_extents.x_bearing;
      // std::cerr << "s_text_size [" << aText << "] " << text_extents.x_advance << ' ' << text_extents.y_bearing << ' ' << text_extents.x_bearing << '\n';
    return {text_extents.x_advance < 0 ? 0.0 : text_extents.x_advance, - text_extents.y_bearing};
}

// ----------------------------------------------------------------------

void acmacs::surface::internal_1::Cairo::text(const acmacs::PointCoordinates& a, std::string_view aText, Color aColor, Pixels aSize, const TextStyle& aTextStyle, Rotation aRotation)
{
    s_text(*this, a, aText, aColor, aSize, aTextStyle, aRotation);

} // acmacs::surface::internal_1::Cairo::text

void acmacs::surface::internal_1::Cairo::text(const acmacs::PointCoordinates& a, std::string_view aText, Color aColor, Scaled aSize, const TextStyle& aTextStyle, Rotation aRotation)
{
    s_text(*this, a, aText, aColor, aSize, aTextStyle, aRotation);

} // acmacs::surface::internal_1::Cairo::text

void acmacs::surface::internal_1::Cairo::text_right_aligned(const acmacs::PointCoordinates& aEnd, std::string_view aText, Color aColor, Pixels aSize, const TextStyle& aTextStyle, Rotation aRotation)
{
    s_text(*this, {aEnd.x() - s_text_size(*this, aText, aSize, aTextStyle, nullptr).width, aEnd.y()}, aText, aColor, aSize, aTextStyle, aRotation);

} // acmacs::surface::internal_1::Cairo::text_right_aligned

void acmacs::surface::internal_1::Cairo::text_right_aligned(const acmacs::PointCoordinates& aEnd, std::string_view aText, Color aColor, Scaled aSize, const TextStyle& aTextStyle, Rotation aRotation)
{
    s_text(*this, {aEnd.x() - s_text_size(*this, aText, aSize, aTextStyle, nullptr).width, aEnd.y()}, aText, aColor, aSize, aTextStyle, aRotation);

} // acmacs::surface::internal_1::Cairo::text_right_aligned

// ----------------------------------------------------------------------

acmacs::Size acmacs::surface::internal_1::Cairo::text_size(std::string_view aText, Pixels aSize, const TextStyle& aTextStyle, double* x_bearing)
{
    return s_text_size(*this, aText, aSize, aTextStyle, x_bearing);

} // acmacs::surface::internal_1::Cairo::text_size

acmacs::Size acmacs::surface::internal_1::Cairo::text_size(std::string_view aText, Scaled aSize, const TextStyle& aTextStyle, double* x_bearing)
{
    return s_text_size(*this, aText, aSize, aTextStyle, x_bearing);

} // acmacs::surface::internal_1::Cairo::text_size

// ----------------------------------------------------------------------

acmacs::surface::PdfCairo::PdfCairo(std::string_view aFilename, double aWidth, double aHeight, double aViewportWidth)
{
    auto surface = cairo_pdf_surface_create(aFilename.empty() ? nullptr : aFilename.data(), aWidth, aHeight);
    mCairoContext = cairo_create(surface);
    cairo_surface_destroy(surface);
    change_width_in_parent(aWidth);
    viewport({acmacs::PointCoordinates(0.0, 0.0), Size{aViewportWidth, aHeight * aViewportWidth / aWidth}});
      // std::cerr << *this << std::endl;

} // PdfCairo::PdfCairo

// ----------------------------------------------------------------------

acmacs::surface::PdfCairo::~PdfCairo()
{
    if (mCairoContext)
        cairo_destroy(mCairoContext);

} // PdfCairo::~PdfCairo

// ----------------------------------------------------------------------

acmacs::surface::PdfBufferCairo::PdfBufferCairo(double aWidth, double aHeight, double aViewportWidth)
{
    auto surface = cairo_pdf_surface_create_for_stream(&writer, this, aWidth, aHeight);
    mCairoContext = cairo_create(surface);
    cairo_surface_destroy(surface);
    change_width_in_parent(aWidth);
    viewport({acmacs::PointCoordinates(0.0, 0.0), Size{aViewportWidth, aHeight * aViewportWidth / aWidth}});

} // acmacs::surface::PdfBufferCairo::PdfBufferCairo

// ----------------------------------------------------------------------

void acmacs::surface::PdfBufferCairo::flush()
{
    cairo_destroy(mCairoContext);
    mCairoContext = nullptr;

} // acmacs::surface::PdfBufferCairo::flush

// ----------------------------------------------------------------------

cairo_status_t acmacs::surface::PdfBufferCairo::writer(void *closure, const unsigned char* data, unsigned int length)
{
    auto* self = reinterpret_cast<PdfBufferCairo*>(closure);
    self->data_.append(reinterpret_cast<const char*>(data), length);
    return CAIRO_STATUS_SUCCESS;

} // acmacs::surface::PdfBufferCairo::writer

// ----------------------------------------------------------------------

acmacs::surface::PdfBufferCairo::~PdfBufferCairo()
{
    if (mCairoContext)
        cairo_destroy(mCairoContext);

} // acmacs::surface::PdfBufferCairo::~PdfBufferCairo

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
