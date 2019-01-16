/**
 * Representation of a grammar symbol.
 */
public class Symbol implements Comparable {
    /** Internal representation of a symbol: a string. */
    private String id;

    /**
     * Creates a symbol from the specified parameter.
     * @param id the symbol as a string.
     */
    public Symbol(String id) {
        this.id = id;
    }

    /**
     * Compares to the specified object.
     * @param o the object to compare to.
     * @return <code>true</code> if equal, <code>false</code> if not.
     */
    public boolean equals(Object o) {
        if (! (o instanceof Symbol)) {
            return false;
        }
        return id.equals(((Symbol) o).id);
    }

    /**
     * Computes the hashcode of a symbol. When using <code>HashSet</code>,
     * as we are, and <code>equals</code> is redefined, we also need to
     * redefine <code>hashCode</code>.
     * @return the hashcode of the symbol.
     */
    public int hashCode() {
        return id.hashCode();
    }

    /**
     * A comparison method, to be able to print sets sorted.
     * @param o the object to compare to.
     * @return a negative integer, zero, or a positive integer as this object
     * is less than, equal to, or greater than the specified object.
     * @exception ClassCastException if the specified object's type prevents
     * it from being compared to this Object.
     */
    public int compareTo(Object o) {
        Symbol s = (Symbol) o;
        return id.compareTo(s.id);
    }

    /**
     * Returns a string representation of this symbol.
     * @return the symbol as a string.
     */
    public String toString() {
        return id;
    }

    /**
     * Converts the specified array of symbols to a string, possibly with
     * spaces between the symbols.
     * @param sym the array of symbols.
     * @param sp when set, spaces are placed between the symbols.
     * @return the resulting string.
     */
    private static String toPossiblySpacedString(boolean sp, Symbol sym[]) {
        String res = new String();
        for (int n = 0; n < sym.length; n++) {
            res += (!sp || res.length() == 0 ? "" : " ") + sym[n].id;
        }
        return res;
    }
 
    /**
     * Converts the specified array of symbols to a string.
     * @param sym the array of symbols.
     * @return the resulting string.
     */
    public static String toString(Symbol sym[]) {
        return toPossiblySpacedString(false, sym);
    }

    /**
     * Converts the specified array of symbols to a string, with spaces between
     * the symbols.
     * @param sym the array of symbols.
     * @return the resulting string.
     */
    public static String toSpacedString(Symbol sym[]) {
        return toPossiblySpacedString(true, sym);
    }
}
