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
    let N1 = 13;
    let N2 = 15;
    let N3 = 17;

    console.log("1.");
    function isPrime1(N) {
        for(let i = 2; i < N; i++) {
            if(N % i == 0) {
                return false;
            }
        }
        return true;
    }
    console.log(N1 + ":" + (isPrime1(N1) ? "OK" : "NG"));
    console.log(N2 + ":" + (isPrime1(N2) ? "OK" : "NG"));
    console.log(N3 + ":" + (isPrime1(N3) ? "OK" : "NG"));

    console.log("2.");
    function isPrime2(N) {
        for(i = 2; i * i <= N; i++) {
            if(N % i == 0)
                return false;
        }
        return true;
    }
    console.log(N1 + ":" + (isPrime2(N1) ? "OK" : "NG"));
    console.log(N2 + ":" + (isPrime2(N2) ? "OK" : "NG"));
    console.log(N3 + ":" + (isPrime2(N3) ? "OK" : "NG"));
    
    console.log("3.");
    let N = 100;
    let list = new Array(N+1);
    // 0,1は素数ではないので外す
    list[0] = false;
    list[1] = false;
    // 探索リストに 2 から N まで true を入れる
    for(let i = 2; i <= N; i++) {
        list[i] = true;
    }
    for(let i = 2; i * i <= N; i++) {
        // 素数の場合
        if(list[i]) {
            // 先頭の数の倍数をリストから篩い落とす
            for(let j = i * 2; j <= N; j += i) {
                list[j] = false;
            }
        }
    }
    console.log(N1 + ":" + (list[N1] ? "OK" : "NG"));
    console.log(N2 + ":" + (list[N2] ? "OK" : "NG"));
    console.log(N3 + ":" + (list[N3] ? "OK" : "NG"));
});

