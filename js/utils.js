export function sval(name, data, dflt = undefined) {
    for (let entry of data) {
        let val = entry[name];
        if (val !== undefined)
            return val;
    }
    return dflt;
}

// ----------------------------------------------------------------------

export function sval_call(name, data, func) {
    for (let entry of data) {
        let val = entry[name];
        if (val !== undefined) {
            func(val);
            break;
        }
    }
}

// ----------------------------------------------------------------------

export function sum_offsets(offsets) {
    if (Array.isArray(offsets)) {
        const reducer = (accumulator, currentValue) => { return {left: accumulator.left + currentValue.left, top: accumulator.top + currentValue.top}; };
        return offsets.reduce(reducer);
    }
    else {
        return offsets;
    }
}

// ----------------------------------------------------------------------

export function offset_sub(offset1, offset2) {
    return {left: offset1.left - offset2.left, top: offset1.top - offset2.top};
}

// ----------------------------------------------------------------------

// https://stackoverflow.com/questions/4810841/how-can-i-pretty-print-json-using-javascript
export function json_syntax_highlight(data) {
    data = data.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
    return data.replace(/("(\\u[a-zA-Z0-9]{4}|\\[^u]|[^\\"])*"(\s*:)?|\b(true|false|null)\b|-?\d+(?:\.\d*)?(?:[eE][+\-]?\d+)?)/g, match => {
        var cls = 'json-syntax-number';
        if (/^"/.test(match)) {
            if (/:$/.test(match)) {
                cls = 'json-syntax-key';
            }
            else if (/^"[0-9a-f]{24}"/.test(match)) {
                cls = 'json-syntax-object-id';
            }
            else {
                cls = 'json-syntax-string';
            }
        }
        else if (/true|false/.test(match)) {
            cls = 'json-syntax-boolean';
        }
        else if (/null/.test(match)) {
            cls = 'json-syntax-null';
        }
        if (cls === 'json-syntax-object-id') {
            const id_itself = match.replace(/"/g, '');
            const href = "/doc/" + id_itself;
            return `"<a class="json-syntax-object-id" href="${href}" target="_blank">${id_itself}</a>"`;
        }
        else
            return '<span class="' + cls + '">' + match + '</span>';
    });
}

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
