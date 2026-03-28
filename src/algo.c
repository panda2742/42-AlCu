#include "algo.h"
#include <stdlib.h>

t_sticks	min(t_sticks a, t_sticks b)
{
	if (a > b)
		return b;
	return a;
}

t_sticks	max(t_sticks a, t_sticks b)
{
	if (a < b)
		return b;
	return a;
}

t_bool	winning_pos_before_play(t_sticks n)
{
	if ((n - 1) % 4 == 0) return FALSE;
	return TRUE;
}

t_strategy	*determine_strategy(t_sticks n, t_strategy *before_strategy, t_bool is_last)
{
	const t_bool	is_winning = winning_pos_before_play(n);

	t_strategy		*strat = malloc(sizeof(t_strategy));
	if (!strat)
		return NULL;

	if (before_strategy == NULL)
	{
		strat->has_to_finish = FALSE;
		strat->has_to_start = is_last ? TRUE : is_winning;
		return strat;
	}
	strat->has_to_finish = !before_strategy->has_to_start;
	strat->has_to_start = is_last ? TRUE : is_winning;
	return strat;
}

t_sticks	round_play(t_sticks n, t_sticks played)
{
	return min(max(1, played), 3);
}

t_sticks	play(t_sticks n, t_strategy *strat)
{
	const t_bool	is_winning = winning_pos_before_play(n);

	if (!is_winning)
		return 1;
	const t_sticks	mod = n % 4;
	if (!strat->has_to_finish)
	{
		switch (mod)
		{
			case 0:
				return 3;
			default:
				return round_play(n, mod - 1);
		}
	}
	else return round_play(n, mod);
}
