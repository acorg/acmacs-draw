#include "surface-cairo.hh"

// ----------------------------------------------------------------------

static void draw(Surface& aSurface);

// ----------------------------------------------------------------------

int main(int argc, const char *argv[])
{
    int exit_code = 0;
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <output.pdf>" << std::endl;
        exit_code = 2;
    }
    try {
        PdfCairo surface(argv[1], 500, 500);
        draw(surface);
    }
    catch (std::exception& err) {
        std::cerr << err.what() << std::endl;
        exit_code = 1;
    }
    return exit_code;
}

// ----------------------------------------------------------------------

// static const char* ana_colors[] =
// {
//     "#03569b",
//     "#e72f27",
//     "#ffc808",
//     "#a2b324",
//     "#a5b8c7",
//     "#049457",
//     "#f1b066",
//     "#742f32",
//     "#9e806e",
//     "#75ada9",
//     "#675b2c",
//     "#a020f0",
//     "#8b8989",
//     "#e9a390",
//     "#dde8cf",
//     "#00939f",
// };

void draw(Surface& aSurface)
{
    aSurface.text({10, 20}, "Ana", "black", Pixels{12});
    double diameter = 15;
    double x = diameter * 2, y = 60;
    for (auto entry: Color::distinct()) {
        aSurface.circle_filled({x, y}, Pixels{diameter}, AspectNormal, NoRotation, "black", Pixels{1}, entry);
        aSurface.text({x + diameter * 2, y + diameter * 0.5}, entry, entry, Pixels{12});
        y += diameter * 3;
    }
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
