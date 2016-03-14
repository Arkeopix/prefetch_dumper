NAME	=	prefetchdump
SRC		=	main.c				\
			utils.c				\
			dump_header.c		\
			dump_info.c			\
			dump_file_metrics.c
OBJ		=	${SRC:.c=.o}
CFLAGS	=	-std=c99 -Wpedantic -Wall -Wextra \
			-Wformat-security -w -Werror -pipe \
			-Wunused
${NAME}:	${OBJ}
			${CC} -g ${OBJ} ${CFLAGS} -o ${NAME}
all:		${NAME}
clean:
			${RM} ${OBJ}
fclean:		clean
			${RM} ${NAME}
re:			fclean all
