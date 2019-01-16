/**
 * A Rytter item actually consists of two parts. The first part is a speculative
 * CYK item, and the second part is a CYK item that, when realizable, makes
 * the speculative item realizable too.
 */
public class RytterItem implements Comparable {
    /** The speculation of this Rytter item. */
    public CykItem speculation;

    /**
     * The condition of this Rytter item. If the condition is fulfilled,
     * the speculation is fulfilled as well.
     */
    public CykItem condition;

    /**
     * Constructs a Rytter item from the specified speculation and condition.
     * @param spec the speculation.
     * @param cond the condition.
     */
    public RytterItem(CykItem spec, CykItem cond) {
        speculation = spec;
        condition = cond;
    }

    /**
     * Compares the specified object with the current Rytter item.
     * @param o the object to compare to.
     * @return <code>true</code> if the objects are equal,
     * <code>false</code> if not.
     */
    public boolean equals(Object o) {
        if (! (o instanceof RytterItem)) {
            return false;
        }
        RytterItem r = (RytterItem) o;
        return r.speculation.equals(speculation)
                && r.condition.equals(condition);
    }

    /**
     * Computes the hashcode of a Rytter item. When using <code>HashSet</code>,
     * as we are, and <code>equals</code> is redefined, we also need to
     * redefine <code>hashCode</code>.
     * @return the hashcode of the Rytter item.
     */
    public int hashCode() {
        return speculation.hashCode() + condition.hashCode();
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
        RytterItem r = (RytterItem) o;
        int cmp = speculation.compareTo(r.speculation);
        if (cmp != 0) {
            return cmp;
        }
        return condition.compareTo(r.condition);
    }

    /**
     * Returns a String representation of the Rytter item.
     * @return a String representation of this Rytter item.
     */
    public String toString() {
        return "(" + speculation.nonTerminal
                + ", " + speculation.startRange
                + ", " + speculation.endRange
                + "; " + condition.nonTerminal
                + ", " + condition.startRange
                + ", " + condition.endRange + ")";
    }
}
