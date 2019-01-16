import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.StreamTokenizer;
import java.util.ArrayList;

/**
 * A sentence consists of an array of symbols, indexed from 1 to the length
 * of the sentence.
 */
public class Sentence {
    /** Buffered input reader. */
    private static BufferedReader in;

    /** Tokenizer for the input. */
    private static StreamTokenizer tokenizer;

    /** Representation of the sentence, an array of symbols. */
    private Symbol[] sentence;

    /**
     * Constructs a sentence from the specified list of symbols.
     * @param l the list of symbols.
     */
    private Sentence(ArrayList<Symbol> l) {
        sentence = l.toArray(new Symbol[l.size()]);
    }

    /**
     * Returns the length of the sentence.
     * @return the length.
     */
    public int length() {
        return sentence.length;
    }

    /**
     * Returns the sentence symbol at the specified position.
     * Note that the underlying representation actually is indexed
     * with 0..n-1 instead of 1..n.
     * @param pos the specified position.
     * @return the symbol at the specified position.
     */
    public Symbol symbolAt(int pos) {
        return sentence[pos-1];
    }

    /**
     * Returns a String representation of the input.
     * @return a String representation of this input.
     */
    public String toString() {
        return Symbol.toString(sentence);
    }

    /**
     * Reads and returns a sentence from the standard input.
     * When there are no more sentences, <code>null</code> is returned.
     * @exception IOException is thrown when an IO error occurs or EOF
     * is seen when it was not expected.
     * @return the sentence read.
     */
    public static Sentence readSentence() throws IOException {
        if (in == null) {
            in = new BufferedReader(new InputStreamReader(System.in));
            tokenizer = new StreamTokenizer(in);
            tokenizer.eolIsSignificant(true);
            tokenizer.lowerCaseMode(false);
            tokenizer.ordinaryChar('"'); tokenizer.ordinaryChar('\'');
        }

        int token;
        ArrayList<Symbol> input = new ArrayList<Symbol>();
        for (;;) {
            token = tokenizer.nextToken();
            if (token == StreamTokenizer.TT_EOF) {
                if (input.size() == 0) {
                    return null;
                }
                throw new IOException("Unexpected EOF");
            }
            if (token == StreamTokenizer.TT_EOL) {
                break;
            }
            if (token == StreamTokenizer.TT_WORD) {
                input.add(new Symbol(tokenizer.sval));
            } else {
                input.add(new Symbol(Character.toString((char) token)));
            }
        }
        return new Sentence(input);
    }
}
