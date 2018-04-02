#pragma once

#include <memory>
#include <vector>
#include <string>

#include "acmacs-base/color.hh"
#include "acmacs-draw/viewport.hh"

// ----------------------------------------------------------------------

namespace acmacs::surface { class Surface; }

namespace acmacs::draw
{
    enum class drawing_stage : size_t { __first, grid, points, labels, serum_circles, procrustes_arrows, legend, title, __last };

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

    class DrawElements
    {
     public:
        DrawElements(std::string filename, double size) : filename_(filename), size_(size) {}
        // virtual ~DrawElements() = default;

        void draw() const;

        void viewport(const acmacs::Viewport& viewport) { viewport_ = viewport; }
        void grid(Scaled step, Color line_color, Pixels line_width);

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
