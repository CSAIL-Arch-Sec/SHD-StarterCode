#include "../shared.hh"
#include "../verif.hh"
#include "../params.hh"

// TODO: Part 1.2
// Replace these with your machine-specific addresses!

#define VICT_ADDR 0x75369000UL

#define ADDR_A 0x75387000UL
#define ADDR_B 0x7534B000UL
#define ADDR_C 0x7532D000UL
#define ADDR_D 0x75369100UL
// END TODO

bool hammer_addresses(uint64_t vict_phys_addr, uint64_t attacker_phys_addr_1, uint64_t attacker_phys_addr_2);
