#include "vmlinux.h"
#include <bpf/bpf_helpers.h>

SEC("tracepoint/syscalls/sys_exit_execve")
int tracepoint__syscalls__sys_exit_execve(struct trace_event_raw_sys_enter *ctx)
{
        bpf_printk("[Info] execve() syscall Exit!\n");
        return 0;
}

char LICENSE[] SEC("license") = "GPL";
