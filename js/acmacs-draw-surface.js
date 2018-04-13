// $(document).ready(function() {
//     var surface = new Surface($('#canvas')); // , {width: 800, height: 800});
//     surface.grid("red", 1);
// });

// ----------------------------------------------------------------------

function draw(canvas, data) {
    var start = new Date();

    var surface = new Surface(canvas, {width: 800, height: 800, viewport: data.viewport});
    if (data.border)
        surface.border(data.border);
    if (data.background)
        surface.background(data.background);
    if (data.grid)
        surface.grid(data.grid);

    var elapsed = new Date() - start;
    console.log("drawing time: " + elapsed + "ms -> " + (1000 / elapsed).toFixed(1) + "fps");
}

// ----------------------------------------------------------------------

class Surface {

    constructor(canvas, args={}) {
        this.canvas = $(canvas);
        this.canvas.prop({width: (args.width || 300), height: (args.height || 300)});
        this.context = this.canvas[0].getContext('2d');
        this.set_viewport(args.viewport);
    }

    grid(args={line_color: "pink", line_width: 1, step: 1}) {
        this.context.beginPath();
        for (var x = this.viewport[0] + args.step; x < this.viewport[0] + this.viewport[2]; x += args.step) {
            this.context.moveTo(x, this.viewport[1]);
            this.context.lineTo(x, this.viewport[1] + this.viewport[3]);
        }
        for (var y = this.viewport[1] + args.step; y < this.viewport[1] + this.viewport[3]; y += args.step) {
            this.context.moveTo(this.viewport[0], y);
            this.context.lineTo(this.viewport[0] + this.viewport[2], y);
        }
        this.context.strokeStyle = args.line_color;
        this.context.lineWidth = args.line_width / this.scale;
        this.context.stroke();
    }

    set_viewport(viewport) {
        if (viewport)
            this.viewport = viewport;
        else
            this.viewport = [0, 0, this.canvas.prop("width"), this.canvas.prop("height")];
        this.scale = this.canvas.prop("width") / viewport[2];
        this.context.scale(this.scale, this.scale);
        this.context.translate(- this.viewport[0], - this.viewport[1]);
    }

    background(args={color: "white"}) {
        this.context.fillStyle = args.color;
        this.context.fillRect.apply(this.context, this.viewport);
    }

    border(args={line_color: "pink", line_width: 1}) {
        this.canvas.css("border", "" + args.line_width + "px solid " + args.line_color);
    }
}

// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
