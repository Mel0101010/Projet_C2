CC = gcc
CFLAGS = -g
LDFLAGS = -lssl -lcrypto

# Source files
SRCS = main.c declare.c connexion.c choice.c fetch.c base64.c tasks.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = bot_client

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)