const version_acmacs_draw_surface = "1";
const COS_PI_6 = Math.cos(Math.PI / 6);

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
    surface.points(data.layout, data.transformation, data.style_index, data.styles);

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

    points(layout, transformation, style_index, styles) {
        var transform = coord => [coord[0] * transformation[0] + coord[1] * transformation[2], coord[0] * transformation[1] + coord[1] * transformation[3]];
        layout.forEach((coord, point_no) => {
            if (coord.length > 1) {
                const style = styles[style_index[point_no]];
                const args = [transform(coord),
                              style.size === undefined ? 5 : style.size,
                              style.fill || "transparent",
                              style.outline || "black",
                              style.outline_width === undefined ? 1 : style.outline_width,
                              style.rotation,
                              style.aspect === undefined ? 1 : style.aspect
                             ];
                switch (style.shape) {
                case "CIRCLE":
                    this.circle_pixels.apply(this, args);
                    break;
                case "BOX":
                    this.box_pixels.apply(this, args);
                    break;
                case "TRIANGLE":
                    this.triangle_pixels.apply(this, args);
                    break;
                }
            }
        });
    }

    circle_scaled(center, size, fill, outline, outline_width, rotation, aspect) {
        this.context.save();
        try {
            this.context.lineWidth = outline_width;
            this.context.fillStyle = fill;
            this.context.strokeStyle = outline;
            this.context.translate(center[0], center[1]);
            if (rotation)
                this.context.rotate(rotation);
            if (aspect && aspect !== 1)
                this.context.scale(aspect, 1);
            this.context.beginPath();
            this.context.arc(0, 0, size / 2, 0, 2*Math.PI);
            this.context.fill();
            this.context.stroke();
        }
        catch(err) {
            console.error('Surface::circle_scaled', err);
        }
        this.context.restore();
    }

    circle_pixels(center, size, fill, outline, outline_width, rotation, aspect) {
        this.circle_scaled(center, size * this.scale_inv, fill, outline, outline_width * this.scale_inv, rotation, aspect);
    }

    box_scaled(center, size, fill, outline, outline_width, rotation, aspect) {
        this.context.save();
        try {
            this.context.lineWidth = outline_width;
            this.context.fillStyle = fill;
            this.context.strokeStyle = outline;
            this.context.translate(center[0], center[1]);
            if (rotation)
                this.context.rotate(rotation);
            this.context.fillRect(-size * aspect, -size, size * aspect, size);
            this.context.strokeRect(-size * aspect, -size, size * aspect, size);
        }
        catch(err) {
            console.error('Surface::box_scaled', err);
        }
        this.context.restore();
    }

    box_pixels(center, size, fill, outline, outline_width, rotation, aspect) {
        this.box_scaled(center, size * this.scale_inv, fill, outline, outline_width * this.scale_inv, rotation, aspect);
    }

    triangle_scaled(center, size, fill, outline, outline_width, rotation, aspect) {
        this.context.save();
        try {
            this.context.lineWidth = outline_width;
            this.context.fillStyle = fill;
            this.context.strokeStyle = outline;
            this.context.translate(center[0], center[1]);
            if (rotation)
                this.context.rotate(rotation);
            this.context.beginPath();
            const radius = size / 2;
            this.context.moveTo(0, -radius);
            this.context.lineTo(-radius * COS_PI_6 * aspect, radius / 2);
            this.context.lineTo(radius * COS_PI_6 * aspect, radius / 2);
            this.context.closePath();
            this.context.fill();
            this.context.stroke();
        }
        catch(err) {
            console.error('Surface::triangle_scaled', err);
        }
        this.context.restore();
    }

    triangle_pixels(center, size, fill, outline, outline_width, rotation, aspect) {
        this.triangle_scaled(center, size * this.scale_inv, fill, outline, outline_width * this.scale_inv, rotation, aspect);
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
