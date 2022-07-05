#include "../shared.hh"
#include "../params.hh"

/*
 * hammer_addresses
 *
 * Performs a double-sided rowhammer attack on a given victim address,
 * given two aggressor addresses.
 *
 * HINT 1: Be patient! Bit-flips won't happen every time.
 *
 * Input: victim address, and two attacker addresses (all *physical*)
 * Output: True if any bits have been flipped, false otherwise.
 *
 */
bool hammer_addresses(uint64_t vict_phys_addr, uint64_t attacker_phys_addr_1, uint64_t attacker_phys_addr_2) {
  bool foundFlip = false; 

  //TODO: Part 1.2

  return foundFlip; 
}

