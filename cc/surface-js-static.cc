#include "acmacs-draw/viewport.hh"
#include "acmacs-draw/surface-js-static.hh"

// ----------------------------------------------------------------------

static void write_html_header(std::ostream& output, const acmacs::Size& canvas_size);
static void write_html_body(std::ostream& output);

// ----------------------------------------------------------------------

acmacs::surface::JsStatic::JsStatic(std::string filename, const acmacs::Size& canvas_size, const acmacs::Viewport& viewport)
    : output_(filename)
{
    write_html_header(output_, canvas_size);
    const auto scale = canvas_size.width / viewport.size.width;
    output_ << indent_ << "__context.scale(" << scale << ',' << scale << ");\n"
            << indent_ << "__context.translate(" << -viewport.origin.x << ',' << -viewport.origin.y << ");\n";

} // acmacs::surface::JsStatic::JsStatic

// ----------------------------------------------------------------------

acmacs::surface::JsStatic::~JsStatic()
{
    write_html_body(output_);

} // acmacs::surface::JsStatic::~JsStatic

// ----------------------------------------------------------------------

void write_html_header(std::ostream& output, const acmacs::Size& canvas_size)
{
    output << R"(<html>
  <head>
    <meta charset="utf-8" />
    <style>
      #canvas {
        margin: 1em;
        border: 1px solid black;
      }
    </style>
    <title>acmacs-draw</title>
    <script>
      function main() {
        var canvas = document.getElementById('canvas');
        canvas.width = )" << canvas_size.width << R"(;
        canvas.height = )" << canvas_size.height << R"(;
        var ctx = canvas.getContext('2d');
        var start = new Date();
        draw(ctx);
        var elapsed = new Date() - start;
        console.log("drawing time: " + elapsed + "ms -> " + (1000 / elapsed).toFixed(1) + "fps");
      }
      function draw(__context) {
)";

} // write_html_header

// ----------------------------------------------------------------------

void write_html_body(std::ostream& output)
{
    output << R"(      }
      document.addEventListener('DOMContentLoaded', main, false);
    </script>
  </head>
  <body>
    <canvas id="canvas"></canvas>
  </body>
</html>
)";

} // write_html_body

// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
