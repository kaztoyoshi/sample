<?php

printf("要素数から考える" . PHP_EOL);
$n = 5;          // 要素数
$size = (1<<$n);
for($bits = 0; $bits < $size; $bits++) {
    $out = "{";
    for($i = 0; $i < $n; $i++) {
        if($bits & (1 << $i)) {
            $out .= $i . ",";
        }
    }
    $out .= "}";
    printf($out . PHP_EOL);
}
printf(PHP_EOL);

printf("配列から考える" . PHP_EOL);
$a = array(5, 8, 2, 7, 4);
$size = (1<<count($a));
for($bits = 0; $bits < $size; $bits++) {
    $out = "{";
    for($i = 0; $i < $n; $i++) {
        if($bits & (1 << $i)) {
            $out .= $a[$i] . ",";
        }
    }
    $out .= "}";
    printf($out . PHP_EOL);
}

?>

