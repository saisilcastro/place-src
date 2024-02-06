#include <place/system/sound-of.h>
#include <place/plugin/graphic/allegro-plugin.h>

SOUND_CALL sound_t *SOUND_TYPE	sound_push(B32 id, BP32 music, BP32 effect, BP32 balance, BP32 speed, play_mode_t mode, void *data)
{
	sound_t	*set;

	set = malloc(sizeof(sound_t));
	if (!set)
		return (NULL);
	set->id = id;
	set->volume->music = music;
	set->volume->effect = effect;
	set->balance = balance;
	set->speed = speed;
	set->type = PLAY_EFFECT;
	set->mode = mode;
	set->data = data;
	return (set);
}

SOUND_CALL sound_t *SOUND_TYPE	sound_load(char *path, B32 id, play_type_t type, play_mode_t mode, audio_t audio, system_t *up)
{
	sound_t	*set;

	set = malloc(sizeof(sound_t));
	if (!set)
		return (NULL);
	set->id = id;
	set->volume->music = 1.0;
	set->volume->effect = 1.0;
	set->balance = 0.0;
	set->speed = 1.0;
	set->type = type;
	set->mode = mode;
	switch (up->audio)
	{
		case SYSTEM_AUDIO_ALLEGRO:
		{
			if (al_is_audio_installed())
			{
				if (type == PLAY_EFFECT)
					set->data = al_load_sample (path);
				else
				{
					set->data = al_load_audio_stream (path, audio.channel, audio.chunk_size);
					if (set->data)
						al_attach_audio_stream_to_mixer (set->data, al_get_default_mixer ());
				}
			}
			else
				set->data = NULL;
		}
		break ;
		default:
		{
			printf("audio standard\n");
			set->data = NULL;
		}
		break ;
	}
	return (set);
}

SOUND_CALL status_t SOUND_TYPE		sound_play(sound_t *sound, system_t *up)
{
	if (!sound || !sound->data || !al_is_audio_installed())
		return (Off);
	switch (up->audio)
	{
		case SYSTEM_AUDIO_PRIMARY:
		break ;
		case SYSTEM_AUDIO_ALLEGRO:
		{
			if (sound->type == PLAY_EFFECT)
				al_play_sample (sound->data, sound->volume->effect, sound->balance, sound->speed, sound->mode + 0x100, NULL);
			else if (sound->type == PLAY_MUSIC)
			{
				if (!al_get_audio_stream_playing(sound->data))
				{
					al_set_audio_stream_playing (sound->data, true);
					return (On);
				}
			}
		}
		break ;
		case SYSTEM_AUDIO_SDL:
		break ;
		case SYSTEM_AUDIO_NONE:
		break ;
	}
	return (Off);
}

SOUND_CALL void SOUND_TYPE		sound_pop(void *data, system_t *up)
{
	sound_t	*sound;

	if (!data || !up)
		return ;
	sound = data;
	switch (up->audio)
	{
		case SYSTEM_AUDIO_PRIMARY:
		break ;
		case SYSTEM_AUDIO_ALLEGRO:
		{
			if (sound->data)
			{
				if (sound->type == PLAY_MUSIC)
					al_destroy_audio_stream (sound->data);
				else if (sound->type == PLAY_EFFECT)
					al_destroy_sample (sound->data);
			}
		}
		break;
		case SYSTEM_AUDIO_SDL:
		break ;
		case SYSTEM_AUDIO_NONE:
		break ;
	}
	free(sound);
}
