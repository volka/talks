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


template <class T>
gsl::span<const unsigned char> byte_view(const T& in)
{
	auto size = in.size() * sizeof(typename T::value_type);
	return { reinterpret_cast<const unsigned char*>(in.data()), static_cast<int64_t>(size) };
}

template <class T>
void print_hex(T in, const int width = 16)
{
	auto data = byte_view(in);

	for (int i = 0; i < data.size(); ++i) {
		if (i % width == 0) std::cout << std::setfill('0') << std::setw(3) << i << ": ";

		std::cout << std::setw(2) << std::hex << static_cast<int>(data[i]);
		if (i % 2 == 1) std::cout << " ";

		if (i % width == width - 1 && i != data.size()) 
			std::cout << "\n";
	}
	std::cout << std::dec << std::endl;
}

std::vector<unsigned char> sha256(gsl::span<const unsigned char> input)
{
	std::vector<unsigned char> output(SHA256_DIGEST_LENGTH,0);
	Ensures(output.size() == SHA256_DIGEST_LENGTH);
	int res = 1;

	SHA256_CTX sha256;
	if (res) res = SHA256_Init(&sha256);
	if (res) res = SHA256_Update(&sha256, input.data(), input.size());
	if (res) res = SHA256_Final(output.data(), &sha256); //  <-- cleans up the SHA256_CTX !
	
	if (!res) throw std::runtime_error("SHA256 failed");
	return output;
}

/*
int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *aad,
	int aad_len, unsigned char *key, unsigned char *iv,
	unsigned char *ciphertext, unsigned char *tag)
{
	EVP_CIPHER_CTX *ctx;

	int len;

	int ciphertext_len;


	// Create and initialise the context
	if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

	// Initialise the encryption operation.
	if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL))
		handleErrors();

	// Set IV length if default 12 bytes (96 bits) is not appropriate
	if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 16, NULL))
		handleErrors();

	// Initialise key and IV 
	if (1 != EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv)) handleErrors();

	// Provide any AAD data. This can be called zero or more times as required
	if (1 != EVP_EncryptUpdate(ctx, NULL, &len, aad, aad_len))
		handleErrors();

	// Provide the message to be encrypted, and obtain the encrypted output.
	// EVP_EncryptUpdate can be called multiple times if necessary
	if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
		handleErrors();
	ciphertext_len = len;

	// Finalise the encryption. Normally ciphertext bytes may be written at
	// this stage, but this does not occur in GCM mode
	if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
	ciphertext_len += len;

	// Get the tag 
	if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag))
		handleErrors();

	// Clean up
	EVP_CIPHER_CTX_free(ctx);

	return ciphertext_len;
}
*/
int main()
{
	std::array<unsigned char, 8> randomized = {0};
	std::string password = "super_secure";
	std::string secret_message = "My Bank Account PIN is 1234, please don't hack me!";

	// Generate some securely randomized data
	RAND_bytes(randomized.data(), randomized.size());

	std::cout << "Some randomly generated data:\n";
	print_hex(randomized);

	// Hash a string using SHA-256
	// Note: don't use with password!
	std::cout << "SHA256 of " << secret_message << ": \n";
	auto hash = sha256(byte_view(secret_message));
	print_hex(hash);
	
	// Hash a pasword securely using PBKDF2
	const int iterations = 10000;
	const EVP_MD* digest_func = EVP_sha256();

	std::array<unsigned char, 32> password_hash;
	// PKCS5_PBKDF2_HMAC_SHA1(password.data(), password.size(), randomized.data(), randomized.size(), 
	//	iterations, password_hash.size(), password_hash.data() );
	auto before = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < 100; ++i)
	{
		randomized[0] = i;
		PKCS5_PBKDF2_HMAC(password.data(), password.size(), randomized.data(), randomized.size(),
			iterations, digest_func, password_hash.size(), password_hash.data());
	}
	auto after = std::chrono::high_resolution_clock::now();

	std::cout << "PBKDF2 SHA256 value for password using " << iterations << " iterations: \n";
	print_hex(password_hash);
	std::cout << "... using SALT: \n";
	print_hex(randomized);
	std::cout << "100 repetitions with " << iterations << " iterations took "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(after - before).count()
		<< " ms.\n";

    return 0;
}
