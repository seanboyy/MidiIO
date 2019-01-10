using System.IO;

namespace MIDILibrary
{
    public class File
    {
        public static Midi Read(string filename)
        {
            var fs = new FileStream(filename, FileMode.Open);
            var len = (int)fs.Length;
            var bits = new byte[len];
            fs.Read(bits, 0, len);
            var midi = new Midi();
            midi.FromBitString(bits, len);
            fs.Close();
            return midi;
        }

        public static void Write(string filename, Midi midi)
        {
            var fs = new FileStream(filename, FileMode.OpenOrCreate);
            fs.Write(midi.ToBitString(), 0, (int)midi.GetLength());
            fs.Flush();
            fs.Close();
        }
    }
}
