/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 20:43:38 by paude-so          #+#    #+#             */
/*   Updated: 2025/04/08 17:36:05 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philosophers.h>

t_philo *philo(void)
{
	static t_philo	philo;
	return (&philo);
}

void	add_philo(ssize_t nb)
{
	ssize_t	i;

	i = 0;
	while (i < nb)
	{
		philo()->num_philo++;
		i++;
	}
}

int	main(int argc, char **argv)
{
	if (argc < 5)
		return (ft_printf("Usage: number_of_philosophers time_to_die time_to_eat time_to_sleep"));
	add_philo(ft_atoi(argv[1]));
	return (0);
}
