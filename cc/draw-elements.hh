#pragma once

#include <memory>
#include <vector>
#include <string>
#include <typeinfo>

#include "acmacs-base/color.hh"
#include "acmacs-base/size-scale.hh"
#include "acmacs-draw/viewport.hh"
#include "acmacs-draw/surface-line.hh"

// ----------------------------------------------------------------------

namespace acmacs
{
    class LayoutInterface;
    class Coordinates;
    class Transformation;
}

namespace acmacs::surface
{
    class Surface;
    class JsStatic;
    class JsDynamic;
}

namespace acmacs::draw
{
    enum class drawing_stage : size_t { __first, background, grid, points, labels, serum_circles, procrustes_arrows, legend, title, border, __last };

    inline void increment(drawing_stage& value)
    {
        value = static_cast<drawing_stage>(static_cast<size_t>(value) + 1);
    }

// ----------------------------------------------------------------------

    class Element
    {
     public:
        Element() {}
        virtual ~Element() = default;
        virtual void draw(drawing_stage stage, surface::Surface& surface) const = 0;
        virtual void draw(drawing_stage stage, surface::JsStatic& /*surface*/) const { if (stage == drawing_stage::__first) std::cerr << "WARNING: draw(JsStatic) not implemented for " << typeid(*this).name() << '\n'; }
        virtual void draw(drawing_stage stage, surface::JsDynamic& /*surface*/) const { if (stage == drawing_stage::__first) std::cerr << "WARNING: draw(JsDynamic) not implemented for " << typeid(*this).name() << '\n'; }

    }; // class Element

// ----------------------------------------------------------------------

    class Title;
    class Legend;
    class Points;

    class DrawElements
    {
     public:
        DrawElements(std::string filename, double size) : filename_(filename), size_(size) {}

        void draw() const;
        const std::string& output() const { return output_; } // if filename_ is "//.json"

        void viewport(const acmacs::Viewport& viewport) { viewport_ = viewport; }
        void background(Color color);
        void grid(Scaled step, Color line_color, Pixels line_width);
        void border(Color line_color, Pixels line_width);
        Title& title(const std::vector<std::string>& lines);
        Legend& legend();
        Points& points(std::shared_ptr<acmacs::LayoutInterface> layout, const acmacs::Transformation& transformation);
        void line(const acmacs::Location& from, const acmacs::Location& to, Color line_color, Pixels line_width);
        void arrow(const acmacs::Location& from, const acmacs::Location& to, Color line_color, Pixels line_width, Color arrow_head_color, bool arrow_head_filled, Pixels arrow_width);
        void rectangle(const acmacs::Location& corner1, const acmacs::Location& corner2, Color color, bool filled, Pixels line_width);
        void circle(const acmacs::Location& center, Scaled size, Color fill_color, Color outline_color, Pixels outline_width, Aspect aspect, Rotation rotation);
        void point(const acmacs::Location& center, Pixels size, Color fill_color, Color outline_color, Pixels outline_width, Aspect aspect, Rotation rotation);
        void sector(const acmacs::Location& center, Scaled size, Color fill_color, Color outline_color, Pixels outline_width, Color radius_color, Pixels radius_width, acmacs::surface::Dash radius_dash, Rotation start, Rotation end);
        void serum_circle(const Coordinates& coordinates, const acmacs::Transformation& transformation, Scaled size, Color fill_color, Color outline_color, Pixels outline_width, Color radius_color, Pixels radius_width, acmacs::surface::Dash radius_dash, Rotation start, Rotation end);
        void continent_map(const acmacs::Location& origin, Pixels size);

        bool add_all_labels() const { return is_json(); }

     private:
        std::string filename_;
        double size_;
        std::vector<std::unique_ptr<Element>> elements_;
        acmacs::Viewport viewport_;
        mutable std::string output_;    // if filename_ is "//.json"

        template <typename Surface> void draw(Surface&& surface) const
            {
                for (auto stage = drawing_stage::__first; stage != drawing_stage::__last; increment(stage)) {
                    for (auto& element : elements_) {
                        element->draw(stage, surface);
                    }
                }
            }

        bool is_json_buffer() const { return filename_ == "//.json"; }
        bool is_json() const { return std::string_view(filename_.data() + filename_.size() - 5, 5) == ".json"; }

    }; // class DrawElements

    // ----------------------------------------------------------------------

} // namespace acmacs::draw

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
