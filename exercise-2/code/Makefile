CC=gcc
CFLAGS= -Wall -pedantic -Wextra

# With the $ variables can be referenced.
# Some automatic variables are used:
# $@: means the target
# $^: means all prerequisites
# $<: means the first prerequisite

all: server client

server: chat_server.o
	@$(CC) $(CFLAGS) chat_server.o -o server

client: chat_client.o
	@$(CC) $(CFLAGS) chat_client.o -o client

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f *.o server client
