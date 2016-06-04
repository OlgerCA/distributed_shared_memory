CC=gcc
CFLAGS= -std=gnu99 -g -Wl,--export-dynamic `pkg-config --cflags glib-2.0`
COMMON = -I../dsm_common
LIBS = -pthread -lm `pkg-config --libs glib-2.0`
CHECK=-Wall -Wextra

all: dsm

dsm: ClientEntry.c FileReader.c Logger.c main.c Server.c ServerForward.c ServerHandle.c ServerPageEntry.c NetworkUtils.c
	$(CC) $(CFLAGS) -o dsm ClientEntry.c FileReader.c Logger.c main.c Server.c ServerForward.c ServerHandle.c ServerPageEntry.c $(COMMON) $(LIBS)
