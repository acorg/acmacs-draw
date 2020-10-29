#include "acmacs-base/color-modifier.hh"
#include "acmacs-base/rjson-v3-helper.hh"
#include "acmacs-draw/drawi-settings.hh"
#include "acmacs-draw/draw-points.hh"

// ----------------------------------------------------------------------

acmacs::drawi::v1::Settings::Settings(acmacs::draw::DrawElementsToFile& draw) : draw_{draw}
{
    draw_.viewport(Viewport{PointCoordinates{-5, -5}, Size{10, 10}});
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
    AD_LOG(acmacs::log::settings, "drawi::apply_built_in \"{}\"", name);
    AD_LOG_INDENT;
    try {
        if (name == "point"sv)
            return apply_point();
        else if (name == "viewport"sv)
            return apply_viewport();
        else if (name == "grid"sv)
            return apply_grid();
        else if (name == "background"sv)
            return apply_background();
        else if (name == "border"sv)
            return apply_border();
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
    bool updated{false};
    getenv("abs"sv).visit([&updated, this]<typename Val>(const Val& value) {
        if constexpr (std::is_same_v<Val, rjson::v3::detail::array>) {
            if (value.size() != 3)
                throw unrecognized{fmt::format("unrecognized: {} (3 numbers expected)", value)};
            draw_.viewport(Viewport{value[0].template to<double>(), value[1].template to<double>(), value[2].template to<double>()});
            updated = true;
        }
        else if constexpr (!std::is_same_v<Val, rjson::v3::detail::null>)
            throw unrecognized{fmt::format("unrecognized: {}", value)};
    });

    // getenv("rel"sv).visit([&updated, this]<typename Val>(const Val& value) {
    //     if constexpr (std::is_same_v<Val, rjson::v3::detail::array>) {
    //         if (value.size() != 3)
    //             throw unrecognized{fmt::format("unrecognized: {} (3 numbers expected)", value)};
    //         // chart_draw().calculate_viewport();
    //         // const auto& orig_viewport = chart_draw().viewport_before_changing();
    //         // const auto new_size = value[2].template to<double>() + orig_viewport.size.width;
    //         // if (new_size < 1)
    //         //     throw unrecognized{"invalid size difference in \"rel\""};
    //         // chart_draw().set_viewport(orig_viewport.origin + acmacs::PointCoordinates{value[0].template to<double>(), value[1].template to<double>()}, new_size);
    //         updated = true;
    //     }
    //     else if constexpr (!std::is_same_v<Val, rjson::v3::detail::null>)
    //         throw unrecognized{fmt::format("unrecognized: {}", value)};
    // });

    if (!updated)
        throw unrecognized{"neither \"abs\" nor \"rel\" found"};
    return true;

} // acmacs::drawi::v1::Settings::apply_viewport

// ----------------------------------------------------------------------

bool acmacs::drawi::v1::Settings::apply_grid()
{
    using namespace std::string_view_literals;
    draw_.grid(Scaled{1.0}, acmacs::color::Modifier{Color{0xCCCCCC} /* grey80 */, rjson::v3::read_color_or_empty(getenv("color"sv))}, rjson::v3::read_number(getenv("line-width"sv), Pixels{1.0}));
    return true;

} // acmacs::drawi::v1::Settings::apply_grid

// ----------------------------------------------------------------------

bool acmacs::drawi::v1::Settings::apply_background()
{
    using namespace std::string_view_literals;
    draw_.background(acmacs::color::Modifier{WHITE, rjson::v3::read_color_or_empty(getenv("color"sv))});
    return true;

} // acmacs::drawi::v1::Settings::apply_background

// ----------------------------------------------------------------------

bool acmacs::drawi::v1::Settings::apply_border()
{
    using namespace std::string_view_literals;
    draw_.border(acmacs::color::Modifier{Color{0xCCCCCC} /* grey80 */, rjson::v3::read_color_or_empty(getenv("color"sv))}, rjson::v3::read_number(getenv("line-width"sv), Pixels{1.0}));
    return true;

} // acmacs::drawi::v1::Settings::apply_border

// ----------------------------------------------------------------------

bool acmacs::drawi::v1::Settings::apply_point()
{
    using namespace std::string_view_literals;
    auto& points = draw_.points();

    auto& style = points.add(getenv("c"sv).visit([]<typename Value>(const Value& value) -> PointCoordinates {
        if constexpr (std::is_same_v<Value, rjson::v3::detail::array>) {
            switch (value.size()) {
                case 2:
                    return PointCoordinates{value[0].template to<double>(), value[1].template to<double>()};
                case 3:
                    return PointCoordinates{value[0].template to<double>(), value[1].template to<double>(), value[2].template to<double>()};
            }
        }
        else if constexpr (std::is_same_v<Value, rjson::v3::detail::null>)
            return PointCoordinates::zero2D;
        throw std::exception{};
    }));

    return true;

} // acmacs::drawi::v1::Settings::apply_point

// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
