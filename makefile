tinyshell: shell.c
	gcc -std=c99 -Wall -Wextra -Werror -o tinysh shell.c

mallex: malloc.c
	gcc -std=c99 -Wall -Wextra -Werror -o mallex malloc.c
