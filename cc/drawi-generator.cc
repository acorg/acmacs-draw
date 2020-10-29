#include "acmacs-base/to-json.hh"
#include "acmacs-base/read-file.hh"
#include "acmacs-draw/drawi-generator.hh"

// ----------------------------------------------------------------------

void acmacs::drawi::v1::Generator::generate(std::string_view filename) const
{
    using namespace to_json;
    array drawi;
    drawi << "border"
          << "grid"
          << object{kv{"N", "viewport"}, kv{"abs", array{viewport().left(), viewport().top(), viewport().size.width, viewport().size.height}}};
    for (const auto& point : points_)
        drawi << object{kv{"N", "point"},
                kv{"c", array{point.coord_.x(), point.coord_.y()}},
                kv{"size", *point.size_},
                kv{"fill", fmt::format("{}", point.fill_)},
                kv{"outline", fmt::format("{}", point.outline_)},
                kv{"outline-width", *point.outline_width_},
                kv{"shape", point.shape_},
                };
    // shape
    object drawi_setup{kv{"drawi", drawi}};
    acmacs::file::write(filename, fmt::format("{:4}", drawi_setup));

} // acmacs::drawi::v1::Generator::generate

// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
