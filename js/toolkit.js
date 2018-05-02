import {sum_offsets} from "./utils.js";

export class ADT_Popup1 {

    constructor(title, content, parent) {
        this.div = $(`<div class='ADT_Popup1 adt-shadow'><div class='adt-x-window-title'><div class='adt-x-close adt-x-left'>X</div><div class='adt-x-title'>${title}</div><div class='adt-x-close adt-x-right'>X</div></div>${content}</div>`).appendTo($("body"));
        this.div.css(sum_offsets($(parent).offset()));
        this.div.find(".adt-x-close").on("click", () => this.destroy());

        this.div.find(".adt-x-title").on("mousedown", evt => this.title_mouse_down(evt));
    }

    destroy() {
        this.div.remove();
    }

    title_mouse_down(evt) {
        this.pos_start = {left: evt.clientX, top: evt.clientY};
        $(document).on("mouseup", evt => this.title_mouse_up(evt)).on("mousemove", evt => this.title_mouse_move(evt));
    }

    title_mouse_up(evt) {
        console.log("title_mouse_up");
        $(document).off(["mouseup", "mousemove"]);
    }

    title_mouse_move(evt) {
        this.pos_current = {left: this.pos_start.left - evt.clientX, top: this.pos_start.top - evt.clientY};
        this.pos_start = {left: evt.clientX, top: evt.clientY};
        this.div.css({left: this.div.offset().left - this.pos_current.left, top: this.div.offset().top - this.pos_current.top});
    }
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
