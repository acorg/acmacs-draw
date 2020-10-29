#include "acmacs-base/to-json.hh"
#include "acmacs-base/read-file.hh"
#include "acmacs-draw/drawi-generator.hh"

// ----------------------------------------------------------------------

void acmacs::drawi::v1::Generator::generate(std::string_view filename) const
{
    using namespace to_json;
    const auto fmtd = [](double val) { return raw{fmt::format("{:7.4f}", val)}; };

    array drawi;
    drawi << "border"
          << "grid"
          << object{kv{"N", "viewport"}, kv{"abs", array{fmtd(viewport().left()), fmtd(viewport().top()), fmtd(viewport().size.width), fmtd(viewport().size.height)}}, json::compact_output::yes};
    for (const auto& point : points_)
        drawi << object{kv{"N", "point"},
                kv{"c", array{fmtd(point.coord_.x()), fmtd(point.coord_.y())}},
                kv{"size", *point.size_},
                kv{"fill", fmt::format("{}", point.fill_)},
                kv{"outline", fmt::format("{}", point.outline_)},
                kv{"outline-width", *point.outline_width_},
                kv{"shape", point.shape_},
                kv{"label", object{kv{"text", point.label_},
                        kv{"offset", array{point.label_offset_.x(), point.label_offset_.y()}},
                        kv{"size", *point.label_size_},
                        kv{"color", fmt::format("{}", point.label_color_)}}},
                json::compact_output::yes};

    object drawi_setup{kv{"drawi", drawi}};
    acmacs::file::write(filename, fmt::format("{:4}", drawi_setup));

} // acmacs::drawi::v1::Generator::generate

// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
