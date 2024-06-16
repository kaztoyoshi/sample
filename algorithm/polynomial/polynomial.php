<?php

// f(2)=x^5-12*x^4+3*x^3-8*x^2+x-5
$X = 2;
// 次数が小さい順に係数を並べる
$A = array(-5, 1, -8, 3, -12, 1);
$N = count($A);

print("1." . PHP_EOL);
function polynomial1($x, $a, $n) {
    $ans = 0;
    for($i = 0; $i < $n; $i++) {
        $ans += $a[$i] * pow($x, $i);
    }
    return $ans;
}
print(polynomial1($X, $A, $N) . PHP_EOL);

print("2." . PHP_EOL);
function polynomial2($x, $a, $n) {
    $ans = 0;
    for($i = $n-1; $i >= 0; $i--) {
        $ans = $ans * $x + $a[$i];
    }
    return $ans;
}
print(polynomial2($X, $A, $N) . PHP_EOL);

?>