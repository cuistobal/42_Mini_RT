# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#              #
#    Updated: 2025/08/09 10:31:03 by chrleroy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = miniRT

# Directories
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = includes
MLX_DIR = minilibx-linux

# Source files
MAIN_SRC = main.c

PARSING_SRC = parsing/parse_scene.c \
              parsing/parse_objects.c \
              parsing/parse_sphere.c \
              parsing/parse_plane.c \
              parsing/parse_cylinder.c \
              parsing/parse_cone.c \
              parsing/parse_cube.c \
              parsing/parse_lights.c \
              parsing/parse_camera.c \
              parsing/parse_utils.c

MATH_SRC = math/vector.c \
		   math/vector1.c \
           math/matrix.c \
           math/ray.c \
           math/intersections.c \
           math/intersect_sphere.c \
           math/intersect_plane.c \
           math/intersect_cylinder.c \
           math/intersect_cone.c \
           math/intersect_cube.c \
           math/object_normals.c \
           math/normal_utils.c \
           math/cube_utils.c \
           math/cone_utils.c

RENDERING_SRC = rendering/raytracer.c \
                rendering/raytracer_utils.c \
                rendering/raycast_optimized.c \
                rendering/progressive_render.c \
                rendering/lighting.c \
                rendering/shadows.c \
                rendering/reflections.c \
                rendering/antialiasing.c

GRAPHICS_SRC = graphics/window.c \
               graphics/image.c \
               graphics/events.c

UTILS_SRC = utils/error.c \
			utils/error1.c \
            utils/memory.c \
            utils/cleanup.c \
            utils/colors.c \
            utils/camera.c \
            utils/bvh.c

SRCS = $(MAIN_SRC) $(PARSING_SRC) $(MATH_SRC) \
       $(RENDERING_SRC) $(GRAPHICS_SRC) $(UTILS_SRC)

# Object files
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3
PFLAGS = -pg
INCLUDES = -I$(INC_DIR) -I$(MLX_DIR)
LIBS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

# Colors for output
GREEN = \033[0;32m
RED = \033[0;31m
BLUE = \033[0;34m
YELLOW = \033[0;33m
NC = \033[0m

# Rules
all: $(MLX_DIR)/libmlx.a $(NAME)

$(NAME): $(OBJS)
	@echo "$(BLUE)Linking $(NAME)...$(NC)"
	@$(CC) $(OBJS) $(LIBS) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) compiled successfully!$(NC)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)Compiling $<...$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(MLX_DIR)/libmlx.a:
	@echo "$(BLUE)Compiling MiniLibX...$(NC)"
	@make -C $(MLX_DIR) > /dev/null 2>&1
	@echo "$(GREEN)✓ MiniLibX compiled!$(NC)"

clean:
	@echo "$(RED)Cleaning object files...$(NC)"
	@rm -rf $(OBJ_DIR)
	@make -C $(MLX_DIR) clean > /dev/null 2>&1

fclean: clean
	@echo "$(RED)Cleaning executable...$(NC)"
	@rm -f $(NAME)

re: fclean all

# Profile target
profile: CFLAGS += $(PFLAGS)
profile: LIBS += $(PFLAGS)
profile: fclean $(MLX_DIR)/libmlx.a $(NAME)
	@echo "$(GREEN)✓ $(NAME) compiled with profiling support!$(NC)"
	@echo "$(YELLOW)Run your program normally, then use 'make analyze' to view results$(NC)"

# Analyze profiling results
analyze:
	@if [ -f gmon.out ]; then \
		echo "$(BLUE)Generating profiling report...$(NC)"; \
		gprof ./$(NAME) gmon.out > profile_report.txt; \
		echo "$(GREEN)✓ Profile report saved to profile_report.txt$(NC)"; \
		echo "$(YELLOW)Top 10 functions by time:$(NC)"; \
		head -30 profile_report.txt | tail -20; \
	else \
		echo "$(RED)No profiling data found. Run your program first after 'make profile'$(NC)"; \
	fi

# Clean profiling data
clean_profile:
	@echo "$(RED)Cleaning profiling data...$(NC)"
	@rm -f gmon.out profile_report.txt

.PHONY: all clean fclean re norm debug test scene profile analyze clean_profile
