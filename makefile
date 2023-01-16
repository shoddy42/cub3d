# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: wkonings <wkonings@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2023/01/16 15:30:52 by wkonings      #+#    #+#                  #
#    Updated: 2023/01/16 15:48:50 by wkonings      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

# ----------------------------------------- #
# --------------- VARIABLES --------------- #
# ----------------------------------------- #

NAME 	:= cub3d
FLAGS 	:= #-Wall -Wextra -Werror #//todo: RENABLE FLAGS WHEN HANDING IN FOR THE LOVE OF GOD
CFLAGS	:= -w -Wunreachable-code -Ofast
DEBUG 	:= #-g #-fsanitize=address
LIBS	:= libft mlx
LIBMLX	:= mlx
LIBFT	:= libft
MAKEFILE:= makefile

# ---------------------------------------- #
# --------------- DIRECTORIES ------------ #
# ---------------------------------------- #

#src and its subdirectories.
SRC_DIR		:= src
S_PARSER	:= parser

OBJ_DIR		:=	obj
OBJ_FOLD		:=  #add all subdirectories here
OBJ_SUB		:=	$(addprefix $(OBJ_DIR)/, $(OBJ_FOLD)) 

BIN_DIR		:= bin
INCLUDE_DIR	:= include
LIB_DIR		:= libft

INCLUDES	:= libft/libft.a mlx/libmlx42.a -lglfw -L /Users/$(USER)/.brew/opt/glfw/lib/
HEADERS_DIR	:= include
INC			:= -I include

# ----------------------------------------- #
# --------------- FILES ------------------- #
# ----------------------------------------- #

#this cant be fully right?
HEADER_FILES:=	cub3d.h
HEADERS		:=	$(addprefix $(INCLUDE_DIR)/, $(HEADER_FILES))

MAIN_FILES	:= main

PARSER_FILES:= ##parser parse_objects parse_utils

FILES	:=	$(MAIN_FILES:%=%.c) \
			# $(addprefix $(S_PARSER)/, $(PARSER_FILES:%=%.c)) \
			# $(addprefix $(S_TOKEN)/, $(TOKEN_FILES:%=%.c)) \
			# $(addprefix $(S_EXEC)/, $(EXEC_FILES:%=%.c)) \
			# $(addprefix $(S_ENV)/, $(ENV_FILES:%=%.c)) \
			# $(addprefix $(S_BUILTIN)/, $(BUILT_FILES:%=%.c))

SRCS	:=	$(addprefix $(SRC_DIR)/, $(FILES))
OBJS	:=	$(addprefix $(OBJ_DIR)/, $(FILES:%.c=%.o))

# ----------------------------------------- #
# --------- COLOURS / FORMATTING ---------- #
# ----------------------------------------- #

#Formatting
RESET	:= \1\33[0m\2
END		:= \1\33[0m\2\3
BANNER	:= banner

#Colours
YELLOW	:= \1\33[38;5;220m\2
BLACK	:= \1\33[38;5;232m\2
ORANGE	:= \1\33[38;5;202m\2
RED		:= \1\33[38;5;196m\2
PINK	:= \1\33[38;5;198m\2
GREEN	:= \1\33[38;5;28m\2
BLUE	:= \1\33[38;5;33m\2
L_BLUE	:= \1\33[38;5;69m\2
D_BLUE	:= \1\33[38;5;21m\2
GREY	:= \1\33[38;5;242m\2
PURPLE	:= \1\33[38;5;92m\2
VIOLET	:= \1\33[38;5;183m\2

# ----------------------------------------- #
# --------------- RECIPES ----------------- #
# ----------------------------------------- #

$(NAME): $(OBJS) $(HEADERS) $(MAKEFILE)
	@make all -C $(LIB_DIR)
	@make -C $(LIBMLX)
	@printf "$(BLUE)Compiling $(YELLOW)$(NAME).\n$(END)"
	@$(CC) -ldl -lglfw -lm $(FLAGS) $(DEBUG) $(SRCS) -o $(NAME) -I include -I mlx/include $(INCLUDES)
	@printf "$(YELLOW)$(NAME) compiled!\n$(END)"
	@make cubed

all: $(BANNER) $(NAME)

$(LIBMLX):
	@git clone https://github.com/codam-coding-college/MLX42.git $(LIBMLX)

$(LIBFT):
	@git clone https://github.com/shoddy42/libft

# $(BIN_DIR):
# 	@printf "$(YELLOW)Creating $(RESET)/$@/ directory.\n$(END)"
# 	@mkdir -p $@

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c | $(LIBFT) $(LIBMLX)
	@mkdir -p $(dir $@)
	@printf "$(YELLOW)Compiling $(PINK)$(notdir $@) $(RESET)from $(RED)$(notdir $<)$(END)\n"
	@$(CC) $(FLAGS) -I include $(INCLUDE_READLINE) -c $< -o $@

flags:
	$(CC) $(FLAGS) $(SOURCES) -lft $(INCLUDES) -lreadline $(READLINE_DIRS) $(INCLUDE_READLINE)
	./$(NAME)

clean:
	/bin/rm -rf $(OBJ_DIR)

fclean:	clean
	/bin/rm -f $(NAME)
	/bin/rm -rf $(BIN_DIR)

mrclean: fclean
	@make fclean -C $(LIBFT)

tooclean: fclean
	/bin/rm -rf $(LIBFT)
	/bin/rm -rf $(LIBMLX)

re: fclean all

tooreal: tooclean re

test: $(NAME)
	@./$(NAME)

pog:
	@printf "\n$(RED)"
	@printf "⣿⣿⣿⣿⡿⠟⠛⠛⠛⠛⠉⠉⠙⠛⠛⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⠟   \n"
	@printf "⣿⣿⣯⣥⣤⣶⣶⣶⣶⣿⣿⣿⣿⣿⣿⣷⣾⣿⣿⣿⣿⣿⣿⣿⣏⣀⣀⣀⡀  \n"
	@printf "⣿⣿⣿⣿⣿⣿⣿⡿⠿⠛⠛⠻⠿⠟⠉⠉⠉⢻⣿⣿⣿⡿⠟⠋⣡⣼⣿⣿⣿⡄ \n"
	@printf "⣿⣿⣿⣟⣭⣤⣶⣶⣿⣿⠃⠀⠀⢀⣀⣤⣶⣿⣿⣿⣿⡅⡀⢀⣩⣤⣤    \n"
	@printf "⣿⣿⣿⣿⣿⣿⣛⡛⠛⠛⠛⢋⣩⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣛⠛⠛⠓⠠   \n"
	@printf "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⣤⣤⣤⣦  \n"
	@printf "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇ \n"
	@printf "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠿⠿⠿⠿⢿⡿⢿⣿⣿⣿⠃ \n"
	@printf "⠿⠿⠿⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣥⣄⣀⣀⠀⠀⠀⠀⠀⢰⣾⣿⣿⠏  \n"
	@printf "⠀⠀⠀⠀⠀⠀⠉⣩⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣤⣜⡻⠋    \n"
	@printf "⣰⣾⣷⣶⣿⣾⣖⣻⣿⣿⡿⣿⣿⣿⣿⠿⠿⠟⠛⠛⠛⠋⠉⠉⢉⡽⠃    \n"
	@printf "⣿⣿⣿⣿⣿⣿⡉⠉⠉⠛⠛⠛⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⡤⠚⠉      \n"
	@printf "⠛⠛⣿⣿⣿⣿⣿⣿⣿⠉⠛⢶⣶⣄⠀⠀⠀⠀⠀⠀⠀⠀⡇        \n"
	@printf "⠠⣾⣿⣿⣿⣿⣿⠿⠟⠃⠀⠀⠀⠈⠲⣴⣦⣤⣤⣤⣶⡾⠁        \n"
	@printf "⠄⠈⠉⠻⢿⣿⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠛⠛⠉          \n"
	@printf "$(END)"

cubed:
	@printf "\n$(RED)"
	@printf "|        | $(YELLOW) ------------\    ,---.  /------------$(RED)  |        |\n"
	@printf "|        | $(YELLOW)  ---------.  \`-./  \"\.-\'  .---------$(RED)   |        |\n"
	@printf "|  ,--.  | $(YELLOW)    --------\   .         /--------$(RED)     |  ,--.  |\n"
	@printf "| ( >< ) | $(YELLOW)       ------\`-.|      .-\'------ $(RED)       | ( >< ) |\n"
	@printf "|  \`--\'  | $(YELLOW)            ---/ \`/\"\  \--- $(RED)            |  \`--\'  |\n"
	@printf "|      . |       $(YELLOW)         \`//_-_\\'  $(RED)        $(ORANGE) cub3d $(RED) |        |\n"
	@printf "| : .  ! |       $(YELLOW)         (.\'   \',) $(RED)               | . : . :|\n"
	@printf "| ! ! .| |                                         | : | ! .|\n"
	@printf "| |_| ;|_|                  .                      |_| !_| !|\n"
	@printf "\`-\' \`-^\'                     \o                      \`-\' \`-^\'\n"
	@printf "           \__________________T>_________________/           \n"
	@printf "           \`-=--=--=--=--=--=---=--=--=--=--=--=-\'           \n"
	@printf "            ] _] _] _] _] _] _L] _] _] _] _] _] _            \n"
	@printf "           \`-------------------------------------\'           \n"
	@printf "           \`u---u---u---u---u---u---u---u---u---u\'           \n"
	@printf "$(END)"

.PHONY: all clean fclean mrclean tooclean re tooreal test banner hell pog flags
