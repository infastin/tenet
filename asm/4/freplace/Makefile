CC = cl
AC = ml
LINK = link
RM = del

LIBS = legacy_stdio_definitions.lib libcmt.lib

SRCS = freplace.asm
OBJS = $(SRCS:.asm=.obj)
OUT = main.exe

all: $(OUT)

$(OBJS): $(SRCS)
	$(AC) /c $(SRCS) /Fo $(OBJS)

main.obj: main.c
	$(CC) /c main.c /Fo main.obj

$(OUT): $(OBJS) main.obj
	$(LINK) $(OBJS) main.obj /out:$(OUT) $(LIBS)

clean: $(OBJS)
	$(RM) *.obj
	
.PHONY: all clean
