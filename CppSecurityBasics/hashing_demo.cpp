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
   res = SHA256_Init(&sha256);
	if (res) res = SHA256_Update(&sha256, input.data(), input.size());
   if (res) res = SHA256_Final(output.data(), &sha256); //  <-- cleans up the SHA256_CTX !
	
   if (!res) {
      SHA256_Final(nullptr, &sha256);
      throw std::runtime_error("SHA256 failed");
   }
	return output;
}

int main()
{
   std::array<unsigned char, 8> randomized = {};
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
