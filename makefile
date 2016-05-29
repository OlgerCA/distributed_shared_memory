CC=gcc
CFLAGS= -std=gnu99 -g -Wl,--export-dynamic
COMMON = -Icommon
LIBS = -pthread -lm
CHECK=-Wall -Wextra

all: dsm dsm_lib app1 app2 app3

dsm:
	(cd dsm; make)

dsm_lib:
	(cd dsm_library; make)

app1:
	(cd application_1; make)

app2:
	(cd application_2; make)

app3:
	(cd application_3; make)