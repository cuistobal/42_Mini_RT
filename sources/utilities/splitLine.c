#include "minirt.h"

//Cette fonction compte le nombre de mots presents dans la chaine.
//Chaque suite de char sans separateur est consideree comme etant un mot.
static int	countArgs(char const *s, char c)
{
	size_t		i;
	size_t		word;

	i = 0;
	while (*s)
	{
		word = 0;
		while (ft_strchr(*s, c) && *s)
			s++;
		while (!ft_strchr(*s, c) && *s)
		{
			if (!word)
			{
				i++;
				word++;
			}
			s++;
		}
	}
	return (i + 1);
}

//Cette fonction permet d'allouer l espace necessaire & copie le mot dans le
//tableau ou il doit etre split.
static char	*ft_fill(const char *s, char c)
{
	char	*word;
	size_t	i;

	i = 0;
	while (s[i] && ft_strchr(c, s[i]))
		i++;
	word = malloc(sizeof(char) * (i + 1));
	if (!word)
		return (0);
	i = 0;
	while (s[i] && s[i] != c)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

//Split - i est l'index de debut de mot, la boucle avance grace a j tant que
//le char est != c, on incremente k pour mettre le mot dans le bon sous tab.
//Si l'allocation foire, on envoie le sosu tableau dans ft_free.
char	**splitLine(char const *s, char *c, int *len)
{
	int		i;
	int		k;
	char	**dest;

	i = 0;
	k = 0;
	if (!s)
		return (NULL);
	*len = countArgs(s, c);
	dest = malloc(sizeof(char *) * *len);
	if (!dest)
		return (NULL);
	while (i < ft_strlen(s) && k < ft_countword(s, c))
	{
		while (ft_strchr(c, s[i]))
			i++;
		dest[k] = fill(s + i, c);
		if (!dest[k])
			return (freeArr(dest, k));
		k++;
		while (s[i] && !ft_strchr(c, s[i]))
			i++;
	}
	return (dest[k] = NULL, dest);
}
