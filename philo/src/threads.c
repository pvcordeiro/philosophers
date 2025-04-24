/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:59:28 by paude-so          #+#    #+#             */
/*   Updated: 2025/04/24 18:04:05 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosophers.h>

static bool	philo_alive(t_philo *philo)
{
	bool	alive;

	pthread_mutex_lock(&philo->philo_mutex);
	alive = philo->status == ALIVE && (all()->num_eat == 0
			|| philo->meals < all()->num_eat);
	if (all()->num_eat > 0 && philo->meals >= all()->num_eat)
		philo->status = FULL;
	pthread_mutex_unlock(&philo->philo_mutex);
	return (alive);
}

static void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (all()->num_philo == 1)
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, TAKE_RIGHT_FORK);
		pthread_mutex_lock(&philo->philo_mutex);
		philo->status = DEAD;
		pthread_mutex_unlock(&philo->philo_mutex);
		pthread_mutex_unlock(philo->right_fork);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		ft_usleep(1);
	while (philo_alive(philo))
	{
		take_forks(philo);
		eat(philo);
		release_forks(philo);
		sleep_philo(philo);
		think(philo);
	}
	return (NULL);
}

static void	*death_monitor(void *arg)
{
	t_list	*node;
	t_list	*p_node;
	t_philo	*philo;
	bool	all_full;

	(void)arg;
	while (42)
	{
		node = all()->philos;
		all_full = all()->num_eat > 0;
		while (node)
		{
			philo = node->data;
			pthread_mutex_lock(&philo->philo_mutex);
			if (philo->status == DEAD || ((get_time()
						- philo->last_meal) > all()->time_to_die
					&& philo->status == ALIVE))
			{
				philo->status = DEAD;
				pthread_mutex_unlock(&philo->philo_mutex);
				print_status(philo, DIE);
				p_node = all()->philos;
				while (p_node)
				{
					pthread_mutex_lock(&((t_philo *)p_node->data)->philo_mutex);
					((t_philo *)p_node->data)->status = DEAD;
					pthread_mutex_unlock(&((t_philo *)p_node->data)->philo_mutex);
					p_node = p_node->next;
				}
				return (NULL);
			}
			if (all_full && philo->meals < all()->num_eat)
				all_full = false;
			pthread_mutex_unlock(&philo->philo_mutex);
			node = node->next;
		}
		if (all_full)
		{
			node = all()->philos;
			while (node)
			{
				pthread_mutex_lock(&((t_philo *)node->data)->philo_mutex);
				((t_philo *)node->data)->status = FULL;
				pthread_mutex_unlock(&((t_philo *)node->data)->philo_mutex);
				node = node->next;
			}
			return (NULL);
		}
	}
	return (NULL);
}

bool	create_threads(void)
{
	t_list		*node;
	t_philo		*philo;

	all()->start_time = get_time();
	node = all()->philos;
	while (node)
	{
		philo = node->data;
		philo->last_meal = all()->start_time;
		if (pthread_create(&philo->thread, NULL, philo_routine, philo) != 0)
			return (ft_printf("Error creating philosopher thread\n"), false);
		node = node->next;
	}
	if (pthread_create(&all()->monitor_thread, NULL, death_monitor, NULL) != 0)
		return (ft_printf("Error creating monitor thread\n"), false);
	return (true);
}

bool	join_threads(void)
{
	t_list	*node;
	t_philo	*philo;

	node = all()->philos;
	while (node)
	{
		philo = node->data;
		if (pthread_join(philo->thread, NULL) != 0)
			return (ft_printf("Error joining philosopher thread\n"), false);
		node = node->next;
	}
	if (pthread_join(all()->monitor_thread, NULL) != 0)
		return (ft_printf("Error joinining monitor thread\n"), false);
	return (true);
}
