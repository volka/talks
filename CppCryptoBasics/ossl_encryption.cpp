#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/rand.h>

#include <gsl/gsl>

#include "util.h"

size_t encrypt(unsigned char *plaintext, size_t plaintext_len, unsigned char *aad,
  size_t aad_len, unsigned char *key, unsigned char *iv,
	unsigned char *ciphertext, unsigned char *tag)
{
  std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx{nullptr, EVP_CIPHER_CTX_free};
	int len;
	int ciphertext_len;

	// Create and initialise the context
  ctx.reset(EVP_CIPHER_CTX_new());
  if (!ctx) osslError("Creating ciphter context failed");

	// Initialise the encryption operation.
  if (1 != EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_gcm(), NULL, NULL, NULL))
  {
    osslError("Initializing encryption operation type failed");
  }

	// Set IV length if default 12 bytes (96 bits) is not appropriate
  if (1 != EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_GCM_SET_IVLEN, 12, NULL))
  {
    osslError("");
  }

  // Initialise key and IV - note: key and iv size depend on cipher, OpenSSL does no bounds check!
  if (1 != EVP_EncryptInit_ex(ctx.get(), NULL, NULL, key, iv))
    osslError("Initializing key/iv failed");

  // Provide any "additional authenticated data". Can be called zero or more times as required
  // This data is not included in the ciphertext!
  if (1 != EVP_EncryptUpdate(ctx.get(), NULL, &len, aad, aad_len))
    osslError("Updating AAD failed");


  // TODO: update with up to block_size-1 bytes at a time

  // Encrypt plaintext, obtain encrypted output. Can be called mulitple times
  if (1 != EVP_EncryptUpdate(ctx.get(), ciphertext, &len, plaintext, static_cast<int>(plaintext_len)))
    osslError("Encrypting data failed");
	ciphertext_len = len;

	// Finalise the encryption. Normally ciphertext bytes may be written at
	// this stage, but this does not occur in GCM mode
  if (1 != EVP_EncryptFinal_ex(ctx.get(), ciphertext + len, &len)) osslError("Finalising encryption failed");
	ciphertext_len += len;

  // Get the tag - this tag "authenticates" the cipherdata
  if (1 != EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_GCM_GET_TAG, 16, tag))
    osslError("Getting GCM tag failed");

  return static_cast<size_t>(ciphertext_len);
}

int decrypt(unsigned char *ciphertext, size_t ciphertext_len, unsigned char *aad,
  size_t aad_len, unsigned char *tag, unsigned char *key, unsigned char *iv,
  unsigned char *plaintext)
{
  std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx{nullptr, &EVP_CIPHER_CTX_free};
  int plaintext_len;
  int len;
  int ret;

  // Create context
  ctx.reset(EVP_CIPHER_CTX_new());
  if (!ctx) osslError("Could not initialize context");

  // Initialize algorithm
  if(!EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_gcm(), NULL, NULL, NULL))
    osslError("DecryptInit failed");

  // Set up the IV size
  if(!EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_GCM_SET_IVLEN, 12, NULL))
    osslError("Setting IV legth for decryption failed, wrong algortihm?");

  // Set the IV and key to use
  if(!EVP_DecryptInit_ex(ctx.get(), NULL, NULL, key, iv))
    osslError("Initializing decryption context with key/iv failed");

  // feed additional data to decrypt
  if(!EVP_DecryptUpdate(ctx.get(), NULL, &plaintext_len, aad, static_cast<int>(aad_len)))
    osslError("Feeding AAD to decryption context failed.");

  // decrypt the ciphertext
  if(!EVP_DecryptUpdate(ctx.get(), plaintext, &len, ciphertext, static_cast<int>(ciphertext_len)))
    osslError("Decrypting data failed");
  plaintext_len = len;

  // set the GCM validation tag
  if(!EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_GCM_SET_TAG, 16, tag))
    osslError("Setting tag failed");

  // finalize the decryption
  ret = EVP_DecryptFinal_ex(ctx.get(), plaintext + len, &len);

  if(ret > 0) // ok - this is a size
  {
    plaintext_len += len;
    return plaintext_len;
  }
  else // decryption error
  {
    return -1;
  }
}

int main()
{
  std::array<unsigned char, 12> iv;
  RAND_bytes(iv.data(), iv.size());

	std::string password = "super_secure";
  std::string secret_message = "My Bank Account PIN is 1234, please don't steal anything!";
  std::string additional_data = "This is not secret, but it should be valid!";

  std::cout << "Data to encrypt: " << secret_message << "\n";
  std::cout << "Additional data: " << additional_data << "\n";

  std::vector<unsigned char> enc(256);
  std::array<unsigned char, 16> tag;

  auto key_from_password = sha256_(byte_view(password));
  std::cout << "\nPassword: '" << password << "' --- generated key:\n";
  print_hex(key_from_password);
  std::cout << "\n";

  auto enc_length = encrypt(
        (uint8_t*) secret_message.data(), secret_message.size(),
        (uint8_t*) additional_data.data(), additional_data.size(),
        key_from_password.data(), iv.data(), enc.data(), tag.data());

  std::cout << "\nTag: \n";
  print_hex(tag);

  enc.resize(enc_length);
  std::cout << "Encrypted data: (len: " << enc_length << ")\n";
  print_hex(enc);
  std::cout << "\n";

  std::vector<char> dec(secret_message.size(), ' ');

  int dec_length = decrypt(enc.data(), enc.size(), (uint8_t*)additional_data.data(), additional_data.size(),
          tag.data(), key_from_password.data(), iv.data(), (uint8_t*)dec.data());

  std::cout << "Decrypted data: (len/result: " << dec_length << ")\n" << dec.data() << "\n\n";

  additional_data[0] = 'F';
  dec_length = decrypt(enc.data(), enc.size(), (uint8_t*)additional_data.data(), additional_data.size(),
          tag.data(), key_from_password.data(), iv.data(), (uint8_t*)dec.data());

  std::cout << "Decrypted data with invalid aad: (len/result: " << dec_length << ")\n" << dec.data() << "\n";

  return 0;

}
