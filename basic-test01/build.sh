# Create BTF(BPF Type Format) File for Current Kernel
bpftool btf dump file /sys/kernel/btf/vmlinux format c > ./src/bpf/vmlinux.h

# Build eBPF Code for Tracing
mkdir build > /dev/null 2>&1

clang -g -O2 -target bpf -D__TARGET_ARCH_x86_64 -I ./src/bpf -c ./src/bpf/syscall_enter.c -o ./build/syscall_enter.o
bpftool gen skeleton ./build/syscall_enter.o > ./src/syscall_enter.h

clang -g -O2 -target bpf -D__TARGET_ARCH_x86_64 -I ./src/bpf -c ./src/bpf/syscall_exit.c -o ./build/syscall_exit.o
bpftool gen skeleton ./build/syscall_exit.o > ./src/syscall_exit.h

# Build Test Code in User-Level
clang -g -O2 -Wall -I ./src/ -c ./src/test_main.c -o ./build/test_main.o
clang -Wall -O2 -g ./build/test_main.o ../libbpf_build/libbpf.a -lelf -lz -o test_main
