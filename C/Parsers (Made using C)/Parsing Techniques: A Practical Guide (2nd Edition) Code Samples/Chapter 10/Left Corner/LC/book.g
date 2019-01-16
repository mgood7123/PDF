/* The grammar from Figure 10.8 */

%start LC, S;

S:
	'n' S_after_F
|	'(' E ')' S_after_F
;
S_after_F:
	S_after_T
;
S_after_T:
	S_after_E
|	'*' F S_after_T
;
S_after_E:
	'+' T S_after_E
|	S_after_S
;
S_after_S:
	epsilon
;

E:
	'n' E_after_F
|	'(' E ')' E_after_F
;
E_after_F:
	E_after_T
;
E_after_T:
	E_after_E
|	'*' F E_after_T
;
E_after_E:
	'+' T E_after_E
|	epsilon
;

T:
	'n' T_after_F
|	'(' E ')' T_after_F
;
T_after_F:
	T_after_T
;
T_after_T:
	'*' F T_after_T
|	epsilon
;

F:
	'n' F_after_F
|	'(' E ')' F_after_F
;
F_after_F:
	epsilon
;

epsilon:
;
