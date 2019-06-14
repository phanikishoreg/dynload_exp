all:clean a b x hello world load

clean:
	rm -f *.so *.so.* load

a: a.h a.c
	gcc a.c -I. --shared -g -fPIC -o liba.so

b: a.h b.c
	gcc b.c -I. --shared -g -fPIC -o libb.so

x: a.h x.c
	gcc x.c -I. --shared -g -fPIC -o libx.so

hello: a.h hello.c
	gcc hello.c -I. --shared -g -fPIC -o hello.so -L. -la

world: a.h world.c
	gcc world.c -I. --shared -g -fPIC -o world.so -L. -lb

load: a.h
	gcc load.c -I. -L. -lx -ldl -Wl,--export-dynamic -g -o load
