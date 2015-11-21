/* Matthew Weingarten        */ 
/* Senior Project            */ 
/* Fall 2015 - Fall 2015     */ 
/* Advisor: Dr. Schmalz      */ 
#include "img_head.h"
#include "img_cpx.h"
#define NEXT_P2(x) ((int) pow(2.0, ceil( LN2(x) ) ) )
int outCols; //columns after padding
double* img2;
void displayWHTImage(double *d, int rows, int cols)
{
	//displays image using ImageMagick's display routine
	normalizeImageArray(d, rows * cols);
	write_image(rows, cols, d, "temp_out.pgm");
	pid_t pid = fork();
	if(pid == 0) //child process
	{
		int status = execl("/usr/bin/display", "display", "temp_out.pgm", (char*) NULL);
		if(status == -1) //error executing display
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
	//uses a divide and conquer approach to compute the Hadamard matrix based on the number of iterations the user needs for their vector
	if(n == 1) //base case, default Hadamard matrix is [.5 .5 .5 -.5]
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
		result[0][0] = .5;
		result[0][1] = .5;
		result[1][0] = .5;
		result[1][1] = -.5;
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
				result[i][j] = result2[i][j] * .5;
			}
		}
		for(i = (1<<(n-1)); i < (1<<n); i++) //top right corner
		{
			for(j = 0; j < (1<<(n-1)); j++)
			{
				result[i][j] = result2[i - (1<<(n-1))][j] * .5;
			}
		}
		for(i = 0; i < (1<<(n-1)); i++) //bottom left corner
		{
			for(j = (1<<(n-1)); j < (1<<n); j++)
			{
				result[i][j] = result2[i][j - (1<<(n-1))] * .5;
			}
		}
		for(i = (1<<(n-1)); i < (1<<n); i++) //bottom right corner, need to invert entries
		{
			for(j = (1<<(n-1)); j < (1<<n); j++)
			{
				result[i][j] = result2[i - (1<<(n-1))][j - (1<<(n-1))] * -.5; //invert
			}
		}
		return result;
	}
}
double* multiply(double* data, double** hadamard1, int n)
{
	//multiplies the data vector and the Hadamard matrix to get the transformed vector
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
	//uses the MATLAB definition of fwht for doing the transform
	//note that their definition is different than the definition used in common literature
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
	double* result = malloc(n * sizeof(double));
	if(result == (double*) NULL) //error allocating space
	{
		printf("Error allocating space.\n");
		exit(1);
	}
	hadamard1 = hadamard(LN2(n)); //get result
	result = multiply(data, hadamard1, n); //multiply matrices
	for(i = 0; i < n; i++)
	{
		data[i] = result[i];
	}
	free(result); //free unnecessary memory
}
void realToDouble2(double *d, int n, double* ret, int retN)
{
    int i;
    for(i = 0; i < n; i++) //copy elements over
    {
    	  ret[i] = d[i];
    }
    for(i = n; i < retN; i++) //zero padded columns in each row
    {
    	  ret[i] = 0.0;
    }
}
void img_wht(int rows, int cols, double *img)
{
   outCols = NEXT_P2(cols); //computes next power of 2
   int size = rows * outCols * sizeof(double);
   printf("WHT Area: %u x %u\n", rows, outCols);
   double* imgA = malloc(size);
   if(imgA == (double*) NULL) //error allocating space
   {
       printf("Error allocating space.\n");
   	   exit(1);
   }
   int i;
   for(i = 0; i < rows; i++)
   {
	   realToDouble2(img + (cols * i), cols, imgA + (i * outCols), outCols); //zeros padded entries of array for each row
   }
   for(i = 0; i < rows; i++) //apply Hadamard transform to each row
   {
	   hadamardTransform(imgA + (i * outCols), outCols);
   }
   displayWHTImage(imgA, rows, outCols); //display result
   img2 = malloc((rows*outCols) * sizeof(double));
   if(img2 == (double*) NULL) //error allocating space
   {
	   printf("Error allocating space.\n");
       exit(1);
   }
   for(i = 0; i < rows*outCols; i++) //copies values over for img_wht_rmse
   {
       img2[i] = imgA[i];
   }
}
void rmseCalculations(double* error, int rows, int cols)
{
	//calculate mean and RMSE of error values for a given error matrix
	int i;
	double sum = 0.0;
	for(i = 0; i < rows * cols; i++) //computes sums of error values
	{
		sum += error[i];
	}
	double mean = sum / (rows * cols); //takes the average of all the error values
	printf("Mean of error values: %.3f\n", mean);
	double variance = 0.0;
	for(i = 0; i < rows * cols; i++)
	{
		variance += pow((error[i] - mean), 2.0); //calculates the variance of the error values 
	}
	double rmse = sqrt(variance) / sqrt(rows * cols); //calculates the RMSE of the error values
	printf("RMSE of error values: %.3f\n", rmse);
}
void img_wht_rmse(int rows, int cols, double *img, double *img1, int blocks)
{
	//performs simple error analysis on the difference between the output of the
	//Walsh-Hadamard transformation and MATLAB's fwht implementation
	//performs this error analysis on the whole image and each segment to better identify any error
	int i;
	img_wht(rows, cols, img); //perform WHT on input image
	for(i = 0; i < rows*outCols; i++)
	{
		img2[i] = round(img2[i]); //round to nearest integer to work with PGM
	}
	double* error = malloc((rows * outCols) * sizeof(double));
	if(error == (double*) NULL) //error allocating space
	{
		printf("Error allocating space.\n");
		exit(1);
	}
	for(i = 0; i < rows * outCols; i++) //takes the difference between each point in image arrays
	{
		error[i] = img2[i] - img1[i];
	}
	printf("For the whole image: \n");
	rmseCalculations(error, rows, outCols); //calculate mean and RMSE of error values along whole image
	double rowSize = (double)(rows) / blocks;
	int rowSize2 = ceil(rowSize); //take the ceiling to achieve a whole number
	double* error2 = malloc((rowSize2 * blocks * outCols) * sizeof(double));
	if(error2 == (double*) NULL) //error allocating space
	{
		printf("Error allocating space.\n");
		exit(1);
	}
	for(i = 0; i < rowSize2 * blocks * outCols; i++)
	{
		if(i >= rows * outCols)
		{
			error2[i] = 0.0; //zero-padding
		}
		else
		{
			error2[i] = error[i];
		}
	}
	int j;
	for(i = 0; i < blocks; i++)
	{
		double* error3 = malloc((rowSize2 * outCols) * sizeof(double));
		if(error3 == (double*) NULL) //error allocating space
		{
			printf("Error allocating space.\n");
			exit(1);
		}
		for(j = 0; j < rowSize2 * outCols; j++)
		{
			error3[j] = error2[i*rowSize2*outCols + j]; //copy over necessary part
		}
		printf("For segment %d, which ranges from rows %d (included) to %d (not included):\n", (i + 1), i*rowSize2, (i + 1)*rowSize2);
		rmseCalculations(error3, rowSize2, outCols); //calculate error over corresponding segment
	}
	free(error);
	free(error2);
}
