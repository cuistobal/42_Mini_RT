/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multithreading.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 07:39:23 by chrleroy          #+#    #+#             */
/*   Updated: 2025/09/26 07:45:50 by chrleroy         ###   ########.fr       */
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

// J ai modifie ton code -> j ai retire les if/else pour reduire le branching, etant donne
// que les valeurs seront toujours valide (la resolution est forcement un multiple de 32)
// J ai utilise << 5 qui equivaut a * 32 (2^5=32) mais en plsu rapide.
// En tout ca nous fait gagner 100 ms
// Dailleurs on devrait garder ces valeur la dans la structure de pool_thread plutoit que
// de la recalculer 3600 fois par frame.
void	create_directive(t_minirt *rt)
{
	int			i;
	int 		y;
	int 		pwidth;
	int 		pheight;
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

static void execute_rendering(t_intels* task)
{
        int             x;
        int             y;
        double  inv_width;
        double  inv_height;

        inv_width = 1.0 / (double)task->rt->mlx.width;
        inv_height = 1.0 / (double)task->rt->mlx.height;
        y = task->ystart;
        while (y < task->yend) 
        {
                x = task->xstart;
                while (x < task->xend)
                {
                        render_pixel_at_coordinates(task->rt, x, y, inv_width, inv_height);
                        x++;
                }
                y++;
        }       
}

// The thread's routine to divide redering into smaller chuncks
void	*render_all_pixels(void *arg)
{
	int			i;
	t_minirt	*rt;
	t_intels	directive;

	rt = (t_minirt *)arg;
	while (1)
	{
		i = 0;
		pthread_mutex_lock(&(rt->args.mutex_queue));
		while (rt->args.ntask == 0 && rt->args.stop == 0)
			pthread_cond_wait(&rt->args.cond_queue, &(rt->args.mutex_queue));
		if (rt->args.stop == 1 && rt->args.ntask == 0)
		{
			pthread_mutex_unlock(&(rt->args.mutex_queue));
			break;
		}
		directive = rt->args.directives_rendering[0];
		while (i < rt->args.ntask - 1)
		{
			rt->args.directives_rendering[i] = rt->args.directives_rendering[i + 1];
			i++;
		}
		rt->args.ntask--;
		pthread_mutex_unlock(&(rt->args.mutex_queue));
		execute_rendering(&directive);
		pthread_mutex_lock(&(rt->args.mutex_queue));
		rt->args.completed_directives++;
		if (rt->args.completed_directives == (rt->args.total_directives))
		{
			rt->args.stop = 1;
			pthread_cond_broadcast(&(rt->args.cond_queue));
		}
		pthread_mutex_unlock(&(rt->args.mutex_queue));
	}
	rt->args.completed_directives = 0;
	return (NULL);
}
