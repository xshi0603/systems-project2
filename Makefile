CC:=gcc
CFLAGS:=-Wall

DEBUG:=true

# make DEBUG=false omits debug info
ifeq ($(DEBUG),true)
	CFLAGS:=$(CFLAGS) -g
endif

# Change as needed
NAME:=main
HEADERS:=
SOURCES:=main.c
OBJECTS:=$(SOURCES:.c=.o)

MKF_PATH:=$(abspath $(lastword $(MAKEFILE_LIST)))
MKF_DIR:=$(patsubst %/,%,$(dir $(MKF_PATH)))

# Path to output executable/library
OUTPUT:=$(NAME)

ifeq ($(OS),Windows_NT)
	OUTPUT:=$(OUTPUT).exe
endif

.PHONY: all print-%

all: $(OUTPUT)

# useful for debugging
# usage: print-VARNAME
print-%:
	@echo $*=$($*)

$(O_PATHS): %.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OUTPUT): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)
