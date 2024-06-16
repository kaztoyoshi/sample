<?php

// X^N
$X = 3;
$N = 50;

print("1." . PHP_EOL);
function pow1($x, $n) {
    $ans = 1;
    while( $n > 0) {
        if($n & 1) {
            $ans *= $x;
        }
        $x *= $x;
        $n >>= 1;
    }
    return $ans;
}
print(pow1($X, $N) . PHP_EOL);

print("1'." . PHP_EOL);
print(pow($X, $N) . PHP_EOL);

?>