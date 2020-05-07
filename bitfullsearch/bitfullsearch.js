process.stdin.resume();
process.stdin.setEncoding('utf8');

var lines = [];
var reader = require('readline').createInterface({
    input: process.stdin,
    output: process.stdout
});

reader.on('line', (line) => {
    lines.push(line);
});

reader.on('close', () => {
    console.log("要素数から考える");
    let n = 5;          // 要素数
    let size = (1<<n);
    for(let bits = 0; bits < size; bits++) {
        let out = "{";
        for(let i = 0; i < n; i++) {
            if(bits & (1 << i)) {
                out += (String(i) + ",");
            }
        }
        out += "}"
        console.log(out);
    }
    console.log();
    
    console.log("配列から考える");
    let array = new Array(5, 8, 2, 7, 4);
    size = (1<<array.length);
    for(let bits = 0; bits < size; bits++) {
        let out = "{";
        for(let i = 0; i < n; i++) {
            if(bits & (1 << i)) {
                out += (String(array[i]) + ",");
            }
        }
        out += "}"
        console.log(out);
    }
    
});

