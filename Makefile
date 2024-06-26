
NAME		:= minishell

LIBFT		:= external_libs/42_libs/libft.a
SUBMOD_FLAG	:= external_libs/42_libs/Makefile

SRC_DIR		:= src
SRCS		:= $(shell find $(SRC_DIR) -iname "*.c")

OBJ_DIR		:= .build
OBJS		:= $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror -Wunused -Wuninitialized -Wunreachable-code -g -MMD
RM			:= rm -rf

TESTS_DIR	:= tests
TESTS_SRCS	:= $(shell find $(TESTS_DIR) -iname "*.c")
TESTS_OBJS	:= $(TESTS_SRCS:$(TESTS_DIR)/%.c=$(OBJ_DIR)/%.o)
TESTS_BINS	:= $(TESTS_SRCS:%.c=%)

.PRECIOUS: $(OBJ_DIR)/%.o

-include $(OBJS:.o=.d)
-include $(TESTS_OBJS:.o=.d)

all: $(NAME)

# rule to make shell
$(NAME): $(SUBMOD_FLAG) $(OBJS) $(LIBFT)
	$(CC) $(OBJS) $(LIBFT) -o $@
	@printf "$(CREATED)" $@ $(CUR_DIR)

$(SUBMOD_FLAG):
	git submodule init
	git submodule update

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(@D) all

# rule to make tester binaries
%: $(OBJ_DIR)/%.o $(OBJS) $(LIBFT)
	$(CC) $(OBJ_DIR)/$*.o $(OBJS) $(LIBFT) -o $(TESTS_DIR)/$*
	@printf "$(CREATED)" $@ $(CUR_DIR)

$(OBJ_DIR)/%.o: $(TESTS_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) -C $(dir $(LIBFT)) clean
	$(RM) $(OBJ_DIR)
	@printf "$(REMOVED)" $(OBJ_DIR) $(CUR_DIR)

fclean: 
	$(MAKE) -C $(dir $(LIBFT)) fclean
	$(RM) $(OBJ_DIR)
	@printf "$(REMOVED)" $(OBJ_DIR) $(CUR_DIR)
	$(RM) $(NAME)
	@printf "$(REMOVED)" $(NAME) $(CUR_DIR)
	$(RM) $(TESTS_BINS)
	@printf "$(REMOVED)" "tests" $(CUR_DIR)

re:
	$(MAKE) clean
	$(MAKE) flean

dry-%:
	$(MAKE) --dry-run --always-make $*

print-%:
	$(info $($*))

.PHONY: clean fclean re all

# ----------------------------------- colors --------------------------------- #

BOLD	:= \033[1m
BLACK	:= \033[30;1m
RED		:= \033[31;1m
GREEN	:= \033[32;1m
YELLOW	:= \033[33;1m
BLUE	:= \033[34;1m
MAGENTA	:= \033[35;1m
CYAN	:= \033[36;1m
WHITE	:= \033[37;1m
RESET	:= \033[0m

# ----------------------------------- messages ------------------------------- #

CUR_DIR := $(dir $(abspath $(firstword $(MAKEFILE_LIST))))
REMOVED	:= $(MAGENTA) $(BOLD) REMOVED %s (%s) $(RESET)\n\n
CREATED	:= $(GREEN) $(BOLD) CREATED %s (%s) $(RESET)\n\n
UPDATED	:= $(GREEN) $(BOLD) CREATED OR UPDATED %s (%s) $(RESET)\n\n
