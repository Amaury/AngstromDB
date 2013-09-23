# ###################################
# #            MAKEFILE             #
# ###################################

# Executable's name
NAME	=	angstrom
# Source files
SRC	=	main.c			\
		thread_writer.c		\
		thread_communication.c	\
		database.c		\
		command_put.c		\
		command_del.c		\
		command_get.c

# ###################################################################

# Paths to header files
IPATH	= -I. -I../include
# Path to libraries and lib's names
LDPATH	= -L. -L../lib -llmdb -lpthread -lnanomsg -lrt -Wl,-rpath -Wl,'$$ORIGIN/../lib'
# Compiler options
EXEOPT	= -O3 # -g for debug

# ###################################################################

CC	= gcc
OBJS	= $(SRC:.c=.o)

# Objects compilation options
CFLAGS	= -ansi -std=c99 -pedantic-errors -Wall -Wextra -Wmissing-prototypes \
	  -Wno-long-long -Wno-unused-parameter -Wno-unused-result -D_GNU_SOURCE -D_THREAD_SAFE \
	  $(IPATH) $(EXEOPT)

# Link options
LDFLAGS	= $(EXEOPT) $(LDPATH)

# ###################################################################

.PHONY: clean all

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)
	strip $(NAME)
	mv $(NAME) ../bin/

clean:
	rm -f $(NAME) $(OBJS) *~ ../bin/$(NAME)

all: clean $(NAME)

.c.o:
	$(CC) $(CFLAGS) -c $<

