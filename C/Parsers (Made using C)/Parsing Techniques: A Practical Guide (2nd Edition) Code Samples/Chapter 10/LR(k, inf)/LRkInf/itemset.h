#define	MAX_STRINGSIZE	100		/* fixed size :-( */

typedef char Symbol;
typedef Symbol SymbolSeq[MAX_STRINGSIZE];
typedef Symbol Item[MAX_STRINGSIZE];
typedef Item ItemSet[MAX_STRINGSIZE];

extern void clear_Item(Item I);
extern int is_Item(const Item I);
extern int is_empty_ItemSet(ItemSet V);
extern void clear_SymbolSeq(Symbol *s);
extern void new_SymbolSeq(Symbol *to, const Symbol *from);
extern void new_SymbolSeq_N(Symbol *to, const Symbol *from, int size);
extern void add_SymbolSeq(Symbol *to, const Symbol *from);
extern void add_Symbol(Symbol *to, Symbol ch);
extern void clear_ItemSet(ItemSet V);
extern void add_Item(ItemSet V, const Item I);
extern void print_ItemSet(const char *name, const Symbol *XY, ItemSet V);
