#include <iostream>

#include <botan/hash.h>
#include <botan/auto_rng.h>
#include <botan/hex.h>
#include <botan/pbkdf.h>
#include <botan/bcrypt.h>

#include "util.h"

int main()
{
  std::array<unsigned char, 8> randomized = {};
  std::string password = "super_secure";
  std::string secret_message = "My Bank Account PIN is 1234, please don't hack me!";

  Botan::AutoSeeded_RNG rng;

  // Generate some securely randomized data
  rng.randomize(randomized.data(), randomized.size());

  std::cout << "Some randomly generated data:\n";
  print_hex(randomized);

  // Hash a string using SHA-256
  // Note: don't use with password!
  std::cout << "SHA256 of " << secret_message << ": \n";

  std::unique_ptr<Botan::HashFunction> sha256(Botan::HashFunction::create("SHA-256"));
  sha256->update(randomized.data(), randomized.size());
  std::cout << "SHA 256: " << Botan::hex_encode(sha256->final()) << "\n";

  // Hash a pasword securely using SCRYPT
  const int iterations = 10000;


  std::unique_ptr<Botan::PBKDF> pbkdf(Botan::get_pbkdf("PBKDF2(SHA-256)"));

  // we measure how long it takes to calculate 100 hashes ...
  auto before = std::chrono::high_resolution_clock::now();

  Botan::OctetString key;
  for (int i = 0; i < 100; ++i)
  {
    key = pbkdf->derive_key(32, password, randomized.data(), randomized.size(), iterations);
  }

  auto after = std::chrono::high_resolution_clock::now();

  std::cout << "PBKDF2 SHA256 value for password using " << iterations << " iterations: \n";
  std::cout << key.as_string() << "\n";
  std::cout << "... using SALT: \n";
  print_hex(randomized);
  std::cout << "100 repetitions with " << iterations << " iterations took "
    << std::chrono::duration_cast<std::chrono::milliseconds>(after - before).count()
    << " ms.\n";

  std::string bcrypt_pw = Botan::generate_bcrypt(password, rng, 10);
  std::cout << "BCRYPT for same PW: " << bcrypt_pw << "\n";
  std::cout << "Verifying: "
            << Botan::check_bcrypt(password, bcrypt_pw) << "\n\n";
}
