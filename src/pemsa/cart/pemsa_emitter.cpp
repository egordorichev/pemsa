#include "pemsa/cart/pemsa_emitter.hpp"

#include <sstream>
#include <iostream>

std::string pemsa_emit(PemsaScanner* scanner) {
	std::stringstream output;
	PemsaToken token;
	bool running = true;
	bool inQuestion = false;

	while (running) {
		token = scanner->scan();

		switch (token.type) {
			case TOKEN_EOF: {
				running = false;
				break;
			}

			case TOKEN_MULTILINE_STRING:
			case TOKEN_STRING: {
				bool multiline = token.type == TOKEN_MULTILINE_STRING;
				output << "\"";

				int start = 1;
				int end = token.length - 1;

				if (multiline) {
					start++;
					end--;
				}

				for (int i = start; i < end; i++) {
					const char* str = token.start + i;
					if (i < end - 1) {
#define CASE(a, b, c, d, u, e) if (str[0] == a && str[1] == b && (str[0] == -53 || (str[2] == c && (str[0] != -16 || str[3] == d)))) { output << "\\" << e; i += (str[0] == -16 ? 3 : (str[0] == -53 ? 1 : 2)); continue; }
#include "pemsa/cart/pemsa_cases.hpp"
#undef CASE
					}

					char c = *str;

					if (c == '\n') {
						output << "\\n";
						continue;
					}

					output << (char) toupper(c);
				}

				output << "\"";
				break;
			}

			case TOKEN_NUMBER: {
				const char* number = token.start;

				if (token.length > 2) {
					if (number[1] == 'x') {
						output << (double) strtoll(number, NULL, 16);

						break;
					}	else if (number[1] == 'b') {
						output << (double) strtoll(number + 2, NULL, 2);
						break;
					}
				}

				output << std::string(token.start, token.length) << " ";
				break;
			}

			case TOKEN_QUESTION: {
				output << "print(";
				inQuestion = true;

				break;
			}

			// Can have a different underlying c-string
			case TOKEN_NOT: {
				output << " not ";
				break;
			}

			// Can have a different underlying c-string
			case TOKEN_TILDA_EQUAL: {
				output << " ~= ";
				break;
			}

			case TOKEN_NEW_LINE: {
				if (inQuestion) {
					inQuestion = false;
					output << ')';
				}

				output << '\n';
				break;
			}

			case TOKEN_COLON_COLON: {
				output << std::string(token.start, token.length);
				break;
			}

			default: {
				output << std::string(token.start, token.length) << " ";
				break;
			}
		}
	}

	return output.str();
}