.PHONY: all clean

CFLGAS+=
LDFLAGS+=-lm     # link to math library

TARGET=train 
TARGET2=test
TARGET3=accuracy

all: $(TARGET) $(TARGET2) $(TARGET3)
# type make/make all to compile test_hmm
clean:
	$(RM) $(TARGET) $(TARGET2) $(TARGET3) # type make clean to remove the compiled file