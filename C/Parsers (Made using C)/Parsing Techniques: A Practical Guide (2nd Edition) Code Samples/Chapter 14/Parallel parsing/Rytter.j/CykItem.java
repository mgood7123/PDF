/**
 * Representation of a CYK item. A CYK item is represented by a non-terminal
 * <code>A</code> and two integers <code>i</code> and <code>j</code>.
 * If the input of the parser consists of symbols <code>a[1] ... a[n]</code>,
 * the item means that non-terminal <code>A</code> recognizes the substring
 * <code>a[i+1] ... a[j]</code>.
 */
public class CykItem implements Comparable {
    /** The nonTerminal as described above. */
    public Symbol nonTerminal;

    /** The <code>i</code> as described above. */
    public int startRange;

    /** The <code>j</code> as described above. */
    public int endRange;

    /**
     * Constructs a CYK item from the given parameters.
     * @param nont the nonterminal.
     * @param start the start index of the substring (- 1).
     * @param end the end index of the substring.
     */
    public CykItem(Symbol nont, int start, int end) {
        nonTerminal = nont;
        startRange = start;
        endRange = end;
    }

    /**
     * Compares the specified object with the current one.
     * @param o the object to compare to.
     * @return <code>true</code> if the objects are equal,
     * <code>false</code> if not.
     */
    public boolean equals(Object o) {
        if (! (o instanceof CykItem)) {
            return false;
        }
        CykItem c = (CykItem) o;
        return startRange == c.startRange
                && endRange == c.endRange
                && nonTerminal.equals(c.nonTerminal);
    }

    /**
     * Computes the hashcode of a CYK item. When using <code>HashSet</code>,
     * as we are, and <code>equals</code> is redefined, we also need to
     * redefine <code>hashCode</code>.
     * @return the hashcode of the CYK item.
     */
    public int hashCode() {
        return startRange + (endRange << 256) + (nonTerminal.hashCode() << 16);
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
        CykItem i = (CykItem) o;
        int cmp = nonTerminal.compareTo(i.nonTerminal);
        if (cmp != 0) {
            return cmp;
        }
        cmp = startRange - i.startRange;
        if (cmp != 0) {
            return cmp;
        }
        return endRange - i.endRange;
    }

    /**
     * Returns a String representation of the CYK item.
     * @return a String representation of this CYK item.
     */
    public String toString() {
        return "(" + nonTerminal + ", " + startRange + ", " + endRange + ")";
    }
}
