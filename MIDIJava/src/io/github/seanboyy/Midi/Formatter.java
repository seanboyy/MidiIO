package io.github.seanboyy.Midi;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

class Formatter{
    private static String formatEvent(Event event){
        if(event instanceof MetaEvent) return formatEvent((MetaEvent)event);
        else if(event instanceof MidiEvent) return formatEvent((MidiEvent)event);
        else if(event instanceof SysexEvent) return formatEvent();
        else return "";
    }

	private static String formatEvent(MetaEvent event){
	    StringBuilder sb = new StringBuilder();
	    ByteBuffer bb = ByteBuffer.allocate(0);
	    switch (event.getMetaEventType()){
            case 0x00:
                sb.append("Sequence Number");
                bb.clear();
                bb = ByteBuffer.allocate(2);
                bb.order(ByteOrder.BIG_ENDIAN);
                bb.put(event.getEventData().get(0));
                bb.put(event.getEventData().get(1));
                sb.append(" ").append(bb.getShort(0)).append("\n");
                break;
            case 0x01:
                sb.append("Text: ");
                for(Byte b : event.getEventData()){
                    sb.append((char)b.byteValue());
                }
                sb.append("\n");
                break;
            case 0x02:
                sb.append("Copyright: ");
                for(Byte b : event.getEventData()){
                    sb.append((char)b.byteValue());
                }
                sb.append("\n");
                break;
            case 0x03:
                sb.append("Track Name: ");
                for(Byte b : event.getEventData()){
                    sb.append((char)b.byteValue());
                }
                sb.append("\n");
                break;
            case 0x04:
                sb.append("Instrument: ");
                for(Byte b : event.getEventData()){
                    sb.append((char)b.byteValue());
                }
                sb.append("\n");
                break;
            case 0x05:
                sb.append("Lyric: ");
                for(Byte b : event.getEventData()){
                    sb.append((char)b.byteValue());
                }
                sb.append("\n");
                break;
            case 0x06:
                sb.append("Mark: ");
                for(Byte b : event.getEventData()){
                    sb.append((char)b.byteValue());
                }
                sb.append("\n");
                break;
            case 0x07:
                sb.append("Cue: ");
                for(Byte b : event.getEventData()){
                    sb.append((char)b.byteValue());
                }
                sb.append("\n");
                break;
            case 0x20:
                sb.append("Mark: ").append(((event.getEventData().get(0) & 0xFF)) + 1).append("\n");
                break;
            case 0x2F:
                sb.append("END OF TRACK\n\n");
                break;
            case 0x51:
                sb.append("Tempo Change: ");
                bb.clear();
                bb = ByteBuffer.allocate(4);
                bb.put((byte)0);
                bb.put(event.getEventData().get(0));
                bb.put(event.getEventData().get(1));
                bb.put(event.getEventData().get(2));
                sb.append((int)(1F / (float)bb.getInt(0) / 60F) * 1e+6).append("\n");
                break;
            case 0x54:
                sb.append("SMTPE Offset: ").append(event.getEventData().get(0) & 0xFF).append(".").append(event.getEventData().get(1) & 0xFF).append(".").append(event.getEventData().get(2) & 0xFF).append(".").append(event.getEventData().get(3) & 0xFF).append(".").append(event.getEventData().get(4) & 0xFF).append("\n");
                break;
            case 0x58:
                sb.append("Time Signature: ").append(event.getEventData().get(0) & 0xFF).append("/").append(0x01 << (event.getEventData().get(1) & 0xFF)).append(", ").append(event.getEventData().get(2) & 0xFF).append(" clocks per tick, ").append(event.getEventData().get(3) & 0xFF).append(" 32nd notes per 24 clocks\n");
                break;
            case 0x59:
                sb.append("Key Signature: ");
                switch(event.getEventData().get(1) & 0xFF){
                    case 0:
                        switch(event.getEventData().get(0)){
                            case -7:
                                sb.append("Cb Major\n");
                                break;
                            case -6:
                                sb.append("Gb Major\n");
                                break;
                            case -5:
                                sb.append("Db Major\n");
                                break;
                            case -4:
                                sb.append("Ab Major\n");
                                break;
                            case -3:
                                sb.append("Eb Major\n");
                                break;
                            case -2:
                                sb.append("Bb Major\n");
                                break;
                            case -1:
                                sb.append("F Major\n");
                                break;
                            case 0:
                                sb.append("C Major\n");
                                break;
                            case 1:
                                sb.append("G Major\n");
                                break;
                            case 2:
                                sb.append("D Major\n");
                                break;
                            case 3:
                                sb.append("A Major\n");
                                break;
                            case 4:
                                sb.append("E Major\n");
                                break;
                            case 5:
                                sb.append("B Major\n");
                                break;
                            case 6:
                                sb.append("F# Major\n");
                                break;
                            case 7:
                                sb.append("C# Major\n");
                                break;
                        }
                        break;
                    case 1:
                        switch(event.getEventData().get(0)) {
                            case -7:
                                sb.append("Ab minor\n");
                                break;
                            case -6:
                                sb.append("Eb minor\n");
                                break;
                            case -5:
                                sb.append("Bb minor\n");
                                break;
                            case -4:
                                sb.append("F minor\n");
                                break;
                            case -3:
                                sb.append("C minor\n");
                                break;
                            case -2:
                                sb.append("G minor\n");
                                break;
                            case -1:
                                sb.append("D minor\n");
                                break;
                            case 0:
                                sb.append("A minor\n");
                                break;
                            case 1:
                                sb.append("E minor\n");
                                break;
                            case 2:
                                sb.append("B minor\n");
                                break;
                            case 3:
                                sb.append("F# minor\n");
                                break;
                            case 4:
                                sb.append("C# minor\n");
                                break;
                            case 5:
                                sb.append("G# minor\n");
                                break;
                            case 6:
                                sb.append("D# minor\n");
                                break;
                            case 7:
                                sb.append("A# minor\n");
                                break;
                        }
                        break;
                }
                break;
            case 0x7F:
                sb.append("Sequencer Specific Event\n");
                break;
        }
	    return sb.toString();
    }

    private static String formatEvent(MidiEvent event){
        return "Midi Event! More specifics in development!\n";
    }

    private static String formatEvent(){
        return "System Exclusive Event\n";
    }

    private static String formatChunk(Chunk chunk){
        if(chunk instanceof HeaderChunk) return formatChunk((HeaderChunk)chunk);
        else if(chunk instanceof TrackChunk) return formatChunk((TrackChunk)chunk);
        else return "";
    }

    private static String formatChunk(HeaderChunk chunk){
        StringBuilder chunkDivision = new StringBuilder();
        short div = chunk.getTickDivision();
        switch ((div & 0x8000) >> 15){
            case 0:
                chunkDivision.append(div & 0x7FFF).append(" ticks per quarter note\n\n");
                break;
            case 1:
                chunkDivision.append(div & 0x7F00).append(" frames per second, ").append(div & 0xFF).append(" ticks per frame\n\n");
                break;
        }
        return "Header Chunk. There are "
                + chunk.getTrackCount()
                + " Track Chunks\n"
                + chunkDivision.toString();
    }

    private static String formatChunk(TrackChunk chunk){
        StringBuilder sb = new StringBuilder();
        for(Event e : chunk.getEvents()){
            sb.append(formatEvent(e));
        }
        return sb.toString();
    }

    static String formatMidi(Midi midi){
        StringBuilder sb = new StringBuilder();
        for(Chunk chunk : midi.getChunks()){
            sb.append(formatChunk(chunk));
        }
        return sb.toString();
    }
}