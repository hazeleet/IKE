SRCDIR = src
HDRDIR = include
OBJDIR = .build

PROGRAM = $(OBJDIR)/IKE

SRCS = $(wildcard $(SRCDIR)/*.c)
HDRS = $(wildcard $(SRCDIR)/*.h)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

CFLAGS = -Wall -Wextra -lconfig -largp

ifeq ($(shell id -u), 0)
all: clean prepare run
else
all: clean prepare bear
CFLAGS += -I/opt/homebrew/include -L/opt/homebrew/lib -Wno-unused-command-line-argument
endif

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	gcc -c $< $(CFLAGS) -I$(HDRDIR) -o $@

$(PROGRAM): $(OBJS)
	gcc $^ $(CFLAGS) -o $@

run: $(PROGRAM)
	$(PROGRAM)

bear:
	bear -- make $(PROGRAM)

prepare:
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)/*
