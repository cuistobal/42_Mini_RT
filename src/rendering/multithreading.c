/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multithreading.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idioumas <idioumas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 07:39:23 by chrleroy          #+#    #+#             */
/*   Updated: 2025/09/28 19:34:31 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

/*
** render_all_pixels - Render all pixels in the scene
*/
void	send_directive(t_intels dirs)
{
	pthread_mutex_lock(&(dirs.rt->args.mutex_queue));
	dirs.rt->args.directives_rendering[dirs.rt->args.ntask] = dirs;
	dirs.rt->args.ntask++;
	pthread_mutex_unlock(&(dirs.rt->args.mutex_queue));
	pthread_cond_signal(&(dirs.rt->args.cond_queue));
}

void	create_directive(t_minirt *rt)
{
	int			i;
	int			y;
	int			pwidth;
	int			pheight;
	t_intels	directives;

	i = 0;
	pwidth = ((rt->mlx.width + 31) >> 5);
	pheight = ((rt->mlx.height + 31) >> 5);
	(rt->args.total_directives) = pwidth * pheight;
	while (i < pheight)
	{
		y = 0;
		while (y < pwidth)
		{
			directives.xstart = y << 5;
			directives.xend = (y << 5) + 32;
			directives.ystart = i << 5;
			directives.yend = (i << 5) + 32;
			directives.rt = rt;
			send_directive(directives);
			y++;
		}
		i++;
	}
}

static void	execute_rendering(t_intels *task)
{
	int		x;
	int		y;

	y = task->ystart;
	while (y < task->yend)
	{
		x = task->xstart;
		while (x < task->xend)
		{
			render_pixel_at_coordinates(task->rt, x, y);
			x++;
		}
		y++;
	}
}

// The thread's routine to divide redering into smaller chuncks
void	*render_all_pixels(void *arg)
{
	t_minirt	*rt;
	t_intels	directive;

	rt = (t_minirt *)arg;
	while (1)
	{
		pthread_mutex_lock(&(rt->args.mutex_queue));
		while (rt->args.ntask == 0 && rt->args.stop == 0)
			pthread_cond_wait(&rt->args.cond_queue, &(rt->args.mutex_queue));
		if (rt->args.stop == 1 && rt->args.ntask == 0)
		{
			pthread_mutex_unlock(&(rt->args.mutex_queue));
			break ;
		}
		directive = update_directives(rt);
		pthread_mutex_unlock(&(rt->args.mutex_queue));
		execute_rendering(&directive);
		pthread_mutex_lock(&(rt->args.mutex_queue));
		rt->args.completed_directives++;
		check_directives(rt);
		pthread_mutex_unlock(&(rt->args.mutex_queue));
	}
	rt->args.completed_directives = 0;
	return (NULL);
}

void	init_multi_thread(t_minirt *rt, pthread_t threads[])
{
	int	i;

	i = 0;
	while (i < NUM_THREAD)
	{
		threads[i] = 0;
		i++;
	}
	if (pthread_mutex_init(&(rt->args.mutex_queue), NULL)
		||pthread_cond_init(&(rt->args.cond_queue), NULL))
		kill_threads_rsc(rt, threads, 1);
	i = 0;
	while (i < NUM_THREAD)
	{
		if (pthread_create(&threads[i], NULL, render_all_pixels, rt))
		{
			perror("Error : Thread creation failed");
			kill_threads_rsc(rt, threads, 1);
		}
		i++;
	}
}
