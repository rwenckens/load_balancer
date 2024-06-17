CC = g++
CFLAGS = -Wall -Werror -std=c++11

TARGET = loadbalancer

OBJS = main.o Loadbalancer.o Webserver.o RequestQueue.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp Loadbalancer.h Webserver.h RequestQueue.h Request.h
	$(CC) $(CFLAGS) -c main.cpp

Loadbalancer.o: Loadbalancer.cpp Loadbalancer.h Webserver.h RequestQueue.h Request.h
	$(CC) $(CFLAGS) -c Loadbalancer.cpp

Webserver.o: Webserver.cpp Webserver.h Request.h
	$(CC) $(CFLAGS) -c Webserver.cpp

RequestQueue.o: RequestQueue.cpp RequestQueue.h Request.h
	$(CC) $(CFLAGS) -c RequestQueue.cpp

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean

