#include <fcntl.h>
#include "get_next_line.h"
#include "vector.h"
#include "libft.h"

t_vector*	readBoard(const char* arg) {
	t_vector*	game = init_vector(sizeof(int));
	char*		line = NULL;
	int			fd;

	if (game == NULL)
		return NULL;

	fd = (arg == NULL ? STDIN_FILENO : open(arg, O_RDONLY));
	if (fd < 0) {
		ft_putendl_fd("ERROR\nCan't open file", 2);
		vector_destroy(game);
		return NULL;
	}

	while ((line = get_next_line(fd)) && line[0] != '\n') {
		const char*	endptr = NULL;
		int			value = ft_strtoi(line, &endptr);
		
		if (value <= 0 || value > 10000 || (*endptr != '\n' && *endptr != '\0')) {
			ft_putendl_fd("ERROR\nNumber must be in range [1,10000]", 2);
			free(line);
			continue;
		}

		if (vector_push(game, &value) < 0) {
			vector_destroy(game);
			if (fd > 0) close(fd);
			ft_putendl_fd("ERROR\nMemory error, stopping.", 2);
			free(line);
			return NULL;
		}

		free(line);
	}

	free(line);
	if (fd > 0) close(fd);
	return game;
}

int	findMax(t_vector* game) {
	int max = 0;

	for (size_t i = 0; i < game->size; i++) {
		max = (((int *) game->tab)[i] > max ? ((int*) game->tab)[i] : max); 
	}
	return max;
}

void	displayBoard(t_vector* game) {
	int	max = findMax(game);
	int	space;

	for (size_t i = 0; i < game->size; i++) {
		int	val = ((int*) game->tab)[i];

		space = (max - val) / 2;
 
		for (int j = 0; j < space; j++) {
			write(1, "  ", 2);
		}

		if (max % 2 != val % 2)
			write(1, " ", 1);

		for (int j = 0; j < val; j++) {
			write(1, "| ", 2);
		}

		write(1, "\n", 1);
	}
}

int	getPlayerMove(t_vector*	game, int fd) {
	int		items;
	char*	line;

	displayBoard(game);
	if (vector_get(game, game->size -1, &items) < 0) {
		ft_putendl_fd("ERROR\nWrong index", 2);
		return -1;
	}

	while (1) {
		line = get_next_line(fd);
		if (!line || line[0] == '\n') {
			ft_putendl_fd("ERROR\nMemory error stopping.", 2);
			return -1;
		}

		const char*	endptr;
		int			value = ft_strtoi(line, &endptr);

		if (value <= 0 || value > 3 || *endptr != '\n') {
			ft_putendl_fd("ERROR\nNumber must be in range [1,3]", 2);
			free(line);
			continue;
		}

		free(line);
		if (value > items) {
			ft_putendl_fd("ERROR\nThere is not enough items on that heap", 2);
			continue;
		}

		items -= value;
		vector_set(game, game->size -1, &items);
		break;
	}

	if (items == 0)
		vector_pop(game);
	return (game->size == 0);
}

bool	getAiMove(t_vector* game, int fd) {
	return getPlayerMove(game, fd);
}

int main(int ac, char* const av[]) {
	t_vector*	game;

	if (ac > 2) {
		ft_putendl_fd("ERROR\nUsage ./Alcu <file>", 2);
		return 1;
	}

	game = readBoard(av[1]);
	int	fd = (av[1] == NULL ? open("/dev/tty", O_RDONLY) : 0);
	while (1) {
		int	ret;
		if (fd < 0) {
			ft_putendl_fd("ERROR\nCan't open stdin", 2);
			break;
		}
		ret = getPlayerMove(game, fd);
		if (ret == 1 || ret == -1) {
			ft_putstr_fd((ret == 1 ? "You loose !\n" : ""), 1);
			break;
		}

		ret = getAiMove(game, fd);
		if (ret == 1 || ret == -1) {
			ft_putstr_fd((ret == 1 ? "You Win !\n" : ""), 1);
			break;
		}
	}

	if (fd > 0) close(fd);
	if (!game)
		return 1;
	vector_destroy(game);
}
