<?php

// 最大公約数を求めるための2つの自然数
$A = 48;
$B = 32;

print("１．" . PHP_EOL);
function gcd1($a, $b) {
    if($b === 0) {
        return $a;
    } else {
        return gcd1($b, $a % $b);
    }
}
print(gcd1($A, $B) . PHP_EOL);

?>