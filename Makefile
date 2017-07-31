CC=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=-lcurl -pthread -lz
SOURCES=main.cpp CurlWrapper/CurlWrapper.cpp ZlibWrapper/ZlibWrapper.cpp ZlibWrapper/junzip/junzip.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=CurlTest

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	-find . -name '*.o' -exec rm -r {} \;
	-rm -f $(EXECUTABLE)

test:
	$ ./$(EXECUTABLE)
