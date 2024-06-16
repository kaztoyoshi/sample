# 最小公倍数

2つの自然数の最小公倍数を求めるアルゴリズム

１．２つの自然数の積はその最小公倍数と最大公約数の積に等しくなる
　　a * b = lcm * gcd


PHPはGMPをインストールするとgmp_lcm()が使える
Pythonのmathモジュールにlcm()は用意されていない
JavaScriptはmath.jsをインストールするとlcm()が使える