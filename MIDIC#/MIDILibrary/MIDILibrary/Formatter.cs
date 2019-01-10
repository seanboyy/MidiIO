namespace MIDILibrary
{
    public class Formatter
    {
        private static string FormatEvent(IEvent _event){
            if (_event is MetaEvent) return FormatEvent((MetaEvent)_event);
            else if (_event is MidiEvent) return FormatEvent((MidiEvent)_event);
            else if (_event is SysexEvent) return FormatEvent();
            else return "";
        }

        private static string FormatEvent(MetaEvent _event)
        {
            string ret = "";
            ret += "After " + _event.TimeDelta.ToNumber() + " ticks, ";
            switch (_event.MetaEventType)
            {
                case 0x00:
                    ret += "Sequence Number ";
                    ushort temp = 0;
                    temp |= _event.EventData[0];
                    temp <<= 8;
                    temp |= _event.EventData[1];
                    ret += temp + "\n";
                    break;
                case 0x01:
                    ret += "Text: ";
                    foreach (byte b in _event.EventData)
                    {
                        ret += (char)b;
                    }
                    ret += "\n";
                    break;
                case 0x02:
                    ret += "Copyright: ";
                    foreach (byte b in _event.EventData)
                    {
                        ret += (char)b;
                    }
                    ret += "\n";
                    break;
                case 0x03:
                    ret += "Track Name: ";
                    foreach (byte b in _event.EventData)
                    {
                        ret += (char)b;
                    }
                    ret += "\n";
                    break;
                case 0x04:
                    ret += "Instrument: ";
                    foreach (byte b in _event.EventData)
                    {
                        ret += (char)b;
                    }
                    ret += "\n";
                    break;
                case 0x05:
                    ret += "Lyric: ";
                    foreach (byte b in _event.EventData)
                    {
                        ret += (char)b;
                    }
                    ret += "\n";
                    break;
                case 0x06:
                    ret += "Mark: ";
                    foreach (byte b in _event.EventData)
                    {
                        ret += (char)b;
                    }
                    ret += "\n";
                    break;
                case 0x07:
                    ret += "Cue: ";
                    foreach (byte b in _event.EventData)
                    {
                        ret += (char)b;
                    }
                    ret += "\n";
                    break;
                case 0x20:
                    ret += "Mark: " + (_event.EventData[0] + 1) + "\n";
                    break;
                case 0x21:
                    ret += "A thing! " + ((_event.EventData[0] & 0x0F) + 1) + "\n";
                    break;
                case 0x2F:
                    ret += "END OF TRACK\n\n";
                    break;
                case 0x51:
                    ret += "Tempo Change: ";
                    uint _temp = 0;
                    _temp |= _event.EventData[0];
                    _temp <<= 8;
                    _temp |= _event.EventData[1];
                    _temp <<= 8;
                    _temp |= _event.EventData[2];
                    ret += ((int)(1F / _temp / 60F) * 1e+6) + "\n";
                    break;
                case 0x54:
                    ret += "SMTPE Offset: " +
                        _event.EventData[0] + "." +
                        _event.EventData[1] + "." +
                        _event.EventData[2] + "." +
                        _event.EventData[3] + "." +
                        _event.EventData[4] + "\n";
                    break;
                case 0x58:
                    ret += "Time Signature: " +
                        _event.EventData[0] + "/" +
                        (0x01 << _event.EventData[1]) + ", " +
                        _event.EventData[2] + "clocks per tick, " +
                        _event.EventData[3] + " 32nd notes per 24 clocks\n";
                    break;
                case 0x59:
                    ret += "Key Signature: ";
                    switch (_event.EventData[1])
                    {
                        case 0:
                            switch ((sbyte)_event.EventData[0])
                            {
                                case -7:
                                    ret += "Cb Major\n";
                                    break;
                                case -6:
                                    ret += "Gb Major\n";
                                    break;
                                case -5:
                                    ret += "Db Major\n";
                                    break;
                                case -4:
                                    ret += "Ab Major\n";
                                    break;
                                case -3:
                                    ret += "Eb Major\n";
                                    break;
                                case -2:
                                    ret += "Bb Major\n";
                                    break;
                                case -1:
                                    ret += "F Major\n";
                                    break;
                                case 0:
                                    ret += "C Major\n";
                                    break;
                                case 1:
                                    ret += "G Major\n";
                                    break;
                                case 2:
                                    ret += "D Major\n";
                                    break;
                                case 3:
                                    ret += "A Major\n";
                                    break;
                                case 4:
                                    ret += "E Major\n";
                                    break;
                                case 5:
                                    ret += "B Major\n";
                                    break;
                                case 6:
                                    ret += "F# Major\n";
                                    break;
                                case 7:
                                    ret += "C# Major\n";
                                    break;
                            }
                            break;
                        case 1:
                            switch ((sbyte)_event.EventData[0])
                            {
                                case -7:
                                    ret += "Ab minor\n";
                                    break;
                                case -6:
                                    ret += "Eb minor\n";
                                    break;
                                case -5:
                                    ret += "Bb minor\n";
                                    break;
                                case -4:
                                    ret += "F minor\n";
                                    break;
                                case -3:
                                    ret += "C minor\n";
                                    break;
                                case -2:
                                    ret += "G minor\n";
                                    break;
                                case -1:
                                    ret += "D minor\n";
                                    break;
                                case 0:
                                    ret += "A minor\n";
                                    break;
                                case 1:
                                    ret += "E minor\n";
                                    break;
                                case 2:
                                    ret += "B minor\n";
                                    break;
                                case 3:
                                    ret += "F# minor\n";
                                    break;
                                case 4:
                                    ret += "C# minor\n";
                                    break;
                                case 5:
                                    ret += "G# minor\n";
                                    break;
                                case 6:
                                    ret += "D# minor\n";
                                    break;
                                case 7:
                                    ret += "A# minor\n";
                                    break;
                            }
                            break;
                    }
                    break;
                case 0x7F:
                    ret += "sequencer specific event\n";
                    break;
            }
            return ret;
        }

        private static string FormatEvent(MidiEvent _event)
        {
            string ret = "";
            ret += "After " + _event.TimeDelta.ToNumber() + " ticks, ";
            switch(_event.EventType & 0xF0)
            {
                case 0x80:
                    ret += "Note off in channel " +
                        ((_event.EventType & 0x0F) + 1) + ": ";
                    string note = "";
                    MidiConstants.NOTE_MAP.TryGetValue(_event.EventData[0], out note);
                    ret += note + " at velocity " +
                        (sbyte)_event.EventData[1] + "\n";
                    break;
                case 0x90:
                    if ((sbyte)_event.EventData[1] > 0x00)
                        ret += "Note on in channel ";
                    else
                        ret += "Note off in channel ";
                    ret += ((_event.EventType & 0x0F) + 1) + ": ";
                    note = "";
                    MidiConstants.NOTE_MAP.TryGetValue(_event.EventData[0], out note);
                    ret += note;
                    if ((sbyte)_event.EventData[1] > 0x00)
                        ret += " at velocity " + (sbyte)_event.EventData[1];
                    ret += "\n";
                    break;
                case 0xA0:
                    ret += "Polyphonic key pressure in channel " +
                        ((_event.EventType & 0x0F) + 1) + ": ";
                    note = "";
                    MidiConstants.NOTE_MAP.TryGetValue(_event.EventData[0], out note);
                    ret += note + " at pressure" +
                        (sbyte)_event.EventData[1] + "\n";
                    break;
                case 0xB0:
                    ret += "Controller change in channel " +
                        ((_event.EventType & 0x0F) + 1) + ": ";
                    switch (_event.EventData[0])
                    {
                        case 0x78:
                            ret += " All sound off";
                            break;
                        case 0x79:
                            ret += "Reset all controllers";
                            break;
                        case 0x7A:
                            if (_event.EventData[1] == 0x00)
                                ret += "Disconnect local keyboard";
                            else
                                ret += "Reconnect local keyboard";
                            break;
                        case 0x7B:
                            ret += "All notes off";
                            break;
                        case 0x7C:
                            ret += "Omni mode off";
                            break;
                        case 0x7D:
                            ret += "Omni mode on";
                            break;
                        case 0x7E:
                            ret += "Mono mode on";
                            break;
                        case 0x7F:
                            ret += "Poly mode on";
                            break;
                    }
                    ret += "\n";
                    break;
                case 0xC0:
                    ret += "Program change in channel " +
                        ((_event.EventType & 0x0F) + 1) + ": ";
                    string instrument = "";
                    MidiConstants.INSTRUMENT_MAP.TryGetValue(_event.EventData[0], out instrument);
                    ret += instrument + "\n";
                    break;
                case 0xD0:
                    ret += "Channel key pressure in channel " +
                        ((_event.EventType & 0x0F) + 1) + "\n";
                    break;
                case 0xE0:
                    ret += "Pitch bend in channel " +
                        ((_event.EventType & 0x0F) + 1) + ": ";
                    ushort bendAmt = 0;
                    bendAmt |= _event.EventData[1];
                    bendAmt <<= 7;
                    bendAmt |= _event.EventData[0];
                    ret += 0x2000 - bendAmt + "\n";
                    break;
            }
            return ret;
        }

        private static string FormatEvent()
        {
            return "System Exclusive Event\n";
        }

        private static string FormatChunk(IChunk chunk)
        {
            if (chunk is HeaderChunk) return FormatChunk((HeaderChunk)chunk);
            else if (chunk is TrackChunk) return FormatChunk((TrackChunk)chunk);
            else return "";
        }

        private static string FormatChunk(HeaderChunk chunk)
        {
            string chunkDivision = "";
            ushort div = chunk.TickDivision;
            switch((div & 0x8000) >> 15)
            {
                case 0:
                    chunkDivision += (div & 0x7FFF) + " ticks per quarter note\n\n";
                    break;
                case 1:
                    chunkDivision += (div & 0x7F00) +
                        " frames per second, " + (div & 0xFF) +
                        " ticks per frame\n\n";
                    break;
            }
            return "Header Chunk. There are " +
                chunk.TrackCount + "Track Chunks\n" +
                chunkDivision;
        }

        private static string FormatChunk(TrackChunk chunk)
        {
            string ret = "";
            foreach (IEvent e in chunk.Events)
            {
                ret += FormatEvent(e);
            }
            return ret;
        }

        public static string FormatMidi(Midi midi)
        {
            string ret = "";
            foreach (IChunk chunk in midi.Chunks)
            {
                ret += FormatChunk(chunk);
            }
            return ret;
        }
    }
}
