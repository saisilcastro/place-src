#include <place/system/mouse-of.h>

MOUSE_CALL void MOUSE_TYPE mouse_set(mouse_t *mouse)
{
	if (!mouse)
		return ;
	mouse->x = 0;
	mouse->y = 0;
	mouse->button = 0x0;
	mouse->wheel = 0x0;
}