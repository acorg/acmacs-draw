#include "acmacs-base/text-style.hh"
#include "acmacs-draw/draw-elements.hh"

// ----------------------------------------------------------------------

namespace acmacs::draw
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

        double height() const { return size_.height; }
        void size(const acmacs::Size& size) const { size_ = size; }

     private:
        acmacs::Location origin_{10, 10};
        mutable acmacs::Size size_{100, 100};
        Color background_ = GREY97;
        Color border_color_ = BLACK;
        Pixels border_width_{0.1};

    }; // class Window

    class Title : public Window
    {
     public:
        Title(const std::vector<std::string>& lines) : lines_(lines) {}

        Title& text_color(Color text_color) { text_color_ = text_color; return *this; }
        Title& text_size(Pixels text_size) { text_size_ = text_size; return *this; }
        Title& text_style(const acmacs::TextStyle& text_style) { text_style_ = text_style; return *this; }
        Title& interline(double interline) { interline_ = interline; return *this; }
        Title& padding(Pixels padding) { padding_ = padding; return *this; }

        void draw(drawing_stage stage, surface::Surface& surface) const override;

     protected:
        void draw_content(surface::Surface& surface) const override;

     private:
        const std::vector<std::string> lines_;
        Color text_color_ = BLACK;
        Pixels text_size_{12};
        acmacs::TextStyle text_style_;
        double interline_{2};
        Pixels padding_{10};

        void set_size(surface::Surface& surface) const;

    }; // class Title

    // class Legend : public Window
    // {
    //  public:
    //     Border(Color line_color, Pixels line_width) : line_color_(line_color), line_width_(line_width) {}

    //     void draw(drawing_stage stage, surface::Surface& surface) const override;

    //  private:
    //     Color line_color_;
    //     Pixels line_width_;

    // }; // class Legend

} // namespace acmacs::draw

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
