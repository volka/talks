#include <botan/rng.h>
#include <botan/auto_rng.h>
#include <botan/system_rng.h>
#include <iostream>

#include "util.h"

int main()
{
  Botan::AutoSeeded_RNG rng;
  std::array<unsigned char, 8> randomized = {};
  std::string some_data{"lalala"};

  if (!rng.is_seeded())
  {
    std::cout << "DANGER: rng not seeded!\n";
  }
  // Generate some securely randomized data
  rng.randomize(randomized.data(), randomized.size());

  std::cout << "Some randomly generated data:\n";
  print_hex(randomized);

  rng.randomize_with_input(randomized.data(), randomized.size(), (uint8_t*)some_data.data(), some_data.size());
  std::cout << "Randomized with additional input:\n";
  print_hex(randomized);

  std::cout << "Adding entropy manually ... usually DON'T do this! \n";
  rng.add_entropy((uint8_t*)some_data.data(), some_data.size());

  Botan::System_RNG rng_sys;
  rng_sys.randomize(randomized.data(), randomized.size());
  std::cout << "Randomized by system (e.g. /dev/urandom) data:\n";
  print_hex(randomized);

  Botan::secure_vector<uint8_t> rand_vec = rng_sys.random_vec(16);
  std::cout << "Randomized into a vector:\n";
  print_hex(rand_vec);
}
