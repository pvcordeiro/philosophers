/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:13:33 by paude-so          #+#    #+#             */
/*   Updated: 2025/04/22 14:03:14 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosophers.h>

size_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_time();
	while ((get_time() - start) < milliseconds)
		usleep(100);
	return (0);
}

bool	init_all(int argc, char **argv)
{
	all()->num_philo = ft_atoll(argv[1]);
	all()->time_to_die = ft_atoll(argv[2]);
	all()->time_to_eat = ft_atoll(argv[3]);
	all()->time_to_sleep = ft_atoll(argv[4]);
	if (argc == 6)
		all()->num_eat = ft_atoll(argv[5]);
	if (pthread_mutex_init(&all()->data_mutex, NULL) != 0)
        return (false);
	return (true);
}

bool	check_positive(int argc, char **argv)
{
	int i;

	i = 0;
	while (++i < argc)
	{
		if (ft_atoll(argv[i]) < 0 || ft_atoll(argv[i]) == 0)
			return (false);
	}
	return (true);
}
