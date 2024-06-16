<?php

$n = 2600;          // ある自然数
print("１．" . PHP_EOL);
$result = array();
$init = 2;
if($n === 1) {
    $result[] = 1;
}
while($n !== 1) {
    $i = $init;
    while($i < 0xFFFFFF) {
        if($n % $i == 0) {
            $result[] = $i;
            $n /= $i;
            break;
        }
        $i++;
    }
    $init = $i;
}
foreach($result as $val) {
    print($val . " ");
}
print(PHP_EOL);

$n = 2600;          // ある自然数
print("２．" . PHP_EOL);
// 素数テーブル作成
$list = array();
// 探索リストに 0 から n までの数値を入れる
for($i = 0; $i <= $n; $i++) {
    $list[$i] = $i;
}
// 偶数の最小素因数は2
for($i = 2; $i <= $n; $i+=2) {
    $list[$i] = 2;
}
// 3 以上の奇数は最小素因数を記録
for($i = 3; $i * $i <= $n; $i+=2) {
    // 奇数 i が i 未満の数の倍数であれば無視
    if($list[$i] < $i) continue;
    // 最小素因数を記録
    for($j = $i; $j <= $n; $j += $i) {
        $list[$j] = $i;
    }
}
$result = array();
while($list[$n] !== 1) {
    array_unshift($result, $list[$n]);
    $n /= $list[$n];
}
foreach($result as $val) {
    print($val . " ");
}
print(PHP_EOL);

?>