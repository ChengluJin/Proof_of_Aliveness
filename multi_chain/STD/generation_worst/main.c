#include <stdio.h>
#include "miracl.h"
#include "parameters_m_STD.h"
#include <time.h>
#include <stdlib.h>
#include "subset_sum_64bits.h"
#include "standard_prg.h"

int main()
{
	miracl* mip = mirsys(1000, 10);
	int i, j , k, l;

	unsigned long long * prg_owf_set = malloc(sizeof(unsigned long long) * PRG_OWF_N_ELEMENT * PRG_OWF_N_BIT);
	unsigned long long * owf_set = malloc(sizeof(unsigned long long) * OWF_N_ELEMENT * OWF_N_BIT);
	unsigned long long * hc_set = malloc(sizeof(unsigned long long) * PRG_HC_N_ELEMENT * PRG_HC_N_BIT);
	unsigned long long * prg_input = malloc(sizeof(unsigned long long) * PRG_OWF_N_BIT);
	unsigned long long * seed = malloc(sizeof(unsigned long long) * PRG_OWF_N_BIT);
	unsigned long long * prg_output = malloc(sizeof(unsigned long long) * PRG_OWF_N_BIT);
	unsigned long long * node = malloc(sizeof(unsigned long long) * OWF_N_BIT);
	unsigned long long * new_node = malloc(sizeof(unsigned long long) * OWF_N_BIT);

	prg_standard_initialize (PRG_OWF_N_ELEMENT, PRG_OWF_N_BIT, PRG_OWF_LEFTOVER_BIT, prg_owf_set, PRG_HC_N_ELEMENT, PRG_HC_N_BIT, PRG_HC_LEFTOVER_BIT, hc_set, prg_input, seed);
	subset_sum_initialize (OWF_N_ELEMENT, OWF_N_BIT, OWF_LEFTOVER_BIT, owf_set);

	clock_t start_subset = clock();
	for (i = 0; i < N_SUB_CHAIN; i++)
	{
		prg_standard_generate (PRG_OWF_N_ELEMENT, PRG_OWF_N_BIT, PRG_OWF_LEFTOVER_BIT, prg_owf_set, PRG_HC_N_ELEMENT, PRG_HC_N_BIT, PRG_HC_LEFTOVER_BIT, hc_set, seed, PRG_LOOP, prg_input, prg_output);
		for (j = 0; j < OWF_N_BIT; j++)
		{
			node[j] = prg_output[j];
		}

		for (j = 0; j < N_NODE_PER_CHAIN; j++)
		{
			subset_sum_xor_specific_448 (OWF_N_ELEMENT, OWF_N_BIT, OWF_LEFTOVER_BIT, owf_set, node, new_node);
			for (k = 0; k < OWF_N_BIT; k++)
			{
				node[k] = new_node[k];
			}

			/*if (j == N_NODE_PER_CHAIN - 1)
			{
				printf("last node\n");
				for (l = 0; l < OWF_N_BIT; l++)
				{
					printf("%llx ", node[l]);
				}
				printf("\n");
			}*/
		}
	}
	clock_t end_subset = clock();
	double elapsed_secs = ((double)(end_subset - start_subset)) / CLOCKS_PER_SEC;
	elapsed_secs = (elapsed_secs * 1000) / (N_SUB_CHAIN * N_NODE_PER_CHAIN);
	printf("Generation: Multi_chain takes %lf milliseconds to generate one proof in the worst case ( the begining of a new sub-chain) in the standard model\n", elapsed_secs);

	return SUCCESS;
}
