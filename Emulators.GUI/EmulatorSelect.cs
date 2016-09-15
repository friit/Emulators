using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Emulators.GUI
{
    public partial class EmulatorSelect : Form
    {
        public EmulatorSelect()
        {
            InitializeComponent();
        }

        private void Chip8Button_Click(object sender, EventArgs e)
        {
            string filePath = "";
            using (OpenFileDialog dialog = new OpenFileDialog())
            {
                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    filePath =  Path.GetDirectoryName(Path.GetDirectoryName(System.IO.Directory.GetCurrentDirectory()));
                    
                    ProcessStartInfo startInfo = new ProcessStartInfo(@"Emulators.CHIP8");
                    Process.Start(startInfo);
                }
            }
        }
    }
}
