export class ADT_Popup1 {

    constructor(content) {
        this.div = $(`<div class='ADT_Popup1'>${content}</div>`).appendTo($("body"));
        this.div.on("click", () => this.destroy());
    }

    destroy() {
        this.div.remove();
    }
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
