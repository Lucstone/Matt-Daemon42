# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lnaidu <lnaidu@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/27 09:08:05 by lnaidu            #+#    #+#              #
#    Updated: 2025/11/27 09:10:41 by lnaidu           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = MattDaemon

CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++17 
INCLUDES = -I./src -I./src/daemon -I./src/lock -I./src/server -I./src/signals -I./src/tintin

CXXFLAGS += $(INCLUDES)

COMMON_SRCS =  $(shell find ./src -name "*.cpp")

MAND_OBJS   = $(COMMON_SRCS:.cpp=.o)

LIBS        = -lyaml-cpp -lpthread -lreadline

RED     = \033[1;31m
GREEN   = \033[1;32m
YELLOW  = \033[1;33m
DEFAULT = \033[0m


all: $(NAME)


$(NAME): $(MAND_OBJS)
	@$(CXX) $(CXXFLAGS) $(MAND_OBJS) -o $@ $(LIBS)
	@echo "$(GREEN)Built $(NAME) (mandatory)$(DEFAULT)"

.cpp.o:
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -f $(MAND_OBJS) 
	@echo "$(YELLOW)Object files deleted$(DEFAULT)"

fclean: clean
	@rm -f $(NAME) 
	@echo "$(RED)Executables deleted$(DEFAULT)"

re: fclean all

.PHONY: all clean fclean re