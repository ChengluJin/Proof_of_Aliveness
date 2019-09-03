#include <stdio.h>
#include "miracl.h"
#include "parameters_hash.h"
#include <time.h>
#include <stdlib.h>

int main()
{

	//generate a random head 
	unsigned char K_string[32];
	srand(time(NULL));
	for (int i = 0; i < 32; i++)
	{
		K_string[i] = rand() & 0xff;
	}


	sha256 sh;
	unsigned char hash[32];


	clock_t start_hash = clock();
	for (int k = 0; k < CHAIN_LEN; k++)
	{
		shs256_init(&sh);
		for (int j=0; j < 32;j++) 
		{
			shs256_process(&sh,K_string[j]);
		}
		shs256_hash(&sh,hash);    

		shs256_init(&sh);
		for (int j=0; j < 32;j++) 
		{
			shs256_process(&sh,hash[j]);
		}
		shs256_hash(&sh,K_string);    


	}
	clock_t end_hash = clock();
	for (int j = 0; j < 32; j++)
	{
	printf("%x ", K_string[j]);
	}	
	printf("\n");
	double elapsed_secs_hash = ((double)(end_hash - start_hash)) / CLOCKS_PER_SEC;
	elapsed_secs_hash = (elapsed_secs_hash* 1000 * 1000) / (2 * CHAIN_LEN);
	printf("Verification: 1 hashes take %lf microseconds on average\n", elapsed_secs_hash);


	return SUCCESS;
}

