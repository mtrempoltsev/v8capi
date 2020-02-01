function check_obj(obj) {
    if (obj.u !== undefined)
        throw new Error('obj.u !== undefined');
    if (obj.b !== true)
        throw new Error('obj.b !== true');
    if (obj.n !== null)
        throw new Error('obj.n !== null');
    if (obj.d1 !== 1.5)
        throw new Error('obj.d1 !== 1.5');
    if (obj.d2 !== -1)
        throw new Error('obj.d2 !== -1');
    if (obj.d3 !== 1)
        throw new Error('obj.d3 !== 1');
    if (obj.d4 !== 9007199254740992)
        throw new Error('obj.d4 !== 9007199254740992');
    if (obj.s1 !== "short")
        throw new Error('obj.s1 !== short');
    if (obj.s2 !== "long long long long long long long long long long long string")
        throw new Error('obj.s2 !== "long long long long long long long long long long long string"');
}

function check_arr(arr) {
    if (!Array.isArray(arr))
        throw new Error('Array.isArray(arr) == false');
    if (arr.length !== 3)
        throw new Error('arr.length !== 3');
    if (arr[0] !== 1)
        throw new Error('arr[0] !== 1');
    if (arr[1] !== 2)
        throw new Error('arr[1] !== 2');
    if (arr[2] !== 3)
        throw new Error('arr[2] !== 3');
}

function check_set(set) {
    if (!(set instanceof Set))
        throw new Error('!(set instanceof Set)');
    if (set.size !== 3)
        throw new Error('set.size !== 3');
    if (!set.has(1))
        throw new Error('!set.has(1)');
    if (!set.has(2))
        throw new Error('!set.has(2)');
    if (!set.has(1))
        throw new Error('!set.has(3)');
}

function check_map(map) {
    if (!(map instanceof Map))
        throw new Error('!(map instanceof Map)');
    if (map.size !== 2)
        throw new Error('map.size !== 2');
    if (map.get(1) !== true)
        throw new Error('map.get(1) !== true');
    if (map.get(2) !== false)
        throw new Error('map.get(2) !== false');
}
