using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using MIDILibrary;

namespace MIDIGUI
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            Midi midi = File.Read("jauntyambience.mid");
            Console.WriteLine(Formatter.FormatMidi(midi));
            File.Write("jauntyambience.mid", midi);
        }
    }
}
