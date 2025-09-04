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

int	parse_material(t_material *material, char *material_tokens[])
{
	int i;

	i = 0;
	if (material_tokens[i])
		material->texture_addr = material_tokens[i];
	if (material_tokens[i + 1])
		material->bump = atoi(material_tokens[i + 1]);
	if (material_tokens[i + 2])
		material->chess = atoi(material_tokens[i + 2]);
	if (!parse_double(material_tokens[i + 3], &material->transparency))
		material->transparency = 0.0;
	if (!parse_double(material_tokens[i + 4], &material->reflection))
		material->reflection = 0.0;
	if (!parse_double(material_tokens[i + 5], &material->refraction_index))
		material->refraction_index = 1.0;
	return (1);
}
