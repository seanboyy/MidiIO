using System.Collections.Generic;

namespace MIDILibrary
{
    public class Midi
    {
        public List<IChunk> Chunks { get; }

        public Midi()
        {
            Chunks = new List<IChunk>();
        }

        public Midi(IEnumerable<IChunk> chunks)
        {
            Chunks = new List<IChunk>();
            Chunks.AddRange(chunks);
        }

        public byte[] ToBitString()
        {
            var bitString = new byte[GetLength()];
            uint cursor = 0;
            foreach (var chunk in Chunks)
            {
                StreamingHelper.StreamObjectToBitString(ref cursor, ref bitString, chunk, chunk.GetLength());
            }
            return bitString;
        }

        public void FromBitString(byte[] bitString, int fileSize)
        {
            uint cursor = 0;
            var signature = StreamingHelper.GetIntFromBitString(ref cursor, ref bitString);
            if (signature != MidiConstants.MIDI_SIGNATURE) throw new MidiException("Unexpected values in signature. Expected MThd (0x4D546864)");
            var length = StreamingHelper.GetIntFromBitString(ref cursor, ref bitString);
            var format = StreamingHelper.GetShortFromBitString(ref cursor, ref bitString);
            var tracks = StreamingHelper.GetShortFromBitString(ref cursor, ref bitString);
            var division = StreamingHelper.GetShortFromBitString(ref cursor, ref bitString);
            Chunks.Add(new HeaderChunk(length, format, tracks, division));
            while(cursor < fileSize)
            {
                signature = StreamingHelper.GetIntFromBitString(ref cursor, ref bitString);
                if (signature != MidiConstants.TRACK_SIGNATURE) throw new MidiException("Unexpected values in signature. Expected MTrk (0x4D54726B)");
                length = StreamingHelper.GetIntFromBitString(ref cursor, ref bitString);
                var endOfTrack = length + cursor;
                var events = new List<IEvent>();
                while(cursor < endOfTrack)
                {
                    var deltaTime = StreamingHelper.GetVariableLengthValueFromBitString(ref cursor, ref bitString);
                    var eventType = StreamingHelper.GetByteFromBitString(ref cursor, ref bitString);
                    if(eventType == MidiConstants.META_EVENT)
                    {
                        var metaEventType = StreamingHelper.GetByteFromBitString(ref cursor, ref bitString);
                        if (metaEventType == MidiConstants.END_OF_TRACK)
                            if (cursor + 1 != endOfTrack) throw new MidiException("Unexpected End of Track event");
                        var eventLength = StreamingHelper.GetVariableLengthValueFromBitString(ref cursor, ref bitString);
                        var eventData = new List<byte>();
                        if(eventLength.ToNumber() != 0)
                            for(uint i = 0; i < eventLength.ToNumber(); ++i)
                            {
                                eventData.Add(StreamingHelper.GetByteFromBitString(ref cursor, ref bitString));
                            }
                        events.Add(new MetaEvent(deltaTime, metaEventType, eventLength, eventData));
                    }
                    else if(eventType < MidiConstants.META_EVENT && eventType >= MidiConstants.SYSEX_EVENT)
                    {
                        var eventLength = StreamingHelper.GetVariableLengthValueFromBitString(ref cursor, ref bitString);
                        var eventData = new List<byte>();
                        if(eventLength.ToNumber() != 0)
                            for(uint i = 0; i < eventLength.ToNumber(); ++i)
                            {
                                eventData.Add(StreamingHelper.GetByteFromBitString(ref cursor, ref bitString));
                            }
                        events.Add(new SysexEvent(deltaTime, eventType, eventLength, eventData));
                    }
                    else if(eventType < MidiConstants.SYSEX_EVENT && eventType >= MidiConstants.NOTE_OFF)
                    {
                        var eventData = new List<byte>();
                        if(eventType < MidiConstants.PROGRAM || eventType >= MidiConstants.PITCH)
                        {
                            eventData.Add(StreamingHelper.GetByteFromBitString(ref cursor, ref bitString));
                            eventData.Add(StreamingHelper.GetByteFromBitString(ref cursor, ref bitString));
                        }
                        else
                            eventData.Add(StreamingHelper.GetByteFromBitString(ref cursor, ref bitString));
                        events.Add(new MidiEvent(deltaTime, eventType, eventData));
                    }
                }
                Chunks.Add(new TrackChunk(length, events));
            }
        }

        public uint GetLength()
        {
            uint length = 0;
            foreach(var chunk in Chunks)
            {
                length += chunk.GetLength();
            }
            return length;
        }
    }
}
