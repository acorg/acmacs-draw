import {sum_offsets, offset_sub} from "./utils.js";

export class ADT_Popup1 {

    constructor(title, content, parent) {
        this.div = $(`<div class='ADT_Popup1 adt-shadow'><div class='adt-x-window-title'><div class='adt-x-close adt-x-left' title='Close'>&times;</div><div class='adt-x-title'>${title}</div><div class='adt-x-close adt-x-right' title='Close'>&times;</div></div><div class='adt-x-content'>${content}</div></div>`).appendTo($("body"));
        this.div.css(sum_offsets($(parent).offset()));
        this.div.find(".adt-x-close").on("click", () => this.destroy());
        this.div.find(".adt-x-title").on("mousedown", evt => this.title_mouse_down(evt));
    }

    destroy() {
        this.div.remove();
    }

    title_mouse_down(evt) {
        this.pos_start = {left: evt.clientX, top: evt.clientY};
        document.onmouseup = evt => this.title_mouse_up(evt);
        document.onmousemove = evt => this.title_mouse_move(evt);
    }

    title_mouse_up(evt) {
        document.onmouseup = document.onmousemove = null;
    }

    title_mouse_move(evt) {
        this.pos_current = offset_sub(this.pos_start, {left: evt.clientX, top: evt.clientY});
        this.pos_start = {left: evt.clientX, top: evt.clientY};
        this.div.css(offset_sub(this.div.offset(), this.pos_current));
    }
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
