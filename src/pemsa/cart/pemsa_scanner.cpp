#include "pemsa/cart/pemsa_scanner.hpp"

#include <cstring>
#include <iostream>

PemsaScanner::PemsaScanner(const char *source) {
	this->current = source;
	this->line = 1;
}

static bool isDigit(char c) {
	return c >= '0' && c <= '9';
}

static bool isDigitOrHex(char c) {
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
}

static bool isAlpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

PemsaToken PemsaScanner::parseIdentifier() {
	while (isAlpha(this->peek()) || isDigit(this->peek())) {
		this->advance();
	}

	return this->makeToken(this->decideIdentifierType());
}

PemsaToken PemsaScanner::parseNumber() {
	while (isDigitOrHex(this->peek())) {
		this->advance();
	}

	char c = peek();

	if ((c == 'x' || c == 'b') && isDigitOrHex(peekNext())) {
		this->advance();

		while (isDigitOrHex(this->peek())) {
			this->advance();
		}

		c = peek();
	}

	if (c == '.' && isDigitOrHex(peekNext())) {
		this->advance();

		while (isDigitOrHex(this->peek())) {
			this->advance();
		}
	}

	return makeToken(TOKEN_NUMBER);
}

PemsaToken PemsaScanner::scan() {
	PemsaToken token = this->skipWhitespace();

	if (token.type != TOKEN_EOF) {
		return token;
	}

	this->start = this->current;

	if (this->isAtEnd()) {
		return this->makeToken(TOKEN_EOF);
	}

	char c = this->advance();

	if (isAlpha(c)) {
		return this->parseIdentifier();
	}

	if (isDigit(c)) {
		return this->parseNumber();
	}

	switch (c) {
		case '@': return this->makeToken(TOKEN_DOG);
		case '$': return this->makeToken(TOKEN_DOLLAR);
		case '^': return this->makeToken(TOKEN_CAP);
		case '&': return this->makeToken(TOKEN_BAND);
		case '|': return this->makeToken(TOKEN_BOR);
		case '#': return this->makeToken(TOKEN_SHARP);
		case '(': return this->makeToken(TOKEN_LEFT_PAREN);
		case ')': return this->makeToken(TOKEN_RIGHT_PAREN);
		case '{': return this->makeToken(TOKEN_LEFT_BRACE);
		case '}': return this->makeToken(TOKEN_RIGHT_BRACE);
		case ']': return this->makeToken(TOKEN_RIGHT_BRACKET);
		case ';': return this->makeToken(TOKEN_SEMICOLON);
		case '?': return this->makeToken(TOKEN_QUESTION);
		case ',': return this->makeToken(TOKEN_COMMA);
		case '\\': return this->makeToken(TOKEN_BACKWARDS_SLASH);
		case ':': return this->makeToken(this->match(':') ? TOKEN_COLON_COLON : TOKEN_COLON);
		case '%': return this->makeToken(this->match('=') ? TOKEN_MODULO_EQUAL : TOKEN_MODULO);
		case '+': return this->makeToken(this->match('=') ? TOKEN_PLUS_EQUAL : TOKEN_PLUS);
		case '-': return this->makeToken(this->match('=') ? TOKEN_MINUS_EQUAL : TOKEN_MINUS);
		case '/': return this->makeToken(this->match('=') ? TOKEN_SLASH_EQUAL : TOKEN_SLASH);
		case '*': return this->makeToken(this->match('=') ? TOKEN_STAR_EQUAL : TOKEN_STAR);
		case '~': return this->makeToken(this->match('=') ? TOKEN_TILDA_EQUAL : TOKEN_TILDA);
		case '!': return this->makeToken(this->match('=') ? TOKEN_TILDA_EQUAL : TOKEN_NOT);
		case '=': return this->makeToken(this->match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
		case '<': return this->makeToken(this->match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
		case '>': return this->makeToken(this->match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);

		case '.': {
			if (this->match('.')) {
				return this->makeToken(this->match('.') ? TOKEN_DOT_DOT_DOT : TOKEN_DOT_DOT);
			}

			return this->makeToken(TOKEN_DOT);
		}

		case '\'':
		case '"': {
			char stringStart = c;
			char prev = c;

			while (!(this->peek() == stringStart && prev != '\\') && !this->isAtEnd()) {
				if (this->peek() == '\n') {
					this->line++;
				}

				prev = this->advance();
			}

			if (this->isAtEnd()) {
				return this->makeErrorToken("Unterminated string");
			}

			// The closing quote
			this->advance();
			return this->makeToken(TOKEN_STRING);
		}

		case '[': {
			if (this->peek() == '[') {
				while (!(this->peek() == ']' && this->peekNext() == ']') && !this->isAtEnd()) {
					if (this->peek() == '\n') {
						this->line++;
					}

					this->advance();
				}

				if (this->isAtEnd()) {
					return this->makeErrorToken("Unterminated string");
				}

				this->advance();
				this->advance();

				return this->makeToken(TOKEN_MULTILINE_STRING);
			}

			return this->makeToken(TOKEN_LEFT_BRACKET);
		}

		case -30:
		case -16:
		case -53: {
			int code[4];
			code[0] = c;
			int len = 2;

			switch (c) {
				case -16: len = 3; break;
				case -53: len = 1; break;
			}

			for (int i = 0; i < len; i++) {
				code[i + 1] = advance();
			}

			// std::cout << len << ": " << code[0] << ", " << code[1] << ", " << code[2] << ", " << code[3] << "\n";
#define CASE(a, b, c, d, e, u, z) if (code[0] == a && code[1] == b && (code[0] == -53 || (code[2] == c && (code[0] != -16 || code[3] == d)))) { return this->makeAsciiToken(z); }
#include "pemsa/cart/pemsa_cases.hpp"
#undef CASE

			break;
		}
	}

	return this->makeToken(TOKEN_UNKNOWN);
}

bool PemsaScanner::isAtEnd() {
	return *this->current == '\0';
}

PemsaToken PemsaScanner::makeToken(PemsaTokenType type) {
	PemsaToken token;

	token.type = type;
	token.line = this->line;

	token.start = this->start;

	if (type == TOKEN_NEW_LINE) {
		token.length = 1;
	} else {
		token.length = (int) (this->current - this->start);
	}

	return token;
}

PemsaToken PemsaScanner::makeErrorToken(const char* message) {
	PemsaToken token;

	token.type = TOKEN_ERROR;
	token.start = message;
	token.length = strlen(message);
	token.line = this->line;

	return token;
}

PemsaToken PemsaScanner::makeAsciiToken(const char *type) {
	PemsaToken token;

	token.type = TOKEN_IDENTIFIER;
	token.start = type;
	token.length = strlen(type);
	token.line = this->line;

	return token;
}

char PemsaScanner::advance() {
	return *this->current++;
}

char PemsaScanner::peek() {
	return *this->current;
}

char PemsaScanner::peekNext() {
	if (isAtEnd()) {
		return '\0';
	}

	return this->current[1];
}

bool PemsaScanner::match(char c) {
	if (this->isAtEnd()) {
		return false;
	}

	if (*this->current != c) {
		return false;
	}

	this->current++;
	return true;
}

PemsaToken PemsaScanner::skipWhitespace() {
	this->start = this->current;

	while (true) {
		char c = this->peek();

		switch (c) {
			case ' ':
			case '\r':
			case '\t': {
				this->advance();
				break;
			}

			case '\n': {
				this->line++;
				this->advance();

				return this->makeToken(TOKEN_NEW_LINE);
			}

			case '-':
			case '/': {
				bool legit = c == '-';

				if (this->peekNext() == c) {
					this->advance();
					this->advance();

					bool multiline = legit && this->peek() == '[' && this->peekNext() == '[';

					if (multiline) {
						int count = 1;

						this->advance();
						this->advance();

						while (count > 0 && !this->isAtEnd()) {
							if (this->peek() == '[' && this->peekNext() == '[') {
								count++;

								this->advance();
								this->advance();
							} else if (this->peek() == ']' && this->peekNext() == ']') {
								count--;

								this->advance();
								this->advance();
							} else {
								this->advance();
							}
						}
					} else {
						while (this->peek() != '\n' && !this->isAtEnd()) {
							this->advance();
						}
					}
				} else {
					return this->makeToken(this->start == this->current ? TOKEN_EOF : TOKEN_WHITESPACE);
				}

				break;
			}

			default: {
				return this->makeToken(this->start == this->current ? TOKEN_EOF : TOKEN_WHITESPACE);
			}
		}
	}
}

PemsaTokenType PemsaScanner::checkKeyword(int start, int length, const char* rest, PemsaTokenType type) {
	if (this->current - this->start == start + length && memcmp(this->start + start, rest, length) == 0) {
		return type;
	}

	return TOKEN_IDENTIFIER;
}

PemsaTokenType PemsaScanner::decideIdentifierType() {
	switch (this->start[0]) {
		case 'a': return this->checkKeyword(1, 2, "nd", TOKEN_AND);
		case 'b': return this->checkKeyword(1, 4, "reak", TOKEN_BREAK);
		case 'd': return this->checkKeyword(1, 1, "o", TOKEN_DO);
		case 'l': return this->checkKeyword(1, 4, "ocal", TOKEN_LOCAL);
		case 'o': return this->checkKeyword(1, 1, "r", TOKEN_OR);
		case 'u': return this->checkKeyword(1, 4, "ntil", TOKEN_UNTIL);
		case 'w': return this->checkKeyword(1, 4, "hile", TOKEN_WHILE);
		case 'g': return this->checkKeyword(1, 3, "oto", TOKEN_GOTO);

		case 'e': {
			if (this->current - this->start > 4) {
				return this->checkKeyword(1, 5, "lseif", TOKEN_ELSE_IF);
			}

			if (this->current - this->start > 1) {
				switch (this->start[1]) {
					case 'n': return this->checkKeyword(2, 1, "d", TOKEN_END);
					case 'l': return this->checkKeyword(2, 2, "se", TOKEN_ELSE);
				}
			}

			break;
		}

		case 'f': {
			if (this->current - this->start > 1) {
				switch (this->start[1]) {
					case 'a': return this->checkKeyword(2, 3, "lse", TOKEN_FALSE);
					case 'o': return this->checkKeyword(2, 1, "r", TOKEN_FOR);
					case 'u': return this->checkKeyword(2, 6, "nction", TOKEN_FUNCTION);
				}
			}

			break;
		}

		case 'i': {
			if (this->current - this->start == 2) {
				switch (this->start[1]) {
					case 'f': return TOKEN_IF;
					case 'n': return TOKEN_IN;
				}
			}

			break;
		}

		case 'n': {
			if (this->current - this->start > 1) {
				switch (this->start[1]) {
					case 'i': return this->checkKeyword(2, 1, "l", TOKEN_NIL);
					case 'o': return this->checkKeyword(2, 1, "t", TOKEN_NOT);
				}
			}

			break;
		}

		case 'r': {
			if (this->current - this->start > 2) {
				switch (this->start[2]) {
					case 'p': return this->checkKeyword(3, 3, "eat", TOKEN_REPEAT);
					case 't': return this->checkKeyword(3, 3, "urn", TOKEN_RETURN);
				}
			}

			break;
		}

		case 't': {
			if (this->current - this->start > 1) {
				switch (this->start[1]) {
					case 'h': return this->checkKeyword(2, 2, "en", TOKEN_THEN);
					case 'r': return this->checkKeyword(2, 2, "ue", TOKEN_TRUE);
				}
			}

			break;
		}
	}

	return TOKEN_IDENTIFIER;
}

const char *PemsaScanner::getCurrent() {
	return this->current;
}