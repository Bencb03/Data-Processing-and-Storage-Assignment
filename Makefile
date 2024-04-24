CC=g++

CFLAGS=-Wall -std=c++11

SOURCE=database.cpp

EXECUTABLE=database

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCE)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SOURCE)
clean:
	rm -f $(EXECUTABLE)
run: all
	./$(EXECUTABLE)
