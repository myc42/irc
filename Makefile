# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ybouroga <ybouroga@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/09 15:17:58 by ybouroga          #+#    #+#              #
#    Updated: 2026/02/16 18:23:54 by ybouroga         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MAKEFLAGS += --no-print-directory

NAME = ircserv
NAME_BONUS = ircserv_bonus
EXE = ./$(NAME)
EXE_BONUS = ./$(NAME_BONUS)

SRCS_DIR = src
REP_OUT = OBJ
INCLUDES = includes

SRCS_COMMON := \
  $(SRCS_DIR)/server/Channel.cpp \
  $(SRCS_DIR)/server/Client.cpp \
  $(SRCS_DIR)/command/CommandMgr.cpp \
  $(SRCS_DIR)/parser/Parser.cpp \
  $(SRCS_DIR)/server/Server.cpp \
  $(SRCS_DIR)/utils/Utils.cpp \
  $(SRCS_DIR)/utils/SignalMgr.cpp \
  $(SRCS_DIR)/command/CmdJoin.cpp \
  $(SRCS_DIR)/command/CmdKick.cpp \
  $(SRCS_DIR)/command/CmdNick.cpp \
  $(SRCS_DIR)/command/CmdPart.cpp \
  $(SRCS_DIR)/command/CmdPass.cpp \
  $(SRCS_DIR)/command/CmdPing.cpp \
  $(SRCS_DIR)/command/CmdTopic.cpp \
  $(SRCS_DIR)/command/CmdUser.cpp \
  $(SRCS_DIR)/command/CmdMode.cpp \
  $(SRCS_DIR)/command/CmdPrivMsg.cpp \
  $(SRCS_DIR)/command/CmdWho.cpp \
  $(SRCS_DIR)/command/CmdInvit.cpp \
  $(SRCS_DIR)/command/CmdQuit.cpp \
  $(SRCS_DIR)/server/Bot.cpp \
  $(SRCS_DIR)/server/ServerCore.cpp \
  $(SRCS_DIR)/command/CmdDCC.cpp \


SRCS := $(SRCS_COMMON) \
  $(SRCS_DIR)/main.cpp \

SRCS_BONUS := $(SRCS_COMMON) \
  $(SRCS_DIR)/main_bonus.cpp \

OBJS := $(SRCS:$(SRCS_DIR)/%.cpp=$(REP_OUT)/%.o)
OBJS_BONUS := $(SRCS_BONUS:$(SRCS_DIR)/%.cpp=$(REP_OUT)/%.o)

DEPS := $(OBJS:.o=.d)

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -I$(INCLUDES) -I$(SRCS_DIR) -DDEBUG=1
RM = rm -rf

all: $(NAME)

$(REP_OUT)/%.o: $(SRCS_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(OBJS_BONUS)
	$(CC) $(CFLAGS) -o $(NAME_BONUS) $(OBJS_BONUS)

clean:
	$(RM) $(OBJS) $(OBJS_BONUS) $(DEPS) $(REP_OUT)

fclean: clean
	$(RM) $(NAME) $(NAME_BONUS)

re: fclean all

0:
	clear && make && $(EXE)
1:
	clear && make && $(EXE) 6667 0

OPT = --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --trace-children=yes

vg0:
	clear && make && valgrind $(OPT) $(EXE)
vg1:
	clear && make && valgrind $(OPT) $(EXE) 6667 0

vg:
	clear && make && \
	valgrind \
	--leak-check=full \
	--show-leak-kinds=all \
	--track-origins=yes \
	--trace-children=yes \
	--track-fds=yes \
	$(EXE) 6667 0

b:
	clear && make bonus && $(EXE_BONUS) 6667 0

vgb:
	clear && make bonus && valgrind $(OPT) $(EXE_BONUS) 6667 0

-include $(DEPS)

gitadd:
	git pull && git add $(shell find . -type f \( -name "*.cpp" -o -name "*.h*" \) \
	  -not -path "./test2/*" \
	  -not -path "./OBJ/*" \
	  -not -path "./tester/*") .gitignore Makefile README.md && git commit -m "$(m)" && git push

.PHONY: all bonus clean fclean re 0 1 vg0 vg1 vg b vgb gitadd print
