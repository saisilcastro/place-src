#include <place/system/audio-of.h>
#include <place/plugin/graphic/allegro-plugin.h>

AUDIO_CALL void AUDIO_TYPE audio_set(audio_t *set, B32 frequency, B32 format, B32 channel, B32 chunk_size)
{
	if (!set)
		return ;
	set->frequency = frequency;
	set->format = format;
	set->channel = channel;
	set->chunk_size = chunk_size;
}