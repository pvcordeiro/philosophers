/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 20:43:38 by paude-so          #+#    #+#             */
/*   Updated: 2025/04/19 09:02:36 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosophers.h>

t_all *all(void)
{
	static t_all	all;

	return (&all);
}

// bool	alive_and_hungry(t_philo *philo)
// {

// }

// void	philo_routine(void *arg)
// {
// 	t_philo	*philo;

// 	while(alive_and_hungry(philo))
// 	{
// 		think(philo);
// 		take_forks(philo);
// 		eat(philo);
// 		release_forks(philo);
// 		sleep(philo);
// 	}
// }

void	create_philos(void)
{
	size_t	i;
	t_philo *philo;
	// pthread_t threads[all()->num_philo];

	i = 0;
	while (++i <= all()->num_philo)
	{
		philo = malloc(sizeof(t_philo));
		if (!philo)
			return ;
		philo->id = i;
		philo->status = ALIVE;
		philo->meals = 0;
		ft_list_add(&all()->philos, philo, free);
		// pthread_create(&threads[i], NULL, philo_routine, &philo[i]);
	}
}

bool	positive(long long arg)
{
	if (arg < 0)
		return (false);
	return (true);
}

void	init_all(int argc, char **argv)
{
	int i = 0;
	while (++i < argc)
	{
		if (!positive(ft_atoll(argv[i])))
			exit(ft_printf("Only positive numbers allowed\n"));
	}
	all()->num_philo = ft_atoll(argv[1]);
	all()->time_to_die = ft_atoll(argv[2]);
	all()->time_to_eat = ft_atoll(argv[3]);
	all()->time_to_sleep = ft_atoll(argv[4]);
	if (argc == 6)
		all()->num_eat = ft_atoll(argv[5]);
}

int	main(int argc, char **argv)
{
	t_list	*current;
	t_philo	*philosopher;

	if (argc < 5 || argc > 6)
		return (ft_printf("Usage: number_of_philosophers time_to_die time_to_eat time_to_sleep Optional: [number_of_times_each_philosopher_must_eat]\n"));
	init_all(argc, argv);
	create_philos();
	ft_printf("number_of_philosophers: %d\ntime_to_die:%d\ntime_to_eat:%d\ntime_to_sleep:%d\n", all()->num_philo, all()->time_to_die, all()->time_to_eat, all()->time_to_sleep);
	if (argc == 6)
		ft_printf("number_of_times_each_philosopher_must_eat:%d\n", all()->num_eat);
	current = all()->philos;
	while(current)
	{
		philosopher = (t_philo *)current->data;
		ft_printf("\nphilo number: %d\nphilo address: %p", philosopher->id, philosopher);
		current = current->next;
	}
	printf("\n");
	return (0);
}
