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
    let n = 2600;          // ある自然数
    console.log("１.");
    let result = new Array();
    let init = 2;
    if(n == 1) {
        result.push(1);
    }
    while(n != 1) {
        let i = init;
        while(i < 0xFFFFFF) {
            if(n % i == 0) {
                result.push(i);
                n /= i;
                break;
            }
            i++;
        }
        init = i;
    }
    result.forEach(e => {
        process.stdout.write(String(e) + " ");
    });
    console.log();

    n = 2600;          // ある自然数
    console.log("２.");
    // 素数テーブル作成
    let list = new Array(n+1);
    // 探索リストに 0 から n までの数値を入れる
    for(let i = 0; i <= n; i++) {
        list[i] = i;
    }
    // 偶数の最小素因数は2
    for(let i = 2; i <= n; i+=2) {
        list[i] = 2;
    }
    // 3 以上の奇数は最小素因数を記録
    for(let i = 3; i * i <= n; i+=2) {
        // 奇数 i が i 未満の数の倍数であれば無視
        if(list[i] < i) continue;
        // 最小素因数を記録
        for(let j = i; j <= n; j+=i) {
            list[j] = i;
        }
    }
    result = new Array();
    while(list[n] != 1) {
        result.unshift(list[n]);
        n /= list[n];
    }
    result.forEach(e => {
        process.stdout.write(String(e) + " ");
    });
    console.log();
});
