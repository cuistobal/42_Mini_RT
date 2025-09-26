/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_material.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idioumas <idioumas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 07:42:33 by chrleroy          #+#    #+#             */
/*   Updated: 2025/09/26 08:02:43 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static void	load_texture(void *mlx_ptr, char *path, t_texture *texture)
{
	void	*img;
	int		bpp;
	int		endian;
	int		line_length;

	if (!texture || !path || !mlx_ptr)
		return ;
	img = mlx_xpm_file_to_image(mlx_ptr, path,
			&texture->width, &texture->height);
	if (!img)
	{
		texture->img_ptr = NULL;
		texture->data = NULL;
		return ;
	}
	texture->img_ptr = img;
	texture->data = (unsigned int *)mlx_get_data_addr(img, &bpp,
			&line_length, &endian);
}

int	parse_material(t_minirt *rt, t_material *material, char *material_tokens[])
{	
	int	i;

	i = 0;
	if (!rt || !material || !material_tokens)
		return (0);
	material->texture_addr = NULL;
	if (material_tokens[i] && material_tokens[i][0] != '\0')
	{
		load_texture(rt->mlx.mlx_ptr, material_tokens[i], &material->texture);
		if (material->texture.data)
			material->texture_addr = &material->texture;
	}
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
	return (1);
}
