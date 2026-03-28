#ifndef ALGO_H
#define ALGO_H

#define K 3

typedef unsigned int	t_sticks;

typedef enum {
	FALSE = 0,
	TRUE = 1
} t_bool;

typedef struct s_strategy
{
	t_bool	has_to_start;
	t_bool	has_to_finish;
}	t_strategy;

t_strategy	determine_strategy(t_sticks n, t_strategy *before_strategy, t_bool is_last);
t_sticks	play(t_sticks n, t_strategy *strat);
t_sticks	min(t_sticks a, t_sticks b);
t_sticks	max(t_sticks a, t_sticks b);

#endif
