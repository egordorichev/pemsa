#include "pemsa/cart/pemsa_emitter.hpp"
#include <sstream>

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

			case TOKEN_STRING: {
				output << "\"";

				for (int i = 1; i < token.length - 1; i++) {
					output << (char) toupper(token.start[i]);
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

			default: {
				output << std::string(token.start, token.length) << " ";
				break;
			}
		}
	}

	return output.str();
}