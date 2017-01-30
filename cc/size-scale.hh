#pragma once

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
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
