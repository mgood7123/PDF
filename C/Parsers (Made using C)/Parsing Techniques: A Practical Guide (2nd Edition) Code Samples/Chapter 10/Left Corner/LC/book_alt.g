/* Alternative LL(1) grammar for the grammar of Figure 10.2 */

%start  LC, S ;

S:
	F_term  S_after_F
;
S_after_E:
	E_after_initial_E  S_after_E
|	S_after_initial_E  S_after_S
;
S_after_initial_E:
;
S_after_F:
	T_after_initial_F  S_after_T
;
S_after_S:
;
S_after_T:
	E_after_initial_T  S_after_E
|	T_after_initial_T  S_after_T
;

E:
	F_term  E_after_F
;
E_after_E:
	E_after_initial_E  E_after_E
|
;
E_after_initial_E:
	'+'  T
;
E_after_F:
	T_after_initial_F  E_after_T
;
E_after_T:
	E_after_initial_T  E_after_E
|	T_after_initial_T  E_after_T
;
E_after_initial_T:
;

T:
	F_term  T_after_F
;
T_after_F:
	T_after_initial_F  T_after_T
;
T_after_initial_F:
;
T_after_T:
	T_after_initial_T  T_after_T
|
;
T_after_initial_T:
	'*'  F
;

F:
	F_term  F_after_F
;
F_after_F:
;
F_term:
	[ 'n' | '('  E  ')' ]
;
