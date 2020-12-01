#include "pemsa/util/pemsa_font.hpp"

static const char* exclamation[] = {
	" x ",
	" x ",
	" x ",
	"   ",
	" x ",
};

static const char* quotes[] = {
	"x x",
	"x x",
	"   ",
	"   ",
	"   ",
};

static const char* hashtag[] = {
	"x x",
	"xxx",
	"x x",
	"xxx",
	"x x",
};

static const char* dollar[] = {
	"xxx",
	"xx ",
	" xx",
	"xxx",
	" x ",
};

static const char* percentage[] = {
	"x x",
	"  x",
	" x ",
	"x  ",
	"x x",
};

static const char* ampersand[] = {
	"xx ",
	"xx ",
	"xx ",
	"x x",
	"xxx",
};

static const char* tone[] = {
	" x ",
	"x  ",
	"   ",
	"   ",
	"   ",
};

static const char* paropen[] = {
	" x ",
	"x  ",
	"x  ",
	"x  ",
	" x ",
};

static const char* parclose[] = {
	" x ",
	"  x",
	"  x",
	"  x",
	" x ",
};

static const char* astherisc[] = {
	"x x",
	" x ",
	"xxx",
	" x ",
	"x x",
};

static const char* plus[] = {
	"   ",
	" x ",
	"xxx",
	" x ",
	"   ",
};

static const char* comma[] = {
	"   ",
	"   ",
	"   ",
	" x ",
	"x  ",
};

static const char* dash[] = {
	"   ",
	"   ",
	"xxx",
	"   ",
	"   ",
};

static const char* dot[] = {
	"   ",
	"   ",
	"   ",
	"   ",
	" x ",
};

static const char* slash[] = {
	"  x",
	" x ",
	" x ",
	" x ",
	"x  ",
};

static const char* digit0[] = {
	"xxx",
	"x x",
	"x x",
	"x x",
	"xxx",
};

static const char* digit1[] = {
	"xx ",
	" x ",
	" x ",
	" x ",
	"xxx",
};

static const char* digit2[] = {
	"xxx",
	"  x",
	"xxx",
	"x  ",
	"xxx",
};

static const char* digit3[] = {
	"xxx",
	"  x",
	" xx",
	"  x",
	"xxx",
};

static const char* digit4[] = {
	"x x",
	"x x",
	"xxx",
	"  x",
	"  x",
};

static const char* digit5[] = {
	"xxx",
	"x  ",
	"xxx",
	"  x",
	"xxx",
};

static const char* digit6[] = {
	"x  ",
	"x  ",
	"xxx",
	"x x",
	"xxx",
};

static const char* digit7[] = {
	"xxx",
	"  x",
	"  x",
	"  x",
	"  x",
};

static const char* digit8[] = {
	"xxx",
	"x x",
	"xxx",
	"x x",
	"xxx",
};

static const char* digit9[] = {
	"xxx",
	"x x",
	"xxx",
	"  x",
	"  x",
};

static const char* colon[] = {
	"   ",
	" x ",
	"   ",
	" x ",
	"   ",
};

static const char* semicolon[] = {
	"   ",
	" x ",
	"   ",
	" x ",
	"x  ",
};

static const char* less[] = {
	"  x",
	" x ",
	"x  ",
	" x ",
	"  x",
};

static const char* equals[] = {
	"   ",
	"xxx",
	"   ",
	"xxx",
	"   ",
};

static const char* greater[] = {
	"x  ",
	" x ",
	"  x",
	" x ",
	"x  ",
};

static const char* question[] = {
	"xxx",
	"  x",
	" xx",
	"   ",
	" x ",
};

static const char* at[] = {
	" x ",
	"x x",
	"x x",
	"x  ",
	" xx",
};

static const char* a[] = {
	"   ",
	"xxx",
	"x x",
	"xxx",
	"x x",
};

static const char* b[] = {
	"   ",
	"xx ",
	"xx ",
	"x x",
	"xxx",
};

static const char* c[] = {
	"   ",
	"xxx",
	"x  ",
	"x  ",
	"xxx",
};

static const char* d[] = {
	"   ",
	"xx ",
	"x x",
	"x x",
	"xx ",
};

static const char* e[] = {
	"   ",
	"xxx",
	"xx ",
	"x  ",
	"xxx",
};

static const char* f[] = {
	"   ",
	"xxx",
	"xx ",
	"x  ",
	"x  ",
};

static const char* g[] = {
	"   ",
	"xxx",
	"x  ",
	"x x",
	"xxx",
};

static const char* h[] = {
	"   ",
	"x x",
	"x x",
	"xxx",
	"x x",
};

static const char* i[] = {
	"   ",
	"xxx",
	" x ",
	" x ",
	"xxx",
};

static const char* j[] = {
	"   ",
	"xxx",
	" x ",
	" x ",
	"xx ",
};

static const char* k[] = {
	"   ",
	"x x",
	"xx ",
	"x x",
	"x x",
};

static const char* l[] = {
	"   ",
	"x  ",
	"x  ",
	"x  ",
	"xxx",
};

static const char* m[] = {
	"   ",
	"xxx",
	"xxx",
	"x x",
	"x x",
};

static const char* n[] = {
	"   ",
	"xx ",
	"x x",
	"x x",
	"x x",
};

static const char* o[] = {
	"   ",
	" xx",
	"x x",
	"x x",
	"xx ",
};

static const char* p[] = {
	"   ",
	"xxx",
	"x x",
	"xxx",
	"x  ",
};

static const char* q[] = {
	"   ",
	" x ",
	"x x",
	"xx ",
	" xx",
};

static const char* r[] = {
	"   ",
	"xxx",
	"x x",
	"xx ",
	"x x",
};

static const char* s[] = {
	"   ",
	" xx",
	"x  ",
	"  x",
	"xx ",
};

static const char* t[] = {
	"   ",
	"xxx",
	" x ",
	" x ",
	" x ",
};

static const char* u[] = {
	"   ",
	"x x",
	"x x",
	"x x",
	" xx",
};

static const char* v[] = {
	"   ",
	"x x",
	"x x",
	"xxx",
	" x ",
};

static const char* w[] = {
	"   ",
	"x x",
	"x x",
	"xxx",
	"xxx",
};

static const char* x[] = {
	"   ",
	"x x",
	" x ",
	"x x",
	"x x",
};

static const char* y[] = {
	"   ",
	"x x",
	"xxx",
	"  x",
	"xxx",
};

static const char* z[] = {
	"   ",
	"xxx",
	"  x",
	"x  ",
	"xxx",
};

static const char* bracketopen[] = {
	"xx ",
	"x  ",
	"x  ",
	"x  ",
	"xx ",
};

static const char* backslash[] = {
	"x  ",
	" x ",
	" x ",
	" x ",
	"  x",
};

static const char* bracketclose[] = {
	" xx",
	"  x",
	"  x",
	"  x",
	" xx",
};

static const char* carat[] = {
	" x ",
	"x x",
	"   ",
	"   ",
	"   ",
};

static const char* underscore[] = {
	"   ",
	"   ",
	"   ",
	"   ",
	"xxx",
};

static const char* backquote[] = {
	" x ",
	"  x",
	"   ",
	"   ",
	"   ",
};

static const char* A[] = {
	"xxx",
	"x x",
	"xxx",
	"x x",
	"x x",
};

static const char* B[] = {
	"xxx",
	"x x",
	"xx ",
	"x x",
	"xxx",
};

static const char* C[] = {
	" xx",
	"x  ",
	"x  ",
	"x  ",
	" xx",
};

static const char* D[] = {
	"xx ",
	"x x",
	"x x",
	"x x",
	"xxx",
};

static const char* E[] = {
	"xxx",
	"x  ",
	"xx ",
	"x  ",
	"xxx",
};

static const char* F[] = {
	"xxx",
	"x  ",
	"xx ",
	"x  ",
	"x  ",
};

static const char* G[] = {
	" xx",
	"x  ",
	"x  ",
	"x x",
	"xxx",
};

static const char* H[] = {
	"x x",
	"x x",
	"xxx",
	"x x",
	"x x",
};

static const char* I[] = {
	"xxx",
	" x ",
	" x ",
	" x ",
	"xxx",
};

static const char* J[] = {
	"xxx",
	" x ",
	" x ",
	" x ",
	"xx ",
};

static const char* K[] = {
	"x x",
	"x x",
	"xx ",
	"x x",
	"x x",
};

static const char* L[] = {
	"x  ",
	"x  ",
	"x  ",
	"x  ",
	"xxx",
};

static const char* M[] = {
	"xxx",
	"xxx",
	"x x",
	"x x",
	"x x",
};

static const char* N[] = {
	"xx ",
	"x x",
	"x x",
	"x x",
	"x x",
};

static const char* O[] = {
	" xx",
	"x x",
	"x x",
	"x x",
	"xx ",
};

static const char* P[] = {
	"xxx",
	"x x",
	"xxx",
	"x  ",
	"x  ",
};

static const char* Q[] = {
	" x ",
	"x x",
	"x x",
	"xx ",
	" xx",
};

static const char* R[] = {
	"xxx",
	"x x",
	"xx ",
	"x x",
	"x x",
};

static const char* S[] = {
	" xx",
	"x  ",
	"xxx",
	"  x",
	"xx ",
};

static const char* T[] = {
	"xxx",
	" x ",
	" x ",
	" x ",
	" x ",
};

static const char* U[] = {
	"x x",
	"x x",
	"x x",
	"x x",
	" xx",
};

static const char* V[] = {
	"x x",
	"x x",
	"x x",
	"x x",
	" x ",
};

static const char* W[] = {
	"x x",
	"x x",
	"x x",
	"xxx",
	"xxx",
};

static const char* X[] = {
	"x x",
	"x x",
	" x ",
	"x x",
	"x x",
};

static const char* Y[] = {
	"x x",
	"x x",
	"xxx",
	"  x",
	"xxx",
};

static const char* Z[] = {
	"xxx",
	"  x",
	" x ",
	"x  ",
	"xxx",
};

static const char* braceopen[] = {
	" xx",
	" x ",
	"xx ",
	" x ",
	" xx",
};

static const char* pipe[] = {
	" x ",
	" x ",
	" x ",
	" x ",
	" x ",
};

static const char* braceclose[] = {
	"xx ",
	" x ",
	" xx",
	" x ",
	"xx ",
};

static const char* tilde[] = {
	"   ",
	"  x",
	"xxx",
	"x  ",
	"   ",
};

static const char* nubbin[] = {
	"   ",
	" x ",
	"x x",
	" x ",
	"   ",
};

static const char* block[] = {
	"xxxxxxx",
	"xxxxxxx",
	"xxxxxxx",
	"xxxxxxx",
	"xxxxxxx",
};

static const char* semiblock[] = {
	"x x x x",
	" x x x ",
	"x x x x",
	" x x x ",
	"x x x x",
};

static const char* alien[] = {
	"x     x",
	"xxxxxxx",
	"x xxx x",
	"x xxx x",
	" xxxxx ",
};

static const char* downbutton[] = {
	" xxxxx ",
	"xx   xx",
	"xx   xx",
	"xxx xxx",
	" xxxxx ",
};

static const char* quasiblock[] = {
	"x   x  ",
	"  x   x",
	"x   x  ",
	"  x   x",
	"x   x  ",
};

static const char* shuriken[] = {
	"  x    ",
	"  xxxx ",
	"  xxx  ",
	" xxxx  ",
	"    x  ",
};

static const char* shinyball[] = {
	"  xxx  ",
	" xxx x ",
	" xxxxx ",
	" xxxxx ",
	"  xxx  ",
};

static const char* heart[] = {
	" xx xx ",
	" xxxxx ",
	" xxxxx ",
	"  xxx  ",
	"   x   ",
};

static const char* sauron[] = {
	"  xxx  ",
	" xx xx ",
	"xxx xxx",
	" xx xx ",
	"  xxx  ",
};

static const char* human[] = {
	"  xxx  ",
	"  xxx  ",
	" xxxxx ",
	"  xxx  ",
	"  x x  ",
};

static const char* house[] = {
	"  xxx  ",
	" xxxxx ",
	"xxxxxxx",
	" x x x ",
	" x xxx ",
};

static const char* leftbutton[] = {
	" xxxxx ",
	"xxx  xx",
	"xx   xx",
	"xxx  xx",
	" xxxxx ",
};

static const char* face[] = {
	"xxxxxxx",
	"x xxx x",
	"xxxxxxx",
	"x     x",
	"xxxxxxx",
};

static const char* note[] = {
	"   xxx ",
	"   x   ",
	"   x   ",
	" xxx   ",
	" xxx   ",
};

static const char* obutton[] = {
	" xxxxx ",
	"xx   xx",
	"xx x xx",
	"xx   xx",
	" xxxxx ",
};

static const char* diamond[] = {
	"   x   ",
	"  xxx  ",
	" xxxxx ",
	"  xxx  ",
	"   x   ",
};

static const char* dotline[] = {
	"       ",
	"       ",
	"x x x x",
	"       ",
	"       ",
};

static const char* rightbutton[] = {
	" xxxxx ",
	"xx  xxx",
	"xx   xx",
	"xx  xxx",
	" xxxxx ",
};

static const char* star[] = {
	"   x   ",
	"  xxx  ",
	"xxxxxxx",
	" xxxxx ",
	" x   x ",
};

static const char* hourglass[] = {
	" xxxxx ",
	"  xxx  ",
	"   x   ",
	"  xxx  ",
	" xxxxx ",
};

static const char* upbutton[] = {
	" xxxxx ",
	"xxx xxx",
	"xx   xx",
	"xx   xx",
	" xxxxx ",
};

static const char* downarrows[] = {
	"       ",
	"x x    ",
	" x  x x",
	"     x ",
	"       ",
};

static const char* trianglewave[] = {
	"       ",
	"x   x  ",
	" x x x ",
	"  x   x",
	"       ",
};

static const char* xbutton[] = {
	" xxxxx ",
	"xx x xx",
	"xxx xxx",
	"xx x xx",
	" xxxxx ",
};

static const char* horizontallines[] = {
	"xxxxxxx",
	"       ",
	"xxxxxxx",
	"       ",
	"xxxxxxx",
};

static const char* verticallines[] = {
	"x x x x",
	"x x x x",
	"x x x x",
	"x x x x",
	"x x x x",
};

static const char** pemsa_font[PEMSA_FONT_SIZE];
static bool setup;

void pemsa_setup_font() {
	if (setup) {
		return;
	}

	setup = true;

	pemsa_font['!'] = exclamation;
	pemsa_font['"'] = quotes;
	pemsa_font['#'] = hashtag;
	pemsa_font['$'] = dollar;
	pemsa_font['%'] = percentage;
	pemsa_font['&'] = ampersand;
	pemsa_font['\''] = tone;
	pemsa_font['('] = paropen;
	pemsa_font[')'] = parclose;
	pemsa_font['*'] = astherisc;
	pemsa_font['+'] = plus;
	pemsa_font[','] = comma;
	pemsa_font['-'] = dash;
	pemsa_font['.'] = dot;
	pemsa_font['/'] = slash;
	pemsa_font['0'] = digit0;
	pemsa_font['1'] = digit1;
	pemsa_font['2'] = digit2;
	pemsa_font['3'] = digit3;
	pemsa_font['4'] = digit4;
	pemsa_font['5'] = digit5;
	pemsa_font['6'] = digit6;
	pemsa_font['7'] = digit7;
	pemsa_font['8'] = digit8;
	pemsa_font['9'] = digit9;
	pemsa_font[':'] = colon;
	pemsa_font[';'] = semicolon;
	pemsa_font['<'] = less;
	pemsa_font['='] = equals;
	pemsa_font['>'] = greater;
	pemsa_font['?'] = question;
	pemsa_font['@'] = at;
	pemsa_font['a'] = a;
	pemsa_font['b'] = b;
	pemsa_font['c'] = c;
	pemsa_font['d'] = d;
	pemsa_font['e'] = e;
	pemsa_font['f'] = f;
	pemsa_font['g'] = g;
	pemsa_font['h'] = h;
	pemsa_font['i'] = i;
	pemsa_font['j'] = j;
	pemsa_font['k'] = k;
	pemsa_font['l'] = l;
	pemsa_font['m'] = m;
	pemsa_font['n'] = n;
	pemsa_font['o'] = o;
	pemsa_font['p'] = p;
	pemsa_font['q'] = q;
	pemsa_font['r'] = r;
	pemsa_font['s'] = s;
	pemsa_font['t'] = t;
	pemsa_font['u'] = u;
	pemsa_font['v'] = v;
	pemsa_font['w'] = w;
	pemsa_font['x'] = x;
	pemsa_font['y'] = y;
	pemsa_font['z'] = z;
	pemsa_font['['] = bracketopen;
	pemsa_font['\\'] = backslash;
	pemsa_font[']'] = bracketclose;
	pemsa_font['^'] = carat;
	pemsa_font['_'] = underscore;
	pemsa_font['`'] = backquote;
	pemsa_font['A'] = A;
	pemsa_font['B'] = B;
	pemsa_font['C'] = C;
	pemsa_font['D'] = D;
	pemsa_font['E'] = E;
	pemsa_font['F'] = F;
	pemsa_font['G'] = G;
	pemsa_font['H'] = H;
	pemsa_font['I'] = I;
	pemsa_font['J'] = J;
	pemsa_font['K'] = K;
	pemsa_font['L'] = L;
	pemsa_font['M'] = M;
	pemsa_font['N'] = N;
	pemsa_font['O'] = O;
	pemsa_font['P'] = P;
	pemsa_font['Q'] = Q;
	pemsa_font['R'] = R;
	pemsa_font['S'] = S;
	pemsa_font['T'] = T;
	pemsa_font['U'] = U;
	pemsa_font['V'] = V;
	pemsa_font['W'] = W;
	pemsa_font['X'] = X;
	pemsa_font['Y'] = Y;
	pemsa_font['Z'] = Z;
	pemsa_font['{'] = braceopen;
	pemsa_font['|'] = pipe;
	pemsa_font['}'] = braceclose;
	pemsa_font['~'] = tilde;
	pemsa_font[127] = nubbin;
	pemsa_font[128] = block;
	pemsa_font[129] = semiblock;
	pemsa_font[130] = alien;
	pemsa_font[131] = downbutton;
	pemsa_font[132] = quasiblock;
	pemsa_font[133] = shuriken;
	pemsa_font[134] = shinyball;
	pemsa_font[135] = heart;
	pemsa_font[136] = sauron;
	pemsa_font[137] = human;
	pemsa_font[138] = house;
	pemsa_font[139] = leftbutton;
	pemsa_font[140] = face;
	pemsa_font[141] = note;
	pemsa_font[142] = obutton;
	pemsa_font[143] = diamond;
	pemsa_font[144] = dotline;
	pemsa_font[145] = rightbutton;
	pemsa_font[146] = star;
	pemsa_font[147] = hourglass;
	pemsa_font[148] = upbutton;
	pemsa_font[149] = downarrows;
	pemsa_font[150] = trianglewave;
	pemsa_font[151] = xbutton;
	pemsa_font[152] = horizontallines;
	pemsa_font[153] = verticallines;
}

const char** pemsa_get_letter(int c) {
	if (c < 0 || c >= PEMSA_FONT_SIZE) {
		return nullptr;
	}

	return pemsa_font[c];
}