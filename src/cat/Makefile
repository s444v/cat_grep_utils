NAME		= s21_cat

SRCS		= cat.c \
		  cat_functions.c \


OBJS		= $(SRCS:.c=.o)


CC		= gcc
RM		= rm -rf
CFLAGS		= -Wall -Werror -Wextra
# INCLUDES	= 

all:
	@$(MAKE) $(NAME)

%.o:    %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME):	$(OBJS)
	@$(CC) $(OBJS) -o $(NAME)
	@echo s21_cat compiled!


clean:
	@$(RM) $(OBJS)
	@echo clean .o files

fclean: clean
	@$(RM) $(NAME)
	@echo removed!

re:	fclean $(NAME)