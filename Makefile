# -----------COLOR-----------#

GREEN  = \033[32m
YELLOW = \033[33m
BLUE   = \033[34m
RED    = \033[31m
RESET  = \033[0m

# -----------RULES-----------#

CFLAGS = -Wall -Wextra -Werror -MMD -MP -g3
CC = cc
AR = ar
ARFLAG = -rcs

# -----------PATHS-----------#

SRCDIR = src/

INCDIR = inc/

LIBDIR = ./

OBJDIR = .obj/

#///////////SUBDIR////////////#


# -----------FILES-----------#

MAIN 	=	main.c algo.c render.c event.c

INC		=

# -----------SRCS-----------#

SRCS =	$(addprefix $(SRCDIR), $(MAIN))

# -----------LIBS------------#

LIB =	libft

LIBA = $(foreach l,$(LIB),$(LIBDIR)$(l)/$(l).a)

LIBS = $(addprefix $(LIBDIR), $(LIB))

LIBINCDIR = $(foreach l, $(LIBS), $(if $(findstring mlx, $(l)), -I $(l), -I $(l)/$(INCDIR)))

# -----------OTHER-----------#

OBJS =	$(patsubst $(SRCDIR)%.c, $(OBJDIR)%.o, $(SRCS))

DEPS =	$(OBJS:.o=.d)

HEADER = $(addprefix $(INCDIR), $(INC))

NAME =	AlCu

# -----------RULES-----------#

ifeq ($(TEXT), 1)
CFLAGS += -DTEXT=1
endif

all: libs $(NAME)

text:
	make re TEXT=1

bonus: libs $(NAME_BONUS)

$(NAME): $(LIBA) $(OBJS)
	$(CC) $(CFLAGS) -lSDL2 $(OBJS) $(MLXFLAG) $(LIBA) -o $(NAME)

$(OBJDIR)%.o: $(SRCDIR)%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -I $(INCDIR) $(LIBINCDIR) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR) $(addprefix $(OBJDIR), $(dir $(patsubst $(SRCDIR)%,%,$(SRCS))))

libs:
	@for dir in $(LIBS); do \
		echo "$(YELLOW)Compiling library $$dir$(RESET)" && $(MAKE) -C $$dir --no-print-directory --silent; \
	done

# -----------UTILS-----------#

clean:
	@echo "$(RED)Removing object files...$(RESET)" && rm -rf $(OBJDIR) $(OBJDIR_BONUS)
	@for dir in $(LIBS); do \
		if [ "$$dir" != "libs/libmlx" ]; then \
			echo "$(RED)Removing $$dir .Obj/ ...$(RESET)" && $(MAKE) $@ -C $$dir --no-print-directory --silent; \
		fi; \
	done

fclean: clean
	@echo "$(RED)Removing executable or library...$(RESET)" && rm -f $(NAME) $(NAME_BONUS)
	@for dir in $(LIBS); do \
		if [ "$$dir" = "libs/libmlx" ]; then \
			echo "$(RED)Removing $$dir lib.a ...$(RESET)" && $(MAKE) clean -C $$dir ; \
		else \
			echo "$(RED)Removing $$dir lib.a ...$(RESET)" && $(MAKE) $@ -C $$dir --no-print-directory --silent; \
		fi; \
	done

re: fclean all

print-%:
	@echo $($(patsubst print-%,%,$@))

-include $(DEPS) $(DEPS_BONUS)

VG			:=      valgrind
VGFLAGS		:=      --leak-check=full --show-leak-kinds=all --track-origins=yes --show-mismatched-frees=yes --track-fds=yes --trace-children=yes --suppressions=sdl2.supp
.PHONY: vg
vg:	all
		$(VG) $(VGFLAGS) ./$(NAME) <out

.PHONY: clean fclean re all bonus libs text
