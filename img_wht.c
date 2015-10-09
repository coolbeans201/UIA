/* Matthew Weingarten        */ 
/* Senior Project            */ 
/* Fall 2015 - Fall 2015     */ 
/* Advisor: Dr. Schmalz      */ 
#include "img_head.h"
#define NEXT_P2(x) ((int) pow(2.0, ceil( LN2(x) ) ) )
void displayWHTImage(double *d, int rows, int cols)
{
	normalizeImageArray(d, rows * cols);
	write_image(rows, cols, d, "temp_out.pgm");
	pid_t pid = fork();
	if(pid == 0) //child process
	{
		int status = execl("/usr/bin/display", "display", "temp_out.pgm", (char*) NULL);
		if(status == -1)
		{
			printf("Error executing display.\n");
			exit(1);
		}
	}
	else //parent process
	{
		int i, loc;
		int status = kill(pid, SIGKILL);
		if(status == -1) //error killing
		{
			printf("Error killing parent process.\n");
			exit(1);
		}
		normalizeImageArray(d, rows * cols);
		status = waitpid(pid, &loc, 0); //wait for previous display to die
		if(status == -1) //error waiting
		{
			printf("Error waiting for previous display to die.\n");
			exit(1);
		}
		status = unlink("temp_out.pgm");
		if(status == -1) //error unlinking
		{
			printf("Error unlinking.\n");
			exit(1);
		}
		write_image(rows, cols, d, "temp_out.pgm");
		pid = fork();
		if(pid == 0) //child process
		{
			status = execl("/usr/bin/display", "display", "temp_out.pgm", (char*) NULL);
			if(status == -1) //error executing
			{
				printf("Error executing.\n");
				exit(1);
			}
		}
		printf("Close display box (click <<X>>) to continue...\n");
		status = waitpid(pid, &loc, 0);
		if(status == -1) //error waiting
		{
			printf("Error waiting.\n");
			exit(1);
		}
		status = unlink("temp_out.pgm");
		if(status == -1) //error unlinking
		{
			printf("Error unlinking.\n");
			exit(1);
		}
	}
}
double** hadamard (int n)
{
	//use a divide and conquer approach to compute the Hadamard matrix based on the number of iterations the user needs for their vector
	if(n == 1) //base case, default Hadamard matrix is [1 1 1 -1]
	{
		double** result = malloc(2 * sizeof(double*)); //allocate space
		if(result == (double**) NULL) //error
		{
			printf("Error allocating space.\n");
			exit(1);
		}
		int i;
		for(i = 0; i < 2; i++)
		{
			result[i] = malloc(2 * sizeof(double)); //allocate space
			if(result[i] == (double*)NULL) //error
			{
				printf("Error allocating space.\n");
				exit(1);
			}
		}
		result[0][0] = 1/sqrt(2);
		result[0][1] = 1/sqrt(2);
		result[1][0] = 1/sqrt(2);
		result[1][1] = -1/sqrt(2);
		return result;
	}
	else //recursive case
	{
		double** result = malloc((1<<n) * sizeof(double *)); //allocate space
		if(result == (double**) NULL) //error
		{
			printf("Error allocating space.\n");
			exit(1);
		}
		int i;
		for(i = 0; i < (1<<n); i++)
		{
			result[i] = malloc((1<<n) * sizeof(double)); //allocate space
			if(result[i] == (double*)NULL) //error
			{
				printf("Error allocating space.\n");
				exit(1);
			}
		}
		double** result2 = malloc((1<<(n-1)) * sizeof(double*)); //allocate space for previous computation
		if(result2 == (double**) NULL) //error
		{
			printf("Error allocating space.\n");
			exit(1);
		}
		for(i = 0; i < 1<<(n-1); i++)
		{
			result2[i] = malloc((1<<(n-1)) * sizeof(double)); //allocate space for previous computation
			if(result2[i] == (double*)NULL) //error
			{
				printf("Error allocating space.\n");
				exit(1);
			}
		}
		result2 = hadamard(n - 1); //recursion
		int j;
		for(i = 0; i < (1<<(n-1)); i++) //top left corner
		{
			for(j = 0; j < (1<<(n-1)); j++)
			{
				result[i][j] = result2[i][j] * 1/sqrt(2);
			}
		}
		for(i = (1<<(n-1)); i < (1<<n); i++) //top right corner
		{
			for(j = 0; j < (1<<(n-1)); j++)
			{
				result[i][j] = result2[i - (1<<(n-1))][j] * 1/sqrt(2);
			}
		}
		for(i = 0; i < (1<<(n-1)); i++) //bottom left corner
		{
			for(j = (1<<(n-1)); j < (1<<n); j++)
			{
				result[i][j] = result2[i][j - (1<<(n-1))] * 1/sqrt(2);
			}
		}
		for(i = (1<<(n-1)); i < (1<<n); i++) //bottom right corner, need to invert entries
		{
			for(j = (1<<(n-1)); j < (1<<n); j++)
			{
				result[i][j] = result2[i - (1<<(n-1))][j - (1<<(n-1))] * -1/sqrt(2); //invert
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
double* multiply(double* data, double** hadamard1, int n)
{
	//Multiplies the data vector and the Hadamard matrix to get the transformed vector
	double*  result = malloc(n * sizeof(double)); //allocate space
	if(result == (double*) NULL) //error
	{
		printf("Error allocating space.\n");
		exit(1);
	}
	int a, b;
	double sum = 0.0;
	for(a = 0; a < n; a++)
	{
		for(b = 0; b < n; b++)
		{
			sum = sum + data[b]*hadamard1[b][a]; //matrix multiplication
		}
		result[a] = sum;
		sum = 0.0;
	}
	return result;
}
void hadamardTransform(double* data, int n)
{
	//gets Hadamard matrix, multiplies it by data vector, and then sets data vector to that result
	double** hadamard1 = malloc(n * sizeof(double*)); //allocate space for Hadamard matrix
	if(hadamard1 == (double**) NULL) //error
	{
		printf("Error allocating space.\n");
		exit(1);
	}
	int i;
	for(i = 0; i < n; i++)
	{
		hadamard1[i] = malloc(n * sizeof(double)); //allocate space for Hadamard matrix
		if(hadamard1[i] == (double*) NULL) //error
		{
			printf("Error allocating space.\n");
			exit(1);
		}
	}
	hadamard1 = hadamard(ilog2(n)); //get result
	data = multiply(data, hadamard1, n); //multiply matrices
}
void img_wht(int rows, int cols, double *img)
{
   int outRows, outCols;
   int i;
   outRows = NEXT_P2(rows); //computes next power of 2
   outCols = NEXT_P2(cols); //computes next power of 2
   int size = outRows * outCols * sizeof(double);
   printf("WHT Area: %u x %u\n", outRows, outCols);
   img = (double*) realloc(img, size);
   if(img == (double*) NULL) //error reallocating space
   {
   	printf("Error reallocating space.\n");
   	exit(1);
   }
	for(i = rows * cols; i < outRows * outCols; i++) //zero other entries
	{
		img[i] = 0.0;
	}
   for(i = 0; i < outCols; i++) //apply Hadamard transform to each row
   {
       hadamardTransform(img + (i * outRows), outRows);
   }
   displayWHTImage(img, rows, cols); //display result
}
