#pragma once

#include <memory>
#include <vector>
#include <string>

// ----------------------------------------------------------------------

namespace acmacs::surface { class PdfCairo; }

namespace acmacs::draw
{
    enum class drawing_stage : size_t { __first, grid, points, labels, serum_circles, procrustes_arrows, title, __last };

    inline void increment(drawing_stage& value)
    {
        value = static_cast<drawing_stage>(static_cast<size_t>(value) + 1);
    }

    template <typename Surface> class Element
    {
     public:
        Element() {}
        virtual ~Element() = default;
        virtual void draw(drawing_stage stage, Surface& surface) const = 0;

    }; // class Element

// ----------------------------------------------------------------------

    class DrawElements
    {
     public:
        DrawElements() = default;
        virtual ~DrawElements() = default;

        virtual void draw() const = 0;
        virtual void grid(double step) = 0;

    }; // class DrawElements

// ----------------------------------------------------------------------

    template <typename Surface> class DrawElementsOn : public DrawElements
    {
     public:
        DrawElementsOn(std::string filename, double size) : filename_(filename), size_(size) {}

        void draw() const override
            {
                Surface surface(filename_, size_, size_);
                for (auto stage = drawing_stage::__first; stage != drawing_stage::__last; increment(stage)) {
                    for (auto& element : elements_) {
                        element->draw(stage, surface);
                    }
                }
            }

        void grid(double step) override;

     protected:
        void add(std::unique_ptr<Element<Surface>> element) { elements_.push_back(std::move(element)); }

     private:
        std::string filename_;
        double size_;
        std::vector<std::unique_ptr<Element<Surface>>> elements_;

    }; // class DrawElementsOn

    // ----------------------------------------------------------------------

    std::shared_ptr<DrawElements> factory(std::string filename, double size);

} // namespace acmacs::draw

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
