# volctl

## Description

This C# console application allows users to adjust the system's audio volume through the command line. It accepts a single argument to set the volume level or mute the system. The program supports the following:

1. **Set Volume**: Pass a number between 0 and 100 to set the system volume to the specified percentage.
2. **Mute Volume**: Pass the argument `-1` to toggle the system volume mute.
3. **Unmute Volume**: If the volume was previously muted, setting a volume level will unmute it first and then adjust the volume.
4. **Help Message**: If no argument is provided or an invalid argument is given, a help message is displayed with usage instructions.

## Usage

```sh
volctl <volume-level>
volctl 50     # Sets the system volume to 50%.
volctl -1     # Toggles the system volume mute.
volctl 30     # Sets the system volume to 30%.
volctl        # Displays the help message if no arguments are passed.
```

## Author
Dan MacCormac

## Website
https://github.com/dmaccormac

## Version
1.1

## Date
2024-12-08
