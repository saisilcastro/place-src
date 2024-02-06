#include <place/system/object-of.h>
#include <string.h>
#include <stdlib.h>

OBJECT_OF_CALL object_t *OBJECT_OF_TYPE object_push(B32 id, char *name, vf2d pos, vf2d vel, vf2d zoom, image_t *image, pixel_t color, system_t *up)
{
	object_t	*set;

	set = malloc(sizeof(object_t));
	if (!set)
		return (NULL);
	set->id = id;
	set->name = strdup(name);
	*(set->size) = !image ? (vf2d){16, 16} : image_size(image, up);
	*(set->pos) = *(set->route) = pos;
	*(set->vel) = vel;
	*(set->zoom) = zoom;
	*(set->axis) = (vf2d){0, 0};
	set->angle = 0;
	set->image = image_clone(image, up);
	*(set->color) = color;
	return (set);
}

OBJECT_OF_CALL void OBJECT_OF_TYPE	object_set(object_t *set, B32 id, char *name, vf2d pos, vf2d vel, vf2d size, vf2d zoom, image_t *image, pixel_t color, system_t *up)
{
	if (!set)
		return ;
	set->id = id;
	set->name = strdup(name);
	*(set->size) = !image ? size : image_size(image, up);
	set->image = image_clone(image, up);
	*(set->pos) = *(set->route) = pos;
	*(set->vel) = vel;
	*(set->zoom) = zoom;
	*(set->axis) = (vf2d){0, 0};
	set->angle = 0;
	*(set->color) = color;
}

OBJECT_OF_CALL status_t OBJECT_OF_TYPE	object_move(object_t *set, long time)
{
	if (!set)
		return (Off);
	if (set->pos->x == set->route->x && set->pos->y == set->route->y)
		return (Off);
	if (set->pos->x == set->route->x)
	{
		if (set->pos->y < set->route->y)
		{
			set->pos->y += set->vel->y * time;
			if (set->pos->y > set->route->y)
			{
				set->pos->y = set->route->y;
				return (Off);
			}
		}
		if (set->pos->y > set->route->y)
		{
			set->pos->y -= set->vel->y * time;
			if (set->pos->y < set->route->y)
			{
				set->pos->y = set->route->y;
				return (Off);
			}
		}
	}
	if (set->pos->y == set->route->y)
	{
		if (set->pos->x < set->route->x)
		{
			set->pos->x += set->vel->x * time;
			if (set->pos->x > set->route->x)
			{
				set->pos->x = set->route->x;
				return (Off);
			}
		}
		if (set->pos->x > set->route->x)
		{
			set->pos->x -= set->vel->x * time;
			if (set->pos->x < set->route->x)
			{
				set->pos->x = set->route->x;
				return (Off);
			}
		}
	}
	if (set->pos->x < set->route->x)
	{
		set->pos->x += set->vel->x * time;
		if (set->pos->y < set->route->y)
		{
			set->pos->y += set->vel->y * time;
			if (set->pos->y > set->route->y)
				set->pos->y = set->route->y;
		}
		if (set->pos->y > set->route->y)
		{
			set->pos->y -= set->vel->y * time;
			if (set->pos->y < set->route->y)
				set->pos->y = set->route->y;
		}
	}
	if (set->pos->x > set->route->x)
	{
		set->pos->x -= set->vel->x * time;
		if (set->pos->y < set->route->y)
		{
			set->pos->y += set->vel->y * time;
			if (set->pos->y > set->route->y)
				set->pos->y = set->route->y;
		}
		if (set->pos->y > set->route->y)
		{
			set->pos->y -= set->vel->y * time;
			if (set->pos->y < set->route->y)
				set->pos->y = set->route->y;
		}
	}
	return (On);
}

OBJECT_OF_CALL void		OBJECT_OF_TYPE object_pop(void *data, system_t *up)
{
	object_t	*obj;

	if (!data)
		return ;
	(void)up;
	obj = data;
	if (obj->name)
		free(obj->name);
	if (obj->image)
		image_remove(obj->image);
	free(obj);
}