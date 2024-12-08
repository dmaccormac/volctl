using NAudio.CoreAudioApi;

public class VolumeControl
{
    public static void SetVolume(int n)
    {
        // Check for mute case
        if (n == -1)
        {
            // Mute the volume (set it to 0%)
            SetMute(true);
            Console.WriteLine("Volume muted.");
            return;
        }

        // Validate the input for volume level between 0 and 100
        if (n < 0 || n > 100)
        {
            Console.WriteLine("Volume must be between 0 and 100.");
            return;
        }

        try
        {
            // Get the default audio playback device
            using (var enumerator = new MMDeviceEnumerator())
            {
                var device = enumerator.GetDefaultAudioEndpoint(DataFlow.Render, Role.Multimedia);

                // If the device is muted, unmute it before adjusting volume
                if (device.AudioEndpointVolume.Mute)
                {
                    SetMute(false);
                    Console.WriteLine("Volume unmuted.");
                }

                // Convert the percentage to a scalar value (0.0f to 1.0f)
                float volumeLevel = n / 100.0f;

                // Set the volume level
                device.AudioEndpointVolume.MasterVolumeLevelScalar = volumeLevel;
                Console.WriteLine($"Volume set to {n}%");
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error setting volume: {ex.Message}");
        }
    }

    // Helper method to mute or unmute the volume
    private static void SetMute(bool mute)
    {
        try
        {
            using (var enumerator = new MMDeviceEnumerator())
            {
                var device = enumerator.GetDefaultAudioEndpoint(DataFlow.Render, Role.Multimedia);
                device.AudioEndpointVolume.Mute = mute;
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error muting/unmuting volume: {ex.Message}");
        }
    }
}
