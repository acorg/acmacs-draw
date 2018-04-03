#include "acmacs-base/text-style.hh"
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

            Window& origin(const acmacs::Location& origin) { origin_ = origin; return *this; }
            Window& size(const acmacs::Size& size) { size_ = size; return *this; }
            Window& background(Color background) { background_ = background; return *this; }
            Window& border_color(Color border_color) { border_color_ = border_color; return *this; }
            Window& border_width(Pixels border_width) { border_width_ = border_width; return *this; }

            void draw(drawing_stage stage, surface::Surface& surface) const override;

         protected:
            virtual void draw_window(surface::Surface& surface) const;
            virtual void draw_content(surface::Surface& surface) const = 0;

              // double height() const { return size_.height; }
            void size(const acmacs::Size& size) const { size_ = size; }

         private:
            acmacs::Location origin_{10, 10};
            mutable acmacs::Size size_{100, 100};
            Color background_ = GREY97;
            Color border_color_ = BLACK;
            Pixels border_width_{0.1};

            acmacs::Location scaled_origin(surface::Surface& surface) const;

        }; // class Window

        template <typename Line> class Box : public Window
        {
         public:
            Box(drawing_stage stage) : stage_(stage) {}

            auto& interline(double interline) { interline_ = interline; return *this; }
            auto& padding(Pixels padding) { padding_ = padding; return *this; }

            void stage(drawing_stage stage) { stage_ = stage; }

            void draw(drawing_stage stage, surface::Surface& surface) const override
            {
                if (stage == stage_) {
                    set_size(surface);
                    Window::draw(stage, surface);
                }
            }

          protected:
            void draw_content(surface::Surface& surface) const override
                {
                    const double padding = surface.convert(padding_).value();
                    double y = padding;
                    for (const auto& line: lines_) {
                        line.draw(surface, {padding, y}, line_height_);
                        y += line_height_ * interline_;
                    }
                }

            std::vector<Line>& lines() { return lines_; }
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
                size({width + padding * 2, line_height_ * (lines_.size() - 1) * interline_ + line_height_ + padding * 2});
            }

        }; // class Box

        class TitleLine
        {
         public:
            TitleLine() = default;
            TitleLine(std::string text) : text_(text) {}

            void text_color(Color text_color) { text_color_ = text_color; }
            void text_size(Pixels text_size) { text_size_ = text_size; }
            void text_style(const acmacs::TextStyle& text_style) { text_style_ = text_style; }

            Pixels text_size() const { return text_size_; }
            const auto& text_style() const { return text_style_; }

            void draw(surface::Surface& surface, const acmacs::Location& origin, double height) const { draw(surface, {origin.x, origin.y + height}); }
            acmacs::Size size(surface::Surface& surface) const { return surface.text_size(text_, text_size_, text_style_); }

         protected:
            void draw(surface::Surface& surface, const acmacs::Location& origin) const { surface.text(origin, text_, text_color_, text_size_, text_style_); }

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
            LegendPointLabel(std::string text, Color point_outline, Color point_fill) : TitleLine(text), point_outline_(point_outline), point_fill_(point_fill) {}

            void point_size(double point_size) { point_size_ = point_size; }

            void draw(surface::Surface& surface, const acmacs::Location& origin, double height) const;
            acmacs::Size size(surface::Surface& surface) const;

         private:
            Pixels point_size_{12};
            Color point_outline_{BLACK}, point_fill_{TRANSPARENT};

        }; // class LegendPointLabel

    } // namespace internal

    class Title : public internal::Box<internal::TitleLine>
    {
     public:
        Title(const std::vector<std::string>& texts) : internal::Box<internal::TitleLine>(drawing_stage::title)
            {
                lines().resize(texts.size());
                std::transform(texts.begin(), texts.end(), begin(), [](const auto& txt) { return txt; });
            }

        auto& text_color(Color text_color) { std::for_each(begin(), end(), [=](auto& line) { line.text_color(text_color); }); return *this; }
        auto& text_size(Pixels text_size) { std::for_each(begin(), end(), [=](auto& line) { line.text_size(text_size); }); return *this; }
        auto& text_style(const acmacs::TextStyle& text_style) { std::for_each(begin(), end(), [&](auto& line) { line.text_style(text_style); }); return *this; }

    }; // class Title

    class Legend : public internal::Box<internal::LegendPointLabel>
    {
     public:
        Legend() : internal::Box<internal::LegendPointLabel>(drawing_stage::legend) {}

        void add(std::string text, Color point_outline, Color point_fill) { lines().emplace_back(text, point_outline, point_fill); }

    }; // class Legend

} // namespace acmacs::draw

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
