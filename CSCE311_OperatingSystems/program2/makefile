CC = g++
CFLAGS = -Wall -pthread
TARGET = mapper

all: $(TARGET)

$(TARGET): map_main.cpp
	$(CC) $(CFLAGS) -o $(TARGET) map_main.cpp

clean:
	rm -f $(TARGET)
