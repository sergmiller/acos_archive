CC     = gcc
CFLAGS = -ansi -pedantic -Wall -g

BINARIES= editor

OBJECTS = main.o                   \
		  cartesian_tree.o         \
		  string_for_text_editor.o \
		  filework.o               \
		  parser.o                 \
		  editor.o                 \
		  insert_functions.o       \
		  replace_function.o       \
		  my_print_pages.o


all: $(BINARIES)


editor: $(OBJECTS)
	$(CC) -o $@ $^


%.o : %.c %.h
	$(CC) $(CFLAGS) -c $<


clean:
	rm -f $(OBJECTS) $(BINARIES)


