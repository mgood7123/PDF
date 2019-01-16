{	This grammar requires the prediction of a number of empty strings
	depending on the number of a's:
		S -> LSa | eps
		L -> eps
}

procedure InitGrammar;					{ Grammar 3 }
    begin
	Start:= 'S';
	StoreRule('S', 'LSa       ');
	StoreRule('S', '          ');
	StoreRule('L', '          ');
    end;

