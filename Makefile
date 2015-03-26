MODULES=\
###ENDMODULES###
#vars
NAME := watcher
DEBUG = 1
PROFILE = 0
# Paths
DSRC := src
DINC := inc 
DBIN = bin
DOBJ = obj
LIBS =
DLIB =
# Commands & flags
CC = c++
NODBGFLAGS = -Werror -O3 -DNDEBUG
DBGFLAGS = -g3 -DDEBUG
PROFFLAGS = -pg
# Conditional flags
SDL_LDFLAGS := $(shell sdl2-config --libs) -framework OpenGL
SDL_CFLAGS := $(shell sdl2-config --cflags)
CFLAGS := -Wall -Wextra -std=c++11 
LDFLAGS := $(addprefix -L, $(DLIB))
LDLIBS := $(addprefix -l, $(LIBS))
LNK := $(CC)
ifeq ($(PROFILE), 1)
	LDFLAGS += $(PROFFLAGS)
	CFLAGS += $(PROFFLAGS)
endif
ifeq ($(DEBUG), 1)
	CFLAGS+= $(DBGFLAGS)
else
	CFLAGS+= $(NODBGFLAGS)
endif
# Path & includes
export FILES=
-include $(DSRC)/Makefile
OBJS:=$(addprefix $(DOBJ)/, $(FILES))
$(foreach i, $(MODULES),\
	$(eval -include $(DSRC)/$(i)/Makefile)\
	$(eval OBJS+=$(addprefix $(DOBJ)/$(i)/, $(FILES)))\
	$(eval FILES=))
OBJS:=$(addsuffix .o, $(OBJS))
CFLAGS+=$(addprefix -I, $(DINC))

all: $(DBIN)/$(NAME)

$(DBIN)/$(NAME): $(OBJS) | $(DBIN)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)
	@echo "[Compilation $(NAME) ok]"

$(DOBJ)/%.o: $(DSRC)/%.cpp | $(DOBJ)
	$(CC) -c $(CFLAGS) $< -o $@ 

$(DBIN):
	@mkdir $@

$(DOBJ):
	@mkdir $@
	@$(foreach i, $(MODULES), mkdir $@/$(i);)

.PHONY: clean
clean:
	@rm -Rf $(DOBJ)
	@echo "[$(RED)Supression des .o de $(BLUE)$(NAME) $(GREEN)ok$(RESET)]"

fclean: clean
	@rm -Rf $(DBIN)
	@echo "[$(RED)Supression de $(BLUE)$(NAME) $(GREEN)ok$(RESET)]"

re: fclean all
