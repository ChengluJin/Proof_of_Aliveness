#include <stdio.h>
#include "miracl.h"
#include "parameters_m_RO.h"
#include <time.h>
#include <stdlib.h>
#include "aes_prg.h"

int main()
{
	aes aes_instance;

	char aes_key[32];

	prg_init_aes (&aes_instance, AES_KEY_LEN);

	char ctr[16] = {0};

	char * random_numbers = malloc (sizeof(char) * AES_PRG_OUT_LEN);

	int i, j;

	sha256 sh;

	unsigned char hash[64];

	unsigned char msg[64];

	clock_t start_subset = clock();
	for (i = 0; i < N_SUB_CHAIN; i++)
	{
		prg_gen_aes (&aes_instance, ctr, random_numbers, AES_PRG_OUT_LEN);
		for (int l = 0; l < AES_PRG_OUT_LEN; l++)
		{
			msg[l] = random_numbers[l];
		}	
		for (j = 0; j < N_NODE_PER_CHAIN; j++)
		{
			shs256_init(&sh);
			for (int k=0; k < AES_PRG_OUT_LEN;k++) 
			{
				shs256_process(&sh,msg[k]);
			}
			shs256_hash(&sh,hash);    

			for (int l = 0; l < 32; l++)
			{
				msg[l] = hash[l];
			}	
		}
	}
	clock_t end_subset = clock();
	double elapsed_secs = ((double)(end_subset - start_subset)) / CLOCKS_PER_SEC;
	elapsed_secs = (elapsed_secs * 1000 * 1000) / (N_SUB_CHAIN * N_NODE_PER_CHAIN);
	printf("Generation: Multi_chain (%d nodes per sub-chain) takes %lf microseconds to generate one proof on average in the RO model\n", N_NODE_PER_CHAIN, elapsed_secs);

	return SUCCESS;
}
