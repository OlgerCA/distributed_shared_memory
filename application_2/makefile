CC=gcc
CFLAGS= -std=gnu99 -g -Wl,--export-dynamic `pkg-config --cflags glib-2.0`
COMMON = -I$(HOME)/SOA/distributed_shared_memory/dsm_common -I$(HOME)/SOA/distributed_shared_memory/dsm_library
LIBS = -pthread -lm `pkg-config --libs glib-2.0`
CHECK=-Wall -Wextra

all: app2

app2: main.c ../dsm_library/dsm_lib.so
	$(CC) $(COMMON) $(CFLAGS) -o app2 main.c $(HOME)/SOA/distributed_shared_memory/dsm_library/dsm_lib.so $(LIBS)

make clean:
	rm app2