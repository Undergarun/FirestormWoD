namespace OpcodeTools
{
	partial class Form1
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
            this.opcodeDecBox = new System.Windows.Forms.TextBox();
            this.offsetBox = new System.Windows.Forms.TextBox();
            this.opcodeHexBox = new System.Windows.Forms.TextBox();
            this.cryptedBox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.specialBox = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.authBox = new System.Windows.Forms.TextBox();
            this.Versions = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.spellBox = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // opcodeDecBox
            // 
            this.opcodeDecBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.opcodeDecBox.Location = new System.Drawing.Point(28, 140);
            this.opcodeDecBox.Name = "opcodeDecBox";
            this.opcodeDecBox.Size = new System.Drawing.Size(51, 20);
            this.opcodeDecBox.TabIndex = 1;
            this.opcodeDecBox.TextChanged += new System.EventHandler(this.OpcodeDecBox_TextChanged);
            // 
            // offsetBox
            // 
            this.offsetBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.offsetBox.Location = new System.Drawing.Point(139, 101);
            this.offsetBox.Name = "offsetBox";
            this.offsetBox.Size = new System.Drawing.Size(46, 20);
            this.offsetBox.TabIndex = 3;
            this.offsetBox.TextChanged += new System.EventHandler(this.offsetBox_TextChanged);
            // 
            // opcodeHexBox
            // 
            this.opcodeHexBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.opcodeHexBox.Location = new System.Drawing.Point(29, 104);
            this.opcodeHexBox.Name = "opcodeHexBox";
            this.opcodeHexBox.Size = new System.Drawing.Size(50, 20);
            this.opcodeHexBox.TabIndex = 0;
            this.opcodeHexBox.TextChanged += new System.EventHandler(this.OpcodeHex_TextChanged);
            // 
            // cryptedBox
            // 
            this.cryptedBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.cryptedBox.Location = new System.Drawing.Point(139, 65);
            this.cryptedBox.Name = "cryptedBox";
            this.cryptedBox.Size = new System.Drawing.Size(46, 20);
            this.cryptedBox.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(136, 49);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(56, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Converted";
            // 
            // label2
            // 
            this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(136, 85);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(56, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Offset dec";
            // 
            // label3
            // 
            this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(25, 124);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(66, 13);
            this.label3.TabIndex = 6;
            this.label3.Text = "Opcode dec";
            // 
            // label4
            // 
            this.label4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(26, 88);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(65, 13);
            this.label4.TabIndex = 7;
            this.label4.Text = "Opcode hex";
            // 
            // label5
            // 
            this.label5.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(136, 122);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(87, 13);
            this.label5.TabIndex = 9;
            this.label5.Text = "Special (jam) hex";
            this.label5.Click += new System.EventHandler(this.label5_Click);
            // 
            // specialBox
            // 
            this.specialBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.specialBox.Location = new System.Drawing.Point(139, 138);
            this.specialBox.Name = "specialBox";
            this.specialBox.Size = new System.Drawing.Size(46, 20);
            this.specialBox.TabIndex = 4;
            this.specialBox.TextChanged += new System.EventHandler(this.specialBox_TextChanged);
            // 
            // label9
            // 
            this.label9.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(97, 117);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(25, 13);
            this.label9.TabIndex = 15;
            this.label9.Text = "<=>";
            // 
            // label11
            // 
            this.label11.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(136, 161);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(50, 13);
            this.label11.TabIndex = 19;
            this.label11.Text = "Auth dec";
            // 
            // authBox
            // 
            this.authBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.authBox.Location = new System.Drawing.Point(139, 177);
            this.authBox.Name = "authBox";
            this.authBox.Size = new System.Drawing.Size(46, 20);
            this.authBox.TabIndex = 5;
            this.authBox.TextChanged += new System.EventHandler(this.authBox_TextChanged);
            // 
            // Versions
            // 
            this.Versions.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.Versions.FormattingEnabled = true;
            this.Versions.Location = new System.Drawing.Point(40, 4);
            this.Versions.Name = "Versions";
            this.Versions.Size = new System.Drawing.Size(142, 21);
            this.Versions.Sorted = true;
            this.Versions.TabIndex = 6;
            this.Versions.SelectedIndexChanged += new System.EventHandler(this.Versions_SelectedIndexChanged);
            // 
            // label6
            // 
            this.label6.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(238, 49);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(113, 13);
            this.label6.TabIndex = 21;
            this.label6.Text = "Spell Special (jam) hex";
            // 
            // textBox1
            // 
            this.spellBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.spellBox.Location = new System.Drawing.Point(241, 65);
            this.spellBox.Name = "spellBox";
            this.spellBox.Size = new System.Drawing.Size(46, 20);
            this.spellBox.TabIndex = 20;
            this.spellBox.TextChanged += new System.EventHandler(this.spellBox_TextChanged);

            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(503, 211);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.spellBox);
            this.Controls.Add(this.Versions);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.authBox);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.specialBox);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.cryptedBox);
            this.Controls.Add(this.opcodeHexBox);
            this.Controls.Add(this.offsetBox);
            this.Controls.Add(this.opcodeDecBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Name = "Form1";
            this.Text = "OpcodeTools";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.TextBox opcodeDecBox;
		private System.Windows.Forms.TextBox offsetBox;
		private System.Windows.Forms.TextBox opcodeHexBox;
		private System.Windows.Forms.TextBox cryptedBox;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox specialBox;
        private System.Windows.Forms.Label label9;
		private System.Windows.Forms.Label label11;
		private System.Windows.Forms.TextBox authBox;
        private System.Windows.Forms.ComboBox Versions;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox spellBox;
	}
}

