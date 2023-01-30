# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: wkonings <wkonings@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2023/01/16 15:30:52 by wkonings      #+#    #+#                  #
#    Updated: 2023/01/24 03:21:29 by wkonings      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

# ----------------------------------------- #
# --------------- VARIABLES --------------- #
# ----------------------------------------- #

NAME 	:= cub3d
FLAGS 	:= #-Wall -Wextra -Werror #//todo: RENABLE FLAGS WHEN HANDING IN FOR THE LOVE OF GOD
CFLAGS	:= -w -Wunreachable-code -Ofast
DEBUG 	:= -g -fsanitize=address
# LIBS	:= $(LIBMLX_A) $(LIBFT_A)
LIBMLX	:= mlx
LIBMLX_A:= $(LIBMLX)/libmlx42.a
LIBFT	:= libft
LIBFT_A	:= $(LIBFT)/libft.a
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

$(NAME): $(LIBFT_A) $(LIBMLX_A) $(OBJS) $(HEADERS) $(MAKEFILE)
	@printf "$(BLUE)Compiling $(YELLOW)$(NAME).\n$(END)"
	@$(CC) -ldl -lglfw -lm $(FLAGS) $(DEBUG) $(SRCS) -o $(NAME) -I include -I mlx/include $(INCLUDES)
	@printf "$(YELLOW)$(NAME) compiled!\n$(END)"
# @make cubed

all: $(BANNER) $(NAME)

$(LIBS): $(LIBMLX_A) $(LIBFT_A)

$(LIBMLX_A): $(LIBMLX)
	@make all -C $(LIBMLX)

$(LIBMLX):
	@git clone https://github.com/codam-coding-college/MLX42.git $(LIBMLX)

$(LIBFT_A): $(LIBFT)
	@make all -C $(LIB_DIR)

$(LIBFT):
	@git clone https://github.com/shoddy42/libft

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@printf "$(YELLOW)Compiling $(PINK)$(notdir $@) $(RESET)from $(RED)$(notdir $<)$(END)\n"
	@$(CC) $(FLAGS) -I include $(INCLUDE_READLINE) -c $< -o $@

clean:
	@printf "$(YELLOW)Cleaning up $(NAME)!\n$(END)"
	/bin/rm -rf $(OBJ_DIR)

fclean:	clean
	/bin/rm -f $(NAME)
	/bin/rm -rf $(BIN_DIR)
	@make fclean -C $(LIBFT)
	@make fclean -C $(LIBMLX)
	@printf "$(L_BLUE)Fully cleaned $(NAME)\n$(END)"

tooclean: clean
	@printf "$(RED)Removing $(LIBFT)\n$(RESET)"
	/bin/rm -rf $(LIBFT)
	@printf "$(RED)Removing $(LIBMLX)\n$(RESET)"
	/bin/rm -rf $(LIBMLX)

re: fclean all

tooreal: tooclean $(NAME)

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
