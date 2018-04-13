const version_acmacs_draw_surface = "1";

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
    surface.points(data.layout, data.transformation);

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

    set_viewport(viewport) {
        if (viewport)
            this.viewport = viewport;
        else
            this.viewport = [0, 0, this.canvas.prop("width"), this.canvas.prop("height")];
        this.scale = this.canvas.prop("width") / viewport[2];
        this.scale_inv = 1 / this.scale;
        this.context.scale(this.scale, this.scale);
        this.context.translate(- this.viewport[0], - this.viewport[1]);
    }

    points(layout, transformation) {
        var transform = coord => [coord[0] * transformation[0] + coord[1] * transformation[2], coord[0] * transformation[1] + coord[1] * transformation[3]];
        layout.forEach((coord, point_no) => {
            if (coord.length > 1) {
                this.circle_pixels(transform(coord), 5, "green", "black", 1);
            }
        });
    }

    circle_scaled(center, size, fill, outline, outline_width) {
        this.context.save();
        try {
            this.context.lineWidth = outline_width;
            this.context.fillStyle = fill;
            this.context.strokeStyle = outline;
            this.context.translate(center[0], center[1]);
            // this.context.rotate();
            // this.context.scale();
            this.context.beginPath();
            this.context.arc(0, 0, size / 2, 0, 2*Math.PI);
            this.context.fill();
            this.context.stroke();
        }
        catch(err) {
            console.error('Surface::circle', err);
        }
        this.context.restore();
    }

    circle_pixels(center, size, fill, outline, outline_width) {
        this.circle_scaled(center, size * this.scale_inv, fill, outline, outline_width * this.scale_inv);
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
