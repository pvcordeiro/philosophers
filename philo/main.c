/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 20:43:38 by paude-so          #+#    #+#             */
/*   Updated: 2025/04/12 19:54:52 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosophers.h>
#include <stdlib.h>
t_tudo *tudo(void)
{
	static t_tudo	tudo;

	return (&tudo);
}


void	create_philos(void)
{
	size_t	i;
	t_philo *philo;

	i = 0;
	while (i < tudo()->num_philo)
	{
		philo = ft_calloc(1, sizeof(t_philo));
		philo->id = i;
		ft_list_add(&tudo()->philos, philo, free);
		i++;
	}
}

void	init_tudo(int argc, char **argv)
{
	tudo()->num_philo = ft_atoll(argv[1]);
	tudo()->time_to_die = ft_atoll(argv[2]);
	tudo()->time_to_eat = ft_atoll(argv[3]);
	tudo()->time_to_sleep = ft_atoll(argv[4]);
	tudo()->use_num_eat = false;
	if (argc == 5)
	{
		tudo()->use_num_eat = true;
		tudo()->num_eat = ft_atoll(argv[5]);
	}
}

int	main(int argc, char **argv)
{
	if (argc < 5)
		return (ft_printf("Usage: number_of_philosophers time_to_die time_to_eat time_to_sleep Optional: [number_of_times_each_philosopher_must_eat]"));
	init_tudo(argc, argv);
	create_philos();
	return (0);
}
