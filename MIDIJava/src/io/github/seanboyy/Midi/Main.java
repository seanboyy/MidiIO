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
        Midi midi2 = File.read("O Shenandoah.mid");
        PrintWriter pw = new PrintWriter("jauntyambience.mid.explanation");
        PrintWriter pw2 = new PrintWriter("O Shenandoah.mid.explanation");
        pw.write(midi.toString());
        pw2.write(midi2.toString());
        pw.flush();
        pw.close();
        pw2.flush();
        pw2.close();
        File.write("O Shenandoah2.mid", midi2);
        File.write("jauntyambience2.mid", midi);
    }
}
