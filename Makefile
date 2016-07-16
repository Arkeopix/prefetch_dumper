NAME	=	prefetchdump

SRC	=	main.c				\
		reader.c			
OBJ	=	${SRC:.c=.o}

CFLAGS	+=	-std=c99 -Wpedantic -Wall -Wextra -g

${NAME}:	${OBJ}
		${CC} ${OBJ} -o ${NAME}

all:		${NAME}

clean:
		${RM} ${OBJ}

fclean:		clean
		${RM} ${NAME}

re:		fclean all
