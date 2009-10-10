LEVEL=../..

TOOLNAME = kensho

USEDLIBS = kensho parser

LINK_COMPONENTS := jit nativecodegen

include $(LEVEL)/Makefile.common

CXXFLAGS += -fexceptions
CXXFLAGS += -I/usr/local/include -I/usr/local/include/boost
LDFLAGS += -L/usr/local/lib -lantlr3c