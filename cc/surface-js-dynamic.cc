#include "acmacs-base/read-file.hh"
#include "acmacs-base/range.hh"
#include "acmacs-base/layout.hh"
#include "acmacs-draw/surface-js-dynamic.hh"

// ----------------------------------------------------------------------

acmacs::surface::JsDynamic::JsDynamic(std::string filename, const acmacs::Size& /*canvas_size*/, const acmacs::Viewport& viewport)
    : filename_(filename) // , viewport_(viewport), scale_(canvas_size.width / viewport.size.width)
{
    data_.set_field("viewport", rjson::v1::array{viewport.origin.x(), viewport.origin.y(), viewport.size.width, viewport.size.height});

} // acmacs::surface::JsDynamic::JsDynamic

// ----------------------------------------------------------------------

acmacs::surface::JsDynamic::~JsDynamic()
{
    if (!filename_.empty())
        acmacs::file::write(filename_, data_.to_json_pp(1));

    // write_html_body(output_);

} // acmacs::surface::JsDynamic::~JsDynamic

// ----------------------------------------------------------------------

rjson::v1::array acmacs::surface::JsDynamic::convert(const acmacs::LayoutInterface& layout) const
{
    rjson::v1::array result;
    for (auto point_no : acmacs::range(layout.number_of_points())) {
        if (layout.point_has_coordinates(point_no)) {
            const auto coord = layout[point_no];
            result.insert(rjson::v1::array(rjson::v1::array::use_iterator, coord.begin(), coord.end()));
        }
        else {
            result.insert(rjson::v1::array{});
        }
    }
    return result;

} // acmacs::surface::JsDynamic::convert

// ----------------------------------------------------------------------

rjson::v1::array acmacs::surface::JsDynamic::convert(const acmacs::Transformation& transformation) const
{
    return {transformation.a, transformation.b, transformation.c, transformation.d};

} // acmacs::surface::JsDynamic::convert

// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
