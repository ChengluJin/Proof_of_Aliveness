#include <stdio.h>
#include "parameters_m_STD.h"
#include <time.h>
#include <stdlib.h>
#include "miracl.h"
#include "subset_sum_64bits.h"
#include <unistd.h>

int prg_standard_initialize (int owf_n_element, int owf_n_bit, int owf_n_leftover_bit, unsigned long long * owf_set, int HC_n_element, int HC_n_bit, int HC_n_leftover_bit, unsigned long long * HC_set, unsigned long long * prg_input, unsigned long long*  seed)
{
	subset_sum_initialize(owf_n_element, owf_n_bit, owf_n_leftover_bit, owf_set);
	subset_sum_initialize(HC_n_element, HC_n_bit, HC_n_leftover_bit, HC_set);
	random_array (owf_n_bit, owf_n_leftover_bit, prg_input);
	sleep(1);
	random_array (owf_n_bit, owf_n_leftover_bit, seed);
	return SUCCESS;
}

int hc (int HC_n_element, int HC_n_bit, int HC_n_leftover_bit, unsigned long long * HC_set, unsigned long long * input, char * hc_output)
{
	int i, j;
	unsigned long long temp_result;
	char temp_bit = 0; 


	for (i = 0; i < HC_n_element; i ++)
	{
		temp_bit = 0;
		for (j = 0; j < HC_n_bit; j++)
		{
			if ((j == HC_n_bit-1) && (HC_n_leftover_bit != 0))
			{
				temp_result = (input[j] & HC_set[i*HC_n_bit + j]) >> HC_n_leftover_bit; 
			}
			else
			{
				temp_result = input[j] & HC_set[i*HC_n_bit + j]; 
			}
			temp_result = ((temp_result >> 32) ^ (temp_result)) & 0xffffffff; 
			temp_result = ((temp_result >> 16) ^ (temp_result)) & 0xffff; 
			temp_result = ((temp_result >> 8) ^ (temp_result)) & 0xff; 
			temp_result = ((temp_result >> 4) ^ (temp_result)) & 0xf; 
			temp_result = ((temp_result >> 2) ^ (temp_result)) & 0x3; 
			temp_result = ((temp_result >> 1) ^ (temp_result)) & 0x1; 
			temp_bit = temp_bit ^ (temp_result & 0x1);
		}
		hc_output[i] = temp_bit;
		//printf("%d\n", temp_bit);
	}
	return SUCCESS;
}

int prg_standard_generate (int owf_n_element, int owf_n_bit, int owf_n_leftover_bit, unsigned long long * owf_set, int HC_n_element, int HC_n_bit, int HC_n_leftover_bit, unsigned long long * HC_set, unsigned long long*  seed, int prg_loop, unsigned long long* prg_input, unsigned long long * prg_output)
{
	int i, j, k;

	unsigned long long* comb_input = malloc(sizeof (unsigned long long) * owf_n_bit); 
	char * hc_output = malloc(sizeof(char) * HC_n_element);

	sha512 sh;
	char temp_hash;
	char hash[64];
	int output_bit_ctr = 0;

	for (i = 0; i < prg_loop; i ++)
	{
		/*printf("before:\n");
		for (k = 0 ; k < owf_n_bit; k++)
		{
			printf("%llx ", comb_input[k]);
		}
		printf("\n");
		for (k = 0 ; k < owf_n_bit; k++)
		{
			printf("%llx ", prg_input[k]);
		}
		printf("\n");
		for (k = 0 ; k < owf_n_bit; k++)
		{
			printf("%llx ", seed[k]);
		}
		printf("\n");*/
		for (j = 0; j < owf_n_bit; j++)
		{
			comb_input[j] = prg_input[j] ^ seed[j];
		}
		/*printf("after xor:\n");
		for (k = 0 ; k < owf_n_bit; k++)
		{
			printf("%llx ", comb_input[k]);
		}
		printf("\n");*/
		for (j = 0; j < PRG_H_SHA512_LOOP; j++)
		{
			shs512_init(&sh);
			for (k=0; k < owf_n_bit;k++) 
			{
				temp_hash = (comb_input[k]) & 0xff;
				shs512_process(&sh,temp_hash);
				temp_hash = (comb_input[k] >> 8) & 0xff;
				shs512_process(&sh,temp_hash);
				temp_hash = (comb_input[k] >> 16) & 0xff;
				shs512_process(&sh,temp_hash);
				temp_hash = (comb_input[k] >> 24) & 0xff;
				shs512_process(&sh,temp_hash);
				temp_hash = (comb_input[k] >> 32) & 0xff;
				shs512_process(&sh,temp_hash);
				temp_hash = (comb_input[k] >> 40) & 0xff;
				shs512_process(&sh,temp_hash);
				temp_hash = (comb_input[k] >> 48) & 0xff;
				shs512_process(&sh,temp_hash);
				temp_hash = (comb_input[k] >> 56) & 0xff;
				shs512_process(&sh,temp_hash);
			}
			shs512_process(&sh, j);
			shs512_hash(&sh,hash);    

			/*for (k = 0; k < 64; k++)
			{
				printf(" %x ", hash[k]);
			}
			printf("before\n");
			for (k = 0; k < PRG_OWF_N_BIT; k++)
			{
				printf("%llx ", seed[k]);
			}*/

			for (k = 0; k < 64; k++)
			{
				seed[(j*64+k)/8] = (seed[(j*64+k)/8] << 8) + (hash[k] & 0xff);
			}
			/*printf("\nafter\n");
			for (k = 0; k < PRG_OWF_N_BIT; k++)
			{
				printf("%llx ", seed[k]);
			}
			printf("\n");*/

		}
		hc (HC_n_element, HC_n_bit, HC_n_leftover_bit, HC_set, comb_input, hc_output);
		subset_sum_xor_generic (owf_n_element, owf_n_bit, owf_n_leftover_bit, owf_set, comb_input, prg_input);
		for (j = 0; j < PRG_HC_OUTPUT_LENGTH - PRG_OUTPUT_PER_LOOP; j++)
		{
			seed[(PRG_H_SHA512_LOOP * 8) + (j/64)] = (seed[(PRG_H_SHA512_LOOP *8) + (j/64)] << 1) + hc_output[j];
		}

		for (j = PRG_HC_OUTPUT_LENGTH - PRG_OUTPUT_PER_LOOP; j < PRG_HC_OUTPUT_LENGTH; j++)
		{
			prg_output[output_bit_ctr/64] = (prg_output[output_bit_ctr/64] << 1) + hc_output[j];
			output_bit_ctr ++ ;
		}
		//printf("output bit%d:", output_bit_ctr);
	}
	prg_output[OWF_N_BIT - 1] = prg_output[OWF_N_BIT - 1] & (0xffffffff >> OWF_LEFTOVER_BIT);

	/*printf("PRG OUT:");
	for (i = 0; i < OWF_N_BIT; i++)
	{
		printf("%llx ", prg_output[i]);
	}
	printf("\n");*/

	return 0;
}
