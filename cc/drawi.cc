#include "acmacs-base/argv.hh"
#include "acmacs-base/string-split.hh"
#include "acmacs-base/string-compare.hh"
#include "acmacs-base/temp-file.hh"
#include "acmacs-base/filesystem.hh"
#include "acmacs-base/quicklook.hh"
#include "acmacs-draw/draw-elements.hh"
#include "acmacs-draw/drawi-settings.hh"

// ----------------------------------------------------------------------

using namespace acmacs::argv;
struct MapiOptions : public acmacs::argv::v2::argv
{
    MapiOptions(int a_argc, const char* const a_argv[], on_error on_err = on_error::exit) { parse(a_argc, a_argv, on_err); }


    option<str_array> apply{*this, 'a', "apply", desc{"comma separated names or json array to apply instead of \"drawi\""}};
    option<bool>      open{*this, "open"};
    option<str_array> verbose{*this, 'v', "verbose", desc{"comma separated list (or multiple switches) of enablers"}};

    argument<str_array> inputs_output{*this, arg_name{"input.drawi|output.pdf"}, mandatory};
};

int main(int argc, char* const argv[])
{
    using namespace std::string_view_literals;
    int exit_code = 0;
    try {
        MapiOptions opt(argc, argv);
        acmacs::log::enable(opt.verbose);

        std::unique_ptr<acmacs::file::temp> temp_file;
        const auto get_output = [&temp_file](const std::vector<std::string_view>& names) -> std::pair<std::string_view, bool> {
            if (names.size() > 1 && (names.back() == "/dev/null"sv || names.back() == "/"sv)) {
                return {{}, false};
            }
            else if (names.size() < 2 || !acmacs::string::endswith(names.back(), ".pdf"sv)) {
                temp_file = make_unique<acmacs::file::temp>(fmt::format("{}.pdf", fs::path(names.front()).stem()));
                return {*temp_file, true};
            }
            else
                return {names.back(), false};
        };
        const auto [output, always_open] = get_output(opt.inputs_output);
        constexpr double output_size = 800;
        {
            acmacs::draw::DrawElementsToFile draw{output, output_size};
            acmacs::drawi::Settings settings{draw};
            for (const auto& input : opt.inputs_output) {
                if (input.find('.') != std::string_view::npos && !acmacs::string::endswith(input, ".pdf"sv))
                    settings.load(input);
            }

            if (!opt.apply.empty()) {
                for (const auto& to_apply : opt.apply) {
                    if (!to_apply.empty()) {
                        if (to_apply[0] == '{' || to_apply[0] == '[') {
                            settings.apply(to_apply);
                        }
                        else {
                            for (const auto& to_apply_one : acmacs::string::split(to_apply))
                                settings.apply(to_apply_one);
                        }
                    }
                }
            }
            else {
                settings.apply("drawi"sv);
            }
            draw.draw();
        }

        if (opt.open || always_open)
            acmacs::open(output, 0, 1);
    }
    catch (std::exception& err) {
        AD_ERROR("{}", err);
        exit_code = 2;
    }
    return exit_code;
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
