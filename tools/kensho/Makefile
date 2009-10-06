LEVEL=../..

TOOLNAME = kensho

USEDLIBS = kensho

LINK_COMPONENTS := support system core engine

include $(LEVEL)/Makefile.common

CXXFLAGS += -fexceptions
CXXFLAGS += -I/usr/local/include -I/usr/local/include/boost
LDFLAGS += -L/usr/local/lib -lantlr3c