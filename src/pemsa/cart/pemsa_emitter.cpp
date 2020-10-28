#include "pemsa/cart/pemsa_emitter.hpp"

#include <sstream>
#include <iostream>

std::string pemsa_emit(PemsaScanner* scanner) {
	std::stringstream output;
	PemsaToken token;
	PemsaToken previous;

	bool running = true;
	bool inQuestion = false;
	bool inIf = false;
	bool checkThen = false;
	bool emitEnd = false;
	int outputBrace = 0;
	int parenCount = 0;

	const char* expressionStart = scanner->getCurrent();
	const char* start = expressionStart;

	// Emoji button setup
	output << "S, D, G, M, I, U = 0, 1, 2, 3, 4, 5\n";

	while (running) {
		if (token.type != TOKEN_WHITESPACE && token.type != TOKEN_NEW_LINE) {
			previous = token;
		}

		token = scanner->scan();

		if (token.type != TOKEN_WHITESPACE && token.type != TOKEN_NEW_LINE) {
			if (outputBrace > 0) {
				if (outputBrace == 1) {
					output << ")";
				}

				outputBrace--;
			}

			if (checkThen) {
				checkThen = false;
				inIf = false;

				if (token.type != TOKEN_THEN) {
					emitEnd = true;
					output << " then ";
				}
			}
		}

		switch (token.type) {
			case TOKEN_BACKWARDS_SLASH: {
				int expressionLength = token.start - expressionStart;
				output.seekp(-expressionLength, output.cur);
				output << "flr(" << std::string(expressionStart, expressionLength) << "/";
				outputBrace = 2;

				break;
			}

			case TOKEN_MINUS_EQUAL:
			case TOKEN_SLASH_EQUAL:
			case TOKEN_STAR_EQUAL:
			case TOKEN_PLUS_EQUAL: {
				output << "=" << std::string(expressionStart, token.start - expressionStart) << std::string(token.start, 1);
				break;
			}

			case TOKEN_ERROR: {
				std::cerr << token.start << "\n";
				break;
			}

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

				output << std::string(token.start, token.length);
				break;
			}

			case TOKEN_QUESTION: {
				output << "print(";
				inQuestion = true;

				break;
			}

			// Can have a different underlying c-string
			case TOKEN_NOT: {
				output << "not";
				break;
			}

			// Can have a different underlying c-string
			case TOKEN_TILDA_EQUAL: {
				output << "~=";
				break;
			}

			case TOKEN_NEW_LINE: {
				if (inQuestion) {
					inQuestion = false;
					output << ')';
				}

				if (emitEnd) {
					emitEnd = false;
					output << " end";
				}

				output << '\n';
				break;
			}

			case TOKEN_RIGHT_PAREN: {
				parenCount--;
				checkThen = parenCount == 0 && inIf;

				output << std::string(token.start, token.length);
				break;
			}

			case TOKEN_LEFT_BRACKET:
			case TOKEN_LEFT_BRACE:
			case TOKEN_LEFT_PAREN: {
				if (token.type == TOKEN_LEFT_PAREN) {
					parenCount++;

					if (previous.type == TOKEN_IF) {
						inIf = true;
					}
				}

				expressionStart = token.start + token.length;
				output << std::string(token.start, token.length);
				break;
			}

			case TOKEN_IDENTIFIER: {
				if (start != token.start) {
					char c = token.start[-1];

					if (c == ' ' || c == '\r' || c == '\n' || c == '\t') {
						expressionStart = token.start;
					}
				}

				output << std::string(token.start, token.length);
				break;
			}

			default: {
				output << std::string(token.start, token.length);
				break;
			}
		}
	}

	return output.str();
}