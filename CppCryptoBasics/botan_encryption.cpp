#include <iostream>

#include <botan/botan.h>
#include <botan/hex.h>
#include <botan/key_filt.h>
#include <botan/pbkdf2.h>

int main()
{
  using namespace Botan;
  using namespace std;
  string password("foobar123");
  string secret("This is a secret message that needs to be encrypted in several blocks");
  secure_vector<uint8_t> secret_data{secret.begin(), secret.end()};

  unique_ptr<PBKDF> pbkdf(get_pbkdf("PBKDF2(SHA-256)"));
  size_t iterations = 10000;

  unique_ptr<StreamCipher> cipher{StreamCipher::create("CTR-BE(AES-128)")};

  AutoSeeded_RNG rng;
  InitializationVector iv_key(rng, 16);
  OctetString key = pbkdf->derive_key(16, password, iv_key.begin(), iv_key.size(), iterations);

  cipher->set_key(key);
  cipher->set_iv(iv_key.begin(), iv_key.size());
  cipher->encrypt(secret_data);

  std::cout << "Encrypted with " << cipher->name() << ": " << hex_encode(secret_data) << "\n";
  return 0;
}
