#include <stdio.h>
#include "miracl.h"
#include "parameters_m_RO.h"
#include <time.h>
#include <stdlib.h>

int prg_init_aes (aes * aes_instance, int key_length)
{

	//initialize PRG
	unsigned long ran;
	time((time_t *)&ran);
	//printf("%x\n", ran);
	//irand(ran);
	//brand();
	//printf("a\n");

	char* key = malloc(sizeof(char) * key_length);
	//char key[AES_KEY_LEN];
	int i;


	for (i = 0; i < key_length; i++)
	{
		//key[i] = brand()&0xff;
		key[i] = i;
		//printf ("%x, ", key[i]);
	}
	//printf("\n");

	aes_init(aes_instance, MR_ECB, key_length, key, NULL);
	return SUCCESS;
}

//AES CTR mode PRG with result converted into 64-bit arrays
int prg_gen_aes_conv (aes * aes_instance, char* ctr, unsigned long long * random_head, int n_bit, int n_leftover_bit ) 
{
	char ctr_copy[16] = {0};
	int i,j, loop;
	int carry = 0;

	if (n_bit%2 == 1)
	{
		loop = (n_bit+1)/2;
	}
	else
	{
		loop = n_bit/2;
	}

	for (j = 0; j < loop; j++)
	{
		for (i = 0; i < 16; i++)
		{
			ctr_copy[i] = ctr[i];
		}

		if ((ctr_copy[0] & 0xff) == 0xff)
		{
			ctr_copy[0] = 0x00;
			carry = 1;
		}
		else
		{
			ctr_copy[0] = ctr_copy[0] + 1;
			carry = 0;
		}

		for (i = 1; i < 16; i++)
		{
			if (carry == 0)
			{
				break;
			}
			else if ((ctr_copy[i] & 0xff) == 0xff)
			{
				ctr_copy[i] = 0x00;
				carry = 1;
			}
			else
			{
				ctr_copy[i] = ctr_copy[i] + 1;
				carry = 0;
			}
		}

		for (i = 0; i < 16; i++)
		{
			ctr[i] = ctr_copy[i];
		}

		aes_encrypt(aes_instance, ctr_copy);

		for (i = 0; i < 8; i++)
		{
			random_head[2*j] = (random_head[2*j] << 8 ) | (ctr_copy[i] & 0xff);
		}

		if ((j != loop -1) || (n_bit%2 == 0))
		{
			for (i = 8; i < 16; i++)
			{
				random_head[2*j+1] = (random_head[2*j+1] << 8 ) | (ctr_copy[i] & 0xff);
			}
		}
	}

	if (n_leftover_bit != 0)
	{
		random_head[n_bit-1] = random_head[n_bit-1] >> n_leftover_bit;
	}

	/*for (i = 0; i < n_bit; i++)
	  {
	  printf("%llx ", random_head[i]);
	  }
	  printf("\n");*/

	return SUCCESS;
}

//AES CTR mode PRG outputing char 
int prg_gen_aes (aes * aes_instance, char* ctr, char* random_numbers, int random_length)
{
	char ctr_copy[16] = {0};
	int i,j, loop;
	int carry = 0;
	int index = 0;

	if (random_length % 16 == 0)
	{
		loop = random_length / 16;
	}
	else
	{
		loop = (random_length / 16) + 1; 
	}

	for (j = 0; j < loop; j++)
	{
		for (i = 0; i < 16; i++)
		{
			ctr_copy[i] = ctr[i];
		}

		if ((ctr_copy[0] & 0xff) == 0xff)
		{
			ctr_copy[0] = 0x00;
			carry = 1;
		}
		else
		{
			ctr_copy[0] = ctr_copy[0] + 1;
			carry = 0;
		}

		for (i = 1; i < 16; i++)
		{
			if (carry == 0)
			{
				break;
			}
			else if ((ctr_copy[i] & 0xff) == 0xff)
			{
				ctr_copy[i] = 0x00;
				carry = 1;
			}
			else
			{
				ctr_copy[i] = ctr_copy[i] + 1;
				carry = 0;
			}
		}

		for (i = 0; i < 16; i++)
		{
			ctr[i] = ctr_copy[i];
		}

		aes_encrypt(aes_instance, ctr_copy);

		for (i = 0; i < 16; i++)
		{
			random_numbers[index] = (ctr_copy[i] & 0xff);
			//printf("%x ", random_numbers[index]);
			index ++ ;
			if (index == random_length)
			{
				break;
			}
		}
	}

	return SUCCESS;
}

/*int main()
{
	aes aes_instance;
	int mode = MR_ECB;

	int status;

	char aes_key[32];
	char a[16];

	prg_init_aes (&aes_instance, AES_KEY_LEN);

	char ctr[16] = {0};

	//unsigned long long* random_head = malloc (sizeof(unsigned long long) * OWF_N_BIT);
	char * random_numbers = malloc (sizeof(char) * AES_PRG_OUT_LEN);

	clock_t start_subset = clock();
	//prg_gen_aes (&aes_instance, ctr, random_head, OWF_N_BIT, OWF_LEFTOVER_BIT );
	prg_gen_aes (&aes_instance, ctr, random_numbers, AES_PRG_OUT_LEN);
	clock_t end_subset = clock();
	double elapsed_secs = ((double)(end_subset - start_subset)) / CLOCKS_PER_SEC;
	printf("PRG = %lf\n", elapsed_secs);

	return SUCCESS;
}*/
