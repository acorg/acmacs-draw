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

// ----------------------------------------------------------------------
/// Local Variables:
/// eval: (if (fboundp 'eu-rename-buffer) (eu-rename-buffer))
/// End:
