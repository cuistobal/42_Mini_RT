/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multithreading_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idioumas <idioumas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 15:11:46 by chrleroy          #+#    #+#             */
/*   Updated: 2025/09/28 16:03:09 by idioumas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	kill_threads_rsc(t_minirt *rt, pthread_t threads[], int verror)
{
	int	i;

	i = 0;
	while (i < NUM_THREAD)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	pthread_mutex_destroy(&(rt->args.mutex_queue));
	pthread_cond_destroy(&(rt->args.cond_queue));
	if (verror)
	{
		cleanup_all(rt);
		exit (verror);
	}
	return (0);
}

t_intels	update_directives(t_minirt	*rt)
{
	t_intels	directive;
	int			i;

	directive = rt->args.directives_rendering[0];
	i = 0;
	while (i < rt->args.ntask - 1)
	{
		rt->args.directives_rendering[i] = rt->args.directives_rendering[i + 1];
		i++;
	}
	rt->args.ntask--;
	return (directive);
}

/*
*  Check if all directives are finished;
*/
void	check_directives(t_minirt	*rt)
{
	if (rt->args.completed_directives == (rt->args.total_directives))
	{
		rt->args.stop = 1;
		pthread_cond_broadcast(&(rt->args.cond_queue));
	}
}
