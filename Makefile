NAME	= scop

CC		= c++
FLAGS	= -Wall -Wextra -Werror -g -std=c++20 

BLDD	= build
SRCD	= srcs
LIBD	= libs

GLFWD		= $(LIBD)/glfw
GLFW_BUILD	= $(GLFWD)/build
GLFW_LIB	= $(GLFW_BUILD)/src/libglfw3.a

SRCS	=	srcs/main.cpp \
			srcs/Gl.cpp \
			srcs/Mesh.cpp
OBJS	=	$(patsubst %.cpp, $(BLDD)/%.o, $(SRCS))

INCS	= -I $(SRCD) -I $(GLFWD)/include -I libs/glfw/deps
LNKS	= -L $(GLFW_BUILD)/src

LIBS	= -lglfw3 -lGL -ldl -lpthread -lm
DEPS	= $(OBJS:.o=.d)

.PHONY: all clean fclean re san valgrind

all: $(GLFW_LIB) $(NAME)

$(NAME): $(OBJS) $(GLFW_LIB)

$(GLFW_LIB):
	@if [ ! -d "$(GLFW_BUILD)" ]; then \
		cmake -S $(GLFWD) -B $(GLFW_BUILD); \
	fi
	cmake --build $(GLFW_BUILD)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $^ -o $@ $(INCS) $(LNKS) $(LIBS)

$(BLDD)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CC) $(FLAGS) -MMD -c $< -o $@ $(INCS)

-include $(DEPS)

clean:
	rm -rf $(BLDD)

fclean: clean
	rm -rf $(NAME)
	rm -rf $(GLFW_BUILD)

re: fclean all

san: FLAGS += -g -fsanitize=address,leak,undefined
san: all

valgrind: all
	valgrind --track-fds=yes --leak-check=full --show-leak-kinds=all --suppressions=glfw_x11_full ./$(NAME)