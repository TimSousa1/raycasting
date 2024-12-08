CC=gcc
FLAGS=-Wall -Wextra

ifdef DEBUG
	FLAGS+=-ggdb
else
	FLAGS+=-O3
endif


ifdef GAMING_MODE
	FLAGS+=-lraylib
	FLAGS+=-D GAMING_MODE
endif

LDLIBS=-lm


SRCDIR := src
SRC := $(shell find $(SRCDIR) -name "*.c")

BUILDDIR := build
_OBJ := $(SRC:%.c=%.o)
OBJ := $(subst $(SRCDIR),$(BUILDDIR),$(_OBJ))

INCDIR := include
INC := $(shell find $(INCDIR) -name "*.h")

OUTNAME := ray

$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(INC) | $(BUILDDIR)
	$(CC) -o $@ -c $< $(FLAGS) -I $(INCDIR) $(LDLIBS)


$(OUTNAME): $(OBJ)
	$(CC) -o $(BUILDDIR)/$@ $^ -I $(INCDIR) $(LDLIBS) $(FLAGS)


$(BUILDDIR):
	mkdir $@


.PHONY: clean

clean:
	rm -r $(BUILDDIR)
