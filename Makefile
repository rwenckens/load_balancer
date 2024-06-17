CC = g++
CFLAGS = -Wall -Werror -std=c++11
TARGET = loadbalancer
OBJS = main.o
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp
clean:
	rm -f $(TARGET) $(OBJS)
.PHONY: all clean
