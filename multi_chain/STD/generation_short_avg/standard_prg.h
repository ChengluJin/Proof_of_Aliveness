int prg_standard_initialize (int owf_n_element, int owf_n_bit, int owf_n_leftover_bit, unsigned long long * owf_set, int HC_n_element, int HC_n_bit, int HC_n_leftover_bit, unsigned long long * HC_set, unsigned long long * prg_input, unsigned long long*  seed);

int hc (int HC_n_element, int HC_n_bit, int HC_n_leftover_bit, unsigned long long * HC_set, unsigned long long * input, char * hc_output);

int prg_standard_generate (int owf_n_element, int owf_n_bit, int owf_n_leftover_bit, unsigned long long * owf_set, int HC_n_element, int HC_n_bit, int HC_n_leftover_bit, unsigned long long * HC_set, unsigned long long*  seed, int prg_loop, unsigned long long* prg_input, unsigned long long * prg_output);
