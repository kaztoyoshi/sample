<?php

$N = 36;    // 整数 N

printf("1." . PHP_EOL);
$out = "{";
for($i = 1; $i <= $N; $i++) {
    if($N % $i == 0) {
        $out .= $i . ",";
    }
}
$out .= "}";
printf($out . PHP_EOL);

printf("2." . PHP_EOL);
$a = array();
for($i = 1; $i * $i <= $N; $i++) {
    if($N % $i == 0) {
        array_push($a, $i);
        if($i * $i != $N) {
            array_push($a, $N / $i);
        }
    }
}
sort($a);
$out = "{";
foreach ($a as $v) {
    $out .= $v . ",";
}
$out .= "}";
printf($out . PHP_EOL);

printf("2'." . PHP_EOL);
$asc = array();     // 昇順
$desc  = array();   // 降順
for($i = 1; $i * $i <= $N; $i++) {
    if($N % $i == 0) {
        array_push($asc, $i);
        if($i * $i != $N) {
            array_unshift($desc, $N / $i);
        }
    }
}
$out = "{";
foreach ($asc as $v) {
    $out .= $v . ",";
}
foreach ($desc as $v) {
    $out .= $v . ",";
}
$out .= "}";
printf($out . PHP_EOL);

?>