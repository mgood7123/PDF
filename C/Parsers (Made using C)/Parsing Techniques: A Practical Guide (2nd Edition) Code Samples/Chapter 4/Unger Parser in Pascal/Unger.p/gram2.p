{	This is the highly ambiguous grammar
		S -> A
		A -> AA | a | eps
}

procedure InitGrammar;					{ Grammar 2 }
    begin
	Start:= 'S';
	StoreRule('S', 'A         ');
	StoreRule('A', 'AA        ');
	StoreRule('A', '          ');
	StoreRule('A', 'a         ');
    end;
