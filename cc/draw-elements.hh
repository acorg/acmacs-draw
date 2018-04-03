#pragma once

#include <memory>
#include <vector>
#include <string>

#include "acmacs-base/color.hh"
#include "acmacs-draw/viewport.hh"

// ----------------------------------------------------------------------

namespace acmacs
{
    class LayoutInterface;
    class Transformation;
}

namespace acmacs::surface
{
    class Surface;
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

    }; // class Element

// ----------------------------------------------------------------------

    class Title;
    class Points;

    class DrawElements
    {
     public:
        DrawElements(std::string filename, double size) : filename_(filename), size_(size) {}

        void draw() const;

        void viewport(const acmacs::Viewport& viewport) { viewport_ = viewport; }
        void background(Color color);
        void grid(Scaled step, Color line_color, Pixels line_width);
        void border(Color line_color, Pixels line_width);
        Title& title(const std::vector<std::string>& lines);
        Points& points(std::shared_ptr<acmacs::LayoutInterface> layout, const acmacs::Transformation& transformation);
        void line(const acmacs::Location& from, const acmacs::Location& to, Color line_color, Pixels line_width);
        void arrow(const acmacs::Location& from, const acmacs::Location& to, Color line_color, Pixels line_width, Color arrow_head_color, bool arrow_head_filled, Pixels arrow_width);
        void rectangle(const acmacs::Location& corner1, const acmacs::Location& corner2, Color color, bool filled, Pixels line_width);
        void circle(const acmacs::Location& center, Scaled size, Color fill_color, Color outline_color, Pixels outline_width, Aspect aspect, Rotation rotation);

     private:
        std::string filename_;
        double size_;
        std::vector<std::unique_ptr<Element>> elements_;
        acmacs::Viewport viewport_;

        template <typename Surface> void draw(Surface&& surface) const
            {
                for (auto stage = drawing_stage::__first; stage != drawing_stage::__last; increment(stage)) {
                    for (auto& element : elements_) {
                        element->draw(stage, surface);
                    }
                }
            }

    }; // class DrawElements

    // ----------------------------------------------------------------------

} // namespace acmacs::draw

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
