#include "pemsa/cart/pemsa_emitter.hpp"
#include "fix16.hpp"

#include <sstream>
#include <iostream>
#include <cstring>
#include <cmath>
#include <climits>
#include <iomanip>

static float strtof(const char* ostr, int base) {
	char* str = (char*) malloc(strlen(ostr) + 1);
	strcpy(str, ostr);
	const char* dot = ".";

	char *beforeDot = strtok(str, dot);
	char *afterDot = strtok(NULL, dot);

	float f = (float) strtol(beforeDot, 0, base);
	int sign = (str[0] == '-' ? -1 : 1);
	char n[2] = { 0 };

	if (afterDot != nullptr) {
		for (int i = 0; afterDot[i]; i++) {
			n[0] = afterDot[i];
			f += strtol(n, 0, base) * pow(base, -(i + 1)) * sign;
		}
	}

	free(str);
	return f;
}

static PemsaTokenType skippableTokens[] = {
	TOKEN_AND, TOKEN_OR,

	TOKEN_SHARP,
	TOKEN_DOT, TOKEN_DOT_DOT, TOKEN_DOT_DOT_DOT,
	TOKEN_EQUAL_EQUAL,

	TOKEN_MINUS, TOKEN_PLUS, TOKEN_STAR, TOKEN_SLASH,
	TOKEN_BACKWARDS_SLASH, TOKEN_LESS_EQUAL,
	TOKEN_GREATER_EQUAL, TOKEN_LESS, TOKEN_GREATER,

	TOKEN_TILDA_EQUAL, TOKEN_MODULO, TOKEN_MODULO_EQUAL,
	TOKEN_CAP,
	TOKEN_BAND, TOKEN_BOR,

	TOKEN_EOF
};

std::string pemsa_emit(PemsaScanner* scanner) {
	std::stringstream output;
	PemsaToken token = PemsaToken();
	PemsaToken previous;

	bool running = true;
	bool inQuestion = false;
	bool inIf = false;
	bool inWhile = false;
	bool checkThen = false;
	bool checkDo = false;
	bool emitEnd = false;
	int outputBrace = 0;
	int parenCount = 0;
	int ifParenStart = 0;
	int whileParenStart = 0;

	const char* expressionStart = scanner->getCurrent();
	const char* start = expressionStart;

#ifndef PEMSA_BLOCK_ADDITIONAL_CODE
	// Emoji button setup
	output << "A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z = -2560.5, 31455.5, 32125.5, 1, 6943.5, 3855.5, 2, -19008.5, 4, -20032.5,0.5, -20128.5, 3, -18402.5, -1632.5, 20927.5, -26208.5, -20192.5, 0, 21845.5, 5, 20767.5, -2624.5, 23130.5, -25792.5, -24351.5\n";
	output << "sub, cocreate, coresume, yield, costatus, debug = string.sub, coroutine.create, coroutine.resume, coroutine.yield, coroutine.status, nil\n";

	output << "function foreach(a, f)\n";
	output << " if not a then return end\n";
	output << " for _, v in ipairs(a) do f(v) end\n";
	output << "end\n";
	output << "function count(a) if not a then return 0 end return #a end\n";
	output << "function arraylen(t)\n";
	output << "\tlocal len = 0\n";
	output << "\tfor i, _ in pairs(t) do\n";
	output << "\t\tif type(i) == \"number\" then\n";
	output << "\t\t\tlen = i\n";
	output << "\t\tend\n";
	output << "\tend\n";
	output << "\treturn len\n";
	output << "end\n";
	output << "function all(a)\n";
	output << "\tif a == nil then\n";
	output << "\t\treturn function() end\n";
	output << "\tend\n";
	output << "\tlocal i = 0\n";
	output << "\tlocal n = arraylen(a)\n";
	output << "\treturn function()\n";
	output << "\t\ti = i + 1\n";
	output << "\t\twhile (a[i] == nil and i <= n) do\n";
	output << "\t\t\ti = i + 1\n";
	output << "\t\tend\n";
	output << "\t\treturn a[i]\n";
	output << "\tend\n";
	output << "end\n";
	output << "function add(a, v)\n";
	output << "	if a == nil then return end\n";
	output << "	table.insert(a, v)\n";
	output << "	return v\n";
	output << "end\n";
	output << "function del(a, dv)\n";
	output << "	if a == nil then return end\n";
	output << "	for i, v in ipairs(a) do\n";
	output << "		if v == dv then\n";
	output << "			table.remove(a, i)\n";
	output << "		return dv\n";
	output << "		end\n";
	output << "	end\n";
	output << "end\n";
#endif

	while (running) {
		if (token.type != TOKEN_WHITESPACE && token.type != TOKEN_NEW_LINE) {
			previous = token;
		}

		token = scanner->scan();

		emitToken:
		if (token.type != TOKEN_WHITESPACE && token.type != TOKEN_NEW_LINE) {
			if (outputBrace > 0) {
				if (outputBrace == 1) {
					output << ")";
				}

				outputBrace--;
			}

			if (checkThen || checkDo) {
				bool wasCheckDo = checkDo;

				checkThen = false;
				checkDo = false;
				inIf = false;
				inWhile = false;

				PemsaTokenType t = token.type;

				if (t != (wasCheckDo ? TOKEN_DO : TOKEN_THEN) && t != TOKEN_OR && t != TOKEN_AND && t != TOKEN_COLON && t != TOKEN_DOT) {
					emitEnd = true;
					output << (wasCheckDo ? " do " : " then ");
				}
			}
		}

		switch (token.type) {
			case TOKEN_MODULO_EQUAL:
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

			case TOKEN_UNKNOWN: {
				// Some utf8 escape codes, other stuff that is not supported by this parser
				break;
			}

			case TOKEN_MULTILINE_STRING:
			case TOKEN_STRING: {
				bool multiline = token.type == TOKEN_MULTILINE_STRING;
				char cc = multiline ? '\"' : *token.start;
				output << cc;

				int start = 1;
				int end = token.length - 1;

				if (multiline) {
					start++;
					end--;
				}

				char prev = '\0';

				for (int i = start; i < end; i++) {
					const char* str = token.start + i;

					if (i < end - 1) {
#define CASE(a, b, c, d, u, e) if (str[0] == a && str[1] == b && (str[0] == -53 || (str[2] == c && (str[0] != -16 || str[3] == d)))) { output << "\\" << e; i += (str[0] == -16 ? 3 : (str[0] == -53 ? 1 : 2)); i += 3; continue; }
#include "pemsa/cart/pemsa_cases.hpp"
#undef CASE
					}

					char c = *str;

					if (c == '\n') {
						output << "\\n";
						continue;
					}

					if (prev != '\\' && multiline && (c == '"' || c == '\\')) {
						output << '\\';
					}

					prev = c;
					output << c;
				}

				output << cc;
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

				inIf = false;
				inWhile = false;
				output << '\n';
				break;
			}

			case TOKEN_RIGHT_PAREN: {
				parenCount--;
				expressionStart = token.start + token.length;

				checkThen = parenCount == ifParenStart && inIf;
				checkDo = parenCount == whileParenStart && inWhile;

				output << std::string(token.start, token.length);

				previous = token;
				token = scanner->scan();
				PemsaTokenType tt = token.type;

				int i = 0;

				while (skippableTokens[i] != TOKEN_EOF) {
					if (tt == skippableTokens[i]) {
						checkThen = false;
						checkDo = false;

						break;
					}

					i++;
				}

				goto emitToken;
				break;
			}

			case TOKEN_LEFT_PAREN: {
				if (token.type == TOKEN_LEFT_PAREN) {
					parenCount++;

					if (previous.type == TOKEN_IF) {
						inIf = true;
						ifParenStart = parenCount - 1;
					} else if (previous.type == TOKEN_WHILE) {
						inWhile = true;
						whileParenStart = parenCount - 1;
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