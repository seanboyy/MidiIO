namespace MIDILibrary
{
    public class VariableLengthValue
    {
        public uint Length { get; private set; }
        public uint Value { get; private set; }
        private const uint VlvMaxIn = 0x0FFFFFFF;
        private const uint VlvMaxVal = 0xFFFFFF7F;

        public VariableLengthValue(uint data)
        {
            ToVariableLength(data);
        }

        public VariableLengthValue(VariableLengthValue other)
        {
            Length = other.Length;
            Value = other.Value;
        }

        public uint ToNumber()
        {
            uint result = 0;
            if (Value > VlvMaxVal) return result;
            byte b1 = 0, b2 = 0, b3 = 0, b4 = 0;
            b4 |= (byte)(Value & 0x0000007F);
            b4 |= (byte)((Value & 0x00000100) >> 1);
            b3 |= (byte)((Value & 0x00007E00) >> 9);
            b3 |= (byte)((Value & 0x00030000) >> 10);
            b2 |= (byte)((Value & 0x007C0000) >> 18);
            b2 |= (byte)((Value & 0x07000000) >> 19);
            b1 |= (byte)((Value & 0x78000000) >> 27);
            result |= b1;
            result <<= 8;
            result |= b2;
            result <<= 8;
            result |= b3;
            result <<= 8;
            result |= b4;
            return result;
        }

        public static uint ToNumber(uint input)
        {
            uint result = 0;
            if (input > VlvMaxVal) return result;
            byte b1 = 0, b2 = 0, b3 = 0, b4 = 0;
            b4 |= (byte)(input & 0x0000007F);
            b4 |= (byte)((input & 0x00000100) >> 1);
            b3 |= (byte)((input & 0x00007E00) >> 9);
            b3 |= (byte)((input & 0x00030000) >> 10);
            b2 |= (byte)((input & 0x007C0000) >> 18);
            b2 |= (byte)((input & 0x07000000) >> 19);
            b1 |= (byte)((input & 0x78000000) >> 27);
            result |= b1;
            result <<= 8;
            result |= b2;
            result <<= 8;
            result |= b3;
            result <<= 8;
            result |= b4;
            return result;
        }

        private void ToVariableLength(uint input)
        {
            Value = 0;
            Length = 0;
            if (input > VlvMaxIn) throw new VariableLengthValueException("Input value too large");
            if (input < 0x00000080)
            {
                Length = 1;
                Value = input;
            }
            else if (input < 0x00004000)
            {
                byte b1 = 0x80;
                byte b2 = 0;
                b2 |= (byte)(input & 0x0000007F);
                b1 |= (byte)((input & 0x00003F80) >> 7);
                Length = 2;
                Value |= b1;
                Value <<= 8;
                Value |= b2;
            }
            else if (input < 0x00200000)
            {
                byte b1 = 0x80;
                byte b2 = 0x80;
                byte b3 = 0;
                b3 |= (byte)(input & 0x0000007F);
                b2 |= (byte)((input & 0x00003F80) >> 7);
                b1 |= (byte)((input & 0x001FC000) >> 14);
                Length = 3;
                Value |= b1;
                Value <<= 8;
                Value |= b2;
                Value <<= 8;
                Value |= b3;
            }
            else if (input < 0x10000000)
            {
                byte b1 = 0x80;
                byte b2 = 0x80;
                byte b3 = 0x80;
                byte b4 = 0;
                b4 |= (byte)(input & 0x0000007F);
                b3 |= (byte)((input & 0x00003F80) >> 7);
                b2 |= (byte)((input & 0x001FC000) >> 14);
                b1 |= (byte)((input & 0x0FE00000) >> 21);
                Length = 4;
                Value |= b1;
                Value <<= 8;
                Value |= b2;
                Value <<= 8;
                Value |= b3;
                Value <<= 8;
                Value |= b4;
            }
        }

        public byte[] ToBitString()
        {
            var bitString = new byte[Length];
            switch (Length)
            {
                case 1:
                    bitString[0] = (byte)Value;
                    break;
                case 2:
                    bitString[0] = (byte)((Value << 16) >> 24);
                    bitString[1] = (byte)((Value << 24) >> 24);
                    break;
                case 3:
                    bitString[0] = (byte)((Value << 8) >> 24);
                    bitString[1] = (byte)((Value << 16) >> 24);
                    bitString[2] = (byte)((Value << 24) >> 24);
                    break;
                case 4:
                    bitString[0] = (byte)(Value >> 24);
                    bitString[1] = (byte)((Value << 8) >> 24);
                    bitString[2] = (byte)((Value << 16) >> 24);
                    bitString[3] = (byte)((Value << 24) >> 24);
                    break;
                default:
                    bitString = null;
                    break;
            }
            return bitString;
        }


    }
}
