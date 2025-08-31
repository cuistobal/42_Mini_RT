/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_xpm_texture.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idioumas <idioumas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 22:37:18 by idioumas          #+#    #+#             */
/*   Updated: 2025/08/31 22:37:23 by idioumas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_texture *load_xpm_texture(void *mlx, char *filename)
{
    t_texture *tex;
    void *img;
    int bpp;
    int size_line;
    int endian;

    tex = (t_texture *)malloc(sizeof(t_texture));
    if (!tex)
        return (NULL);

    img = mlx_xpm_file_to_image(mlx, filename, &tex->width, &tex->height);
    if (!img)
    {
        free(tex);
        return (NULL);
    }
    tex->data = (unsigned int *)mlx_get_data_addr(img, &bpp, &size_line, &endian);
    return (tex);
}
