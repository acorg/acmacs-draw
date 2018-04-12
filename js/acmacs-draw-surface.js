$(document).ready(function() {
    var surface = new Surface($('#canvas')); // , {width: 800, height: 800});
    surface.grid("red", 1);
});

// ----------------------------------------------------------------------

class Surface {
    constructor(canvas, args={}) {
        this.canvas = $(canvas);
        this.canvas.prop({width: (args.width || 300), height: (args.height || 300)});
        if (args.viewport)
            this.viewport = args.viewport;
        else
            this.viewport = [0, 0, this.canvas.prop("width"), this.canvas.prop("height")];
        this.context = this.canvas[0].getContext('2d');
    }

    grid(line_color, line_width) {
        const step = 50;
        this.context.beginPath();
        for (var x = this.viewport[0] + step; x < this.viewport[0] + this.viewport[2]; x += step) {
            this.context.moveTo(x, this.viewport[1]);
            this.context.lineTo(x, this.viewport[1] + this.viewport[3]);
        }
        for (var y = this.viewport[1] + step; y < this.viewport[1] + this.viewport[3]; y += step) {
            this.context.moveTo(this.viewport[0], y);
            this.context.lineTo(this.viewport[0] + this.viewport[2], y);
        }
        this.context.strokeStyle = line_color;
        this.context.lineWidth = line_width;
        this.context.stroke();
    }
}

// ----------------------------------------------------------------------
