CC = arm-linux-gnueabihf-gcc
CFLAGS = -Wall -I/usr/local/arm-libmodbus/include/modbus
LDFLAGS = -L/usr/local/arm-libmodbus/lib -lmodbus

all: master slave

master: master.o
	$(CC) -o master master.o $(LDFLAGS)

slave: slave.o
	$(CC) -o slave slave.o $(LDFLAGS)

master.o: master.c config.h
	$(CC) $(CFLAGS) -c master.c -o master.o

slave.o: slave.c config.h
	$(CC) $(CFLAGS) -c slave.c -o slave.o

clean:
	rm -f master slave *.o