#include "minirt.h"

//
static void	maxHeapify(int *arr, int size, int i)
{
	int	temp;
	int	left;
	int	right;
	int	largest;

    largest = i;
    left = 2 * i + 1;
    right = 2 * i + 2;
    if (left < size && arr[left] > arr[largest])
        largest = left;
    if (right < size && arr[right] > arr[largest])
        largest = right;
    if (largest != i)
    {
		temp = arr[i];
		arr[i] = arr[largest];
		arr[largest] = temp;
        maxHeapify(arr, size, largest);
    }
}

//
static void	minHeapify(int *arr, int size, int i)
{
	int	temp;
	int	left;
	int	right;
	int	smallest;

    smallest = i;
    left = 2 * i + 1;
    right = 2 * i + 2;
    if (left < size && arr[left] < arr[smallest])
        smallest = left;
    if (right < size && arr[right] < arr[smallest])
        smallest = right;
    if (smallest != i)
    {
		temp = arr[i];
		arr[i] = arr[smallest];
		arr[smallest] = temp;
        minHeapify(arr, size, smallest);
    }
}


//
static void	buildMinHeap(int *arr, int size, void (*sorter)(int *, int, int))
{
	int i;

	i = (size << 1) - 1;
	while (i--)
		sorter(arr, size, i);
}

//
static void	buildMaxHeap(int *arr, int size, void (*sorter)(int *, int, int))
{
	int i;

	i = (size << 1) - 1;
	while (i--)
		sorter(arr, size, i);
}

//Applies heap sort on the array based on the boolean scenario
int	*heapSort(int *arr, int size, bool type)
{
	int		i;
	int		temp;
	void	(*sorter)(int *, int, int);
	void	(*builder)(int *, int, void (*)(int *, int, int));

	i = size - 1;
	if (type)
	{
		builder = buildMaxHeap;
		sorter = maxHeapify;
	}
	else
	{
		builder = buildMinHeap;
		sorter = minHeapify;
	}
	builder(arr, size, sorter);
	while (i > 0)
	{
		temp = arr[0];
		arr[0] = arr[i];
		arr[i] = temp;
		sorter(arr, i, 0);
		i--;
	}
	return (arr);
}
