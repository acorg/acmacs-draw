#include "acmacs-base/color-distinct.hh"
#include "surface-cairo.hh"

// ----------------------------------------------------------------------

static void draw(acmacs::surface::Surface& aSurface);

// ----------------------------------------------------------------------

int main(int argc, const char *argv[])
{
    int exit_code = 0;
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <output.pdf>" << std::endl;
        exit_code = 2;
    }
    try {
        acmacs::surface::PdfCairo surface(argv[1], 500, 500);
        draw(surface);
    }
    catch (std::exception& err) {
        std::cerr << err.what() << std::endl;
        exit_code = 1;
    }
    return exit_code;
}

// ----------------------------------------------------------------------

void draw(acmacs::surface::Surface& aSurface)
{
    aSurface.text({10, 20}, "Ana", BLACK, Pixels{12});
    double diameter = 15;
    double x = diameter * 2, y = 60;
    for (auto entry: acmacs::color::distinct()) {
        aSurface.circle_filled({x, y}, Pixels{diameter}, AspectNormal, NoRotation, BLACK, Pixels{1}, acmacs::surface::Dash::NoDash, entry);
        aSurface.text({x + diameter * 2, y + diameter * 0.5}, fmt::format("{}", entry), entry, Pixels{12});
        y += diameter * 3;
    }
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
