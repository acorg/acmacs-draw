#include "acmacs-draw/surface-js-static.hh"

// ----------------------------------------------------------------------

static void write_html_header(std::ostream& output, const acmacs::Size& canvas_size);
static void write_html_body(std::ostream& output);

// ----------------------------------------------------------------------

acmacs::surface::JsStatic::JsStatic(std::string filename, const acmacs::Size& canvas_size, const acmacs::Viewport& viewport)
    : output_(filename), viewport_(viewport), scale_(canvas_size.width / viewport.size.width)
{
    write_html_header(output_, canvas_size);
    add("var viewport = [", viewport.origin.x, ',', viewport.origin.y, ',', viewport.size.width, ',', viewport.size.height, "];");
    context_func("scale", scale_, scale_);
    context_func("translate", -viewport.origin.x, -viewport.origin.y);

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
      function grid(__context, viewport, step, line_color, line_width) {
        __context.beginPath();
        for (var x = viewport[0] + step; x < viewport[0] + viewport[2]; x += step) {
            __context.moveTo(x, viewport[1]);
            __context.lineTo(x, viewport[1] + viewport[3]);
        }
        for (var y = viewport[1] + step; y < viewport[1] + viewport[3]; y += step) {
            __context.moveTo(viewport[0], y);
            __context.lineTo(viewport[0] + viewport[2], y);
        }
        __context.strokeStyle = line_color;
        __context.lineWidth = line_width;
        __context.stroke();
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
