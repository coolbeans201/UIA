#include <stdlib.h>
#include <stdio.h>
int** hadamard (int n)
{
	//use a divide and conquer approach to compute the Hadamard matrix based on the number of iterations the user needs for their vector
	if(n == 1) //base case, default Hadamard matrix is [1 1 1 -1]
	{
		int** result = malloc(2 * sizeof(int*)); //allocate space
		if(result == (int**) NULL) //error
		{
			printf("Error allocating space.\n");
			exit(0);
		}
		int i;
		for(i = 0; i < 2; i++)
		{
			result[i] = malloc(2 * sizeof(int)); //allocate space
			if(result[i] == (int*)NULL) //error
			{
				printf("Error allocating space.\n");
				exit(0);
			}
		}
		result[0][0] = 1;
		result[0][1] = 1;
		result[1][0] = 1;
		result[1][1] = -1;
		return result;
	}
	else
	{
		int** result = malloc((1<<n) * sizeof(int *)); //allocate space
		if(result == (int**) NULL) //error
		{
			printf("Error allocating space.\n");
			exit(0);
		}
		int i;
		for(i = 0; i < (1<<n); i++)
		{
			result[i] = malloc((1<<n) * sizeof(int)); //allocate space
			if(result[i] == (int*)NULL) //error
			{
				printf("Error allocating space.\n");
				exit(0);
			}
		}
		int** result2 = malloc((1<<(n-1)) * sizeof(int *)); //allocate space for previous computation
		if(result2 == (int**) NULL) //error
		{
			printf("Error allocating space.\n");
			exit(0);
		}
		for(i = 0; i < 1<<(n-1); i++)
		{
			result2[i] = malloc((1<<(n-1)) * sizeof(int)); //allocate space for previous computation
			if(result2[i] == (int*)NULL) //error
			{
				printf("Error allocating space.\n");
				exit(0);
			}
		}
		result2 = hadamard(n - 1); //recursion
		int j;
		for(i = 0; i < (1<<(n-1)); i++) //top left corner
		{
			for(j = 0; j < (1<<(n-1)); j++)
			{
				result[i][j] = result2[i][j];
			}
		}
		for(i = (1<<(n-1)); i < (1<<n); i++) //top right corner
		{
			for(j = 0; j < (1<<(n-1)); j++)
			{
				result[i][j] = result2[i - (1<<(n-1))][j];
			}
		}
		for(i = 0; i < (1<<(n-1)); i++) //bottom left corner
		{
			for(j = (1<<(n-1)); j < (1<<n); j++)
			{
				result[i][j] = result2[i][j - (1<<(n-1))];
			}
		}
		for(i = (1<<(n-1)); i < (1<<n); i++) //bottom right corner, need to invert entries
		{
			for(j = (1<<(n-1)); j < (1<<n); j++)
			{
				result[i][j] = -1 * result2[i - (1<<(n-1))][j - (1<<(n-1))]; //invert
			}
		}
		return result;
	}
}
int ilog2(int x)
{
	//uses right shifts to compute the log base 2 of a number
    long l2 = 0;
    for (; x; x >>=1) //right shift to divide by 2
	{
		++l2;
	}
    return l2;
}
int** multiply(int* data, int** hadamard1, int n)
{
	//Multiplies the data vector and the Hadamard matrix to get the transformed vector
	int** result = malloc(1 * sizeof(int*)); //allocate space
	if(result == (int**) NULL) //error
	{
		printf("Error allocating space.\n");
		exit(0);
	}
	result[0] = malloc(n * sizeof(int)); //allocate space
	if(result[0] == (int*) NULL) //error
	{
		printf("Error allocating space.\n");
		exit(0);
	}
	int a, b, c, sum = 0;
	for(a = 0; a < 1; a++)
    	{
		for(b = 0; b < n; b++)
		{
			for(c = 0; c < n; c++)
			{
				sum = sum + data[c]*hadamard1[c][b]; //matrix multiplication
			}
			result[a][b] = sum;
			sum = 0;
		}
	}
	return result;
}
int** hadamardTransform(int* data, int n)
{
	//gets Hadamard matrix, multiplies it by vector, and then returns result
	int** hadamard1 = malloc(n * sizeof(int*)); //allocate space for Hadamard matrix
	if(hadamard1 == (int**) NULL) //error
	{
		printf("Error allocating space.\n");
		exit(0);
	}
	int i;
	for(i = 0; i < n; i++)
	{
		hadamard1[i] = malloc(n * sizeof(int)); //allocate space for Hadamard matrix
		if(hadamard1[i] == (int*) NULL) //error
		{
			printf("Error allocating space.\n");
			exit(0);
		}
	}
	hadamard1 = hadamard(ilog2(n)); //get result
	int** finalResult = malloc(1 * sizeof(int*)); //allocate space for matrix multiplication
	if(finalResult == (int**) NULL) //error
	{
		printf("Error allocating space.\n");
		exit(0);
	}
	finalResult[0] = malloc(n * sizeof(int)); //allocate space for matrix multiplication
	if(finalResult[0] == (int*) NULL) //error
	{
		printf("Error allocating space.\n");
		exit(0);
	}
	finalResult = multiply(data, hadamard1, n); //multiply matrices
   return finalResult;
}
void printResult(int** finalResult, int n)
{
	//prints out result of Hadamard transformation
	printf("The Hadamard transform applied to the original vector is: \n");
	int i;
	for(i = 0; i < n; i++)
	{
		printf("%d ", finalResult[0][i]);
	}
	printf("\n");
}
int main(int argc, char** argv)
{
	//Calculates Hadamard transform for a vector passed in from the command line
	if(argc < 3) //not enough arguments
	{
		printf("Entered too few arguments. The vector must have a size of a power of two greater than one. Please try again with correct input.\n");
		exit(0);
	}
	int* data = malloc((argc - 1) * sizeof(int)); //argc - 1 represents the number of vector elements since the first argument is always reserved for the program name
	if(data == (int*) NULL) //error
	{
		printf("Error allocating space.\n");
		exit(0);
	}
	int i;
	for(i = 0; i < (argc - 1); i++)
	{
		if(sscanf(argv[i + 1], "%d ", &data[i]) != 1) //not an integer, error
		{
			printf("Invalid input. Vector must only contain integers. Please try again.\n");
			exit(0);
		}
	}
	int** finalResult = malloc(1 * sizeof(int*)); //allocate space
	if(finalResult == (int**) NULL) //error
	{
		printf("Error allocating space.\n");
		exit(0);
	}
	finalResult[0] = malloc((argc - 1) * sizeof(int)); //allocate space for final result
	if(finalResult[0] = (int*) NULL) //error
	{
		printf("Error allocating space.\n");
		exit(0);
	}
	finalResult = hadamardTransform(data, (argc - 1));
	printResult(finalResult, (argc - 1));
	return 0;
}
