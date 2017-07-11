// SslDemo.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/rand.h>

#include <gsl/gsl>

#include "util.h"

int main()
{
    std::array<unsigned char, 8> randomized = {};

	// Generate some securely randomized data
  int result = RAND_bytes(randomized.data(), randomized.size());

  if (result == 1)
  {
    std::cout << "Some randomly generated data:\n";
    print_hex(randomized);
  } else {
    // 0 == not random, -1 == not supported by engine
    std::cout << "RAND_bytes failed, do not use data!\n";
  }

  RAND_pseudo_bytes(randomized.data(), randomized.size());
  std::cout << "Some pseudo-randomly generated data:\n";
  print_hex(randomized);

  const RAND_METHOD* rm = RAND_get_rand_method();
  std::cout << "Default engine used: " << (rm == RAND_SSLeay()) << "\n";

  return 0;
}
