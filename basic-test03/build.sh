# Build eBPF Code for Tracing
mkdir build > /dev/null 2>&1

clang -g -O2 -target bpf -D__TARGET_ARCH_x86_64 -I ./src/bpf -c ./src/bpf/syscall_enter.c -o ./build/syscall_enter.o

bpftool gen object ./build/minimal.skel.o ./build/syscall_enter.o
bpftool gen skeleton ./build/minimal.skel.o name minimal_bpf > ./src/minimal.skel.h

# Build Test Code in User-Level
clang -g -O2 -Wall -I ./src/ -c ./src/test_main.c -o ./build/test_main.o
clang -Wall -O2 -g ./build/test_main.o ../libbpf_build/libbpf.a -lelf -lz -o test_main