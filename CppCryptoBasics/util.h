// CppSecurityBasics util.h : Help with converting and printing types for the examples
//

#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/rand.h>

#include <gsl/gsl>

[[noreturn]]
void osslError(const std::string& msg);

template <class T>
gsl::span<const unsigned char> const_byte_view(const T& in)
{
	auto size = in.size() * sizeof(typename T::value_type);
  return { reinterpret_cast<const unsigned char*>(in.data()), static_cast<int64_t>(size) };
}

template <class T>
gsl::span<unsigned char> byte_view(T& in)
{
  auto size = in.size() * sizeof(typename T::value_type);
  return {
    reinterpret_cast<unsigned char*>(const_cast<typename T::value_type*>(in.data())),
    static_cast<int64_t>(size)
  };
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

std::vector<unsigned char> sha256_(gsl::span<const unsigned char> input);
