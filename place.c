#include <place/place.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static inline void	place_function(place_t *set);

PLACE_CALL place_t *PLACE_TYPE	place_get(void)
{
	static place_t	place;
	system_t		up;

	if (!place.gear->size->x || !place.gear->size->y)
	{
		up.system = SYSTEM_ALLEGRO;
		up.driver = GRAPHIC_PRIMARY;
		up.audio = SYSTEM_AUDIO_ALLEGRO;
		up.network = SYSTEM_NETWORK_PRIMARY;
		machine_set(place.gear, up, "machine", vf2d_start(640, 480));
		place_function(&place);
	}
	return (&place);
}

static inline void	place_map_set(image_t *image)
{
	place_t	*place;

	if (!image)
		return ;
	place = place_get();
	place->gear->map->image = image;
	*(place->gear->map->size) = image_size(image, place->gear->up);
}

static inline void place_resize(void)
{
	place_get()->gear->resize(place_get()->gear);
}

static inline void	place_search_res(B32 *pos)
{
	place_get()->gear->search_res(place_get()->gear->size, pos, place_get()->gear->up);
}

static inline status_t	place_previos_res(vf2d *size, int *pos)
{
	return (place_get()->gear->prev_res(size, pos, place_get()->gear->up));
}

static inline status_t	place_next_res(vf2d *size, int *pos)
{
	return (place_get()->gear->next_res(size, pos, place_get()->gear->up));
}

static inline void	place_run(void *data)
{
	place_t			*place;
	chained_t		*update;
	vf2d			map;
	vf2d			player;
	object_t		*obj;
	double			one;
	double			two;

	place = place_get();
	if (!place->gear->start(place->gear))
		return ;
	if (place->create)
		place->create(data);
	one = time_now();
	two = time_now();
	while (place->gear->running(place->gear))
	{
		two = time_now();
		place->time = two - one;
		one = two;
		if (place->update)
			place->update(data);
		if (place->gear->map->image)
		{
			obj = &*(place->gear->map);
			if (obj->pos->x < 0)
				obj->pos->x = 0;
			if (obj->pos->x > (obj->size->x * obj->zoom->x) - place->gear->size->x)
				obj->pos->x = (obj->size->x * obj->zoom->x) - place->gear->size->x;
			if (obj->pos->y < 0)
				obj->pos->y = 0;
			if (obj->pos->y > (obj->size->y * obj->zoom->y) - place->gear->size->y)
				obj->pos->y = (obj->size->y * obj->zoom->y) - place->gear->size->y;
			map = vf2d_start(-obj->pos->x, -obj->pos->y);
			image_draw(obj->image, obj->color, obj->axis[0], map, obj->zoom[0], obj->angle, place->gear->up);
		}
		update = place->gear->obj;
		while (update)
		{
			obj = update->data;
			if (obj)
			{
				if ((obj->pos->x >= place->gear->map->pos->x - (obj->size->x * obj->zoom->x) && obj->pos->x <= place->gear->map->pos->x + place->gear->size->x)
				&& (obj->pos->y >= place->gear->map->pos->y - (obj->size->y * obj->zoom->y) && obj->pos->y <= place->gear->map->pos->y + place->gear->size->y))
				{
					player = vf2d_start(obj->pos->x + map.x, obj->pos->y + map.y);
					image_draw(obj->image, obj->color, obj->axis[0], player, obj->zoom[0], obj->angle, place->gear->up);
				}
			}
			update = update->next;
		}
		if (place->draw)
			place->draw(data);
		place->gear->show(place->gear);
	}
	place->destroy(place);
}
static inline void		place_camera_object(object_t *object)
{
	machine_t	*gear;
	vf2d		pos;

	if (!object)
		return ;
	gear = place_get()->gear;
	pos.x = object->pos->x - ((gear->size->x - object->size->x) * .5);
	pos.y = object->pos->y - ((gear->size->y - object->size->y) * .5);
	*(gear->map->pos) = pos;
}

static inline void		place_camera_mouse(BP32 w_offset, BP32 h_offset)
{
	place_t	*place;
	vf2d	min;
	vf2d	max;

	place = place_get();
	if (place->mouse_down(MACHINE_MOUSE_RIGHT))
	{
		min.x = place->gear->map->pos->x;
		max.x = place->gear->map->pos->x + place->gear->size->x;
		min.y = place->gear->map->pos->y;
		max.y = place->gear->map->pos->y + place->gear->size->y;
		if (place->gear->mouse->x >= min.x && place->gear->mouse->x <= min.x + w_offset)
			place->gear->map->pos->x -= place->gear->map->vel->x;
		if (place->gear->mouse->x >= max.x - w_offset && place->gear->mouse->x <= max.x)
			place->gear->map->pos->x += place->gear->map->vel->x;
		if (place->gear->mouse->y >= min.y && place->gear->mouse->y <= min.y + h_offset)
			place->gear->map->pos->y -= place->gear->map->vel->y;
		if (place->gear->mouse->y >= max.y - h_offset && place->gear->mouse->y <= max.y)
			place->gear->map->pos->y += place->gear->map->vel->y;
	}
}

static inline status_t	place_mouse_down(MACHINE_MOUSE_BUTTON button)
{
	return (place_get()->gear->mouse->button & (1 << button));
}

static inline status_t	place_mouse_release(MACHINE_MOUSE_BUTTON button)
{
	static	status_t	pressed[MACHINE_MOUSE_MAX];
	static	status_t	released[MACHINE_MOUSE_MAX];

	if (place_mouse_down(button))
		pressed[button] = On;
	if (!place_mouse_down(button))
	{
		if (pressed[button])
		{
			released[button] = On;
			pressed[button] = Off;
		}
		else
			released[button] = Off;
	}
	return (released[button]);
}

static inline status_t	place_mouse_over(object_t *object)
{
	mouse_t	mouse;

	if (!object)
		return (Off);
	mouse = *(place_get()->gear->mouse);
	if (mouse.x >= object->pos->x && mouse.x <= object->pos->x + (object->size->x * object->zoom->x)
	&& mouse.y >= object->pos->y && mouse.y <= object->pos->y + (object->size->y * object->zoom->y))
		return (On);
	return (Off);
}

static inline status_t	place_key_down(keyboard_map key)
{
	return (place_get()->gear->key[key]);
}

static inline image_t	*place_image_select(B32 id)
{
	chained_t	*update;
	image_t		*image;

	update = place_get()->gear->img;
	while (update)
	{
		image = update->data;
		if (image && image->id == id)
			return (image);
		update = update->next;
	}
	return (NULL);
}

static inline vf2d	place_image_size(image_t *image)
{
	return (image_size(image, place_get()->gear->up));
}

static inline image_t	*place_image_load(B8 *path, B32 id, image_flip_t flip)
{
	system_t	*up;

	if (!path || place_image_select(id))
		return (NULL);
	up = place_get()->gear->up;
	switch (up->system)
	{
		case SYSTEM_CONSOLE:
		break ;
		case SYSTEM_PRIMARY:
		{
			printf("loading image\n");
			return (image_load(path, id, flip, up));
		}
		case SYSTEM_ALLEGRO:
			return (image_load(path, id, flip, up));
		case SYSTEM_SDL:
		break ;
		case SYSTEM_MAIN_NONE:
		break ;
	}
	return (NULL);
}

static inline image_t	*place_image_text_create(font_t *font, B32 id, const B8 *str, ...)
{
	va_list		list;
	int			len;
	image_t		*image;
	system_t	*up;

	image = NULL;
	if (!str || !font || !font->data || place_image_select(id))
		return (image);
	va_start(list, str);
	len = vscprintf (str, list);
	char	buffer[len + 1];
	vsnprintf (buffer, len + 1, (const char *) str, list);
	va_end(list);
	up = place_get()->gear->up;
	switch (up->system)
	{
		case SYSTEM_CONSOLE:
		break ;
		case SYSTEM_PRIMARY:
		break ;
		case SYSTEM_ALLEGRO:
		{
			vf2d size = font_size(font, buffer, up);
			image = image_create(id, size, up);
			if (image)
			{
				image_target_self(image, up);
				font_draw(font, (vf2d){0, 0}, buffer, place_get()->gear->up);
				image_target_window(place_get()->gear->plugin, up);
			}
		}
		break ;
		case SYSTEM_SDL:
		break ;
		case SYSTEM_MAIN_NONE:
		break ;
	}
	return (image);
}

static inline void	place_image_next_first(image_t *image)
{
	if (chained_next_first(&place_get()->gear->img, chained_push(image)))
	{}
}

static inline void	place_image_next_last(image_t *image)
{
	if (chained_next_last(&place_get()->gear->img, chained_push(image)))
	{}
}

static inline void	place_image_remove(B32 id)
{
	chained_t	*upd;
	chained_t	*prev;
	chained_t	*next;

	prev = NULL;
	upd = place_get()->gear->img;
	while (upd)
	{
		next = upd->next;
		prev = upd->prev;
		if (upd->data && ((image_t *)upd->data)->id == id)
		{
			image_pop(upd->data, place_get()->gear->up);
			if (prev)
			{
				prev->next = next;
				if (next)
					next->prev = prev;
				upd = prev;
				break ;
			}
			else
			{
				if (next)
					printf(">>%i\n", ((image_t *)next->data)->id);
				else
					printf("fucking\n");
				// upd = next;
				// upd->prev = NULL;
			}
		}
		upd = next;
	}
}

static inline void	place_image_pop(void)
{
	chained_pop(&place_get()->gear->img, place_get()->gear->up, image_pop);
}

static inline font_t	*place_font_load(B8 *path, B32 id, B32 size)
{
	system_t	*up;
	pixel_t		color;

	up = &*(place_get()->gear->up);
	color = pixel_rgb_start(255, 255, 255);
	switch (up->system)
	{
		case SYSTEM_CONSOLE:
		break ;
		case SYSTEM_PRIMARY:
		break ;
		case SYSTEM_ALLEGRO:
			return (font_load(id, size, path, &color, up));
		case SYSTEM_SDL:
		break ;
		case SYSTEM_MAIN_NONE:
		break ;
	}
	return (NULL);
}

static inline font_t	*place_font_select(B32 id)
{
	chained_t	*update;
	font_t		*font;

	update = place_get()->gear->font;
	while (update)
	{
		font = update->data;
		if (font && font->id == id)
			return (font);
		update = update->next;
	}
	return (NULL);
}


static inline void	place_font_next_first(font_t *font)
{
	if (chained_next_first(&place_get()->gear->font, chained_push(font)))
	{}
}

static inline void	place_font_next_last(font_t *font)
{
	if (chained_next_last(&place_get()->gear->font, chained_push(font)))
	{}
}

static inline void	place_font_pop(void)
{
	chained_pop(&place_get()->gear->font, place_get()->gear->up, font_pop);
}


static inline sound_t	*place_sound_load(B8 *path, B32 id, play_type_t type, play_mode_t mode)
{
	machine_t	*gear;

	gear = &*(place_get()->gear);
	switch (gear->up->audio)
	{
		case SYSTEM_AUDIO_PRIMARY:
		break ;
		case SYSTEM_AUDIO_ALLEGRO:
			return (sound_load(path, id, type, mode, *(gear->audio), gear->up));
		break ;
		case SYSTEM_AUDIO_SDL:
		break ;
		case SYSTEM_AUDIO_NONE:
		break ;
	}
	return (NULL);
}

static inline sound_t	*place_sound_select(B32 id)
{
	chained_t	*update;
	sound_t		*sound;

	update = place_get()->gear->snd;
	while (update)
	{
		sound = update->data;
		if (sound && sound->id == id)
			return (sound);
		update = update->next;
	}
	return (NULL);
}

static inline void	place_sound_next_first(sound_t *sound)
{
	if (chained_next_first(&place_get()->gear->snd, chained_push(sound)))
	{}
}

static inline void	place_sound_next_last(sound_t *sound)
{
	if (chained_next_last(&place_get()->gear->snd, chained_push(sound)))
	{}
}

static inline status_t	place_sound_play(sound_t *sound)
{
	if (!sound)
		return (Off);
	sound_play(sound, place_get()->gear->up);
	return (On);
}

static inline void	place_sound_pop(void)
{
	chained_pop(&place_get()->gear->snd, place_get()->gear->up, sound_pop);
}

static inline object_t	*place_object_push(B32 id, char *name, vf2d pos, vf2d vel, vf2d zoom, image_t *image, pixel_t color)
{
	object_t	*set;

	set = malloc(sizeof(object_t));
	if (!set)
		return (NULL);
	set->id = id;
	set->name = strdup(name);
	*(set->size) = !image ? (vf2d){16, 16} : image_size(image, place_get()->gear->up);
	set->image = image ? image_push(0, image->flip, image->data) : NULL;
	*(set->pos) = *(set->route) = pos;
	*(set->vel) = vel;
	*(set->zoom) = zoom;
	*(set->axis) = (vf2d){0, 0};
	set->angle = 0;
	*(set->color) = color;
	return (set);
}

static inline object_t	*place_object_select(B32 id)
{
	chained_t	*update;
	object_t	*object;

	update = place_get()->gear->obj;
	while (update)
	{
		object = update->data;
		if (object->id == id)
			return (object);
		update = update->next;
	}
	return (NULL);
}

static inline void	place_object_next_first(object_t *object)
{
	if (chained_next_first(&place_get()->gear->obj, chained_push(object)))
	{}
}

static inline void	place_object_next_last(object_t *object)
{
	if (chained_next_last(&place_get()->gear->obj, chained_push(object)))
	{}
}

static inline float	place_object_width(object_t *object)
{
	return (object ? object->size->x * object->zoom->x : 0.0);
}

static inline float	place_object_height(object_t *object)
{
	return (object ? object->size->y * object->zoom->y : 0.0);
}

static inline void	place_object_zoom_fit(object_t *object, vf2d size)
{
	if (!object)
		return ;
	object->zoom->x = size.x / object->size->x;
	object->zoom->y = size.y / object->size->y;
}

static inline void	place_object_pop(void)
{
	chained_pop(&place_get()->gear->obj, place_get()->gear->up, object_pop);
}

static inline void	place_stop(void)
{
	place_get()->gear->event &= ~(1 << MACHINE_RUNNING);
}

static inline void	place_destroy(place_t *set)
{
	set->gear->pop(set->gear);
}

static inline void	place_function(place_t *set)
{
	set->run = place_run;
	set->resize = place_resize;
	set->search_res = place_search_res;
	set->prev_res = place_previos_res;
	set->next_res = place_next_res;
	set->map_set = place_map_set;
	set->image_load = place_image_load;
	set->image_text_create = place_image_text_create;
	set->image_select = place_image_select;
	set->image_size = place_image_size;
	set->image_next_first = place_image_next_first;
	set->image_next_last = place_image_next_last;
	set->image_remove = place_image_remove;
	set->image_pop = place_image_pop;
	set->font_load = place_font_load;
	set->font_select = place_font_select;
	set->font_next_first = place_font_next_first;
	set->font_next_last = place_font_next_last;
	set->font_pop = place_font_pop;
	set->sound_load = place_sound_load;
	set->sound_select = place_sound_select;
	set->sound_next_first = place_sound_next_first;
	set->sound_next_last = place_sound_next_last;
	set->sound_play = place_sound_play;
	set->sound_pop = place_sound_pop;
	set->object_push = place_object_push;
	set->object_select = place_object_select;
	set->object_next_first = place_object_next_first;
	set->object_next_last = place_object_next_last;
	set->object_width = place_object_width;
	set->object_height = place_object_height;
	set->object_zoom_fit = place_object_zoom_fit;
	set->object_pop = place_object_pop;
	set->start = NULL;
	set->camera_object = place_camera_object;
	set->camera_mouse = place_camera_mouse;
	set->mouse_down = place_mouse_down;
	set->mouse_release = place_mouse_release;
	set->mouse_over = place_mouse_over;
	set->key_down = place_key_down;
	set->create = NULL;
	set->update = NULL;
	set->draw = NULL;
	set->stop = place_stop;
	set->destroy = place_destroy;
}