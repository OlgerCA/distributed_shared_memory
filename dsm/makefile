CC=gcc
CFLAGS= -std=gnu99 -g -Wl,--export-dynamic `pkg-config --cflags glib-2.0`
COMMON = -I$(HOME)/SOA/distributed_shared_memory/dsm_common
LIBS = -pthread -lm `pkg-config --libs glib-2.0`
CHECK=-Wall -Wextra

all: dsm

dsm: ClientEntry.c NetworkUtils.c FileReader.c Logger.c main.c Server.c ServerForward.c ServerHandle.c ServerPageEntry.c
	$(CC) $(CFLAGS) -o dsm ClientEntry.c NetworkUtils.c FileReader.c Logger.c main.c Server.c ServerForward.c ServerHandle.c ServerPageEntry.c $(COMMON) $(LIBS)

clean:
	rm -rf *.o dsm