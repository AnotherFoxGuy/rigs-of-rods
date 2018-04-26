using System;
using System.Diagnostics;
using System.Windows.Forms;
using WixSharp;
using WixSharp.Forms;
using RoRSetup;

namespace RoRInstaller
{
    class Program
    {
        static void Main()
        {
            // DON'T FORGET to add NuGet package "WixSharp".
            //new File(@"RoRRelease\Program.cs")))

            var project = new ManagedProject("Rigs of Rods",
				new InstallDir(@"%ProgramFiles%\Rigs of Rods",	new Files(@"RoRRelease\*")),
				new Dir(@"%PersonalFolder%\Rigs of Rods 0.4\",	new Files(@"Skeleton\*"))
			)
            {
                GUID = new Guid("6fe30b47-2577-43ad-9095-1861ba25889b"),

                //project.ManagedUI = ManagedUI.Empty;    //no standard UI dialogs
                //project.ManagedUI = ManagedUI.Default;  //all standard UI dialogs

                //custom set of standard UI dialogs
                ManagedUI = new ManagedUI()
            };

			project.LicenceFile = "COPYING.rtf";
			project.Version = new Version(0,4,7);
			project.BackgroundImage = @"Icons\RoRSetupLarge.bmp";
			project.OutFileName = "RoR_0.4.7.0_Setup";


			project.ControlPanelInfo.Readme = "https://github.com/RigsOfRods/rigs-of-rods";
            project.ControlPanelInfo.HelpLink = "https://forum.rigsofrods.org/support/";
            //project.ControlPanelInfo.ProductIcon = "app_icon.ico";
            project.ControlPanelInfo.Contact = "AnotherFoxGuy";
            project.ControlPanelInfo.Manufacturer = "Rigs of Rods Team";
            project.ControlPanelInfo.InstallLocation = "[INSTALLDIR]";

            project.ManagedUI.InstallDialogs.Add(Dialogs.Welcome)
				.Add(Dialogs.Licence)
                .Add(Dialogs.InstallDir)
                .Add(Dialogs.Progress)
				.Add<DownloadContentPackDialog>()
				.Add(Dialogs.Exit);

            project.ManagedUI.ModifyDialogs.Add(Dialogs.MaintenanceType)
                .Add(Dialogs.Progress)
                .Add(Dialogs.Exit);

            project.Load += Msi_Load;
            project.BeforeInstall += Msi_BeforeInstall;
            project.AfterInstall += Msi_AfterInstall;

            //project.SourceBaseDir = "<input dir path>";
            //project.OutDir = "<output dir path>";

            ValidateAssemblyCompatibility();

            project.BuildMsi();
        }

        static void Msi_Load(SetupEventArgs e)
        {
            //if (!e.IsUninstalling)
            //	MessageBox.Show(e.ToString(), "Load");
        }

        static void Msi_BeforeInstall(SetupEventArgs e)
        {
            //if (!e.IsUninstalling)
            //	MessageBox.Show(e.ToString(), "BeforeInstall");
        }

        static void Msi_AfterInstall(SetupEventArgs e)
        {
            //if (!e.IsUISupressed && !e.IsUninstalling)
            //	MessageBox.Show(e.ToString(), "AfterExecute");
        }

        static void ValidateAssemblyCompatibility()
        {
            var assembly = System.Reflection.Assembly.GetExecutingAssembly();

            if (!assembly.ImageRuntimeVersion.StartsWith("v2."))
            {
                Console.WriteLine(
                    "Warning: assembly '{0}' is compiled for {1} runtime, which may not be compatible with the CLR version hosted by MSI. " +
                    "The incompatibility is particularly possible for the EmbeddedUI scenarios. " +
                    "The safest way to solve the problem is to compile the assembly for v3.5 Target Framework.",
                    assembly.GetName().Name, assembly.ImageRuntimeVersion);
            }
        }
    }
}