ifneq ($(OS), Windows_NT)
	CREATE = mkdir -p $(1)
	REMOVE = rm -rf $(1)
	NAME = libplace.so
	SRC_SYSTEM = 
else
	CC = gcc
	CREATE = mkdir -p $(1)
	REMOVE = rm -rf $(1)
	NAME = libplace.dll
	SRC_SYSTEM = windows-plugin.c
endif
PLUGIN = ../../../../../../people/$(SYSTEM)/c/release/plugin/x$(PLATFORM)
RELEASE = ../../../release
INCLUDE = 	-I$(RELEASE)/include \
			-I$(PLUGIN)/include
VPATH = src: ./system src: ./system/graphic src: ./plugin/graphic
ALLEGRO_PLUGIN_SRC = allegro-plugin.c
GRAPHIC =	font-of.c \
			image-of.c \
			pixel-of.c
SRC = 	$(ALLEGRO_PLUGIN_SRC) \
		$(SRC_SYSTEM) \
		$(GRAPHIC) \
		timer-of.c \
		audio-of.c \
		chained-of.c \
		math-of.c \
		mouse-of.c \
		object-of.c \
		sound-of.c \
		machine.c \
		place.c
OBJ = obj
SRCOBJ = $(SRC:%.c=${OBJ}/%.o)
LIB = -L$(PLUGIN)/lib
ALLEGRO_PLUGIN_IMAGE = -lallegro_image -lallegro_font -lallegro_ttf
ALLEGRO_PLUGIN_AUDIO = -lallegro_audio -lallegro_acodec
ALLEGRO_PLUGIN_VIDEO = -lallegro_video
ALLEGRO_PLUGIN = -lallegro $(ALLEGRO_PLUGIN_IMAGE) $(ALLEGRO_PLUGIN_AUDIO) $(ALLEGRO_PLUGIN_VIDEO)
ALLEGRO_MONOLITH = -lallegro_monolith.dll
ENGINE =  $(ALLEGRO_PLUGIN)
FLAG = -Wall -Werror -Wextra

all: $(NAME)
$(NAME): $(SRCOBJ)
	@$(CC) $^ -shared -fPIC -o $(RELEASE)/bin/x$(PLATFORM)/$(NAME) $(LIB) $(ENGINE)
	@echo -n $(RELEASE)/bin/x$(PLATFORM)/$(NAME) created sucessfully!
${OBJ}/%.o : %.c
	@$(call CREATE,${OBJ})
	@$(CC) -c $< -o $@ $(INCLUDE) $(FLAG)
clean:
	@$(call REMOVE,${OBJ})
fclean: clean
	@$(call REMOVE,$(RELEASE)/bin/x$(PLATFORM)/$(NAME))
re: fclean all