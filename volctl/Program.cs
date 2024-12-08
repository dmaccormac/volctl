/***************************************************************************
 * volctl.cs - Command Line Volume Control
 * 
 * Description:
 * This C# console application allows users to adjust the system's audio
 * volume through the command line. It accepts a single argument to set the
 * volume level or mute the system. The program supports the following:
 * 
 * 1. **Set Volume**: Pass a number between 0 and 100 to set the system 
 *    volume to the specified percentage.
 * 2. **Mute Volume**: Pass the argument `-1` to mute the system volume.
 * 3. **Unmute Volume**: If the volume was previously muted, setting a volume 
 *    level will unmute it first and then adjust the volume.
 * 4. **Help Message**: If no argument is provided or an invalid argument is 
 *    given, a help message is displayed with usage instructions.
 * 
 * Usage:
 *    volctl <volume-level>
 * 
 * Example Commands:
 *    volctl 50     - Sets the system volume to 50%.
 *    volctl -1     - Toggle the system volume mute.
 *    volctl 30     - Sets the system volume to 30%.
 *    volctl        - Displays the help message if no arguments are passed.
 * 
 * Author: Dan MacCormac
 * Date: 2024-12-08
 * Version: 1.1
 * Website: https://github.com/dmaccormac/volctl
 *  
 ***************************************************************************/


class Program
{
    static void Main(string[] args)
    {
        // Check if we have exactly one argument
        if (args.Length == 1)
        {
            // Try to parse the argument as an integer
            if (int.TryParse(args[0], out int volumeLevel))
            {
                // Call the SetVolume method from the VolumeControl class
                VolumeControl.SetVolume(volumeLevel);
            }
            else
            {
                Console.WriteLine("Invalid input. Please enter a number between 0 and 100 or -1 to mute.");
            }
        }
        else
        {
            // Show help message if no argument is passed or if there are too many arguments
            ShowHelp();
        }

    }

    static void ShowHelp()
    {
        Console.WriteLine("Usage: volctl <volume-level>");
        Console.WriteLine("<volume-level> is a number between 0 and 100, or -1 to toggle mute.");
        Console.WriteLine("Example: volctl 50");
        Console.WriteLine("https://github.com/dmaccormac/volctl");
    }
}
