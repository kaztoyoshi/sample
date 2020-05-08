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
    let N = 36;         // 整数 N
    console.log("1.");
    let out = "{";
    for(let i = 1; i <= N; i++) {
        if(N % i == 0) {
            out += (String(i) + ",");
        }
    }
    out += "}"
    console.log(out);

    console.log("2.");
    let array = new Array();
    for(let i = 1; i * i <= N; i++) {
        if(N % i == 0) {
            array.push(i);
            if(i * i != N)
                array.push(N / i);
        }
    }
    array.sort((a,b) => {
        if(a > b) return 1;
        if(a < b) return -1;
        return 0;
    });
    out = "{";
    array.forEach(element => {
        out += (String(element) + ",");
    });
    out += "}"
    console.log(out);

    console.log("2'.");
    let asc = new Array();  // 昇順
    let desc = new Array(); // 降順
    for(let i = 1; i * i <= N; i++) {
        if(N % i == 0) {
            asc.push(i);
            if(i * i != N)
                desc.unshift(N / i);
        }
    }
    out = "{";
    asc.forEach(element => {
        out += (String(element) + ",");
    });
    desc.forEach(element => {
        out += (String(element) + ",");
    });
    out += "}"
    console.log(out);
});

