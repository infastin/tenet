CC = cl
AC = ml
LINK = link
RM = del

LIBS = legacy_stdio_definitions.lib libcmt.lib

SRCS = fcopy.asm main.asm
OBJS = $(SRCS:.asm=.obj)
OUT = main.exe

all: $(OUT)

$(OBJS): $(SRCS)
	$(AC) /c $(SRCS) /Fo $(OBJS)

$(OUT): $(OBJS)
	$(LINK) $(OBJS) /out:$(OUT) $(LIBS)

clean: $(OBJS)
	$(RM) *.obj
	
.PHONY: all clean
