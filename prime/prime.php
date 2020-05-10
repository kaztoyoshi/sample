<?php

$N1 = 13;
$N2 = 15;
$N3 = 17;

print("１．" . PHP_EOL);
function isPrime1($N) {
    for($i = 2; $i < $N; $i++) {
        if($N % $i == 0)
            return false;
    }
    return true;
}
print($N1 . ":" . (isPrime1($N1) ? "OK" : "NG") . PHP_EOL);
print($N2 . ":" . (isPrime1($N2) ? "OK" : "NG") . PHP_EOL);
print($N3 . ":" . (isPrime1($N3) ? "OK" : "NG") . PHP_EOL);

print("２．" . PHP_EOL);
function isPrime2($N) {
    for($i = 2; $i * $i <= $N; $i++) {
        if($N % $i == 0)
            return false;
    }
    return true;
}
print($N1 . ":" . (isPrime2($N1) ? "OK" : "NG") . PHP_EOL);
print($N2 . ":" . (isPrime2($N2) ? "OK" : "NG") . PHP_EOL);
print($N3 . ":" . (isPrime2($N3) ? "OK" : "NG") . PHP_EOL);

print("３．" . PHP_EOL);
$N = 100;
$list = array();
// 0,1は素数ではないので外す
$list[0] = false;
$list[1] = false;
// 探索リストに 2 から N まで true を入れる
for($i = 2; $i <= $N; $i++) {
    $list[$i] = true;
}
for($i = 2; $i * $i <= $N; $i++) {
    // 素数の場合
    if($list[$i]) {
        // 先頭の数の倍数をリストから篩い落とす
        for($j = $i * 2; $j <= $N; $j += $i) {
            $list[$j] = false;
        }
    }
}
print($N1 . ":" . ($list[$N1] ? "OK" : "NG") . PHP_EOL);
print($N2 . ":" . ($list[$N2] ? "OK" : "NG") . PHP_EOL);
print($N3 . ":" . ($list[$N3] ? "OK" : "NG") . PHP_EOL);

print("３'．" . PHP_EOL);
$N = 100;
$list = array();
// 探索リストに 2 から N までの整数を昇順で入れる。
for($i = 2; $i <= $N; $i++) {
    $list[$i] = $i;
}
for($i = 2; $i * $i <= $N; $i++) {
    if(isset($list[$i])) {
        // 先頭の数の倍数をリストから篩い落とす
        for($j = $i * 2; $j <= $N; $j += $i) {
            unset($list[$j]);
        }
    }
}
print($N1 . ":" . (isset($list[$N1]) ? "OK" : "NG") . PHP_EOL);
print($N2 . ":" . (isset($list[$N2]) ? "OK" : "NG") . PHP_EOL);
print($N3 . ":" . (isset($list[$N3]) ? "OK" : "NG") . PHP_EOL);

?>