#include <place/plugin/graphic/allegro-plugin.h>

static inline keyboard_map	allegro_to_key(int key);

ALLEGRO_PLUGIN_CALL allegro_plugin_t *ALLEGRO_PLUGIN_TYPE	allegro_plugin_push(void)
{
	allegro_plugin_t	*plugin;

	plugin = malloc(sizeof(allegro_plugin_t));
	if (!plugin)
		return (NULL);
	plugin->window = NULL;
	plugin->event_row = NULL;
	return (plugin);
}

static inline void	allegro_joystick_start(machine_t *gear, allegro_plugin_t *plugin)
{
	(void)gear;
	if (!al_install_joystick ())
	{
		printf("joystick not started");
		return ;
	}
	al_reconfigure_joysticks ();
	if (al_get_num_joysticks () > 0) {
	    // ALL_CAST(machine->plugin)->joystick = al_get_joystick (al_get_num_joysticks () - 1);
	    // gear->joystick.button_max = al_get_joystick_num_buttons (
	    // ALL_CAST(machine->plugin)->joystick) + 1;
	    // machine->joystick.button = (STATUS *) calloc (machine->joystick.button_max, sizeof(STATUS));
	    al_register_event_source (plugin->event_row, al_get_joystick_event_source ());
	}
}

static inline void	allegro_subsystem(machine_t *gear)
{
	if (gear->up->audio == SYSTEM_AUDIO_ALLEGRO)
	{
		if (!al_install_audio ())
			printf ("could not init audio\n");
		if (!al_init_acodec_addon ())
			printf ("could not init audio codec");
		if (!al_reserve_samples (64))
			printf ("could not reserve samples\n");
	}
#if (defined(WIN32) || defined(WINDOWS_XP))
    if (!al_init_video_addon ())
	printf ("could not init video addon\n");
#endif
    printf ("allegro subsystem started\n");
}

static inline status_t	allegro_video_start(machine_t *gear, allegro_plugin_t *plugin)
{
	al_set_new_window_title (gear->title);
	plugin->window = al_create_display(gear->size->x, gear->size->y);
	if (!plugin->window)
		return (Off);
	return (On);
}

ALLEGRO_PLUGIN_CALL status_t	ALLEGRO_PLUGIN_TYPE allegro_start(machine_t *gear)
{
	allegro_plugin_t	*plugin;

	if (!gear || !al_init())
		return (Off);
	allegro_subsystem(gear);
	if (!al_install_keyboard ()) return (Off);
	if (!al_install_mouse ()) return (Off);
	plugin = gear->plugin;
	if (!allegro_video_start(gear, plugin))
		return (Off);
	allegro_joystick_start(gear, plugin);
	al_init_user_event_source (&plugin->event_fake);
	if (!al_init_image_addon ()) return (Off);
	al_init_font_addon ();
	if (!al_init_ttf_addon ()) return (Off);
	plugin->event_row = al_create_event_queue();
	if (!plugin->event_row)
		return (Off);
	al_register_event_source(plugin->event_row, al_get_display_event_source(plugin->window));
	al_register_event_source (plugin->event_row, al_get_keyboard_event_source ());
	al_register_event_source (plugin->event_row, al_get_mouse_event_source ());
	al_register_event_source (plugin->event_row, &plugin->event_fake);
	return (On);
}

ALLEGRO_PLUGIN_CALL status_t ALLEGRO_PLUGIN_TYPE	allegro_plugin_event(machine_t *gear, vf2d offset)
{
	allegro_plugin_t	*plugin;
	ALLEGRO_EVENT		event;

	if (!gear)
		return (Off);
	(void)offset;
	plugin = gear->plugin;
	if (al_is_mouse_installed())
	{
		ALLEGRO_MOUSE_STATE mstate;

		al_get_mouse_state(&mstate);
		gear->mouse->x = mstate.x + offset.x;
		gear->mouse->y = mstate.y + offset.y;
	}
	while (al_get_next_event (plugin->event_row, &event)) {
		switch (event.type)
		{
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				gear->event &= ~(1 << MACHINE_RUNNING);
			break ;
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: {
				switch(event.mouse.button)
				{
					case 1:
						gear->mouse->button |= (1 << MACHINE_MOUSE_LEFT);
					break ;
					case 2:
						gear->mouse->button |= (1 << MACHINE_MOUSE_RIGHT);
					break ;
					case 3:
						gear->mouse->button |= (1 << MACHINE_MOUSE_MIDDLE);
					break ;
				}
			}
			break ;
			case ALLEGRO_EVENT_MOUSE_BUTTON_UP: {
				switch(event.mouse.button)
				{
					case 1:
						gear->mouse->button &= ~(1 << MACHINE_MOUSE_LEFT);
					break ;
					case 2:
						gear->mouse->button &= ~(1 << MACHINE_MOUSE_RIGHT);
					break ;
					case 3:
						gear->mouse->button &= ~(1 << MACHINE_MOUSE_MIDDLE);
					break ;
				}
			}
			break ;
			case ALLEGRO_EVENT_KEY_DOWN:
				gear->key[allegro_to_key(event.keyboard.keycode)] = On;
			break ;
			case ALLEGRO_EVENT_KEY_UP:
				gear->key[allegro_to_key(event.keyboard.keycode)] = Off;
			break ;
		}
	}
	return (gear->event & (1 << MACHINE_RUNNING));
}

ALLEGRO_PLUGIN_CALL void ALLEGRO_PLUGIN_TYPE	allegro_plugin_pop(allegro_plugin_t *plugin)
{
	if (plugin)
	{
		if (plugin->window)
			al_destroy_display(plugin->window);
		if (plugin->event_row)
			al_destroy_event_queue(plugin->event_row);
	}
	al_destroy_user_event_source (&plugin->event_fake);
    al_shutdown_ttf_addon ();
    al_shutdown_font_addon ();
    //al_shutdown_primitives_addon ();
    al_shutdown_image_addon ();
    al_uninstall_keyboard ();
    al_uninstall_mouse ();
    al_uninstall_joystick ();
	al_uninstall_audio();
}

static inline keyboard_map	allegro_to_key(int key)
{
	switch (key)
	{
		case ALLEGRO_KEY_A:
			return (KEY_A);
		case ALLEGRO_KEY_B:
			return (KEY_B);
		case ALLEGRO_KEY_C:
			return (KEY_C);
		case ALLEGRO_KEY_D:
			return (KEY_D);
		case ALLEGRO_KEY_E:
			return (KEY_E);
		case ALLEGRO_KEY_F:
			return (KEY_F);
		case ALLEGRO_KEY_G:
			return (KEY_G);
		case ALLEGRO_KEY_H:
			return (KEY_H);
		case ALLEGRO_KEY_I:
			return (KEY_I);
		case ALLEGRO_KEY_J:
			return (KEY_J);
		case ALLEGRO_KEY_K:
			return (KEY_K);
		case ALLEGRO_KEY_L:
			return (KEY_L);
		case ALLEGRO_KEY_M:
			return (KEY_M);
		case ALLEGRO_KEY_N:
			return (KEY_N);
		case ALLEGRO_KEY_O:
			return (KEY_O);
		case ALLEGRO_KEY_P:
			return (KEY_P);
		case ALLEGRO_KEY_Q:
			return (KEY_Q);
		case ALLEGRO_KEY_R:
			return (KEY_R);
		case ALLEGRO_KEY_S:
			return (KEY_S);
		case ALLEGRO_KEY_T:
			return (KEY_T);
		case ALLEGRO_KEY_U:
			return (KEY_U);
		case ALLEGRO_KEY_V:
			return (KEY_V);
		case ALLEGRO_KEY_W:
			return (KEY_W);
		case ALLEGRO_KEY_X:
			return (KEY_X);
		case ALLEGRO_KEY_Y:
			return (KEY_Y);
		case ALLEGRO_KEY_Z:
			return (KEY_Z);
		case ALLEGRO_KEY_0:
			return (KEY_0);
		case ALLEGRO_KEY_1:
			return (KEY_1);
		case ALLEGRO_KEY_2:
			return (KEY_2);
		case ALLEGRO_KEY_3:
			return (KEY_3);
		case ALLEGRO_KEY_4:
			return (KEY_4);
		case ALLEGRO_KEY_5:
			return (KEY_5);
		case ALLEGRO_KEY_6:
			return (KEY_6);
		case ALLEGRO_KEY_7:
			return (KEY_7);
		case ALLEGRO_KEY_8:
			return (KEY_8);
		case ALLEGRO_KEY_9:
			return (KEY_9);
		case ALLEGRO_KEY_PAD_0:
			return (KEY_PAD_0);
		case ALLEGRO_KEY_PAD_1:
			return (KEY_PAD_1);
		case ALLEGRO_KEY_PAD_2:
			return (KEY_PAD_2);
		case ALLEGRO_KEY_PAD_3:
			return (KEY_PAD_3);
		case ALLEGRO_KEY_PAD_4:
			return (KEY_PAD_4);
		case ALLEGRO_KEY_PAD_5:
			return (KEY_PAD_5);
		case ALLEGRO_KEY_PAD_6:
			return (KEY_PAD_6);
		case ALLEGRO_KEY_PAD_7:
			return (KEY_PAD_7);
		case ALLEGRO_KEY_PAD_8:
			return (KEY_PAD_8);
		case ALLEGRO_KEY_PAD_9:
			return (KEY_PAD_9);
		case ALLEGRO_KEY_F1:
	    	return (KEY_F1);
		case ALLEGRO_KEY_F2:
	    	return (KEY_F2);
		case ALLEGRO_KEY_F3:
	    	return (KEY_F3);
		case ALLEGRO_KEY_F4:
	    	return (KEY_F4);
		case ALLEGRO_KEY_F5:
	    	return (KEY_F5);
		case ALLEGRO_KEY_F6:
	    	return (KEY_F6);
		case ALLEGRO_KEY_F7:
	    	return (KEY_F7);
		case ALLEGRO_KEY_F8:
	    	return (KEY_F8);
		case ALLEGRO_KEY_F9:
	    	return (KEY_F9);
		case ALLEGRO_KEY_F10:
	    	return (KEY_F10);
		case ALLEGRO_KEY_F11:
	    	return (KEY_F11);
		case ALLEGRO_KEY_F12:
	    	return (KEY_F12);
		case ALLEGRO_KEY_ESCAPE:
	    	return (KEY_ESC);
		case ALLEGRO_KEY_TILDE:
	    	return (KEY_TILDE);
		case ALLEGRO_KEY_MINUS:
	    	return (KEY_MINUS);
		case ALLEGRO_KEY_EQUALS:
	    	return (KEY_EQUALS);
		case ALLEGRO_KEY_BACKSPACE:
	    	return (KEY_BACKSPACE);
		case ALLEGRO_KEY_TAB:
	    	return (KEY_TAB);
		case ALLEGRO_KEY_OPENBRACE:
	    	return (KEY_OPENBRACE);
		case ALLEGRO_KEY_CLOSEBRACE:
	    	return (KEY_CLOSEBRACE);
		case ALLEGRO_KEY_ENTER:
	    	return (KEY_ENTER);
		case ALLEGRO_KEY_SEMICOLON:
	    	return (KEY_SEMICOLON);
		case ALLEGRO_KEY_SEMICOLON2:
	    	return (KEY_SEMICOLON2);
		case ALLEGRO_KEY_QUOTE:
	    	return (KEY_QUOTE);
		case ALLEGRO_KEY_BACKSLASH:
	    	return (KEY_BACKSLASH);
		case ALLEGRO_KEY_BACKSLASH2:
	    	return (KEY_BACKSLASH2);
		case ALLEGRO_KEY_FULLSTOP:
	    	return (KEY_FULLSTOP);
		case ALLEGRO_KEY_SLASH:
	    	return (KEY_SLASH);
		case ALLEGRO_KEY_SPACE:
	    	return (KEY_SPACE);
		case ALLEGRO_KEY_INSERT:
	    	return (KEY_INSERT);
		case ALLEGRO_KEY_DELETE:
	    	return (KEY_DELETE);
		case ALLEGRO_KEY_HOME:
	    	return (KEY_HOME);
		case ALLEGRO_KEY_END:
	    	return (KEY_END);
		case ALLEGRO_KEY_PGUP:
	    	return (KEY_PGUP);
		case ALLEGRO_KEY_PGDN:
	    	return (KEY_PGDN);
		case ALLEGRO_KEY_LEFT:
	    	return (KEY_LEFT);
		case ALLEGRO_KEY_RIGHT:
	    	return (KEY_RIGHT);
		case ALLEGRO_KEY_UP:
	    	return (KEY_UP);
		case ALLEGRO_KEY_DOWN:
	    	return (KEY_DOWN);
		case ALLEGRO_KEY_PAD_SLASH:
	    	return (KEY_PAD_SLASH);
		case ALLEGRO_KEY_PAD_ASTERISK:
	    	return (KEY_PAD_ASTERISK);
		case ALLEGRO_KEY_PAD_MINUS:
	    	return (KEY_PAD_MINUS);
		case ALLEGRO_KEY_PAD_PLUS:
	    	return (KEY_PAD_PLUS);
		case ALLEGRO_KEY_PAD_DELETE:
	    	return (KEY_PAD_DELETE);
		case ALLEGRO_KEY_PAD_ENTER:
	    	return (KEY_PAD_ENTER);
		case ALLEGRO_KEY_PRINTSCREEN:
	    	return (KEY_PRINTSCREEN);
		case ALLEGRO_KEY_PAUSE:
	    	return (KEY_PAUSE);
		case ALLEGRO_KEY_ABNT_C1:
	    	return (KEY_ABNT_C1);
		case ALLEGRO_KEY_YEN:
	    	return (KEY_YEN);
		case ALLEGRO_KEY_KANA:
	    	return (KEY_KANA);
		case ALLEGRO_KEY_CONVERT:
	    	return (KEY_CONVERT);
		case ALLEGRO_KEY_NOCONVERT:
	    	return (KEY_NOCONVERT);
		case ALLEGRO_KEY_AT:
	    	return (KEY_AT);
		case ALLEGRO_KEY_CIRCUMFLEX:
	    	return (KEY_CIRCUMFLEX);
		case ALLEGRO_KEY_COLON2:
	    	return (KEY_COLON2);
		case ALLEGRO_KEY_KANJI:
	    	return (KEY_KANJI);
		case ALLEGRO_KEY_PAD_EQUALS:
	    	return (KEY_PAD_EQUALS);
		case ALLEGRO_KEY_BACKQUOTE:
	    	return (KEY_BACKQUOTE);
		case ALLEGRO_KEY_COMMAND:
	    	return (KEY_COMMAND);
		case ALLEGRO_KEY_UNKNOWN:
	    	return (KEY_UNKNOWN);
		case ALLEGRO_KEY_LSHIFT:
	    	return (KEY_LSHIFT);
		case ALLEGRO_KEY_LCTRL:
	    	return (KEY_LCTRL);
		case ALLEGRO_KEY_RCTRL:
	    	return (KEY_RCTRL);
		case ALLEGRO_KEY_ALT:
	    	return (KEY_LALT);
		case ALLEGRO_KEY_ALTGR:
	    	return (KEY_RALT);
		case ALLEGRO_KEY_LWIN:
	    	return (KEY_LWIN);
		case ALLEGRO_KEY_RWIN:
	    	return (KEY_RWIN);
		case ALLEGRO_KEY_MENU:
	    	return (KEY_MENU);
		case ALLEGRO_KEY_SCROLLLOCK:
	    	return (KEY_SCROLLLOCK);
		case ALLEGRO_KEY_NUMLOCK:
	    	return (KEY_NUMLOCK);
		case ALLEGRO_KEY_CAPSLOCK:
	    	return (KEY_CAPSLOCK);
	}
	return (KEY_NONE);
}

ALLEGRO_PLUGIN_CALL void ALLEGRO_PLUGIN_TYPE	allegro_search_res(vf2d *size, int *pos)
{
	ALLEGRO_DISPLAY_MODE	info;
	int						i;

	i = -1;
	i = al_get_num_display_modes ();
	while (--i >= 0)
	{
		if (al_get_display_mode (i, &info) != NULL)
		{
			if (size->x == info.width && size->y == info.height)
				*pos = i;
		}
	}
}

ALLEGRO_PLUGIN_CALL status_t ALLEGRO_PLUGIN_TYPE	allegro_next_res(vf2d *size, int *pos)
{
	ALLEGRO_DISPLAY_MODE	info;
	int						max;

	max = al_get_num_display_modes () -1;
	if (al_get_display_mode (*pos, &info) != NULL)
	{
		if (*pos > max)
		{
			*pos = 0;
			al_get_display_mode (*pos, &info);
			size->x = info.width;
			size->y = info.height;
			return (On);
		}
		while ((size->x == info.width && size->y == info.height))
		{
			if (al_get_display_mode (*pos, &info) == NULL || *pos > max)
				break ;
			*pos += 1;
		}
		size->x = info.width;
		size->y = info.height;
		return (On);
	}
	return (Off);
}

ALLEGRO_PLUGIN_CALL status_t ALLEGRO_PLUGIN_TYPE	allegro_prev_res(vf2d *size, int *pos)
{
	ALLEGRO_DISPLAY_MODE	info;
	int						max;

	max = al_get_num_display_modes () -1;
	if (al_get_display_mode (*pos, &info) != NULL)
	{
		if (*pos < 0)
		{
			*pos = max;
			al_get_display_mode (*pos, &info);
			size->x = info.width;
			size->y = info.height;
			return (On);
		}
		while ((size->x == info.width && size->y == info.height))
		{
			if (al_get_display_mode (*pos, &info) == NULL || *pos > max)
				break ;
			*pos -= 1;
		}
		size->x = info.width;
		size->y = info.height;
		return (On);
	}
	return (Off);
}