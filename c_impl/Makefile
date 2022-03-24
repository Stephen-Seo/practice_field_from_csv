COMMON_FLAGS = -Wall -Wextra -Wpedantic
ifdef DEBUG
	CFLAGS = ${COMMON_FLAGS} -O0 -g
else
	CFLAGS = ${COMMON_FLAGS} -O3 -DNDEBUG
endif

SOURCES = \
	src/main.o \
	src/argparse.o \
	src/csventry.o \
	src/strdlist.o

all: fields_from_csv

fields_from_csv: ${SOURCES}
	${CC} ${CFLAGS} -o fields_from_csv $^

.PHONY: clean

clean:
	rm -f src/*.o
	rm -f fields_from_csv
