#ifndef PEMSA_SCANNER_HPP
#define PEMSA_SCANNER_HPP

enum PemsaTokenType {
	TOKEN_AND, TOKEN_BREAK, TOKEN_DO,
	TOKEN_ELSE, TOKEN_ELSE_IF, TOKEN_END,
	TOKEN_FALSE, TOKEN_FOR, TOKEN_FUNCTION,
	TOKEN_IF, TOKEN_IN, TOKEN_LOCAL, TOKEN_NIL,
	TOKEN_NOT, TOKEN_OR, TOKEN_REPEAT, TOKEN_RETURN,
	TOKEN_THEN, TOKEN_TRUE, TOKEN_UNTIL, TOKEN_SHARP,
	TOKEN_WHILE, TOKEN_DOT, TOKEN_DOT_DOT, TOKEN_DOT_DOT_DOT,
	TOKEN_EQUAL, TOKEN_EQUAL_EQUAL, TOKEN_PLUS_EQUAL,
	TOKEN_MINUS_EQUAL, TOKEN_SLASH_EQUAL, TOKEN_STAR_EQUAL,
	TOKEN_MINUS, TOKEN_PLUS, TOKEN_STAR, TOKEN_SLASH,
	TOKEN_BACKWARDS_SLASH, TOKEN_LESS_EQUAL,
	TOKEN_GREATER_EQUAL, TOKEN_LESS, TOKEN_GREATER,
	TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN, TOKEN_LEFT_BRACE,
	TOKEN_RIGHT_BRACKET, TOKEN_LEFT_BRACKET,
	TOKEN_COLON, TOKEN_COLON_COLON, TOKEN_QUESTION,
	TOKEN_SEMICOLON, TOKEN_COMMA, TOKEN_TILDA_EQUAL, TOKEN_GOTO,
	TOKEN_RIGHT_BRACE, TOKEN_NUMBER, TOKEN_IDENTIFIER,
	TOKEN_STRING, TOKEN_MULTILINE_STRING, TOKEN_NEW_LINE, TOKEN_EOF,
	TOKEN_ERROR, TOKEN_MODULO, TOKEN_MODULO_EQUAL,
	TOKEN_WHITESPACE, TOKEN_TILDA, TOKEN_CAP, TOKEN_UNKNOWN,
	TOKEN_BAND, TOKEN_BOR, TOKEN_DOG, TOKEN_DOLLAR
};

struct PemsaToken {
	PemsaTokenType type;
	const char* start;
	int length;
	int line;
};

class PemsaScanner {
	public:
		PemsaScanner(const char* source);

		PemsaToken scan();
		const char* getCurrent();
	private:
		const char* start;
		const char* current;

		int line;

		PemsaToken parseIdentifier();
		PemsaToken parseNumber();

		bool isAtEnd();

		PemsaToken makeToken(PemsaTokenType type);
		PemsaToken makeErrorToken(const char* message);
		PemsaToken makeAsciiToken(const char* type);

		char advance();
		char peek();
		char peekNext();
		bool match(char c);
		PemsaToken skipWhitespace();

		PemsaTokenType checkKeyword(int start, int length, const char *rest, PemsaTokenType type);
		PemsaTokenType decideIdentifierType();
};

#endif