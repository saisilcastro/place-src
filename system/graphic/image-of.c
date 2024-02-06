#include <place/system/graphic/image-of.h>
#include <place/plugin/graphic/allegro-plugin.h>

IMAGE_OF_CALL image_t	*IMAGE_OF_TYPE image_push(B32 id, image_flip_t flip, void *data)
{
	image_t	*set;

	set = malloc(sizeof(image_t));
	if (!set)
		return (NULL);
	set->id = id;
	set->data = data;
	set->flip = flip;
	return (set);
}

IMAGE_OF_CALL image_t *IMAGE_OF_TYPE	image_create(B32 id, vf2d size, system_t *up)
{
	image_t	*set;

	if (!up)
		return (NULL);
	set = malloc(sizeof(image_t));
	if (!set)
		return (NULL);
	set->id = id;
	set->flip = IMAGE_FLIP_NONE;
	set->data = NULL;
	switch (up->system)
	{
		case SYSTEM_CONSOLE:
		break ;
		case SYSTEM_PRIMARY:
		break ;
		case SYSTEM_ALLEGRO:
			set->data = al_create_bitmap(size.x, size.y);
		break ;
		case SYSTEM_SDL:
		break ;
		case SYSTEM_MAIN_NONE:
		break ;
	}
	return (set);
}

IMAGE_OF_CALL image_t	*IMAGE_OF_TYPE image_load(char * path, B32 id, image_flip_t flip, system_t *up)
{
	image_t	*set;

	set = malloc(sizeof(image_t));
	if (!set)
		return (NULL);
	set->id = id;
	set->flip = flip;
	switch (up->system)
	{
		case SYSTEM_CONSOLE:
			set->data = NULL;
		break ;
		case SYSTEM_PRIMARY:
			set->data = NULL;
		break ;
		case SYSTEM_ALLEGRO:
			set->data = al_load_bitmap(path);
		break ;
		case SYSTEM_SDL:
			set->data = NULL;
		break ;
		case SYSTEM_MAIN_NONE:
			set->data = NULL;
		break ;
	}
	//set->data = stbi_load(path, &set->w, &set->h, &set->bpp, 0);
	return (set);
}

IMAGE_OF_CALL image_t *IMAGE_OF_TYPE image_clone(image_t *set, system_t *up)
{
	image_t	*clone;

	if (!set || !up)
		return (NULL);
	clone = image_push(set->id, set->flip, NULL);
	switch (up->system)
	{
		case SYSTEM_CONSOLE:
		break ;
		case SYSTEM_PRIMARY:
		break ;
		case SYSTEM_ALLEGRO:
			clone->data = al_clone_bitmap(set->data);
		break ;
		case SYSTEM_SDL:
		break ;
		case SYSTEM_MAIN_NONE:
		break ;
	}
	return (clone);
}

IMAGE_OF_CALL void IMAGE_OF_TYPE	image_target_self(image_t *image, system_t *up)
{
	if (!image || !image->data || !up)
		return ;
	switch (up->system)
	{
		case SYSTEM_CONSOLE:
		case SYSTEM_PRIMARY:
		break ;
		case SYSTEM_ALLEGRO:
			al_set_target_bitmap (image->data);
		break ;
		case SYSTEM_SDL:
		case SYSTEM_MAIN_NONE:
		break ;
	}
}

IMAGE_OF_CALL void IMAGE_OF_TYPE	image_target_window(void * plugin, system_t *up)
{
	if (!plugin || !up)
		return ;
	switch (up->system)
	{
		case SYSTEM_CONSOLE:
		case SYSTEM_PRIMARY:
		break ;
		case SYSTEM_ALLEGRO:
			al_set_target_backbuffer (((allegro_plugin_t *)plugin)->window);
		break ;
		case SYSTEM_SDL:
		case SYSTEM_MAIN_NONE:
		break ;
	}
}

IMAGE_OF_CALL void IMAGE_OF_TYPE image_draw(image_t *set, pixel_t *color, vf2d center, vf2d pos, vf2d zoom, float angle, system_t *up)
{
	if (!set || !set->data || !color || !up)
		return ;
	switch (up->system)
	{
		case SYSTEM_CONSOLE:
		case SYSTEM_PRIMARY:
		break ;
		case SYSTEM_ALLEGRO:
			al_draw_tinted_scaled_rotated_bitmap(set->data, al_map_rgba(color->r, color->g, color->b, color->a), center.x, center.y, pos.x, pos.y, zoom.x, zoom.y, angle, set->flip);
		break;
		case SYSTEM_SDL:
		case SYSTEM_MAIN_NONE:
		break ;
	}
}

IMAGE_OF_CALL void IMAGE_OF_TYPE	image_put_pixel(pixel_t *color, vf2d pos, system_t *up)
{
	if (!color || !up)
		return ;
	switch (up->system)
	{
		case SYSTEM_CONSOLE:
		case SYSTEM_PRIMARY:
		break ;
		case SYSTEM_ALLEGRO:
			al_put_pixel(pos.x, pos.y, al_map_rgba(color->r, color->g, color->b, color->a));
		break;
		case SYSTEM_SDL:
		case SYSTEM_MAIN_NONE:
		break ;
	}
}

IMAGE_OF_CALL vf2d IMAGE_OF_TYPE	image_size(image_t *set, system_t *up)
{
	vf2d	size = {0, 0};

	if (!set || !set->data || !up)
		return (size);
	switch (up->system)
	{
		case SYSTEM_CONSOLE:
		case SYSTEM_PRIMARY:
		break ;
		case SYSTEM_ALLEGRO:
		{
			size.x = al_get_bitmap_width(set->data);
			size.y = al_get_bitmap_height(set->data);
		}
		break;
		case SYSTEM_SDL:
		case SYSTEM_MAIN_NONE:
		break ;
	}
	return (size);
}

IMAGE_OF_CALL void IMAGE_OF_TYPE image_remove(image_t *set)
{
	if (!set)
		return ;
	free(set);
}

IMAGE_OF_CALL void	IMAGE_OF_TYPE image_pointer_pop(void **set, system_t *up)
{
	if (!*set)
		return ;
	switch (up->system)
	{
		case SYSTEM_CONSOLE:
		case SYSTEM_PRIMARY:
		{
			printf("removing primary image\n");
			if (((image_t *)set)->data)
			 	free(((image_t *)set)->data);
		}
		break ;
		case SYSTEM_ALLEGRO:
		{
			al_destroy_bitmap(((image_t *)*set)->data);
			((image_t *)*set)->data = NULL;
		}
		break ;
		case SYSTEM_SDL:
		case SYSTEM_MAIN_NONE:
		break ;
	}
	free(*set);
	*set = NULL;
}

IMAGE_OF_CALL void	IMAGE_OF_TYPE image_pop(void *data, system_t *up)
{
	image_t	*set;

	set = data;
	if (!set)
		return ;
	switch (up->system)
	{
		case SYSTEM_CONSOLE:
		case SYSTEM_PRIMARY:
		{
			printf("removing primary image\n");
			if (set->data)
			 	free(set->data);
		}
		break ;
		case SYSTEM_ALLEGRO:
		{
			al_destroy_bitmap(set->data);
			set->data = NULL;
		}
		break ;
		case SYSTEM_SDL:
		case SYSTEM_MAIN_NONE:
		break ;
	}
	free(set);
	set = NULL;
}