/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_window.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idioumas <idioumas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:58:02 by idioumas          #+#    #+#             */
/*   Updated: 2025/06/28 17:34:00 by idioumas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
# include <math.h>


void mlx_put_color(char *img_addr, int x, int y, int color, int line_len, int bpp)
{
    // Calculate memory offset to the pixel
    int offset = y * line_len + x * (bpp / 8);

    // Write the color into the image buffer
    *(unsigned int *)(img_addr + offset) = color;
}

void init_window(t_minirt *scene)
{
    int tmp_width = 1600;
    int tmp_height = 1200;
    void *tmp_img;
    void *img_addr;
    int bpp;
    int line_length;
    int endian;

    scene->mlxptr = mlx_init();
    /*if (scene->mlxptr); // <-
        // free*/
    scene->mlxwin = mlx_new_window(scene->mlxptr,tmp_width, tmp_height, "MiniRT");
    /*if (scene->mlxwin); // <-
        //free*/
    // testing
    tmp_img = mlx_new_image(scene->mlxptr, tmp_width, tmp_height);
    img_addr = mlx_get_data_addr(tmp_img, &bpp,&line_length, &endian);
    //int y = 0;
    int x = 50;
    while(x  < 500)
    {
        mlx_put_color((char *)img_addr,x, 400+ 50 * sin(x), 0x00FF0000, line_length,bpp);
        x++;
    }

    x = 50;
    while(x  < 500)
    {
        mlx_put_color((char *)img_addr,x,2*x, 0x00FF0000, line_length,bpp);
        x++;
    }

    //unsigned int * tmp_add =  (unsigned int*)tmp_img_addr;
    
    //mlx_destroy_image(intel->mlx_ptr, intel->img);
    mlx_put_image_to_window(scene->mlxptr, scene->mlxwin,tmp_img, 0, 0);
    // destroy
    mlx_loop(scene->mlxptr);

}