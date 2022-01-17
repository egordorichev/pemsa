#ifndef PEMSA_EMITTER_HPP
#define PEMSA_EMITTER_HPP

#include "pemsa/cart/pemsa_scanner.hpp"
#include <string>

// #define PEMSA_BLOCK_ADDITIONAL_CODE
std::string pemsa_pre_emit(std::string source, const char* cartPath = nullptr);
std::string pemsa_emit(PemsaScanner* scanner);

#endif