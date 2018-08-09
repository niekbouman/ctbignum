### Constant-Time verification with `ct-verif`

To check e.g. comparison, invoke as
```
ct-verif.rb --clang-options "-x c++ -std=c++14 -O3 -I <include path for ctbignum> -Wno-c++1z-extensions" -e _Z15generic_wrapperI10LessEqThanbLm4EEvPT0_PmS3_ wrapper.cpp
```

The correct mangled function names can be found by:
```
clang++ -c -emit-llvm -std=c++14 -I <include paths for ctverif, smack and ctbignum> wrapper.cpp -o - | llvm-dis -o - | grep wrapper
```

### Obtaining and Running ct-verif
A Dockerfile + build instructions can be found [here](https://github.com/niekbouman/verifying-constant-time/tree/cplusplus)
