CC=gcc
CFLAGS= -std=gnu99 -g -Wl,--export-dynamic `pkg-config --cflags glib-2.0`
COMMON = -I../dsm_common -I../dsm_library
LIBS = -pthread -lm `pkg-config --libs glib-2.0`
CHECK=-Wall -Wextra

all: app3

app3: main.c ../dsm_library/dsm_lib.so
	$(CC) $(COMMON) $(CFLAGS) -o app3 main.c ../dsm_library/dsm_lib.so $(LIBS)

make clean:
	rm app3