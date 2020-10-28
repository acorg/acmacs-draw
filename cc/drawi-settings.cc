#include "acmacs-draw/drawi-settings.hh"
#include "acmacs-draw/drawi-draw.hh"

// ----------------------------------------------------------------------

acmacs::drawi::v1::Settings::Settings(Draw& draw) : draw_{draw}
{
    draw_.settings(this);
    update_env();

} // acmacs::drawi::v1::Settings

// ----------------------------------------------------------------------

void acmacs::drawi::v1::Settings::update_env()
{

} // acmacs::drawi::v1::update_env

// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
