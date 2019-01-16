{$C+: distinguish between upper and lower case }
program parse(input, output);
{   This is an exhaustive backtracking recursive-descent parser that will
    correctly parse according to the grammar
        S -> D C | A B
        A -> a | a A
        B -> b c | b B c
        D -> a b | a D b
        C -> c | c C
    It implements proper backtracking by only checking one symbol at a
    time and passing the rest of the alternative as a parameter for
    evaluation on a lower level. A more naive backtracking parser will not
    accept e.g. aabc.
}

const   infinity = 100;                         { large enough }

type    str = packed array[1..10] of char;

var     tp: integer;                            { index in text }
        length: integer;                        { number of symbols in text }
        rp: integer;                            { index in rules }
        text: array [1..infinity] of char;      { input text }
        rules: array [1..infinity] of str;      { store rules used }

{ administration of rules used }
procedure pushrule (s: str); begin rp := rp + 1; rules[rp] := s end;
procedure poprule; begin rp := rp - 1 end;

procedure endmark;              { recognize end and report success }
        var     i: integer;
begin   if text[tp] = '#' then begin
              writeln('Derivation:');
              for i := 1 to rp do writeln('    ', rules[i]);
        end
end;

procedure a(procedure tail);    { recognize an 'a' and call tail }
begin if text[tp] = 'a' then begin tp := tp + 1; tail; tp := tp - 1 end end;

procedure b(procedure tail);    { recognize a 'b' and call tail }
begin if text[tp] = 'b' then begin tp := tp + 1; tail; tp := tp - 1 end end;

procedure c(procedure tail);    { recognize a 'c' and call tail }
begin if text[tp] = 'c' then begin tp := tp + 1; tail; tp := tp - 1 end end;

procedure A(procedure tail);    { recognize an 'A' and call tail }
        { procedures for the alternative tails }
        procedure t; begin tail end;
        procedure At; begin A(tail) end;
begin
        pushrule('A -> a    ');  a(t); poprule;
        pushrule('A -> aA   '); a(At); poprule
end;

procedure B(procedure tail);    { recognize a 'B' and call tail }
        procedure ct; begin c(tail) end;
        procedure Bct;
                procedure ct; begin c(tail) end;
        begin B(ct) end;
begin
        pushrule('B -> bc   ');  b(ct); poprule;
        pushrule('B -> bBc  '); b(Bct); poprule
end;

procedure D(procedure tail);    { recognize a 'D' and call tail }
        procedure bt; begin b(tail) end;
        procedure Dbt;
                procedure bt; begin b(tail) end;
        begin D(bt) end;
begin
        pushrule('D -> ab   ');  a(bt); poprule;
        pushrule('D -> aDb  '); a(Dbt); poprule
end;

procedure C(procedure tail);    { recognize a 'C' and call tail }
        procedure t; begin tail end;
        procedure Ct; begin C(tail) end;
begin
        pushrule('C -> c    ');  c(t); poprule;
        pushrule('C -> cC   '); c(Ct); poprule
end;

procedure S(procedure tail);    { recognize a 'S' and call tail }
        procedure Ct; begin C(tail) end;
        procedure Bt; begin B(tail) end;
begin
        pushrule('S -> DC   '); D(Ct); poprule;
        pushrule('S -> AB   '); A(Bt); poprule
end;

function readline: boolean;
begin
        write('> '); length := 1;
        if not eof then
        begin   while not eoln do begin
                        read(text[length]); length := length + 1;
                end;
                readln; readline := true
        end
        else readline := false;
end;

procedure parser;
begin   text[length] := '#'; tp := 1; rp := 0; S(endmark) end;

begin while readline do parser end.
