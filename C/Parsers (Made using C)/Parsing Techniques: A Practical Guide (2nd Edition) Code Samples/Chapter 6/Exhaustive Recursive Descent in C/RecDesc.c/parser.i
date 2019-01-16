static void a(void (*tail)(void)) /* recognize an 'a' and call tail */
{ if (text[tp] == 'a') { tp++; tail(); --tp; } }

static void b(void (*tail)(void)) /* recognize a 'b' and call tail */
{ if (text[tp] == 'b') { tp++; tail(); --tp; } }

static void c(void (*tail)(void)) /* recognize a 'c' and call tail */
{ if (text[tp] == 'c') { tp++; tail(); --tp; } }

static void A(void (*tail)(void)) /* recognize an 'A' and call tail */
{
	pushrule("A -> a"); a(tail); poprule();
	static void A_t(void) { A(tail); }
	pushrule("A -> aA"); a(A_t); poprule();
}

static void B(void (*tail)(void)) /* recognize a 'B' and call tail */
{
	static void c_t(void) { c(tail); }
	pushrule("B -> bc");  b(c_t); poprule();
	static void Bc_t(void) { B(c_t); }
	pushrule("B -> bBc"); b(Bc_t); poprule();
}

static void D(void (*tail)(void)) /* recognize a 'D' and call tail */
{
	static void b_t(void) { b(tail); }
	pushrule("D -> ab");  a(b_t); poprule();
	static void Db_t(void) { D(b_t); }
	pushrule("D -> aDb"); a(Db_t); poprule();
}

static void C(void (*tail)(void)) /* recognize a 'C' and call tail */
{
	pushrule("C -> c");  c(tail); poprule();
	static void C_t(void) { C(tail); }
	pushrule("C -> cC"); c(C_t); poprule();
}

static void S(void (*tail)(void)) /* recognize a 'S' and call tail */
{
	static void C_t(void) { C(tail); }
	pushrule("S -> DC"); D(C_t); poprule();
	static void B_t(void) { B(tail); }
	pushrule("S -> AB"); A(B_t); poprule();
}

static void endmark(void)      /* recognize end and report success */
{ if (text[tp] == '#') parsing_found(); }

static void parser(void) { tp = plp = 0; S(endmark); }

int main(void) { while ( getline()) parser(); return 0; }
