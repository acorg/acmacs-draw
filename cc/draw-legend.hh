#include "acmacs-base/point-style.hh"
#include "acmacs-draw/surface.hh"
#include "acmacs-draw/draw-elements.hh"

// ----------------------------------------------------------------------

namespace acmacs::draw
{
    namespace internal
    {
        class Window : public Element
        {
          public:
            Window() = default;
            Window(const acmacs::PointCoordinates& origin) : origin_(origin) {}

            auto& origin(const acmacs::PointCoordinates& origin)
            {
                origin_ = origin;
                return *this;
            }
            auto& size(const acmacs::Size& size)
            {
                size_ = size;
                return *this;
            }
            auto& background(Color background)
            {
                background_ = background;
                return *this;
            }
            auto& border_color(Color border_color)
            {
                border_color_ = border_color;
                return *this;
            }
            auto& border_width(Pixels border_width)
            {
                border_width_ = border_width;
                return *this;
            }

            void draw(drawing_stage stage, surface::Surface& surface) const override;
            // void draw(drawing_stage stage, surface::JsStatic& surface) const override;

          protected:
            virtual void draw_window(surface::Surface& surface) const;
            virtual void draw_content(surface::Surface& surface) const = 0;
            // virtual void draw_window(surface::JsStatic& surface) const;
            // virtual void draw_content(surface::JsStatic& surface) const = 0;

            // double height() const { return size_.height; }
            void size(const acmacs::Size& size) const { size_ = size; }
            acmacs::PointCoordinates scaled_origin(surface::Surface& surface) const;
            // acmacs::PointCoordinates scaled_origin(surface::JsStatic& surface) const;
            acmacs::PointCoordinates origin() const { return origin_; }

          private:
            acmacs::PointCoordinates origin_{10, 10};
            mutable acmacs::Size size_{100, 100};
            Color background_ = GREY97;
            Color border_color_ = BLACK;
            Pixels border_width_{0.1};

            void scaled_origin_adjust(acmacs::PointCoordinates& origin, const acmacs::Size& surface_size) const;

        }; // class Window

        template <typename Line> class Box : public Window
        {
          public:
            Box(drawing_stage stage) : stage_(stage) {}

            auto& interline(double interline)
            {
                interline_ = interline;
                return *this;
            }
            auto& padding(Pixels padding)
            {
                padding_ = padding;
                return *this;
            }

            void stage(drawing_stage stage) { stage_ = stage; }

            void draw(drawing_stage stage, surface::Surface& surface) const override
            {
                if (stage == stage_) {
                    set_size(surface);
                    Window::draw(stage, surface);
                }
            }

            // void draw_field(drawing_stage stage, surface::JsDynamic& surface, const char* field) const
            //     {
            //         if (stage == stage_) {
            //             for (const auto& line : lines())
            //                 line.draw(surface, field);
            //         }
            //     }

          protected:
            void draw_content(surface::Surface& surface) const override
            {
                const double padding = surface.convert(padding_).value();
                double y = padding;
                for (const auto& line : lines_) {
                    line.draw(surface, {padding, y}, line_height_);
                    y += line_height_ * interline_;
                }
            }

            std::vector<Line>& lines() { return lines_; }
            const std::vector<Line>& lines() const { return lines_; }
            using iterator = typename std::vector<Line>::iterator;
            iterator begin() { return lines_.begin(); }
            iterator end() { return lines_.end(); }

          private:
            drawing_stage stage_;
            std::vector<Line> lines_;
            double interline_{2};
            Pixels padding_{10};
            mutable double line_height_ = 0;

            void set_size(surface::Surface& surface) const
            {
                double width = 0;
                for (const auto& line : lines_) {
                    const acmacs::Size line_size = line.size(surface);
                    if (line_size.width > width)
                        width = line_size.width;
                    if (line_size.height > line_height_)
                        line_height_ = line_size.height;
                }
                const double padding = surface.convert(padding_).value();
                size({width + padding * 2, line_height_ * static_cast<double>(lines_.size() - 1) * interline_ + line_height_ + padding * 2});
            }

        }; // class Box

        class TitleLine
        {
          public:
            TitleLine() = default;
            TitleLine(std::string_view text) : text_{text} {}
            TitleLine(std::string_view text, Color text_color, Pixels text_size, const acmacs::TextStyle& text_style)
                : text_{text}, text_color_(text_color), text_size_(text_size), text_style_(text_style)
            {
            }

            void text_color(Color text_color) { text_color_ = text_color; }
            void text_size(Pixels text_size) { text_size_ = text_size; }
            void text_style(const acmacs::TextStyle& text_style) { text_style_ = text_style; }

            Pixels text_size() const { return text_size_; }
            const auto& text_style() const { return text_style_; }

            void draw(surface::Surface& surface, const acmacs::PointCoordinates& origin, double height) const { draw(surface, {origin.x(), origin.y() + height}); }
            // void draw(surface::JsDynamic& surface, const char* field) const;
            acmacs::Size size(surface::Surface& surface) const { return surface.text_size(text_, text_size_, text_style_); }

          protected:
            void draw(surface::Surface& surface, const acmacs::PointCoordinates& origin) const { surface.text(origin, text_, text_color_, text_size_, text_style_); }

          private:
            std::string text_;
            Color text_color_{BLACK};
            Pixels text_size_{12};
            acmacs::TextStyle text_style_;

        }; // class TitleLine

        class LegendPointLabel : public TitleLine
        {
          public:
            LegendPointLabel() = default;
            LegendPointLabel(std::string_view text, Pixels point_size, Color point_outline, Pixels point_outline_width, Color point_fill)
                : TitleLine(text), point_size_(point_size), point_outline_width_{point_outline_width}, point_outline_(point_outline), point_fill_(point_fill)
            {
            }
            LegendPointLabel(std::string_view text, Color text_color, Pixels text_size, const acmacs::TextStyle& text_style, Pixels point_size, Color point_outline, Pixels point_outline_width,
                             Color point_fill, acmacs::PointShape::Shape point_shape)
                : TitleLine(text, text_color, text_size, text_style), point_size_(point_size), point_outline_width_{point_outline_width}, point_outline_(point_outline),
                  point_fill_(point_fill), shape_{point_shape}
            {
            }

            void point_size(Pixels point_size) { point_size_ = point_size; }

            void draw(surface::Surface& surface, const acmacs::PointCoordinates& origin, double height) const;
            acmacs::Size size(surface::Surface& surface) const;
            void shape(acmacs::PointShape::Shape shape) { shape_ = shape; }

          private:
            Pixels point_size_{12}, point_outline_width_{1};
            Color point_outline_{BLACK}, point_fill_{TRANSPARENT};
            acmacs::PointShape::Shape shape_{acmacs::PointShape::Circle};

        }; // class LegendPointLabel

    } // namespace internal

    class Title : public internal::Box<internal::TitleLine>
    {
      public:
        Title(const std::vector<std::string>& texts) : internal::Box<internal::TitleLine>(drawing_stage::title)
        {
            lines().resize(texts.size());
            std::transform(texts.begin(), texts.end(), begin(), [](const auto& txt) -> std::string_view { return std::string_view{txt}; });
        }

        auto& text_color(Color text_color)
        {
            std::for_each(begin(), end(), [=](auto& line) { line.text_color(text_color); });
            return *this;
        }
        auto& text_size(Pixels text_size)
        {
            std::for_each(begin(), end(), [=](auto& line) { line.text_size(text_size); });
            return *this;
        }
        auto& text_style(const acmacs::TextStyle& text_style)
        {
            std::for_each(begin(), end(), [&](auto& line) { line.text_style(text_style); });
            return *this;
        }

        using internal::Box<internal::TitleLine>::draw;
        // void draw(drawing_stage stage, surface::JsDynamic& surface) const override { internal::Box<internal::TitleLine>::draw_field(stage, surface, "title"); }

    }; // class Title

    class Legend : public internal::Box<internal::LegendPointLabel>
    {
      public:
        Legend() : internal::Box<internal::LegendPointLabel>(drawing_stage::legend) {}

        void add(std::string_view text, Pixels point_size, Color point_outline, Pixels point_outline_width, Color point_fill)
        {
            lines().emplace_back(text, point_size, point_outline, point_outline_width, point_fill);
        }
        void add(std::string_view text, Color text_color, Pixels text_size, const acmacs::TextStyle& text_style, Pixels point_size, Color point_outline, Pixels point_outline_width, Color point_fill, acmacs::PointShape::Shape point_shape)
        {
            lines().emplace_back(text, text_color, text_size, text_style, point_size, point_outline, point_outline_width, point_fill, point_shape);
        }

    }; // class Legend

    class ContinentMap : public Element
    {
      public:
        ContinentMap(const acmacs::PointCoordinates& origin, Pixels size) : origin_(origin), size_(size) {}

        void draw(drawing_stage stage, surface::Surface& surface) const override;

      private:
        acmacs::PointCoordinates origin_;
        Pixels size_;

    }; // class ContinentMap

} // namespace acmacs::draw

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
