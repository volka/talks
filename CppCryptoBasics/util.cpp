// SslDemo.cpp : Defines the entry point for the console application.
//
#include "util.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/rand.h>

[[noreturn]]
void osslError(const std::string& msg)
{
  std::cout << "Error: " << msg << "\n";
  auto code = ERR_get_error();
  auto ossl_msg = ERR_error_string(code, nullptr);
  std::cout << "OpenSSL error: (" << code << ") " << ossl_msg << "\n";
  exit(0);
}

std::vector<unsigned char> sha256_(gsl::span<const unsigned char> input)
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
