#error No supported
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
    class Layout;
    class Transformation;
}

namespace acmacs::surface
{
    class JsDynamic
    {
      public:
        JsDynamic(std::string filename, const acmacs::Size& canvas_size, const acmacs::Viewport& viewport);
        ~JsDynamic();

        std::string write_to_string(size_t indent = 1) const { return data_.to_json_pp(indent); }

        void add_field(const char* name, rjson::v1::value&& value) { data_.set_field(name, std::move(value)); }
        rjson::v1::object& add_object(const char* name) { return data_.get_or_add(name, rjson::v1::object{}); }
        rjson::v1::array& add_array(const char* name) { return data_.get_or_add(name, rjson::v1::array{}); }

        // const acmacs::Viewport& viewport() const { return viewport_; }
        // double convert(Pixels a) const { return a.value() / scale_; }
        rjson::v1::number convert(Pixels a) const { return a.value(); }
        rjson::v1::number convert(Scaled a) const { return a.value(); }
        rjson::v1::number convert(Rotation a) const { return a.value(); }
        rjson::v1::number convert(Aspect a) const { return a.value(); }
        rjson::v1::number convert(double a) const { return a; }
        // int convert(int a) const { return a; }
        rjson::v1::string convert(Color a) const { return a.to_hex_string(); }
        // const char* convert(const char* a) const { return a; }
        rjson::v1::array convert(const std::vector<size_t> list) const { return rjson::v1::array(rjson::v1::array::use_iterator, list.begin(), list.end()); }
        rjson::v1::array convert(const acmacs::Layout& layout) const;
        rjson::v1::array convert(const acmacs::Transformation& transformation) const;
        rjson::v1::boolean convert(bool a) { return a; }

     private:
        rjson::v1::object data_;
        std::string filename_;
        // acmacs::Viewport viewport_;
        // double scale_;

    }; // class JsDynamic

} // namespace acmacs::surface

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
