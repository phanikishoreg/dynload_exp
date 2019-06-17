CC=clang
RM=rm -f

all:clean a b x hello world load

clean:
	$(RM) *.so *.so.* load

a: a.h a.c
	$(CC) a.c -I. --shared -g -fPIC -o liba.so

b: a.h b.c
	$(CC) b.c -I. --shared -g -fPIC -o libb.so

x: a.h x.c
	$(CC) x.c -I. --shared -g -fPIC -o libx.so

hello: a.h hello.c
	$(CC) hello.c -O3 --shared -fPIC -I. -g -o hello.so -L. -la

world: a.h world.c
	$(CC) world.c -O3 --shared -fPIC -I. -g -o world.so -L. -lb

load: a.h
	$(CC) load.c -I. -L. -lx -ldl -Wl,--export-dynamic -g -o load
