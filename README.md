# Dynamic Loading (libdl) Experiments

In an executable, if we have some global functions/variables, 

1. Are those accessible in the shared-objects? here hello and world objects?
Yes, they are accessible within the SOs.
Tested with variables `base` and `bound` and `whatever` function.

1. Does the compiler optimize to use a register for frequently accessed global variables outside of the shared object with -fPIC?
Yes, it does use registers for the global variables outside of the shared-object and not do a PIC load on each access.
An inline function `check_bound()` accesses `base` and `incr_bound()` accesses the `bound` global variables from the executable and the assembler output shows that compiler optimizes the use of these global variables to access using `rbx` and `r14` registers.
Below is the assembly for that.
```
hello_main():
/home/xxxxx/research/me/misc/test/dynload_exp/hello.c:32
        }
}

int hello_main(void)
{
        printf("%p:%p %p:%u\n", &base, base, &bound, bound);
 800:   41 56                   push   %r14
 802:   53                      push   %rbx
 803:   50                      push   %rax
 804:   48 8b 1d ad 07 20 00    mov    0x2007ad(%rip),%rbx        # 200fb8 <base>
 80b:   48 8b 13                mov    (%rbx),%rdx
 80e:   4c 8b 35 db 07 20 00    mov    0x2007db(%rip),%r14        # 200ff0 <bound>
 815:   45 8b 06                mov    (%r14),%r8d
 818:   48 8d 3d c2 01 00 00    lea    0x1c2(%rip),%rdi        # 9e1 <_fini+0x9>
 81f:   31 c0                   xor    %eax,%eax
 821:   48 89 de                mov    %rbx,%rsi
 824:   4c 89 f1                mov    %r14,%rcx
 827:   e8 a4 fe ff ff          callq  6d0 <printf@plt>
/home/xxxxx/research/me/misc/test/dynload_exp/hello.c:34

        printf("Hello!\n");
 82c:   48 8d 3d cc 01 00 00    lea    0x1cc(%rip),%rdi        # 9ff <_fini+0x27>
 833:   e8 78 fe ff ff          callq  6b0 <puts@plt>
check_bound():
/home/xxxxx/research/me/misc/test/dynload_exp/hello.c:23
        if (base) { 
 838:   48 83 3b 00             cmpq   $0x0,(%rbx)
 83c:   74 1c                   je     85a <hello_main+0x5a>
/home/xxxxx/research/me/misc/test/dynload_exp/hello.c:24
                x++;
 83e:   8b 05 04 08 20 00       mov    0x200804(%rip),%eax        # 201048 <check_bound.x>
incr_bound():
/home/xxxxx/research/me/misc/test/dynload_exp/hello.c:15
        bound++;
 844:   41 83 06 01             addl   $0x1,(%r14)
check_bound():
/home/xxxxx/research/me/misc/test/dynload_exp/hello.c:24
                x++;
 848:   83 c0 01                add    $0x1,%eax
 84b:   89 05 f7 07 20 00       mov    %eax,0x2007f7(%rip)        # 201048 <check_bound.x>
/home/xxxxx/research/me/misc/test/dynload_exp/hello.c:26
                if (x % 2) more_mem();
 851:   a8 01                   test   $0x1,%al
 853:   74 05                   je     85a <hello_main+0x5a>
 855:   e8 66 fe ff ff          callq  6c0 <more_mem@plt>
hello_main():
/home/xxxxx/research/me/misc/test/dynload_exp/hello.c:36
        check_bound();
        printf("Hello!\n");
 85a:   48 8d 3d 9e 01 00 00    lea    0x19e(%rip),%rdi        # 9ff <_fini+0x27>
 861:   e8 4a fe ff ff          callq  6b0 <puts@plt>
check_bound():
/home/xxxxx/research/me/misc/test/dynload_exp/hello.c:23
        if (base) { 
 866:   48 83 3b 00             cmpq   $0x0,(%rbx)
 86a:   74 1c                   je     888 <hello_main+0x88>
/home/xxxxx/research/me/misc/test/dynload_exp/hello.c:24
                x++;
 86c:   8b 05 d6 07 20 00       mov    0x2007d6(%rip),%eax        # 201048 <check_bound.x>
incr_bound():
/home/xxxxx/research/me/misc/test/dynload_exp/hello.c:15
        bound++;
 872:   41 83 06 01             addl   $0x1,(%r14)
check_bound():
/home/xxxxx/research/me/misc/test/dynload_exp/hello.c:24
                x++;
 876:   83 c0 01                add    $0x1,%eax
 879:   89 05 c9 07 20 00       mov    %eax,0x2007c9(%rip)        # 201048 <check_bound.x>
/home/xxxxx/research/me/misc/test/dynload_exp/hello.c:26
                if (x % 2) more_mem();
 87f:   a8 01                   test   $0x1,%al

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


## Example output

```
./load hello.so world.so
```

produces
```
xxxxx@xxxxx:~/research/me/misc/test/dynload_exp$ ./load hello.so world.so
base: 0x938a20 bound: 16384
Hello 0x7fe2e7d535f0 0x7fe2e7d53620 0x7fe2e7d53650!
a's a is 1
a's b is 1
a's c is 1
Whatever:1
0x602088:0x938a20 0x602090:16384
Hello!
Hello!
base: 0x93ce40 bound: 24578
Hello!
Hello!
base: 0x93ce40 bound: 32772
Hello!
Hello!
base: 0x93ce40 bound: 40966
Hello!
0x602088:0x93ce40 0x602090:40967
Hello!
base: 0x93ce40 bound: 49160
Hello!
Hello!
base: 0x93ce40 bound: 57354
Hello!
Hello!
base: 0x93ce40 bound: 65548
Hello!
Hello!
base: 0x93ce40 bound: 73742
World 0x7fe2e794f5f0 0x7fe2e794f620 0x7fe2e794f650!
b's a is 2
b's b is 2
b's c is 2
Whatever:2
World!
World!
0x400940 0x4008e0 0x400930
x's a is 3
x's b is 3
x's c is 3
xxxxx@xxxxx:~/research/me/misc/test/dynload_exp$ 
```
