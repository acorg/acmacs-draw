#include <iostream>
#include <string>
#include <memory>

#include "surface-cairo.hh"

// ----------------------------------------------------------------------

void draw(acmacs::surface::Surface& aSurface);
void draw1(acmacs::surface::Surface& aSurface);

// ----------------------------------------------------------------------

int main(int /*argc*/, const char */*argv*/[])
{
    int exit_code = 0;
    try {
        std::cerr << "INFO: generating /tmp/tc.pdf\n";
        acmacs::surface::PdfCairo surface("/tmp/tc.pdf", 500, 850);
        draw(surface);
        auto& sub1 = surface.subsurface({110, 720}, Scaled{500}, {acmacs::PointCoordinates::zero2D, acmacs::Size{1000, 1000}}, false);
        draw(sub1);
        auto& sub2 = sub1.subsurface({110, 470}, Scaled{500}, {acmacs::PointCoordinates::zero2D, acmacs::Size{1000, 1000}}, true);
        draw(sub2);
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
    const auto& v = aSurface.viewport();
    aSurface.rectangle_filled(v.origin, v.size, WHITE, Pixels{0}, WHITE);
    aSurface.grid(Scaled{aSurface.viewport().size.width / 10}, 0xC0808080, Pixels{1});
    aSurface.rectangle(v.origin, v.size, 0xC0808080, Pixels{2});

    aSurface.line({150, 50}, {950, 160}, Color{"red2"}, Pixels{2});
    aSurface.circle_filled({100, 200}, Scaled{50}, Aspect{0.5}, Rotation{0.25}, BLACK, Pixels{1}, acmacs::surface::Dash::NoDash, 0xC0FF0000);
    aSurface.circle_filled({150, 200}, Pixels{10}, Aspect{1.4}, Rotation{0.75}, BLACK, Pixels{1}, acmacs::surface::Dash::NoDash, 0xC0FFB000);
    aSurface.square_filled({200, 200}, Scaled{50}, Aspect{1.0}, Rotation{1.0}, BLACK, Pixels{1}, 0xC0FF00B0);
    aSurface.square_filled({250, 200}, Pixels{10}, Aspect{1.0}, Rotation{0.0}, BLACK, Pixels{1}, 0xC0FF00B0);
    aSurface.triangle_filled({300, 200}, Scaled{50}, Aspect{1.0}, Rotation{0.0}, BLACK, Pixels{1}, 0xC000B0B0);
    aSurface.triangle_filled({350, 200}, Pixels{10}, Aspect{1.0}, Rotation{0.0}, BLACK, Pixels{1}, 0xC000B0B0);

    aSurface.double_arrow({100, 300}, {600, 250}, BLUE, Pixels{2}, Pixels{6});

    aSurface.text({700, 500}, "Pan-dan", MAGENTA, Scaled{30});
    acmacs::Size s1 = aSurface.text_size("Pan-dan", Scaled{30});
    aSurface.text({700 + s1.width, 500}, "-dati", MAGENTA, Scaled{30});

    aSurface.text({700, 550}, "Pan-dan", MAGENTA, Pixels{10});
    acmacs::Size s2 = aSurface.text_size("Pan-dan", Pixels{10});
    aSurface.text({700 + s2.width, 550}, "-dati", PURPLE, Pixels{10});

      // aSurface.text({700, 600}, "Wan-dan-dati", BLUE, 20);
      // aSurface.text({700, 700}, "Wan-dan-dati", BLUE, 30);
}

// ----------------------------------------------------------------------

void draw1(acmacs::surface::Surface& /*aSurface*/)
{
    // aSurface.background(0x40F0F0FF);
    // aSurface.grid(200, "grey80", 5);
    // aSurface.border("grey50", 20);
    // aSurface.circle({500, 100}, 200, 1.0, 0.0, BLACK, 10);
    // aSurface.circle({1000, 0}, 200, 1.0, 0.0, BLACK, 10);
    // aSurface.circle({1000, 1700}, 200, 1.0, 0.0, BLACK, 10);
    // aSurface.line({100, 100}, {300, 300}, "red", 10);
    // aSurface.rectangle({100, 170}, {50, 70}, "orange", 5);
    // aSurface.rectangle_filled({120, 200}, {250, 70}, "brown", 5, "#8080ffff");
    // aSurface.circle({150, 170}, 100, 0.5, M_PI / 4.0, "violet", 5);
    // aSurface.circle_filled({200, 220}, 100, 2, 0, 0x80FFA0FF, 5, 0x80A0FFA0);
    // aSurface.square_filled({300, 420}, 100, 1.1, 0.1, 0x80A0A0FF, 5, 0x80C0FFA0);
    // aSurface.triangle_filled({100, 420}, 100, 1.0, 0.0, 0x80FFA080, 5, 0x80FFFFA0);
    // aSurface.double_arrow({100, 420}, {300, 550}, "blue", 2, 10);
    // aSurface.double_arrow({300, 420}, {100, 550}, "blue4", 2, 10);

    // aSurface.text({50, 600}, "Vozyaku", "green4", 100);
    // auto text_size = aSurface.text_size("Vozyak", 100);
    // aSurface.text({50 + text_size.width, 600}, "uVozyaku", "green3", 100);
    // aSurface.text({500, 750}, "Vozyaku", "green4", 100, TextStyle("Arial", TextStyle::Slant::Italic, TextStyle::Weight::Bold), 0.5);
    // aSurface.text({50, 800}, "Va", "red2", 100);
    // aSurface.text({50, 800 - text_size.height}, "Va", "red2", 100);
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
