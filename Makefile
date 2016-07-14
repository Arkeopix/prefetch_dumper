NAME	=	prefetchdump
SRC	=	main.c				\
		utils.c				\
		reader.c			\
		dump_info.c			\
		dump_file_metrics.c		\
		dump_trace_chains.c		\
		dump_filename_strings.c
OBJ	=	${SRC:.c=.o}
CFLAGS	=	-std=c99 -Wpedantic -Wall -Wextra \
		-Wformat-security -w -Werror -pipe \
		-Wunused
${NAME}:	${OBJ}
		${CC} ${OBJ} -g  ${CFLAGS} -o ${NAME}
all:		${NAME}
clean:
		${RM} ${OBJ}
fclean:		clean
		${RM} ${NAME}
re:		fclean all
