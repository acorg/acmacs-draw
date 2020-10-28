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

bool acmacs::drawi::v1::Settings::apply_built_in(std::string_view name) // returns true if built-in command with that name found and applied
{
    using namespace std::string_view_literals;
    AD_LOG(acmacs::log::settings, "mapi::apply_built_in \"{}\"", name);
    AD_LOG_INDENT;
    try {
        if (name == "point"sv)
            return apply_point();
        else if (name == "viewport"sv)
            return apply_viewport();
        else if (name == "grid"sv)
            return apply_grid();
        return acmacs::settings::v3::Data::apply_built_in(name);
    }
    catch (std::exception& err) {
        throw error{fmt::format("cannot apply drawi built in \"{}\": {}", name, err)};
    }

} // acmacs::drawi::v1::Settings::apply_built_in

// ----------------------------------------------------------------------

bool acmacs::drawi::v1::Settings::apply_viewport()
{
    using namespace std::string_view_literals;
    return true;

} // acmacs::drawi::v1::Settings::apply_viewport

// ----------------------------------------------------------------------

bool acmacs::drawi::v1::Settings::apply_grid()
{
    using namespace std::string_view_literals;
    return true;

} // acmacs::drawi::v1::Settings::apply_grid

// ----------------------------------------------------------------------

bool acmacs::drawi::v1::Settings::apply_point()
{
    using namespace std::string_view_literals;
    return true;

} // acmacs::drawi::v1::Settings::apply_point

// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
