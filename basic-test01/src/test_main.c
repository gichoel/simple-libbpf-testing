#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/resource.h>

#include <bpf/libbpf.h>
#include <bpf/bpf.h>

/* These header file are automatically generated */
#include "syscall_enter.h"
#include "syscall_exit.h"

void read_trace_pipe(void)
{
	int trace_fd;

	trace_fd = open("/sys/kernel/debug/tracing/trace_pipe", O_RDONLY, 0);
	if (trace_fd < 0)
		return;

	while (1) {
		static char buf[4096];
		ssize_t sz;

		sz = read(trace_fd, buf, sizeof(buf) - 1);
		if (sz > 0) {
			buf[sz] = 0;
			puts(buf);
		}
	}
}

int main(void)
{
	/* These structures are automatically generated */
	struct syscall_enter *enter_obj;
	struct syscall_exit *exit_obj;

	int err = 0;

	struct rlimit rlim = {
		.rlim_cur = 512UL << 20,
		.rlim_max = 512UL << 20,
	};

	err = setrlimit(RLIMIT_MEMLOCK, &rlim);
	if (err) {
		fprintf(stderr, "failed to change rlimit\n");
		return 1;
	}


	enter_obj = syscall_enter__open();
	if (!enter_obj) {
		fprintf(stderr, "failed to open and/or load BPF object\n");
		return 1;
	}

	exit_obj = syscall_exit__open();
	if (!exit_obj) {
		fprintf(stderr, "failed to open and/or load BPF object\n");
		return 1;
	}


	err = syscall_enter__load(enter_obj);
	if (err) {
		fprintf(stderr, "failed to load BPF object %d\n", err);
		goto cleanup;
	}
	err = syscall_exit__load(exit_obj);
	if (err) {
		fprintf(stderr, "failed to load BPF object %d\n", err);
		goto cleanup;
	}


	err = syscall_enter__attach(enter_obj);
	if (err) {
		fprintf(stderr, "failed to attach BPF programs\n");
		goto cleanup;
	}
	err = syscall_exit__attach(exit_obj);
	if (err) {
		fprintf(stderr, "failed to attach BPF programs\n");
		goto cleanup;
	}

	read_trace_pipe();

cleanup:
	syscall_enter__destroy(enter_obj);
	syscall_exit__destroy(exit_obj);
	return err != 0;
}
