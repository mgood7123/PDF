import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;
import java.util.NoSuchElementException;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.StreamTokenizer;

/**
 * Representation of a grammar. For the Rytter parser, it should be in
 * CNF format, but this is not checked for now!
 */
public class Grammar implements Iterable<Rule> {
    /** List of production rules. */
    private ArrayList<Rule> ruleList = new ArrayList<Rule>();

    /** Set of nonterminals. */
    private HashSet<Symbol> nonTerminals = new HashSet<Symbol>();

    /** Start symbol of the grammar. Implicitly the first LHS seen. */
    public Symbol startSym = null;

    /**
     * Constructor, reads the grammar from the specified file.
     * @param filename the file from which the grammar must be read.
     * @exception IOException is thrown when an error occurs while reading
     * the grammar.
     */
    public Grammar(String filename) throws IOException {

        readGrammar(filename);

        // Check that grammar is in CNF.
        for (Rule r : ruleList) {
            if (r.rhs.length == 1) {
                // rhs should be a single terminal
                if (nonTerminals.contains(r.rhs[0])) {
                    throw new RuntimeException("Rule " + r
                            + " is not in CNF: rhs is a single nonterminal");
                }
            } else if (r.rhs.length == 2) {
                // rhs consist of two nonterminals
                if (! nonTerminals.contains(r.rhs[0])) {
                    throw new RuntimeException("Rule " + r
                            + " is not in CNF: rhs[0] is a terminal");
                }
                if (! nonTerminals.contains(r.rhs[1])) {
                    throw new RuntimeException("Rule " + r
                            + " is not in CNF: rhs[1] is a terminal");
                }
            } else {
                throw new RuntimeException("Rule " + r
                        + " is not in CNF: rhs length should be 1 or 2.");
            }
        }
    }

    /**
     * Returns an iterator over the current grammar.
     * @return the iterator.
     */
    public Iterator<Rule> iterator() {
        return ruleList.iterator();
    }

    /**
     * Constructs a rule from the specified parameters and adds it to the
     * grammar.
     * @param lhs the left-hand side of the rule.
     * @param rhs the right-hand side of the rule.
     */
    private void rule(Symbol lhs, Symbol[] rhs) {
        ruleList.add(new Rule(lhs, rhs));
        if (startSym == null) {
            // First left-hand side seen is the start symbol.
            startSym = lhs;
        }
        nonTerminals.add(lhs);
    }

    /**
     * Access method for grammar rules.
     * @return the <code>n</code>-th rule.
     */
    Rule rule(int n) {
        return ruleList.get(n);
    }

    /**
     * Returns the number of rules in the grammar.
     * @return the number of rules.
     */
    int nRules() {
        return ruleList.size();
    }

    /**
     * Reads the grammar from the specified file.
     * @param filename the file to read the grammar from.
     * @exception IOException is thrown when an error occurs while reading
     * the grammar.
     */
    private void readGrammar(String filename) throws IOException {
        BufferedReader in = new BufferedReader(new FileReader(filename));
        StreamTokenizer tokenizer = new StreamTokenizer(in);
        int token;

        tokenizer.eolIsSignificant(true);
        tokenizer.lowerCaseMode(false);
        tokenizer.ordinaryChar('"');
        tokenizer.ordinaryChar('\'');
        tokenizer.commentChar('#');

        for (;;) {
            do {
                token = tokenizer.nextToken();
            } while (token == StreamTokenizer.TT_EOL);
            if (token == StreamTokenizer.TT_EOF) {
                break;
            }
            if (token != StreamTokenizer.TT_WORD) {
                throw new IOException("Non-terminal expected");
            }
            Symbol lhs = new Symbol(tokenizer.sval);
            token = tokenizer.nextToken();
            if (token != ':') {
                throw new IOException(": expected");
            }
            ArrayList<Symbol> rhsList = new ArrayList<Symbol>();
            for (;;) {
                token = tokenizer.nextToken();
                if (token == StreamTokenizer.TT_EOF) {
                    throw new IOException("Unexpected EOF");
                }
                if (token == StreamTokenizer.TT_EOL) {
                    break;
                }
                if (token == StreamTokenizer.TT_WORD) {
                    rhsList.add(new Symbol(tokenizer.sval));
                } else {
                    rhsList.add(new Symbol(Character.toString((char) token)));
                }
            }
            rule(lhs, rhsList.toArray(new Symbol[rhsList.size()]));
        }
    }
}
