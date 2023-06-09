CC = gcc
CFLAGS  = -g -Wall

all: hinfosvc

$(TARGET): hinfosvc.c
	$(CC) $(CFLAGS) -o hinfosvc hinfosvc.c

clean:
	$(RM) $(TARGET)
