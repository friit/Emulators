namespace Emulators.GUI
{
    partial class EmulatorSelect
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.Chip8Button = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // Chip8Button
            // 
            this.Chip8Button.Location = new System.Drawing.Point(12, 87);
            this.Chip8Button.Name = "Chip8Button";
            this.Chip8Button.Size = new System.Drawing.Size(75, 23);
            this.Chip8Button.TabIndex = 0;
            this.Chip8Button.Text = "Chip8";
            this.Chip8Button.UseVisualStyleBackColor = true;
            this.Chip8Button.Click += new System.EventHandler(this.Chip8Button_Click);
            // 
            // EmulatorSelect
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(404, 336);
            this.Controls.Add(this.Chip8Button);
            this.Name = "EmulatorSelect";
            this.Text = "Form1";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button Chip8Button;
    }
}

