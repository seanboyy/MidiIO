namespace MIDILibrary
{
    public class StreamingHelper
    {
        public static void StreamValueToBitString(ref uint cursor, ref byte[] bitString, uint value)
        {
            for (var i = 0; i < 4; ++i)
            {
                bitString[cursor++] = (byte)((value << (8 * i)) >> (8 * 3));
            }
        }

        public static void StreamValueToBitString(ref uint cursor, ref byte[] bitString, ushort value)
        {
            for (var i = 0; i < 2; ++i)
            {
                bitString[cursor++] = (byte)((value << (8 * i)) >> (8));
            }
        }

        public static void StreamValueToBitString(ref uint cursor, ref byte[] bitString, char value)
        {
            bitString[cursor++] = (byte)value;
        }

        public static void StreamValueToBitString(ref uint cursor, ref byte[] bitString, byte value)
        {
            bitString[cursor++] = value;
        }

        public static void StreamObjectToBitString(ref uint cursor, ref byte[] bitString, VariableLengthValue obj, ulong length)
        {
            var objString = obj.ToBitString();
            for (ulong i = 0; i < length; ++i)
            {
                bitString[cursor++] = objString[(int)i];
            }
        }
        
        public static void StreamObjectToBitString(ref uint cursor, ref byte[] bitString, IChunk obj, ulong length)
        {
            var objString = obj.ToBitString();
            for (ulong i = 0; i < length; ++i)
            {
                bitString[cursor++] = objString[(int)i];
            }
        }


        public static void StreamObjectToBitString(ref uint cursor, ref byte[] bitString, IEvent obj, ulong length)
        {
            var objString = obj.ToBitString();
            for (ulong i = 0; i < length; ++i)
            {
                bitString[cursor++] = objString[(int)i];
            }
        }

        public static byte GetByteFromBitString(ref uint cursor, ref byte[] bitString)
        {
            return bitString[cursor++];
        }

        public static ushort GetShortFromBitString(ref uint cursor, ref byte[] bitString)
        {
            ushort ret = 0x0000;
            ret |= bitString[cursor++];
            ret <<= 8;
            ret |= bitString[cursor++];
            return ret;
        }

        public static uint GetIntFromBitString(ref uint cursor, ref byte[] bitString)
        {
            uint ret = 0x00000000;
            ret |= bitString[cursor++];
            ret <<= 8;
            ret |= bitString[cursor++];
            ret <<= 8;
            ret |= bitString[cursor++];
            ret <<= 8;
            ret |= bitString[cursor++];
            return ret;
        }

        public static VariableLengthValue GetVariableLengthValueFromBitString(ref uint cursor, ref byte[] bitString)
        {
            uint length = 1;
            for (uint i = 0; i < length; ++i)
            {
                var tempByte = GetByteFromBitString(ref cursor, ref bitString);
                if ((tempByte & 0x80) == 0x80) length++;
            }
            cursor -= length;
            uint val = 0x00000000;
            for(uint i = 0; i < length; ++i)
            {
                val |= GetByteFromBitString(ref cursor, ref bitString);
                if(i + 1 < length) val <<= 8;
            }
            return new VariableLengthValue(VariableLengthValue.ToNumber(val));
        }
    }
}
