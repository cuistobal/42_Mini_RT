NAME	= miniRT

CC		= cc
CFLAGS	= -Wall -Werror -Wextra -g3
RM		= rm -f

SRC_DIR	= sources
OBJ_DIR	= objects
INC_DIR	= includes

# Source files from both directories
SRC		= $(shell find $(SRC_DIR) -name "*.c")
OBJ		= $(SRC:%.c=$(OBJ_DIR)/%.o)

# Dependencies
DEPS	= $(OBJ:.o=.d)

# Create object directories
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -MMD -MP -I$(INC_DIR) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	@$(RM) -r $(OBJ_DIR)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re

