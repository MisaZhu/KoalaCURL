ifneq ($(CROSS_COMPILE),)
$(info CROSS_COMPILE=$(CROSS_COMPILE))
endif

CC := $(CROSS_COMPILE)gcc
CXX := $(CROSS_COMPILE)g++
AR := $(CROSS_COMPILE)ar
LD := $(CROSS_COMPILE)g++

KOALA = ../KoalaJS/build
CFLAGS = -Wall -fPIC -g -I./ -I$(KOALA)/include

NATIVE_DIR = native

NATIVE = $(NATIVE_DIR)/ClassLoader.cpp \
	  $(NATIVE_DIR)/CURL/JSCURL.cpp

LDFLAGS = -L$(KOALA) -lcurl -lKoalaJS

TARGET = build

all:
	mkdir -p $(TARGET)
	$(LD) $(CFLAGS) -shared -o $(TARGET)/KoalaCURL.so $(NATIVE) $(LDFLAGS)
	rm -fr *.dSYM

clean:
	rm -fr $(TARGET)
	rm -fr *.o *.dSYM *.bcode

install:
	mkdir -p /usr/local/koala/modules
	cp build/*.so /usr/local/koala/modules
	cp -r classes /usr/local/koala
