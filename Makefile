CC=gcc
RM=rm -rvf

EDCFLAGS:= -Wall -std=gnu11 -O2 -I./ -I../ -DUNIT_TEST $(CFLAGS)
EDLDFLAGS:= -lpthread -lm $(LDFLAGS)

TARGET=tmptester.out
DEPS=tmp123.o \
../spibus/spibus.o \
../gpiodev/gpiodev.o

all: $(TARGET)
	sudo ./$(TARGET)

$(TARGET): $(DEPS)
	$(CC) $(EDCFLAGS) -o $(TARGET) $(DEPS) $(EDLDFLAGS)


%.o: %.c
	$(CC) $(EDCFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) $(TARGET)
	$(RM) $(DEPS)