This source code was developed by referring to the github repository at the link below.
<br>
  -> https://github.com/sartura/ebpf-hello-world

## Dependency
Run `build_libbpf.sh` from the parent folder first

## TODO
In `test_main.c`, we load an automatically generated header file and use the structures, functions, etc. contained in it.

However, this is inefficient, so we need to think about whether there is another way to use the automatically generated structures, functions, etc.
