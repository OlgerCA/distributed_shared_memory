CC=gcc
CFLAGS= -std=gnu99 -g -Wl,--export-dynamic
COMMON = -I../dsm_common
LIBS = -lm -pthread
CHECK=-Wall -Wextra

all:dsm_lib.so

dsm_lib.so: ClientHandle.c ClientPageEntry.h ClientRequest.c DSMClient.c Globals.h
	$(CC) $(COMMON) -shared -fpic ClientHandle.c ClientPageEntry.h ClientRequest.c DSMClient.c Globals.h -o dsm_lib.so

clean:
	rm dsm_lib.so