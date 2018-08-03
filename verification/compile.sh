LLVM_SUFFIX="-mp-6.0"

if [ "$#" -ne 2 ]; then
  echo "Use: $0 <filename (without extension)> <function name>"
  exit 1
fi

echo 'Compiling...'
eval "clang++$LLVM_SUFFIX -I../include -std=c++17 -c -emit-llvm -o $1.bc $1.cpp" && \
echo 'done.' && \
echo && \
echo 'Mangled function name has become:' && \
eval "llvm-dis$LLVM_SUFFIX -o - $1.bc | python3 extract_mangled_name.py $2" && \
exit 0


