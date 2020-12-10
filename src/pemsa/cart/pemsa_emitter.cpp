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
	bool insertParen = false;
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
	output <<R"(
A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z = -2560.5, 31455.5, 32125.5, 1, 6943.5, 3855.5, 2, -19008.5, 4, -20032.5,0.5, -20128.5, 3, -18402.5, -1632.5, 20927.5, -26208.5, -20192.5, 0, 21845.5, 5, 20767.5, -2624.5, 23130.5, -25792.5, -24351.5 
sub, cocreate, coresume, yield, costatus, debug, run = string.sub, coroutine.create, coroutine.resume, coroutine.yield, coroutine.status, nil, __reset

function foreach(a, f)
 if not a then return end
 local copy={}
 for i=1,#a do
  if a[i] ~= nil then table.insert(copy,a[i]) end
 end
 for i=1,#copy do
  f(copy[i])
 end
end
function count(a) if not a then return 0 end return #a end 
function arraylen(t)
 local len = 0 
 for i, _ in pairs(t) do 
  if type(i) == "number" then
   len = i 
  end 
 end 
 return len 
end 
function all(a) 
 if a == nil then 
  return function() end 
 end 
 local i = 0
 local n = arraylen(a) 
 return function() 
  i = i + 1 
  while (a[i] == nil and i <= n) do 
   i = i + 1 
  end 
  return a[i] 
 end 
end 
function add(a, v, i)
	if a == nil then return end 
	if i then
		table.insert(a, i, v)
	else
		table.insert(a, v)
	end
	return v
end
function del(a, dv)
	if a == nil then return end
	for i, v in ipairs(a) do
		if v == dv then
			table.remove(a, i)
			return dv
		end
	end
end
function deli(a, i)
	if a ~= nil then table.remove(a, i) end
end
local __menu_options_custom={}
local __current_option=1
local __menu_on=false
local __menu_functions={}
local __favorite=false
function __update_menu()
	if not btnp(6) and not __menu_on then return end

	local __menu_options={}
	for o in all(__menu_options_custom) do
		add(__menu_options,o)
	end

	add(__menu_options,"continue",1)
	add(__menu_options,"favorite")
	add(__menu_options,"reset cart")

	if btnp(6) then
		if __menu_on and __current_option==#__menu_options-1 then
			__favorite=not __favorite
		else
			__menu_on=not __menu_on
			__set_audio_paused(__menu_on)
			__set_paused(__menu_on)

			if not __menu_on then
				local fn=__menu_functions[__current_option-1]
				if __current_option==#__menu_options then fn=__reset end
				if fn then fn() end
				cls()
			end
		end
	end

	if not __menu_on then return end

	if btnp(2) then
		__current_option=__current_option-1
		if __current_option<1 then
			__current_option=#__menu_options
		end
	end

	if btnp(3) then
		__current_option=__current_option+1
		if __current_option>#__menu_options then
			__current_option=1
		end
	end

	local h=10+#__menu_options*8
	local x=24
	local y=(128-h)/2
	rectfill(x,y,x+81,y+h-1,0)
	rect(x+1,y+1,x+80,y+h-2,7)

	local ax=x+5
	local ay=y-1+__current_option*8

	for i=0,2 do
		line(ax+i,ay+i,ax+i,ay+4-i,7)
	end

	for i=1,#__menu_options do
		local current=__current_option==i
		print(__menu_options[i],x+11+(__menu_options[i] and 1 or 0),y-1+i*8,7)
		if i==#__menu_options-1 then
			print("\135",x+51,y-1+i*8,__favorite and 8 or 13)
		end
	end
end
function menuitem(i,name,fn)
	if i<1 or i>5 then return end
	__menu_options_custom[i]=name
	__menu_functions[i]=fn
end
function rnd(i)
	if type(i)=="table" then return i[flr(__rnd(#i))+1] end
	return __rnd(i)
end
)";
#endif

	while (running) {
		if (token.type != TOKEN_WHITESPACE && token.type != TOKEN_NEW_LINE) {
			previous = token;
		}

		token = scanner->scan();

		emitToken:
		if (insertParen) {
			output << ")";
			insertParen = false;
		}

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
#define CASE(a, b, c, d, u, e, z) if (str[0] == a && str[1] == b && (str[0] == -53 || (str[2] == c && (str[0] != -16 || str[3] == d)))) { output << "\\" << e; i += (str[0] == -16 ? 3 : (str[0] == -53 ? 1 : 2)); i += 3; continue; }
#include "pemsa/cart/pemsa_cases.hpp"
#undef CASE
					}

					char c = *str;

					if (c == '\n') {
						// \n after [[ and empty line is ignored by lua
						if (!multiline || i != start) {
							output << "\\n";
						}

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

				expressionStart = token.start + token.length;
				output << '\n';
				break;
			}

			case TOKEN_RIGHT_PAREN: {
				parenCount--;
				expressionStart = token.start + token.length;

				checkThen = parenCount == ifParenStart && inIf;
				checkDo = parenCount == whileParenStart && inWhile;

				output << std::string(token.start, token.length);

				do {
					previous = token;
					token = scanner->scan();
				} while (token.type == TOKEN_WHITESPACE);

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

					if (c == ' ' || c == '\r' || c == '\n' || c == ' ') {
						expressionStart = token.start;
					}
				}

				output << std::string(token.start, token.length);
				break;
			}

			case TOKEN_DO:
			case TOKEN_BREAK:
			case TOKEN_UNTIL:
			case TOKEN_REPEAT:
			case TOKEN_ELSE:
			case TOKEN_ELSE_IF:
			case TOKEN_END:
			case TOKEN_IF:
			case TOKEN_EQUAL:
			case TOKEN_THEN: {
				expressionStart = token.start + token.length;
				output << std::string(token.start, token.length);
				break;
			}

			/*case TOKEN_MODULO: {
				PemsaTokenType t = previous.type;
				if (t == TOKEN_NUMBER || t == TOKEN_IDENTIFIER || t == TOKEN_RIGHT_PAREN || t == TOKEN_RIGHT_BRACKET) {
					output << "%";
				} else {
					insertParen = true;
					output << "peek2";
				}

				break;
			}

			case TOKEN_DOG: {
				insertParen = true;
				output << "peek";
				break;
			}

			case TOKEN_DOLLAR: {
				insertParen = true;
				output << "peek4";
				break;
			}*/

			default: {
				output << std::string(token.start, token.length);
				break;
			}
		}
	}

	return output.str();
}