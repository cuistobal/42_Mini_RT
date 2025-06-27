#include "minirt.h"

static double  appendDot(const char *str, double result, int sign, bool *error)
{
    double  divisor;
    double  decimals;

    divisor = 1;
    decimals = 0.0;
    if (*str != '.')
        return (*error = isspace(*str), result * sign);
    str++;
    while (*str >= '0' && *str <= '9')
    {
        decimals = decimals * 10.0 + (*str++ - '0');
        divisor *= 10.0;
    }
    result += decimals / divisor;
    return (result * sign);
}

void    handleSign(const char *str, int *sign, int *i)
{
    if (str[*i] == '-' || str[*i] == '+')
    {
        if (str[*i] == '-')
            *sign = -1;
        (*i)++;
    }
}

double ft_atof(const char *str, bool *error)
{
    int     i;
    int     sign;
    double  result;

    i = 0;
    sign = 1;
    result = 0.0;
    if (!str)
        return (*error = false, result);
    while (isspace(str[i]))
        i++;
    handleSign(str, &sign, &i);
    while (str[i] >= '0' && str[i] <= '9')
        result = result * 10.0 + (str[i++] - '0');
    if (str[i])
        return (appendDot(str + i, result, sign, error));    
    return (result * sign);
}
