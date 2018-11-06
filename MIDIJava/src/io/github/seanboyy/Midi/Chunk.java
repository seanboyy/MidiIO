package io.github.seanboyy.Midi;

/**
 * Interface Chunk.
 */
public interface Chunk {
    /**
     * Convert the chunk to binary data.
     * Usually for writing to a Midi file.
     * @return A byte array of chunk data
     */
    byte[] toBitString();

    /**
     * Get the length of the chunk.
     * This includes the size of the value for length.
     * @return The length of the chunk
     */
    int getLength();
}
