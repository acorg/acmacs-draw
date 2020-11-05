#include "acmacs-base/to-json.hh"
#include "acmacs-base/read-file.hh"
#include "acmacs-draw/drawi-generator.hh"

// ----------------------------------------------------------------------

namespace drawi_detail
{
    const auto fmtd = [](double val) { return to_json::raw{fmt::format("{:7.4f}", val)}; };
    static const auto update = [](to_json::object& target, const auto& element, const auto& default_element, auto field_ref, std::string_view field_name, auto& convert) {
        if (std::invoke(field_ref, element) != std::invoke(field_ref, default_element))
            target << to_json::kv{field_name, convert(std::invoke(field_ref, element))};
    };
    static const auto identity = [](const auto& val) { return val; };
    static const auto to_string = [](const auto& val) { return fmt::format("{}", val); };
    static const auto deref = [](const auto& val) { return *val; };
    static const auto make_offset = [](const auto& val) { return to_json::array{val.x(), val.y()}; };

} // namespace drawi_detail

// ----------------------------------------------------------------------

void acmacs::drawi::v1::Generator::generate(std::string_view filename) const
{
    using namespace to_json;
    const auto fmtd = drawi_detail::fmtd;

    array drawi;
    drawi << "border"
          << "grid"
          << object{kv{"N", "viewport"}, kv{"abs", array{fmtd(viewport().left()), fmtd(viewport().top()), fmtd(viewport().size.width), fmtd(viewport().size.height)}}, json::compact_output::yes};

    for (const auto& element : elements_)
        drawi << element->generate();

    drawi << object{kv{"?N", "point-modify"},
                kv{"select", "all"},
                kv{"size", 10},
                kv{"fill", "green:p+0.7"},
                kv{"outline", "black"},
                kv{"outline-width", 1},
                kv{"shape", "triangle"},
                kv{"label", object{
                    kv{"offset", array{0, 1}},
                    kv{"size", 7},
                    kv{"color", "black"}}},
            json::compact_output::yes};

    object drawi_setup{kv{"drawi", drawi}};
    acmacs::file::write(filename, fmt::format("{:4}", drawi_setup));

} // acmacs::drawi::v1::Generator::generate

// ----------------------------------------------------------------------

to_json::object acmacs::drawi::v1::Generator::Point::generate() const
{
    using namespace to_json;
    object pnt{kv{"N", "point"}, kv{"c", array{drawi_detail::fmtd(coord_.x()), drawi_detail::fmtd(coord_.y())}}};
    generate_content(pnt);
    return pnt;

} // acmacs::drawi::v1::Generator::Point::generate

// ----------------------------------------------------------------------

void acmacs::drawi::v1::Generator::Point::generate_content(to_json::object& target) const
{
    using namespace std::string_view_literals;
    using namespace to_json;

    const Point default_point;

    object label;
    drawi_detail::update(label, *this, default_point, &Point::label_, "text", drawi_detail::identity);
    drawi_detail::update(label, *this, default_point, &Point::label_size_, "size", drawi_detail::deref);
    drawi_detail::update(label, *this, default_point, &Point::label_color_, "color", drawi_detail::to_string);
    drawi_detail::update(label, *this, default_point, &Point::label_offset_, "offset", drawi_detail::make_offset);
    if (!label.empty())
        target << kv{"label"sv, std::move(label)};

    drawi_detail::update(target, *this, default_point, &Point::size_, "size", drawi_detail::deref);
    drawi_detail::update(target, *this, default_point, &Point::fill_, "fill", drawi_detail::to_string);
    drawi_detail::update(target, *this, default_point, &Point::outline_, "outline", drawi_detail::to_string);
    drawi_detail::update(target, *this, default_point, &Point::outline_width_, "outline_width", drawi_detail::deref);
    drawi_detail::update(target, *this, default_point, &Point::shape_, "shape", drawi_detail::identity);

} // acmacs::drawi::v1::Generator::Point::generate_content

// ----------------------------------------------------------------------

to_json::object acmacs::drawi::v1::Generator::PointModify::generate() const
{
    using namespace std::string_view_literals;
    using namespace to_json;

    object pnt{kv{"N"sv, "point-modify"sv}};
    if (select_.has_value())
        pnt << kv{"select"sv, object{kv{select_->first, select_->second}}};
    else
        pnt << kv{"select"sv, "all"sv};

    generate_content(pnt);
    return pnt;

} // acmacs::drawi::v1::Generator::PointModify::generate

// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
