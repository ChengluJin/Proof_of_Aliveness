#include <stdio.h>
#include "parameters_64.h"
#include <time.h>
#include <stdlib.h>
#include "miracl.h"

int  subset_sum_initialize (int n_element, int n_bit, int n_leftover_bit, unsigned long long * set)
{

	if ((n_element <= 0) || (n_bit <= 0))
	{
		return FAILURE;
	}

	int i, j;

	//initialize PRG
	unsigned long ran;
	time((time_t *)&ran);
	irand(ran);
	unsigned long long modulus = 1;
	modulus = modulus << 32;

	for (i = 0; i < n_element; i++)
	{
		for (j = 0; j < n_bit; j++)
		{
			set[i*n_bit+j] = brand()%modulus;
			set[i*n_bit+j] = set[i*n_bit + j] << 31;
			set[i*n_bit+j] = set[i*n_bit + j] + brand()%modulus;
			if (j == n_bit - 1)
			{
				set[i*n_bit+j] = set[i*n_bit + j] >> n_leftover_bit;
			}
			//printf ("%llx, ", set[i*n_bit + j]);
		}
		//printf("\n");
	}
	return SUCCESS;
}

int  random_array (int n_bit, int n_leftover_bit, unsigned long long * a)
{

	if (n_bit <= 0)
	{
		return FAILURE;
	}

	int i;

	//initialize PRG
	unsigned long ran;
	time((time_t *)&ran);
	irand(ran);
	unsigned long long modulus = 1;
	modulus = modulus << 32;

	for (i = 0; i < n_bit; i++)
	{
		a[i] = brand()%modulus;
		a[i] = a[i] << 31;
		a[i] = a[i] + brand()%modulus;
		if (i == n_bit - 1)
		{
			a[i] = a[i] >> n_leftover_bit;
		}
		printf ("%llx, ", a[i]);
	}
	printf("\n");
	return SUCCESS;
}

int subset_sum_xor (int n_element, int n_bit, int n_leftover_bit, unsigned long long* set, unsigned long long* input, unsigned long long * output)
{
	/*if ((n_element <= 0) || (n_bit <= 0))
	{
		return FAILURE;
	}*/


	int i, j, index_element;
	int j_top;
	int j_index;
	int k;

	for (i = 0; i < n_bit; i++)
	{
		if (i == n_bit -1)
		{
			j_top = 64-n_leftover_bit;
		}
		else
		{
			j_top = 64;
		}
		for (j = 0; j < j_top; j++)
		{
			if (input[i] & 0x1 == 1)
			{
				index_element = ((i * 64) + j) * n_bit;
				/*for (k = 0; k < n_bit; k++)
				{
					//printf("%llx, ", output[k]);
					output[k] = set[index_element+k] ^ output[k];
					//printf("index i %d, j %d, k %d, index_element %d\n", i, j, k, index_element);
				}*/
				//printf("\n");
				//Specific program for n_bit = 7
				output[0] = set[index_element] ^ output[0];
				output[1] = set[index_element+1] ^ output[1];
				output[2] = set[index_element+2] ^ output[2];
				output[3] = set[index_element+3] ^ output[3];
				output[4] = set[index_element+4] ^ output[4];
				output[5] = set[index_element+5] ^ output[5];
				output[6] = set[index_element+6] ^ output[6];
			}
			input[i] = input[i] >> 1;
		}
	}

	return SUCCESS;
}

int main()
{
	miracl* mip = mirsys(1000,10);
	unsigned long long * owf_set = malloc(sizeof(unsigned long long) * OWF_N_ELEMENT * OWF_N_BIT);
	int status = FAILURE;
	status  = subset_sum_initialize(OWF_N_ELEMENT, OWF_N_BIT, OWF_LEFTOVER_BIT, owf_set);

	unsigned long long* a;
	unsigned long long* b;

	a = malloc (sizeof(unsigned long long) * OWF_N_BIT);
	b = malloc (sizeof(unsigned long long) * OWF_N_BIT);

	status = random_array(OWF_N_BIT, OWF_LEFTOVER_BIT, a);

	clock_t start_subset = clock();
	for (int i = 0; i < CHAIN_LEN; i++)
	{
		status = subset_sum_xor (OWF_N_ELEMENT, OWF_N_BIT, OWF_LEFTOVER_BIT, owf_set, a, b);
		status = subset_sum_xor (OWF_N_ELEMENT, OWF_N_BIT, OWF_LEFTOVER_BIT, owf_set, b, a);
	}
	clock_t end_subset = clock();

	double elapsed_secs = ((double)(end_subset - start_subset)) / CLOCKS_PER_SEC;
	elapsed_secs = (elapsed_secs * 1000 * 1000) / (2 * CHAIN_LEN);
	printf("Verification: 1 owf evaluation takes %lf microseconds\n", elapsed_secs);
	return SUCCESS;
}
