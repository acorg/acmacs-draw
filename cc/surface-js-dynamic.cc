#include "acmacs-base/read-file.hh"
#include "acmacs-draw/surface-js-dynamic.hh"

// ----------------------------------------------------------------------

acmacs::surface::JsDynamic::JsDynamic(std::string filename, const acmacs::Size& canvas_size, const acmacs::Viewport& viewport)
    : filename_(filename) // , viewport_(viewport), scale_(canvas_size.width / viewport.size.width)
{
    data_.set_field("viewport", rjson::array{viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height});

} // acmacs::surface::JsDynamic::JsDynamic

// ----------------------------------------------------------------------

acmacs::surface::JsDynamic::~JsDynamic()
{
    acmacs::file::write(filename_, data_.to_json_pp(1));

    // write_html_body(output_);

} // acmacs::surface::JsDynamic::~JsDynamic

// ----------------------------------------------------------------------


// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
