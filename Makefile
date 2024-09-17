NAME = minishell
SRC = minishell.c init.c
OBJ := $(SRC:.c=.o)
CFLAGS = -Wall -Wextra -Werror
CC = gcc

LIB_DIR = lib
LIB_NAME = readline-8.2
LIB_ARCHIVE = $(LIB_NAME).tar.gz
LIB_URL = https://ftp.gnu.org/gnu/readline/$(LIB_ARCHIVE)
LIB_BUILD_DIR = $(LIB_DIR)/$(LIB_NAME)/build

all: $(LIB_BUILD_DIR) $(NAME)

$(LIB_BUILD_DIR):
	@mkdir -p $(LIB_DIR)
	@curl -L $(LIB_URL) -o $(LIB_DIR)/$(LIB_ARCHIVE)
	@tar -xzf $(LIB_DIR)/$(LIB_ARCHIVE) -C $(LIB_DIR)
	@cd $(LIB_DIR)/$(LIB_NAME) && ./configure --prefix=$(PWD)/$(LIB_BUILD_DIR)
	@make -C $(LIB_DIR)/$(LIB_NAME)
	@make -C $(LIB_DIR)/$(LIB_NAME) install

$(NAME): $(OBJ) $(LIB_BUILD_DIR)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -L$(LIB_BUILD_DIR)/lib -I$(LIB_BUILD_DIR)/include -lreadline

clean:
	rm -f $(OBJ)
	rm -rf $(LIB_BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: re fclean clean all
