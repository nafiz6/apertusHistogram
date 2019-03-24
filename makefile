CC = gcc

CFLAGS = -g -Wall

TARGET = Histogram

all : $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)
