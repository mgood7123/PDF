import java.util.Arrays;
import java.util.HashSet;
import java.util.Iterator;
import java.io.IOException;

/**
 * This class implements a Rytter parser (or rather, a Rytter recognizer).
 * This implementation is sequential, and no effort has been made to make
 * it efficient, so it is only useful for demonstration purposes.
 * The grammar is read from a file whose name is given as program parameter.
 * The input sentences are read from standard input.
 */
public class Rytter {
    /** Set when printing added Rytter items, for the book. */
    static boolean bookSets = false;

    /** The set of realizable CYK items. */
    private HashSet<CykItem> cykItems;

    /** The set of realizable Rytter items. */
    private HashSet<RytterItem> rytterItems;

    /** The grammar. */
    Grammar grammar;

    /** The length of the input sentence. */
    int inputLen;

    /**
     * Constructs a Rytter recognizer for the specified grammar.
     * @param g the grammar.
     */
    public Rytter(Grammar g) {
        grammar = g;
    }

    /**
     * Prints the set of realizable Rytter items.
     * @param s a header string.
     */
    private void printRytterItems(String s, HashSet<RytterItem> h) {
        RytterItem[] array = h.toArray(new RytterItem[h.size()]);
        Arrays.sort(array);
        System.out.println(s);
        for (int i = 0; i < array.length; i++) {
            System.out.println("    " + array[i]);
        }
    }

    /**
     * Prints the set of realizable CYK items.
     * @param s a header string.
     */
    private void printCykItems(String s, HashSet<CykItem> h) {
        CykItem[] array = h.toArray(new CykItem[h.size()]);
        Arrays.sort(array);
        System.out.println(s);
        for (int i = 0; i < array.length; i++) {
            System.out.println("    " + array[i]);
        }
    }

    /**
     * This is the initialization phase for the Rytter algorithm, and could
     * also be used for a CYK parser. It creates all realizable CYK items
     * of length 1.
     * @param sentence the sentence to recognize.
     */
    private void initialize(Sentence sentence) {
        cykItems = new HashSet<CykItem>();
        rytterItems = new HashSet<RytterItem>();
        inputLen = sentence.length();

        for (Rule r : grammar) {
            if (r.rhs.length == 1) {
                for (int i = 1; i <= inputLen; i++) {
                    Symbol sym = sentence.symbolAt(i);
                    if (sym.equals(r.rhs[0])) {
                        cykItems.add(new CykItem(r.lhs, i-1, i));
                    }
                }
            }
        }
    }

    /**
     * This is the speculative phase of the Rytter algorithm. For all currently
     * realizable CYK items (A, i, j) it adds the following items to the
     * set of Rytter items:
     *    for all rules X -> AY, for all k | j<k<=len:
     *       add (X,i,k;Y,j,k)    (if (Y,j,k) becomes realizable,
     *                             (X,i,k) becomes realizable).
     *    for all rules G -> HA, for all k | 1<k<i:
     *       add (G,k,j;H,k,i)    (if (H,k.i) becomes realizable,
     *                             (G,k,j) becomes realizable).
     * @return <code>true</code> if the set of Rytter items changed.
     */
    private boolean propose() {
        HashSet<RytterItem> proposals = new HashSet<RytterItem>();

        for (CykItem cykItem : cykItems) {
            for (Rule r : grammar) {
                if (r.rhs.length == 2) {
                    if (r.rhs[0].equals(cykItem.nonTerminal)) {
                        for (int k = cykItem.endRange+1; k <= inputLen; k++) {
                            proposals.add(new RytterItem(
                                        new CykItem(r.lhs, cykItem.startRange, k),
                                        new CykItem(r.rhs[1], cykItem.endRange, k)));
                        }
                    }
                    if (r.rhs[1].equals(cykItem.nonTerminal)) {
                        for (int i = 0; i < cykItem.startRange; i++) {
                            proposals.add(new RytterItem(
                                        new CykItem(r.lhs, i, cykItem.endRange),
                                        new CykItem(r.rhs[0], i, cykItem.startRange)));
                        }
                    }
                }
            }
        }
        if (rytterItems.containsAll(proposals)) {
            return false;
        }
        if (bookSets) {
            proposals.removeAll(rytterItems);
            printRytterItems("Proposal phase added:", proposals);
        }
        rytterItems.addAll(proposals);
        return true;
    }

    /**
     * Additional combine after Brent and GoldSchlager.
     */
    private boolean combine1() {
        HashSet<RytterItem> additions = new HashSet<RytterItem>();

        for (CykItem cykItem : cykItems) {
            for (Rule r : grammar) {
                if (r.rhs.length == 2) {
                    if (r.rhs[1].equals(cykItem.nonTerminal)) {
                        for (RytterItem item : rytterItems) {
                            if (item.speculation.nonTerminal.equals(r.rhs[0])
                                    && item.speculation.endRange == cykItem.startRange) {
                                additions.add(new RytterItem(
                                            new CykItem(r.lhs,
                                                item.speculation.startRange,
                                                cykItem.endRange),
                                            item.condition));
                            }
                        }
                    }
                    if (r.rhs[0].equals(cykItem.nonTerminal)) {
                        for (RytterItem item : rytterItems) {
                            if (item.speculation.nonTerminal.equals(r.rhs[1])
                                    && item.speculation.startRange == cykItem.endRange) {
                                additions.add(new RytterItem(
                                            new CykItem(r.lhs,
                                                cykItem.startRange,
                                                item.speculation.endRange),
                                            item.condition));
                            }
                        }
                    }
                }
            }
        }
        if (rytterItems.containsAll(additions)) {
            return false;
        }
        if (bookSets) {
            additions.removeAll(rytterItems);
            printRytterItems("Combine phase 1 added:", additions);
        }
        rytterItems.addAll(additions);
        return true;
    }

    /**
     * This is the combine phase of the Rytter algorithm. If the speculative
     * part of Rytter item R1 is equal to the conditional part of Rytter item
     * R2, we can add a new Rytter item with the conditional part of Rytter item
     * R1 and the speculative part of Rytter item R2.
     * This is pure logic: if A implies B and B implies C, then A implies C.
     * @return <code>true</code> if new Rytter items are added.
     */
    private boolean combine() {
        HashSet<RytterItem> additions = new HashSet<RytterItem>();
        for (RytterItem item1 : rytterItems) {
            for (RytterItem item2 : rytterItems) {
                if (item1.speculation.equals(item2.condition)) {
                    additions.add(new RytterItem(
                                item2.speculation, item1.condition));
                }
            }
        }
        if (rytterItems.containsAll(additions)) {
            return false;
        }
        if (bookSets) {
            additions.removeAll(rytterItems);
            printRytterItems("Combine phase added:", additions);
        }
        rytterItems.addAll(additions);
        return true;
    }

    /**
     * This is the recognize phase of the Rytter algorithm. If the conditional
     * CYK item in a Rytter item is realizable, the speculative CYK item in
     * the Rytter item becomes realizable as well.
     * Logic: if A implies B, and A is true, then B is true.
     * @return <code>true</code> if the set of realizable CYK items grew.
     */
    private boolean recognize() {
        HashSet<CykItem> additions = new HashSet<CykItem>();

        for (RytterItem item : rytterItems) {
            if (cykItems.contains(item.condition)) {
                additions.add(item.speculation);
            }
        }
        if (cykItems.containsAll(additions)) {
            return false;
        }
        if (bookSets) {
            additions.removeAll(cykItems);
            printCykItems("Recognize phase added:", additions);
        }
        cykItems.addAll(additions);
        return true;
    }

    /**
     * Combines CYK items.
     * @return <code>true</code> if the set of realizable CYK items grew.
     */
    private boolean cykRecognize() {
        HashSet<CykItem> additions = new HashSet<CykItem>();

        for (Rule r : grammar) {
            if (r.rhs.length == 2) {
                for (CykItem item1 : cykItems) {
                    if (r.rhs[0].equals(item1.nonTerminal)) {
                        for (CykItem item2 : cykItems) {
                            if (r.rhs[1].equals(item2.nonTerminal)
                                    && item1.endRange == item2.startRange) {
                                additions.add(new CykItem(r.lhs,
                                            item1.startRange, item2.endRange));
                            }
                        }
                    }
                }
            }
        }
        if (cykItems.containsAll(additions)) {
            return false;
        }
        if (bookSets) {
            additions.removeAll(cykItems);
            printCykItems("CYK recognize phase added:", additions);
        }
        cykItems.addAll(additions);
        return true;
    }

    /**
     * Attempts to recognize the specified sentence with the Rytter parser.
     * @param sentence the sentence to be recognized.
     * @return <code>true</code> if the sentence is recognized.
     */
    public boolean recognize(Sentence sentence) {
        initialize(sentence);

        int countIter = 0;
        boolean change = true;
        CykItem target = new CykItem(grammar.startSym, 0, inputLen);

        printCykItems("CYK items after initialize:", cykItems);

        if (cykItems.contains(target)) {
            System.out.println("No iterations needed");
            return true;
        }

        // Iterate until everything is stable.
        while (change) {
            change = false;
            countIter++;
            if (propose()) {
                change = true;
                printRytterItems("Iteration " + countIter
                        + " Rytter items after propose:", rytterItems);
            }
            if (combine1()) {
                change = true;
                printRytterItems("Iteration " + countIter
                        + " Rytter items after combine1:", rytterItems);
            }
            if (combine()) {
                change = true;
                printRytterItems("Iteration " + countIter
                        + " Rytter items after combine:", rytterItems);
            }
            if (cykRecognize()) {
                change = true;
                printCykItems("Iteration " + countIter
                        + " CYK items after CYK recognize:", cykItems);
                // If the set of realizable CYK items contains the target,
                // we are done.
                if (cykItems.contains(target)) {
                    System.out.println("\nIteration " + countIter
                            + ": sentence recognized");
                }
            }
            if (recognize()) {
                change = true;
                printCykItems("Iteration " + countIter
                        + " CYK items after recognize:", cykItems);
                // If the set of realizable CYK items contains the target,
                // we are done.
                if (cykItems.contains(target)) {
                    System.out.println("\nIteration " + countIter
                            + ": sentence recognized");
                }
            }
        }

        System.out.println("No more changes after " + (countIter-1)
                + " iterations");

        return cykItems.contains(target);
    }

    /**
     * Main program. 
     * There is one, optional, argument: the grammar filename.
     * @param args the program arguments.
     */
    public static void main(String[] args) {
        String userGrammarName = "UserGrammar1";
        boolean userGrammarSeen = false;

        for (int i = 0; i < args.length; i++) {
            if (false) {
            } else if (args[i].equals("-book")) {
                bookSets = true;
            } else if (! userGrammarSeen) {
                userGrammarSeen = true;
                userGrammarName = args[i];
            } else {
                System.err.println("Usage: java Rytter [ -book ] [ <grammarfile> ]");
                System.exit(1);
            }
        }

        try {
            Rytter r = new Rytter(new Grammar(userGrammarName));
            for (;;) {
                Sentence input = Sentence.readSentence();
                if (input == null) {
                    break;
                }
                if (r.recognize(input)) {
                    System.out.println("" + input + " recognized!");
                } else {
                    System.out.println("" + input + " NOT recognized!");
                }
            }
        } catch(Throwable e) {
            System.out.println("Got Exception: " + e);
            e.printStackTrace();
            System.exit(1);
        }
    }
}
