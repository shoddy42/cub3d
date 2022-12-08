# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: wkonings <wkonings@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2019/10/29 16:21:56 by wkonings      #+#    #+#                  #
#    Updated: 2022/12/08 19:29:05 by wkonings      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

# ----------------------------------------- #
# --------------- VARIABLES --------------- #
# ----------------------------------------- #

NAME = cub3d
FLAGS = #-Wall -Wextra -Werror #//todo: RENABLE FLAGS WHEN HANDING IN FOR THE LOVE OF GOD
DEBUG = -g -fsanitize=address

# ---------------------------------------- #
# --------------- DIRECTORIES ------------ #d
# ---------------------------------------- #

#src and its subdirectories.
SRC_DIR		:= src

OBJ_DIR		:=	obj

BIN_DIR		:= bin
INCLUDE_DIR	:= include
LIB_DIR		:= libft

INCLUDES	:= libft/libft.a
HEADERS_DIR	:= include
INC			:= -I include

# ----------------------------------------- #
# --------------- FILES ------------------- #
# ----------------------------------------- #

#this cant be fully right?
HEADER_FILES:=	
HEADERS		:=	$(addprefix $(INCLUDE_DIR)/, $(HEADER_FILES))
MLX			:= mlx

MAIN_FILES	:= main

FILES	:=	$(MAIN_FILES:%=%.c) 
LIBS	:=	
SRCS	:=	$(addprefix $(SRC_DIR)/, $(FILES))
OBJS	:=	$(addprefix $(OBJ_DIR)/, $(FILES:%.c=%.o))

# ----------------------------------------- #
# --------------- RECIPES ----------------- #
# ----------------------------------------- #

$(NAME): $(OBJS) $(LIBS) $(HEADERS) | $(BIN_DIR) $(MLX)
	@make -C $(LIB_DIR)
	@echo COMPILING EXECUTABLE
	@gcc $(FLAGS) $(DEBUG) $(SRCS) -o $(NAME) -I include $(INCLUDES)

echo:
	@echo $(SRCS)

all: $(NAME)

$(LIBS):
	@echo making libft
	@make -C $(LIB_DIR)
	@echo "MAKING LIBFT RN YEA"

#todo: Add libft git clone and an option for "upto date" which will pull mlx42 and libft if needed.
#todo: BUT FIRST, UPDATE LIBFT!!!!!
$(MLX):
	@echo MLX IS MISSING, CLONING MLX NOW!
	git clone https://github.com/codam-coding-college/MLX42.git $(MLX)

$(LIBFT):
	@echo LIBFT IS MISSING, CLONING IT NOW :d
	git clone 

$(BIN_DIR):
	@echo "\n"Creating /$@/ directory.
	@mkdir -p $@

$(OBJ_SUB):
	@mkdir -p $@

$(OBJ_DIR): | $(OBJ_SUB)
	@echo hello?
	@mkdir -p $@

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo Compiling $@ from $< 
	@$(CC) $(FLAGS) -I include $(INCLUDE_READLINE) -c $< -o $@

flags:
	gcc $(FLAGS) $(SOURCES) -lft $(INCLUDES) -lreadline $(READLINE_DIRS) $(INCLUDE_READLINE)
	./$(NAME)

clean:
	/bin/rm -rf $(OBJ_DIR)

fclean:	clean
	/bin/rm -f $(NAME)

mrclean: fclean
	@make fclean -C $(LIB_DIR)

re: fclean all

test: $(NAME)
	@./$(NAME)

.PHONY: all clean fclean re test
