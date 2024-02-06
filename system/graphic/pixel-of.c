#include <place/system/graphic/pixel-of.h>
#include <stdlib.h>

PIXEL_OF_CALL pixel_t	PIXEL_OF_TYPE pixel_start(void)
{
	return ((pixel_t){255, 255, 255, 255});
}

PIXEL_OF_CALL pixel_t	*PIXEL_OF_TYPE pixel_set(B8U r, B8U g, B8U b, B8U a)
{
	pixel_t	*set;

	set = malloc(sizeof(pixel_t));
	if (!set)
		return (NULL);
	*set = (pixel_t){r, g, b, a};
	return (set);
}

PIXEL_OF_CALL pixel_t PIXEL_OF_TYPE pixel_rgb_start(B8U r, B8U g, B8U b)
{
	return ((pixel_t){r, g, b, 255});
}

PIXEL_OF_CALL pixel_t PIXEL_OF_TYPE pixel_rgba_start(B8U r, B8U g, B8U b, B8U a)
{
	return ((pixel_t){r, g, b, a});
}

PIXEL_OF_CALL void PIXEL_OF_TYPE pixel_rgb_set(pixel_t * p, B8U r, B8U g, B8U b)
{
	*p = (pixel_t){r, g, b, 255};
}

PIXEL_OF_CALL void PIXEL_OF_TYPE pixel_rgba_set(pixel_t * p, B8U r, B8U g, B8U b, B8U a)
{
	*p = (pixel_t){r, g, b, a};
}

PIXEL_OF_CALL void PIXEL_OF_TYPE pixel_of_show_console(pixel_t * p)
{
	printf("R[%i] G[%i] B[%i] A[%i]\n", p->r, p->g, p->b, p->a);
}

PIXEL_OF_CALL void PIXEL_OF_TYPE pixel_destroy(pixel_t * p)
{
	if (!p)
		return ;
	free(p);
}
