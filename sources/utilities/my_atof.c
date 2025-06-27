#include "minirt.h"

double  appendDot(const char *str, double result, int sign)
{
    double  div;
    double  dot;

    div = 0.0;
    dot = 0.0;
    if (*str == '.')
    {
        str++;
        while (*str >= '0' && *str <= '9')
        {
            fraction = fraction * 10.0 + (*str++ - '0');
            divisor *= 10.0;
        }
        result += fraction / divisor;
    }
    return (result * sign);

}

double ft_atof(const char *str)
{
    int     sign;
    double  result;

    sign = 1;
    result = 0.0;
    while (is_space(*str))
        str++;
    if (*str == '-' || *str == '+')
        sign = (*str++ == '-') ? -1 : 1;
    while (*str >= '0' && *str <= '9')
        result = result * 10.0 + (*str++ - '0');
    return (appendDot(str, result, sign);    
}
