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
    // X^N
    let X = 3;
    let N = 50;

    console.log("1.");
    function pow1(x, n) {
        let ans = 1;
        while(n > 0) {
            if(n & 1) {
                ans *= x;
            }
            x *= x;
            n >>= 1;
        }
        return ans;
    }
    console.log(pow1(X, N));

    console.log("1'.");
    console.log(Math.pow(X, N));
});
