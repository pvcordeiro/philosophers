/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:13:33 by paude-so          #+#    #+#             */
/*   Updated: 2025/04/24 18:39:50 by paude-so         ###   ########.fr       */
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

bool	check_valid_arg(int argc, char **argv)
{
	int	i;

	i = 0;
	while (++i < argc)
	{
		if (ft_atoll(argv[i]) < 0 || ft_atoll(argv[i]) == 0)
			return (false);
	}
	return (true);
}

void	handle_one(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	print_status(philo, TAKE_FORK);
	pthread_mutex_lock(&philo->philo_mutex);
	philo->status = DEAD;
	pthread_mutex_unlock(&philo->philo_mutex);
	pthread_mutex_unlock(philo->right_fork);
}
