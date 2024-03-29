NAME     = ft_ping
RM       = rm -rf
CC       = gcc
CFLAGS   = -Wall -Wextra -Werror
CPPFLAGS = -Ilibft/include
LDFLAGS  = -Llibft
LDLIBS   = -lft
INC_NAME =
SRC_DIR  = src
SRC_NAME = main.c\
			build_addrinfo.c\
			setup_socket.c\
			ping_loop.c\
			ping_sleep.c\
			build_icmp.c\
			print_packet.c\
			in_cksum.c\
			tv_diff_in_ms.c
OBJ_NAME = $(SRC_NAME:.c=.o)
OBJ_DIR  = obj
INC_NAME = ft_ping.h
INC      = $(addprefix $(SRC_DIR)/,$(INC_NAME))
LIB      = libft.a
SRC      = $(addprefix $(SRC_DIR)/,$(SRC_NAME))
OBJ      = $(addprefix $(OBJ_DIR)/,$(OBJ_NAME))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(OBJ) -o $(NAME) $(LDLIBS) 

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(INC) | $(LIB)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(LIB):
	$(MAKE) -C libft

debug: CPPFLAGS += -g -fsanitize=address
debug: all

check: all
	$(MAKE) -C test
	./test/test

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all check clean fclean re
