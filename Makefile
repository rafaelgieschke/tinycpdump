all: vdedump
CFLAGS += -Wall -Os -static -s

# apt-get install libvdeplug-dev
LDLIBS += $(shell pkg-config --libs vdeplug)
