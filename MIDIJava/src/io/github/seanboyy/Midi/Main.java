package io.github.seanboyy.Midi;

import java.io.IOException;
import java.io.PrintWriter;

/**
 * Main class for testing.
 * Will be removed in final release.
 */
public class Main {
    /**
     * Application main entry method.
     * @param args list of arguments to pass. Will be empty.
     * @throws IOException if files not able to be io'd.
     */
    public static void main(final String[] args) throws IOException {
        Midi midi = File.read("jauntyambience.mid");
        PrintWriter pw = new PrintWriter("jauntyambience.mid.explanation");
        pw.write(midi.toString());
        pw.flush();
        pw.close();
        File.write("jauntyambience2.mid", midi);
    }
}
