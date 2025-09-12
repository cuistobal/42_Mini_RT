/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_material.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idioumas <idioumas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 07:42:33 by chrleroy          #+#    #+#             */
/*   Updated: 2025/09/12 08:20:24 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

t_texture *load_texture(void *mlx_ptr, char *path)
{
    t_texture   *tex;
    void        *img;
    int         bpp;
    int         line_length;
    int         endian;

    tex = malloc(sizeof(t_texture));
    if (!tex)
        return (NULL);
    img = mlx_xpm_file_to_image(mlx_ptr, path, &tex->width, &tex->height);
    if (!img)
    {
        free(tex);
        return (NULL);
    }
    tex->data = (unsigned int *)mlx_get_data_addr(img, &bpp, &line_length, &endian);
    return (tex);
}

int	parse_material(t_minirt *rt, t_material *material, char *material_tokens[])
{	
	int i;
	int	w;
	int h;

	i = 0;
	w = 0;
	h = 0;
	if (!rt || !material || !material_tokens)
		return (0);
	material->bump_addr = NULL;
    if (material_tokens[i] && material_tokens[i][0] != '\0')
        material->bump_addr = mlx_xpm_file_to_image(rt->mlx.mlx_ptr, material_tokens[i], &w, &h);
	else if (!parse_int(material_tokens[i + 1], &material->bump))
		material->bump = 0;
	if (!parse_int(material_tokens[i + 2], &material->chess))
		material->chess = 0;
	if (!parse_double(material_tokens[i + 3], &material->transparency))
		material->transparency = 0.0;
	if (!parse_double(material_tokens[i + 4], &material->reflection))
		material->reflection = 0.0;
	if (!parse_double(material_tokens[i + 5], &material->refraction_index))
		material->refraction_index = 1.0;
	material->bump_addr = load_texture(rt->mlx.mlx_ptr, "./bump.xpm");
	if (material->bump_addr!= NULL)
		printf("ok");
	return (1);
}
