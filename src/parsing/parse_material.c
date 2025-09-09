/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_material.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrleroy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 07:42:33 by chrleroy          #+#    #+#             */
/*   Updated: 2025/09/04 08:39:05 by chrleroy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	parse_material(t_minirt *rt, t_material *material, char *material_tokens[])
{	
	int i;
	int	w;
	int h;

	i = 0;
	w = 0;
	h = 0;
	// Unnecessary check
	if (!material || !material_tokens)
		return (0);

	material->texture_addr = NULL;
    if (material_tokens[i] && material_tokens[i][0] != '\0')
        material->texture_addr = mlx_xpm_file_to_image(rt->mlx.mlx_ptr, material_tokens[i], &w, &h);

	else if (!parse_int(material_tokens[i + 1], &material->bump))
	{
		material->bump = 0;
		//printf("Error\nInvalid bump map value\n");
	}	

	if (!parse_int(material_tokens[i + 2], &material->chess))
	{
//		printf("Error\nInvalid chess value\n");
		material->chess = 0;
	}

	if (!parse_double(material_tokens[i + 3], &material->transparency))
		material->transparency = 0.0;

	if (!parse_double(material_tokens[i + 4], &material->reflection))
		material->reflection = 0.0;

	if (!parse_double(material_tokens[i + 5], &material->refraction_index))
		material->refraction_index = 1.0;

	return (1);
}
