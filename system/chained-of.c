#include <place/system/chained-of.h>
#include <stdlib.h>

CHAINED_CALL chained_t	*CHAINED_TYPE chained_push(void *data)
{
	chained_t	*set;

	set = malloc(sizeof(chained_t));
	if (!set)
		return (NULL);
	set->data = data;
	set->prev = NULL;
	set->next = NULL;
	return (set);
}

CHAINED_CALL status_t	CHAINED_TYPE chained_next_first(chained_t **head, chained_t *set)
{
	if (!set)
		return (Off);
	set->next = *head;
    if (*head)
      (*head)->prev = set;
    *head = set;
	return (On);
}

CHAINED_CALL status_t	CHAINED_TYPE chained_next_last(chained_t **head, chained_t *set)
{
	chained_t	*upd;

	if (!set)
		return (Off);
	if (!*head)
	{
		*head = set;
		return (On);
	}
	upd = *head;
	while (upd->next)
		upd = upd->next;
	set->prev = upd;
	upd->next = set;
	return (On);
}

CHAINED_CALL void		CHAINED_TYPE chained_pop(chained_t **head, system_t *up, void (*delete)(void *data, system_t *up))
{
	chained_t	*next;

	while (*head)
	{
		next = (*head)->next;
		if (delete)
			delete((*head)->data, up);
		free(*head);
		*head = next;
	}
}
