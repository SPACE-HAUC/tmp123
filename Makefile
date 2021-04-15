CC=gcc
RM=rm -vf

EDCFLAGS:= -Wall -std=gnu11 -O2 -I./ -I../ -DUNIT_TEST_TMP123 $(CFLAGS)
EDLDFLAGS:= -lpthread -lm $(LDFLAGS)

TARGET=tmptester.out
DRIVER_DIR=../
DEPS=tmp123.o \
	$(DRIVER_DIR)/spibus/spibus.o \
	$(DRIVER_DIR)/gpiodev/gpiodev.o

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