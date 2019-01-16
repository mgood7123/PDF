/**
 * Representation of a grammar rule, left-hand side and right-hand side.
 */
public class Rule {
    /** The left-hand side of the rule. */
    public Symbol lhs;

    /** the right-hand side of the rule. */
    public Symbol[] rhs;

    /**
     * Constructs a grammar rule with the specified parameters.
     * @param lhs the left-hand side of the rule.
     * @param rhs the right-hand side of the rule.
     */
    public Rule(Symbol lhs, Symbol[] rhs) {
        this.lhs = lhs;
        this.rhs = rhs;
    }

    /**
     * Returns a String representation of the grammar rule.
     * @return a String representation.
     */
    public String toString() {
        return lhs.toString() + " -> \"" + Symbol.toString(rhs) + "\"";
    }
}
