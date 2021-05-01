all: tinycpdump
# apt-get install musl-tools
CC := musl-gcc
CFLAGS += -Wall -Os -static -s
