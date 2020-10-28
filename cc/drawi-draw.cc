#include "acmacs-draw/drawi-draw.hh"

// ----------------------------------------------------------------------

acmacs::drawi::v1::Draw::Draw(std::string_view filename, double size)
    : surface_(filename, size, size, size)
{

} // acmacs::drawi::v1::Draw::Draw

// ----------------------------------------------------------------------

void acmacs::drawi::v1::Draw::draw() const
{

} // acmacs::drawi::v1::Draw::draw

// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
