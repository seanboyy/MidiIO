namespace MIDILibrary
{
    public interface IChunk
    {
        byte[] ToBitString();

        uint GetLength();
    }
}
