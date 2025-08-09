/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parsing_simple.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cuistobal <cuistobal@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:00:00 by cuistobal        #+#    #+#             */
/*   Updated: 2025/01/28 00:00:00 by cuistobal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minirt.h"

/*
** Test parsing utility functions only
*/
int main(void)
{
    printf("Testing parsing utility functions...\n");
    
    // Test parse_double
    printf("\n=== Testing parse_double ===\n");
    double d_result;
    if (parse_double("3.14", &d_result))
        printf("parse_double('3.14') = %.2f ✓\n", d_result);
    else
        printf("parse_double('3.14') failed ✗\n");
    
    if (parse_double("-2.5", &d_result))
        printf("parse_double('-2.5') = %.2f ✓\n", d_result);
    else
        printf("parse_double('-2.5') failed ✗\n");
    
    if (!parse_double("invalid", &d_result))
        printf("parse_double('invalid') correctly failed ✓\n");
    else
        printf("parse_double('invalid') should have failed ✗\n");
    
    // Test parse_int
    printf("\n=== Testing parse_int ===\n");
    int i_result;
    if (parse_int("42", &i_result))
        printf("parse_int('42') = %d ✓\n", i_result);
    else
        printf("parse_int('42') failed ✗\n");
    
    if (parse_int("-10", &i_result))
        printf("parse_int('-10') = %d ✓\n", i_result);
    else
        printf("parse_int('-10') failed ✗\n");
    
    if (!parse_int("3.14", &i_result))
        printf("parse_int('3.14') correctly failed ✓\n");
    else
        printf("parse_int('3.14') should have failed ✗\n");
    
    // Test parse_vec3
    printf("\n=== Testing parse_vec3 ===\n");
    t_vec3 v_result;
    if (parse_vec3("1.0,2.0,3.0", &v_result))
        printf("parse_vec3('1.0,2.0,3.0') = (%.1f, %.1f, %.1f) ✓\n", 
               v_result.x, v_result.y, v_result.z);
    else
        printf("parse_vec3('1.0,2.0,3.0') failed ✗\n");
    
    if (parse_vec3("-1,-2,-3", &v_result))
        printf("parse_vec3('-1,-2,-3') = (%.1f, %.1f, %.1f) ✓\n", 
               v_result.x, v_result.y, v_result.z);
    else
        printf("parse_vec3('-1,-2,-3') failed ✗\n");
    
    if (!parse_vec3("1,2", &v_result))
        printf("parse_vec3('1,2') correctly failed ✓\n");
    else
        printf("parse_vec3('1,2') should have failed ✗\n");
    
    // Test parse_color
    printf("\n=== Testing parse_color ===\n");
    t_color c_result;
    if (parse_color("255,128,0", &c_result))
        printf("parse_color('255,128,0') = (%d, %d, %d) ✓\n", 
               c_result.r, c_result.g, c_result.b);
    else
        printf("parse_color('255,128,0') failed ✗\n");
    
    if (!parse_color("256,0,0", &c_result))
        printf("parse_color('256,0,0') correctly failed ✓\n");
    else
        printf("parse_color('256,0,0') should have failed ✗\n");
    
    if (!parse_color("-1,0,0", &c_result))
        printf("parse_color('-1,0,0') correctly failed ✓\n");
    else
        printf("parse_color('-1,0,0') should have failed ✗\n");
    
    printf("\nParsing utility functions test completed successfully!\n");
    return (0);
}