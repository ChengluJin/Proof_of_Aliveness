#include <stdio.h>
#include "miracl.h"
#include "parameters_m_RO.h"
#include <time.h>
#include <stdlib.h>
#include "aes_prg.h"

int main()
{
	aes aes_instance;

	prg_init_aes (&aes_instance, AES_KEY_LEN);

	char ctr[16] = {0};

	char * random_numbers = malloc (sizeof(char) * AES_PRG_OUT_LEN);

	int i, j;

	sha256 sh;

	unsigned char hash[32];

	unsigned char msg[32];

	unsigned char * verification_key = malloc (sizeof (unsigned char) * N_SUB_CHAIN * 32);

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

		for (int l = 0; l < 32; l++)
		{
			verification_key[i* 32 + l] = hash[l];
		}
			
	}
	

	//generate one time signature
	shs256_init (&sh);
	char ctr_temp[16] = {0};
	char temp_bit;
	clock_t start_subset = clock();
	for (int i = 0; i < N_SUB_CHAIN * 32; i++)
	{
		shs256_process(&sh, verification_key[i]);
	}

	shs256_hash(&sh, hash);
	for (int i = 0; i < 32; i++)
	{
		for(int j = 0 ; j < 8; j++)
		{
			temp_bit = (hash[i] >> j) & 0x1;
			if (temp_bit == 0)
			{
				prg_gen_aes (&aes_instance, ctr_temp, random_numbers, AES_PRG_OUT_LEN); 
				ctr_increment (ctr_temp, AES_PRG_OUT_LEN);
			}
			else
			{
				ctr_increment (ctr_temp, AES_PRG_OUT_LEN);
				prg_gen_aes (&aes_instance, ctr_temp, random_numbers, AES_PRG_OUT_LEN);
			}
		}
	}
	clock_t end_subset = clock();
	double elapsed_secs = ((double)(end_subset - start_subset)) * (1000) / CLOCKS_PER_SEC;
	printf("Replenishment: Multi_chain (%d sub-chains) takes %lf milliseconds in the RO model\n", N_SUB_CHAIN, elapsed_secs);

	return SUCCESS;
}
