#define AES_KEY_LEN 16
#define AES_PRG_OUT_LEN 32 //in number of bytes
#define N_SUB_CHAIN 10 //arbitrary positive integer
#define N_NODE_PER_CHAIN 1 // The worst case occurs when a new sub-chain begins, so it has one PRG and one OWF to compute.
//#define N_NODE_PER_CHAIN 8192 // 8192 = (2^22 / 512)
//#define N_NODE_PER_CHAIN 64 // 8192 = (2^15 / 512)

#define SUCCESS 0
#define FAILURE -1
