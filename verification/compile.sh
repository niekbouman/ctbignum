LLVM_SUFFIX="-mp-6.0"

echo 'Compiling...'
eval "clang++$LLVM_SUFFIX -std=c++17 -c -emit-llvm -o add.bc add.cpp" && \
echo 'done.' && \
echo && \
echo 'Mangled function name has become:' && \
eval "llvm-dis$LLVM_SUFFIX -o - add.bc | python3 extract_mangled_name.py add"


