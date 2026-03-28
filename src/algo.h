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

#endif
