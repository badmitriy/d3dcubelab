using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TemplateApp
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            comboBox1.SelectedIndex = 0;
        }
        bool isDiagonal = true;
        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            isDiagonal = comboBox1.SelectedIndex == 0;
            dxControl1.RenderParameter = isDiagonal ? 1 : 0;
            dxControl1.Invalidate();
        }

        private void Form1_Shown(object sender, EventArgs e)
        {
            dxControl1.EnablePaint = true;
            dxControl1.Invalidate();
        }
    }
}
