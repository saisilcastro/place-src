#include <place/plugin/graphic/allegro-plugin.h>
#include <place/system/object-of.h>
#include <place/machine.h>
#include <stdlib.h>

static inline void	machine_function(machine_t *set);

static inline void	allegro_create(machine_t * gear) {
    int	init = 0;
	if (!gear || !gear->plugin)
		return;
    if (((allegro_plugin_t *)gear->plugin)->window) {
		al_destroy_display (((allegro_plugin_t *)gear->plugin)->window);
		((allegro_plugin_t *)gear->plugin)->window = 0;
    }
    if (BIT_IS_ON(gear->event, MACHINE_FULLSCREEN))
		init = ALLEGRO_FULLSCREEN;
    else
		init &= ~ALLEGRO_FULLSCREEN;
	al_set_new_display_flags (init);
    al_set_new_display_adapter (ALLEGRO_DEFAULT_DISPLAY_ADAPTER);
    ((allegro_plugin_t *)gear->plugin)->window = al_create_display (gear->size->x, gear->size->y);
    if (((allegro_plugin_t *)gear->plugin)->window) {
		al_register_event_source (((allegro_plugin_t *)gear->plugin)->event_row, al_get_display_event_source (((allegro_plugin_t *)gear->plugin)->window));
		al_register_event_source (((allegro_plugin_t *)gear->plugin)->event_row, &((allegro_plugin_t *)gear->plugin)->event_fake);
    }
}

MACHINE_CALL void MACHINE_TYPE	machine_set(machine_t *set, system_t up, char *title, vf2d size)
{
	int	i;

	object_set(set->map, 0, "game", vf2d_start(0, 0), vf2d_start(0.1, 0.1), size, (vf2d){1, 1}, NULL, pixel_rgb_start(255, 255, 255), &up);
	*(set->up) = up;
	set->title = title;
	*(set->size) = size;
	set->event = 0x81;
	mouse_set(set->mouse);
	audio_set(set->audio, 44100, 32, 2, 4096);
	set->img = NULL;
	set->obj = NULL;
	set->snd = NULL;
	set->font = NULL;
	set->plugin = NULL;
	i = -1;
	while (++i < KEYBOARD_MAX)
		set->key[i] = Off;
	machine_function(set);
}

static inline status_t	machine_start(machine_t *set)
{
	if (!set)
		return (Off);
	switch (set->up->system)
	{
		case SYSTEM_CONSOLE:
		break ;
		case SYSTEM_PRIMARY:
		break;
		case SYSTEM_ALLEGRO:
		{
			set->plugin = allegro_plugin_push();
			return (allegro_start(set));
		}
		case SYSTEM_SDL:
		break;
		case SYSTEM_MAIN_NONE:
		break ;
	}
	return (Off);
}

static inline void	machine_resize(machine_t *set)
{
	if (!set)
		return ;
	switch (set->up->system)
	{
		case SYSTEM_CONSOLE:
		break ;
		case SYSTEM_PRIMARY:
		break ;
		case SYSTEM_ALLEGRO:
			allegro_create(set);
		break ;
		case SYSTEM_SDL:
		break ;
		case SYSTEM_MAIN_NONE:
		break ;
	}
}

static inline void	machine_search_res(vf2d *size, B32 *pos, system_t *up)
{
	if (!size || !pos || !up)
		return ;
	switch (up->system)
	{
		case SYSTEM_CONSOLE:
		break ;
		case SYSTEM_PRIMARY:
		break ;
		case SYSTEM_ALLEGRO:
			allegro_search_res(size, pos);
		case SYSTEM_SDL:
		break ;
		case SYSTEM_MAIN_NONE:
		break ;
	}
}

static inline status_t	machine_prev_res(vf2d *size, int *pos, system_t *up)
{
	switch (up->system)
	{
		case SYSTEM_CONSOLE:
		break ;
		case SYSTEM_PRIMARY:
		break ;
		case SYSTEM_ALLEGRO:
			return (allegro_prev_res(size, pos));
		case SYSTEM_SDL:
		break ;
		case SYSTEM_MAIN_NONE:
		break ;
	}
	return (Off);
}

static inline status_t	machine_next_res(vf2d *size, int *pos, system_t *up)
{
	switch (up->system)
	{
		case SYSTEM_CONSOLE:
		break ;
		case SYSTEM_PRIMARY:
		break ;
		case SYSTEM_ALLEGRO:
			return (allegro_next_res(size, pos));
		case SYSTEM_SDL:
		break ;
		case SYSTEM_MAIN_NONE:
		break ;
	}
	return (Off);
}

static inline status_t	machine_running(machine_t *set)
{
	if (!set)
		return (Off);
	switch (set->up->system)
	{
		case SYSTEM_CONSOLE:
		break ;
		case SYSTEM_PRIMARY:
		break ;
		case SYSTEM_ALLEGRO:
		{
			al_clear_to_color(al_map_rgb(255, 255, 255));
			return (allegro_plugin_event(set, set->map->pos[0]));
		}
		break ;
		case SYSTEM_SDL:
		break ;
		case SYSTEM_MAIN_NONE:
		break ;
	}
	return (Off);
}

static inline void	machine_show(machine_t *set)
{
	switch (set->up->system)
	{
		case SYSTEM_CONSOLE:
		break ;
		case SYSTEM_PRIMARY:
		break ;
		case SYSTEM_ALLEGRO:
		{
			al_flip_display ();
		}
		break ;
		case SYSTEM_SDL:
		break ;
		case SYSTEM_MAIN_NONE:
		break ;
	}
}

static inline void map_pop(object_t *map)
{
	if (map->name)
		free(map->name);
}

static inline void	machine_pop(machine_t *set)
{
	map_pop(set->map);
	chained_pop(&set->obj, set->up, object_pop);
	chained_pop(&set->img, set->up, image_pop);
	chained_pop(&set->snd, set->up, sound_pop);
	chained_pop(&set->font, set->up, font_pop);
	if (set->plugin)
	{
		switch(set->up->system)
		{
			case SYSTEM_CONSOLE:
			break ;
			case SYSTEM_PRIMARY:
			break ;
			case SYSTEM_ALLEGRO:
				allegro_plugin_pop(set->plugin);
			break ;
			case SYSTEM_SDL:
			break ;
			case SYSTEM_MAIN_NONE:
			break ;
		}
	}
}

static inline void	machine_function(machine_t *set)
{
	set->start = machine_start;
	set->resize = machine_resize;
	set->search_res = machine_search_res;
	set->prev_res = machine_prev_res;
	set->next_res = machine_next_res;
	set->running = machine_running;
	set->show = machine_show;
	set->pop = machine_pop;
}

MACHINE_CALL int MACHINE_TYPE vscprintf(const char *format, va_list pargs)
{
	int		len; 
    va_list	argcopy;

    va_copy(argcopy, pargs); 
    len = vsnprintf(NULL, 0, format, argcopy); 
    va_end(argcopy); 
    return (len);
}

MACHINE_CALL int MACHINE_TYPE scprintf(const char *format, ...)
{
	int 	len;
    va_list args;
 
    va_start(args, format);
    len = vscprintf(format, args);
    va_end(args);
    return (len);
}
