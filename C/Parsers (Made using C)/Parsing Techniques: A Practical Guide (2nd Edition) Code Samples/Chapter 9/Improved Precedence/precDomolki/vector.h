#define	M_WIDTH		12	/* grammar-dependent; will be checked */

typedef struct {char bit[M_WIDTH];} vector;

extern const vector Zero;
extern int M_width;

extern vector And(const vector a, const vector b);
extern vector Or(const vector a, const vector b);
extern int Eq(const vector a, const vector b);
extern vector Half(const vector a);
extern vector Row(const vector a[], int i);

extern int First1BitPos(const vector a);
extern int Number1Bits(const vector a, int n);

extern void pr_vector(const vector a);
