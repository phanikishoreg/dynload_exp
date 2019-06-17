# Dynamic Loading (libdl) Experiments

In an executable, if we have some global functions/variables, 

1. Are those accessible in the shared-objects? here hello and world objects?
Yes, they are accessible within the SOs.
Tested with variables `base` and `bound` and `whatever` function.

1. Does the compiler optimize to use a register for frequently accessed global variables outside of the shared object with -fPIC?
Yes, it does use a cached register and not do a PIC load on each access.
There is an "static inline" function `check_bound()` which accesses both `base` and `bound` and increments a `static variable` and `bound`.
Below is the assembly for that.
You can see how multiple calls to `check_bound()` uses the cached `rbx` and `r14` to access the globals that are defined outside of the shared-object and in the executable (load is the name of the executable).
```
00000000000007b0 <hello_main>:
hello_main():
.../test/dynload_exp/hello.c:21
        if (base) { x++; bound++; }
}

int hello_main(void)
{
        printf("%p:%p %p:%u\n", &base, base, &bound, bound);
 7b0:   41 56                   push   %r14
 7b2:   53                      push   %rbx
 7b3:   50                      push   %rax
 7b4:   48 8b 1d fd 07 20 00    mov    0x2007fd(%rip),%rbx        # 200fb8 <base>
 7bb:   48 8b 13                mov    (%rbx),%rdx
 7be:   4c 8b 35 2b 08 20 00    mov    0x20082b(%rip),%r14        # 200ff0 <bound>
 7c5:   45 8b 06                mov    (%r14),%r8d
 7c8:   48 8d 3d 52 01 00 00    lea    0x152(%rip),%rdi        # 921 <_fini+0x9>
 7cf:   31 c0                   xor    %eax,%eax
 7d1:   48 89 de                mov    %rbx,%rsi
 7d4:   4c 89 f1                mov    %r14,%rcx
 7d7:   e8 a4 fe ff ff          callq  680 <printf@plt>
.../test/dynload_exp/hello.c:23

        printf("Hello!\n");
 7dc:   48 8d 3d 5c 01 00 00    lea    0x15c(%rip),%rdi        # 93f <_fini+0x27>
 7e3:   e8 88 fe ff ff          callq  670 <puts@plt>
check_bound():
.../test/dynload_exp/hello.c:16
        if (base) { x++; bound++; }
 7e8:   48 83 3b 00             cmpq   $0x0,(%rbx)
 7ec:   74 0b                   je     7f9 <hello_main+0x49>
 7ee:   83 05 4b 08 20 00 01    addl   $0x1,0x20084b(%rip)        # 201040 <check_bound.x>
 7f5:   41 83 06 01             addl   $0x1,(%r14)
hello_main():
.../test/dynload_exp/hello.c:25
        check_bound();
        printf("Hello!\n");
 7f9:   48 8d 3d 3f 01 00 00    lea    0x13f(%rip),%rdi        # 93f <_fini+0x27>
 800:   e8 6b fe ff ff          callq  670 <puts@plt>
check_bound():
.../test/dynload_exp/hello.c:16
        if (base) { x++; bound++; }
 805:   48 83 3b 00             cmpq   $0x0,(%rbx)
 809:   74 0b                   je     816 <hello_main+0x66>
 80b:   83 05 2e 08 20 00 01    addl   $0x1,0x20082e(%rip)        # 201040 <check_bound.x>
 812:   41 83 06 01             addl   $0x1,(%r14)
hello_main():
.../test/dynload_exp/hello.c:27
        check_bound();
        printf("Hello!\n");
 816:   48 8d 3d 22 01 00 00    lea    0x122(%rip),%rdi        # 93f <_fini+0x27>
 81d:   e8 4e fe ff ff          callq  670 <puts@plt>
check_bound():
.../test/dynload_exp/hello.c:16
        if (base) { x++; bound++; }
 822:   48 83 3b 00             cmpq   $0x0,(%rbx)
 826:   74 0b                   je     833 <hello_main+0x83>
 828:   83 05 11 08 20 00 01    addl   $0x1,0x200811(%rip)        # 201040 <check_bound.x>
 82f:   41 83 06 01             addl   $0x1,(%r14)
hello_main():
.../test/dynload_exp/hello.c:29
        check_bound();
        printf("Hello!\n");
 833:   48 8d 3d 05 01 00 00    lea    0x105(%rip),%rdi        # 93f <_fini+0x27>

```

## COMPILE

```
make clean all
```

## RUN

```
. libenv.sh

./load hello.so world.so

#or

./load hello.so
```
load application loads hello.so and world.so shared objects and their dependencies.
