#include "acmacs-draw/surface-js-dynamic.hh"

// ----------------------------------------------------------------------

acmacs::surface::JsDynamic::JsDynamic(std::string filename, const acmacs::Size& canvas_size, const acmacs::Viewport& viewport)
    : output_(filename), viewport_(viewport), scale_(canvas_size.width / viewport.size.width)
{
    // write_html_header(output_, canvas_size);
    // add("var viewport = [", viewport.origin.x, ',', viewport.origin.y, ',', viewport.size.width, ',', viewport.size.height, "];");
    // context_func("scale", scale_, scale_);
    // context_func("translate", -viewport.origin.x, -viewport.origin.y);

} // acmacs::surface::JsDynamic::JsDynamic

// ----------------------------------------------------------------------

acmacs::surface::JsDynamic::~JsDynamic()
{
    // write_html_body(output_);

} // acmacs::surface::JsDynamic::~JsDynamic

// ----------------------------------------------------------------------


// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
