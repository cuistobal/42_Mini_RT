NAME	= miniRT

CC		= cc
CFLAGS	= -Wall -Werror -Wextra -g3
RM		= rm -f

SRC_DIR	= sources
OBJ_DIR	= objects
INC_DIR	= includes

MLX_DIR = $(INC_DIR)/minilibx-linux
MLX_LIB = $(MLX_DIR)/libmlx.a

LIBFT_DIR = $(SRC_DIR)/utilities/Libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a

SRC		= $(shell find $(SRC_DIR) -name "*.c")
OBJ		= $(SRC:%.c=$(OBJ_DIR)/%.o)

DEPS	= $(OBJ:.o=.d)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -MMD -MP -I$(INC_DIR) -I$(LIBFT_DIR)/includes -c $< -o $@

all: $(NAME)

$(MLX_LIB):
	@$(MAKE) -C $(MLX_DIR) > /dev/null

$(LIBFT_LIB):
	@$(MAKE) -C $(LIBFT_DIR) > /dev/null

$(NAME): $(MLX_LIB) $(LIBFT_LIB) $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -L$(MLX_DIR) -lmlx -L$(LIBFT_DIR) -lft -lX11 -lXext -lm -o $(NAME)
	@echo "L3t'5 g3T th15 p@rTy 5T@rt!ng R1gHT"

clean:
	@$(RM) -r $(OBJ_DIR)
	@$(MAKE) -C $(MLX_DIR) clean > /dev/null
	@$(MAKE) -C $(LIBFT_DIR) clean > /dev/null

fclean: clean
	@$(RM) $(NAME)
	@$(MAKE) -C $(MLX_DIR) clean > /dev/null
	@$(MAKE) -C $(LIBFT_DIR) fclean > /dev/null

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
