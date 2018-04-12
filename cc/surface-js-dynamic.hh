#pragma once

#include <memory>

#include "acmacs-base/color.hh"
#include "acmacs-base/rjson.hh"
#include "acmacs-draw/viewport.hh"

// ----------------------------------------------------------------------

namespace acmacs
{
    class Size;
    class Viewport;
}

namespace acmacs::surface
{
    class JsDynamic
    {
      public:
        JsDynamic(std::string filename, const acmacs::Size& canvas_size, const acmacs::Viewport& viewport);
        ~JsDynamic();

        // const acmacs::Viewport& viewport() const { return viewport_; }
        // double convert(Pixels a) const { return a.value() / scale_; }
        // double convert(Scaled a) const { return a.value(); }
        // double convert(Rotation a) const { return a.value(); }
        // double convert(Aspect a) const { return a.value(); }
        // double convert(double a) const { return a; }
        // int convert(int a) const { return a; }
        // std::string convert(Color a) const { return '"' + a.to_hex_string() + '"'; }
        // const char* convert(const char* a) const { return a; }

     private:
        rjson::object data_;
        std::string filename_;
        // acmacs::Viewport viewport_;
        // double scale_;

    }; // class JsDynamic

} // namespace acmacs::surface

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
