#include "acmacs-base/to-json.hh"
#include "acmacs-base/read-file.hh"
#include "acmacs-draw/drawi-generator.hh"

// ----------------------------------------------------------------------

void acmacs::drawi::v1::Generator::generate(std::string_view filename) const
{
    using namespace to_json;

    const auto fmtd = [](double val) { return raw{fmt::format("{:7.4f}", val)}; };

    const Point default_point;
    const auto update = [&default_point](object& target, const Point& point, auto field_ref, std::string_view field_name, auto& convert) {
        if (std::invoke(field_ref, point) != std::invoke(field_ref, default_point))
            target << kv{field_name, convert(std::invoke(field_ref, point))};
    };
    const auto identity = [](const auto& val) { return val; };
    const auto to_string = [](const auto& val) { return fmt::format("{}", val); };
    const auto deref = [](const auto& val) { return *val; };
    const auto make_offset = [](const auto& val) { return array{val.x(), val.y()}; };

    array drawi;
    drawi << "border"
          << "grid"
          << object{kv{"N", "viewport"}, kv{"abs", array{fmtd(viewport().left()), fmtd(viewport().top()), fmtd(viewport().size.width), fmtd(viewport().size.height)}}, json::compact_output::yes};

    for (const auto& point : points_) {
        object label{kv{"text", point.label_}};
        update(label, point, &Point::label_size_, "size", deref);
        update(label, point, &Point::label_color_, "color", to_string);
        update(label, point, &Point::label_offset_, "offset", make_offset);

        object pnt{kv{"N", "point"},
                kv{"c", array{fmtd(point.coord_.x()), fmtd(point.coord_.y())}},
                kv{"label", std::move(label)}};
        update(pnt, point, &Point::size_, "size", deref);
        update(pnt, point, &Point::fill_, "fill", to_string);
        update(pnt, point, &Point::outline_, "outline", to_string);
        update(pnt, point, &Point::outline_width_, "outline_width", deref);
        update(pnt, point, &Point::shape_, "shape", identity);
        pnt << json::compact_output::yes;
        drawi << std::move(pnt);
    }

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


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
