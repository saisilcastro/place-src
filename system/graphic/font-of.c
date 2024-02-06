#include <place/system/graphic/font-of.h>
#include <place/plugin/graphic/allegro-plugin.h>

FONT_OF_CALL font_t *FONT_OF_TYPE	font_load(B32 id, B32 size, B8 * path, pixel_t *color, system_t *up)
{
	font_t	*set;

	set = malloc(sizeof(font_t));
	if (!set)
		return (NULL);
	set->id = id;
	set->size = size;
	*(set->color) = *color;
	switch (up->system)
	{
		case SYSTEM_CONSOLE:
		case SYSTEM_PRIMARY:
		break ;
		case SYSTEM_ALLEGRO:
			set->data = al_load_font(path, size, 0);
		break ;
		case SYSTEM_SDL:
		break ;
		case SYSTEM_MAIN_NONE:
			set->data = NULL;
		break ;
	}
	return (set);
}

FONT_OF_CALL void FONT_OF_TYPE	font_draw(font_t *font, vf2d pos, char *str, system_t *up)
{
	if (!str || !up || !font)
		return ;
	switch (up->system)
	{
		case SYSTEM_CONSOLE:
		case SYSTEM_PRIMARY:
		break ;
		case SYSTEM_ALLEGRO:
			al_draw_text(font->data, al_map_rgba(font->color->r, font->color->g, font->color->b, font->color->a), pos.x, pos.y, ALLEGRO_ALIGN_LEFT, str);
		break ;
		case SYSTEM_SDL:
		break ;
		case SYSTEM_MAIN_NONE:
		break ;
	}
}

FONT_OF_CALL vf2d FONT_OF_TYPE	font_size(font_t *font, char *buffer, system_t *up)
{
	vf2d	size = {0, 0};

	if (!font || !buffer)
		return (size);
	switch (up->system)
	{
		case SYSTEM_CONSOLE:
		case SYSTEM_PRIMARY:
		break ;
		case SYSTEM_ALLEGRO:
		{
			size.x = al_get_text_width(font->data, buffer);
			size.y = al_get_font_line_height(font->data);
		}
		break ;
		case SYSTEM_SDL:
		break ;
		case SYSTEM_MAIN_NONE:
		break ;
	}
	return (size);
}

FONT_OF_CALL void FONT_OF_TYPE	font_pop(void *data, system_t *up)
{
	font_t	*set;

	set = data;
	if (!set)
		return ;
	switch (up->system)
	{
		case SYSTEM_CONSOLE:
		case SYSTEM_PRIMARY:
		break ;
		case SYSTEM_ALLEGRO:
			al_destroy_font(set->data);
		break ;
		case SYSTEM_SDL:
		break ;
		case SYSTEM_MAIN_NONE:
		break ;
	}
	free(set);
}