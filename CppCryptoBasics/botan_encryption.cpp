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
  if (!pbkdf) throw std::runtime_error("PBKDF2 not found");

  size_t iterations = 10000;

  AutoSeeded_RNG rng;
  InitializationVector iv_key(rng, 24);
  OctetString key = pbkdf->derive_key(16, password, iv_key.begin(), iv_key.size(), iterations);

  unique_ptr<StreamCipher> stream{StreamCipher::create("Salsa20")};
  if (!stream) throw std::runtime_error("Stream Cipher Salsa20 not found!");

  stream->set_key(key);
  stream->set_iv(iv_key.begin(), iv_key.size());
  // encrypt whole message, in place
  stream->encrypt(secret_data);
  std::cout << "Encrypted with " << stream->name() << ": \n" << hex_encode(secret_data) << "\n\n";


  // CTR as a stream cipher
  unique_ptr<Cipher_Mode> ctr{get_cipher_mode("AES-128/CTR-BE", ENCRYPTION)};
  if (!ctr) throw std::runtime_error("Cipher AES-128/CTR not found");

  secret_data.assign(secret.begin(), secret.begin()+secret.size()/2);
  secure_vector<uint8_t> secret_data_2(secret.begin()+secret.size()/2, secret.end());
  ctr->set_key(key);
  ctr->start(iv_key.begin(), 16);
  ctr->update(secret_data);
  ctr->finish(secret_data_2);
  std::cout << "Encrypted with " << ctr->name() << ": \n" << hex_encode(secret_data) << hex_encode(secret_data_2) << "\n\n";

  // the same, but in one go
  ctr->clear();
  ctr->set_key(key);
  secret_data.assign(secret.begin(), secret.end());
  ctr->start(iv_key.begin(), 16);
  ctr->finish(secret_data);
  std::cout << "Encrypted in one go: \n" << hex_encode(secret_data) << "\n\n";

  return 0;
}
