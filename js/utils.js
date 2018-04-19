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
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
