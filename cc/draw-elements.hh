#pragma once

#include <memory>
#include <vector>
#include <string>
#include <optional>

#include "acmacs-base/color.hh"
#include "acmacs-base/line.hh"
#include "acmacs-base/size-scale.hh"
#include "acmacs-draw/viewport.hh"
#include "acmacs-draw/surface-line.hh"

// ----------------------------------------------------------------------

namespace acmacs
{
    class Layout;
    class Transformation;
}

namespace acmacs::surface
{
    class Surface;
    // class JsStatic;
    // class JsDynamic;
}

namespace acmacs::draw
{
    enum class drawing_stage : size_t { _first, background, grid, points, labels, serum_circles, procrustes_arrows, legend, title, border, _last };

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
        virtual void transform(const Transformation&) const {}
        virtual void draw(drawing_stage stage, surface::Surface& surface) const = 0;
        // virtual void draw(drawing_stage stage, surface::JsStatic& /*surface*/) const { if (stage == drawing_stage::_first) std::cerr << "WARNING: draw(JsStatic) not implemented for " << typeid(*this).name() << '\n'; }
        // virtual void draw(drawing_stage stage, surface::JsDynamic& /*surface*/) const { if (stage == drawing_stage::_first) std::cerr << "WARNING: draw(JsDynamic) not implemented for " << typeid(*this).name() << '\n'; }


    }; // class Element

// ----------------------------------------------------------------------

    class Title;
    class Legend;
    class Points;
    class Path;

    class DrawElements
    {
     public:
        DrawElements() = default;
        virtual ~DrawElements() = default;

        void draw() const;
        const std::string& output() const { return output_; } // if filename_ is "//.json"

        void viewport(const Viewport& viewport) { viewport_ = viewport; }
        constexpr const Viewport& viewport() const { return viewport_; }
        void background(Color color);
        void grid(Scaled step, Color line_color, Pixels line_width);
        void border(Color line_color, Pixels line_width);
        Title& title(const std::vector<std::string>& lines);
        Legend& legend();
        Points& points();
        Points& points(std::shared_ptr<acmacs::Layout> layout, const Transformation& transformation);
        void line(const acmacs::PointCoordinates& from, const acmacs::PointCoordinates& to, Color line_color, Pixels line_width, bool apply_transformation = false);
        void line(LineDefinedByEquation line, Color line_color, Pixels line_width, bool apply_transformation = true);
        acmacs::draw::Path& path(const std::vector<acmacs::PointCoordinates>& path, Color line_color, Pixels line_width, std::optional<Color> close_and_fill);
        void arrow(const acmacs::PointCoordinates& from, const acmacs::PointCoordinates& to, Color line_color, Pixels line_width, Color arrow_head_color, bool arrow_head_filled, Pixels arrow_width, bool apply_transformation = false);
        void rectangle(const acmacs::PointCoordinates& corner1, const acmacs::PointCoordinates& corner2, Color color, bool filled, Pixels line_width);
        void circle(const acmacs::PointCoordinates& center, Scaled size, Color fill_color, Color outline_color, Pixels outline_width, Aspect aspect, Rotation rotation);
        void point(const acmacs::PointCoordinates& center, Pixels size, Color fill_color, Color outline_color, Pixels outline_width, Aspect aspect, Rotation rotation, std::string label);
        void sector(const acmacs::PointCoordinates& center, Scaled size, Color fill_color, Color outline_color, Pixels outline_width, Color radius_color, Pixels radius_width, acmacs::surface::Dash radius_dash, Rotation start, Rotation end);
        void serum_circle(const PointCoordinates& coordinates, const Transformation& transformation, Scaled size, Color fill_color, Color outline_color, Pixels outline_width, acmacs::surface::Dash outline_dash, Color radius_color, Pixels radius_width, acmacs::surface::Dash radius_dash, Rotation start, Rotation end);
        void continent_map(const acmacs::PointCoordinates& origin, Pixels size);

        virtual bool add_all_labels() const { return false; }

        template <typename Element, typename ... Args> Element& add(Args&& ... args)
        {
            auto ptr = std::make_unique<Element>(std::forward<Args>(args) ...);
            auto& element = *ptr;
            elements_.push_back(std::move(ptr));
            return element;
        }

      protected:
        virtual void generate() const = 0;

        template <typename Surface> void draw(Surface&& surface) const
            {
                for (auto stage = drawing_stage::_first; stage != drawing_stage::_last; increment(stage)) {
                    for (const auto& element : elements_) {
                        // AD_DEBUG("draw {} stage:{}", typeid(*element).name(), stage);
                        element->draw(stage, surface);
                    }
                }
            }

     private:
        std::vector<std::unique_ptr<Element>> elements_;
        acmacs::Viewport viewport_;
        mutable std::string output_;    // if filename_ is "//.json"

    }; // class DrawElements

    // ----------------------------------------------------------------------

    class DrawElementsToFile : public DrawElements
    {
      public:
        DrawElementsToFile(std::string_view filename, double size) : filename_(filename), size_(size) {}

        bool add_all_labels() const override { return is_json(); }

      protected:
        void generate() const override;

      private:
        std::string filename_;
        double size_;

        bool is_json_buffer() const { return filename_ == "//.json"; }
        bool is_json() const { return std::string_view(filename_.data() + filename_.size() - 5, 5) == ".json"; }

    }; // class DrawElementsToFile


    class DrawElementsToSurface : public DrawElements
    {
      public:
        DrawElementsToSurface(acmacs::surface::Surface& surface) : surface_{surface} {}

      protected:
        void generate() const override;

      private:
        acmacs::surface::Surface& surface_;

    }; // class DrawElementsToSurface


} // namespace acmacs::draw

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
