#include "acmacs-base/to-json.hh"
#include "acmacs-base/read-file.hh"
#include "acmacs-draw/drawi-generator.hh"

// ----------------------------------------------------------------------

void acmacs::drawi::v1::Generator::generate(std::string_view filename) const
{
    using namespace to_json;
    array drawi;
    drawi << "border"
          << "grid" << object{key_val{"N", "viewport"}, key_val{"abs", array{viewport().left(), viewport().top(), viewport().size.width, viewport().size.height}}};
    object drawi_setup;
    drawi_setup << key_val{"drawi", drawi};
    acmacs::file::write(filename, fmt::format("{}", drawi_setup));

} // acmacs::drawi::v1::Generator::generate

// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
