namespace MIDILibrary
{
    public interface IEvent
    {
        byte[] ToBitString();

        uint GetLength();
    }
}
