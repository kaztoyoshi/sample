# 最大公約数

2つの自然数の最大公約数を求めるアルゴリズム

１．ユークリッドの互除法
　　a と b の最大公約数と b と b を a で割った余りの最大公約数が等しくなる
　　O(log min(a,b))


PHPはGMPをインストールするとgmp_gcd()が使える
Pythonはmathモジュールにgcd()が用意されている
JavaScriptはmath.jsをインストールするとcd()が使える