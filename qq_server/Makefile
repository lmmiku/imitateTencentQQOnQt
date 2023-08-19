CC := g++
CFLAGS := -std=c++11 -Wall -Wextra
LDFLAGS := -lmysqlcppconn -pthread

SRC := qq_server.cpp server_udp.cpp server_tcp.cpp server_mysql.cpp Logger.cpp
OBJ := $(SRC:.cpp=.o)
EXECUTABLE := qq_server

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXECUTABLE)
