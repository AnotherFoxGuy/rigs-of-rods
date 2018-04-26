using System;
using System.ComponentModel;
using System.IO;
using System.IO.Compression;
using System.Net;
using System.Windows.Forms;
using WixSharp;
using WixSharp.UI.Forms;

namespace RoRSetup
{
	public partial class DownloadContentPackDialog : ManagedForm, IManagedDialog
	{
		private WebClient client;
		private string fileName;

		public DownloadContentPackDialog()
		{
			//NOTE: If this assembly is compiled for v4.0.30319 runtime, it may not be compatible with the MSI hosted CLR.
			//The incompatibility is particularly possible for the Embedded UI scenarios. 
			//The safest way to avoid the problem is to compile the assembly for v3.5 Target Framework.WixSharp Setup
			InitializeComponent();
			fileName = $"{Path.GetTempPath()}RoRcontentpack.zip";
		}

		void dialog_Load(object sender, EventArgs e)
		{
			banner.Image = MsiRuntime.Session.GetEmbeddedBitmap("WixUI_Bmp_Banner");
			Text = "[ProductName] Setup";

			//resolve all Control.Text cases with embedded MSI properties (e.g. 'ProductName') and *.wxl file entries
			base.Localize();
		}

		private void DownloadContentButton_Click(object sender, EventArgs e)
		{
			next.Enabled = false;
			back.Enabled = false;
			client = new WebClient();
			client.DownloadFileAsync(new Uri("http://dl.rigsofrods.org/pack_contentpack04.zip"), fileName);
			client.DownloadProgressChanged += UpdateDlProgress;
			client.DownloadFileCompleted += ExtractZip;
		}

		private void ExtractZip(object sender, AsyncCompletedEventArgs e)
		{
			if(e.Error == null)
			{
				ZipFile.ExtractToDirectory(fileName, 
					$@"{Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments)}\Rigs of Rods 0.4\packs");
				MessageBox.Show("Content pack installed");
				DownloadContentButton.Enabled = false;
			}
			else
				MessageBox.Show(e.Error.Message);

			next.Enabled = true;
			back.Enabled = true;
		}

		private void UpdateDlProgress(object sender, DownloadProgressChangedEventArgs e)
		{
			progressBar1.Value = e.ProgressPercentage;
		}
		

		void back_Click(object sender, EventArgs e)
		{
			Shell.GoPrev();
		}

		void next_Click(object sender, EventArgs e)
		{
			Shell.GoNext();
		}

		void cancel_Click(object sender, EventArgs e)
		{
			if(client.IsBusy)
			{
				client.CancelAsync();
				next.Enabled = true;
				back.Enabled = true;
			}
				
			else
				Shell.Cancel();
		}

		
	}
}