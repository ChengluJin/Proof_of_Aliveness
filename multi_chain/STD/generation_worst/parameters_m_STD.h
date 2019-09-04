#define N_SUB_CHAIN 10 //arbitrary positive integer
//#define N_NODE_PER_CHAIN 64 // 2^15 / 512 = 64
//#define N_NODE_PER_CHAIN 8192 // 2^22 / 512 = 8192
#define N_NODE_PER_CHAIN 1 // The worst case occurs when a new sub-chain begins, so it has one PRG and one OWF to compute

#define PRG_OWF_N_ELEMENT 2200
#define PRG_OWF_N_BIT 35
#define PRG_OWF_LEFTOVER_BIT 40
#define PRG_LOOP 4
#define PRG_OUTPUT_PER_LOOP 110
#define PRG_HC_N_ELEMENT 262
#define PRG_HC_N_BIT 35
#define PRG_HC_LEFTOVER_BIT 40
#define PRG_HC_OUTPUT_LENGTH 262 //bits
#define PRG_HC_OUTPUT_LENGTH_LONG 5 //in number of 64bits
#define PRG_H_SHA512_LOOP 4

#define SUCCESS 0
#define FAILURE -1

#define OWF_N_ELEMENT 440
#define OWF_N_BIT 7
#define OWF_LEFTOVER_BIT 8
