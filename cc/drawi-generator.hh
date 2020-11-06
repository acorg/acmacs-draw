#pragma once

#include "acmacs-base/color.hh"
#include "acmacs-draw/viewport.hh"

// ----------------------------------------------------------------------

namespace to_json::inline v2
{
    class object;
}

namespace acmacs::drawi::inline v1
{
    class Generator
    {
      public:
        void generate(std::string_view filename) const;

        constexpr Viewport& viewport() { return viewport_; }
        constexpr const Viewport& viewport() const { return viewport_; }

        void title(std::string_view a_title) { title_ = a_title; }
        constexpr const std::string& title() const { return title_; }

        struct Element
        {
            virtual ~Element() = default;
            virtual to_json::object generate() const = 0;
        };

        struct Point : public Element
        {
            static inline const std::string_view Circle{"circle"};
            static inline const std::string_view Egg{"egg"};
            static inline const std::string_view UglyEgg{"uglyegg"};
            static inline const std::string_view Box{"box"};
            static inline const std::string_view Triangle{"triangle"};

            to_json::object generate() const override;
            void generate_content(to_json::object& target) const;

            Point& coord(const PointCoordinates& new_coord) { coord_ = new_coord; return *this; }
            Point& fill(Color a_fill) { fill_ = a_fill; return *this; }
            Point& outline(Color a_outline) { outline_ = a_outline; return *this; }
            Point& outline_width(Pixels a_outline_width) { outline_width_ = a_outline_width; return *this; }
            Point& size(Pixels a_size) { size_ = a_size; return *this; }
            Point& shape(std::string_view a_shape) { shape_ = a_shape; return *this; }
            Point& label(std::string_view a_label) { label_ = a_label; return *this; }
            Point& label_size(Pixels size) { label_size_ = size; return *this; }
            Point& label_offset(const PointCoordinates& offset) { label_offset_ = offset; return *this; }
            Point& label_color(Color a_color) { label_color_ = a_color; return *this; }

            PointCoordinates coord_{PointCoordinates::zero2D};
            Color fill_{TRANSPARENT};
            Color outline_{BLACK};
            Pixels outline_width_{1};
            Pixels size_{5};
            std::string shape_{Circle};
            std::string label_{};
            PointCoordinates label_offset_{0, 1.1};
            Pixels label_size_{10};
            Color label_color_{BLACK};
        };

        struct PointModify : public Point
        {
            to_json::object generate() const override;

            PointModify& select(std::string_view k, std::string_view v)
            {
                select_ = std::make_pair(std::string{k}, std::string{v});
                return *this;
            }

            std::optional<std::pair<std::string, std::string>> select_;
        };

        template <typename Elt> Elt& add()
        {
            auto ptr = std::make_unique<Elt>();
            auto& elt = *ptr;
            *elements_.emplace_back(std::move(ptr));
            return elt;
        }

      private:
        Viewport viewport_{-5, -5, 10};
        std::string title_{};
        std::vector<std::unique_ptr<Element>> elements_;
    };

} // namespace acmacs::drawi::inline v1

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
