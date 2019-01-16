{	This is the LL(inf) grammar
		S -> A
		A -> ( A ]  |  B
		B -> ( B )  |  eps
}

procedure InitGrammar;					{ Grammar 1}
    begin
	Start:= 'S';
	StoreRule('S', 'A         ');
	StoreRule('A', '(A]       ');
	StoreRule('A', 'B         ');
	StoreRule('B', '(B)       ');
	StoreRule('B', '          ');
    end;
