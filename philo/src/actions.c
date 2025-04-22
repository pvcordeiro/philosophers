/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:02:25 by paude-so          #+#    #+#             */
/*   Updated: 2025/04/22 13:02:58 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosophers.h>

void	take_forks(t_philo *philo)
{
	if (philo->id % 2)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, TAKE_LEFT_FORK);
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, TAKE_RIGHT_FORK);
		return ;
	}
	pthread_mutex_lock(philo->right_fork);
	print_status(philo, TAKE_RIGHT_FORK);
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, TAKE_LEFT_FORK);
}

void	eat(t_philo *philo)
{
	print_status(philo, EAT);
	ft_usleep(all()->time_to_eat);
	pthread_mutex_lock(&philo->philo_mutex);
	philo->last_meal = get_time();
	philo->meals++;
	pthread_mutex_unlock(&philo->philo_mutex);
	
}

void	think(t_philo *philo)
{
	print_status(philo, THINK);
}

void	sleep_philo(t_philo *philo)
{
	print_status(philo, SLEEP);
	ft_usleep(all()->time_to_sleep);
}

void	release_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}
