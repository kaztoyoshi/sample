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
    // f(2)=x^5-12*x^4+3*x^3-8*x^2+x-5
    let X = 2;
    // 次数が小さい順に係数を並べる
    let A = new Array(-5, 1, -8, 3, -12, 1);
    let N = A.length;

    console.log("1.");
    function polynomial1(x, a, n) {
        let ans = 0;
        for(let i = 0; i < n; i++) {
            ans += a[i] * Math.pow(x, i);
        }
        return ans;
    }
    console.log(polynomial1(X, A, N));

    console.log("2'.");
    function polynomial2(x, a, n) {
        let ans = 0;
        for(let i = n-1; i >= 0; i--) {
            ans = ans * x + a[i];
        }
        return ans;
    }
    console.log(polynomial2(X, A, N));
});
