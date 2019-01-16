procedure InitGrammar;                          { Grammar 4 }
    begin
        Start:= 'S';
        StoreRule('S', 'LSR       ');
        StoreRule('S', '          ');
        StoreRule('L', '(         ');
        StoreRule('L', '          ');
        StoreRule('R', ')         ');
    end;

