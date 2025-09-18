// program: volctl.cpp
// description: command line tool to control system volume on Windows
// author: Dan MacCormac <dmaccormac@gmail.com>
// modified: 2025-09-17

#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <iostream>


// Function to toggle system volume mute
bool toggleMute() {
    HRESULT hr;
    bool result = false;

    // Initialize COM
    hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        return false;
    }

    IMMDeviceEnumerator* pEnumerator = nullptr;
    IMMDevice* pDevice = nullptr;
    IAudioEndpointVolume* pEndpointVolume = nullptr;

    // Get device enumerator
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL,
        __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
    if (FAILED(hr)) {
        CoUninitialize();
        return false;
    }

    // Get default audio endpoint
    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
    if (FAILED(hr)) {
        pEnumerator->Release();
        CoUninitialize();
        return false;
    }

    // Get endpoint volume interface
    hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pEndpointVolume);
    if (FAILED(hr)) {
        pDevice->Release();
        pEnumerator->Release();
        CoUninitialize();
        return false;
    }

    // Get current mute state
    BOOL bMute = FALSE;
    hr = pEndpointVolume->GetMute(&bMute);
    if (FAILED(hr)) {
        pEndpointVolume->Release();
        pDevice->Release();
        pEnumerator->Release();
        CoUninitialize();
        return false;
    }

    // Toggle mute
    hr = pEndpointVolume->SetMute(!bMute, NULL);
    if (SUCCEEDED(hr)) {
        result = true;
    }

    // Cleanup
    pEndpointVolume->Release();
    pDevice->Release();
    pEnumerator->Release();
    CoUninitialize();

    return result;
}

// Function to set system volume (0-100)
bool SetVolume(int n) {
    if (n < 0) n = 0;
    if (n > 100) n = 100;

    HRESULT hr;
    bool result = false;

    hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        return false;
    }

    IMMDeviceEnumerator* pEnumerator = nullptr;
    IMMDevice* pDevice = nullptr;
    IAudioEndpointVolume* pEndpointVolume = nullptr;

    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL,
        __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
    if (FAILED(hr)) {
        CoUninitialize();
        return false;
    }

    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
    if (FAILED(hr)) {
        pEnumerator->Release();
        CoUninitialize();
        return false;
    }

    hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pEndpointVolume);
    if (FAILED(hr)) {
        pDevice->Release();
        pEnumerator->Release();
        CoUninitialize();
        return false;
    }

    // Convert n (0-100) to float (0.0-1.0)
    float volumeLevel = n / 100.0f;
    hr = pEndpointVolume->SetMasterVolumeLevelScalar(volumeLevel, NULL);
    if (SUCCEEDED(hr)) {
        result = true;
    }

    pEndpointVolume->Release();
    pDevice->Release();
    pEnumerator->Release();
    CoUninitialize();

    return result;
}

// Function to check if system volume is muted
bool isMuted() {

	HRESULT hr;
	bool result = false;
	hr = CoInitialize(NULL);
	if (FAILED(hr)) {
		return false;
	}
	IMMDeviceEnumerator* pEnumerator = nullptr;
	IMMDevice* pDevice = nullptr;
	IAudioEndpointVolume* pEndpointVolume = nullptr;
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL,
		__uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
	if (FAILED(hr)) {
		CoUninitialize();
		return false;
	}
	hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
	if (FAILED(hr)) {
		pEnumerator->Release();
		CoUninitialize();
		return false;
	}
	hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pEndpointVolume);
	if (FAILED(hr)) {
		pDevice->Release();
		pEnumerator->Release();
		CoUninitialize();
		return false;
	}
	BOOL bMute = FALSE;
	hr = pEndpointVolume->GetMute(&bMute);
	if (SUCCEEDED(hr)) {
		result = bMute ? true : false;
	}
	pEndpointVolume->Release();
	pDevice->Release();
	pEnumerator->Release();
	CoUninitialize();
	return result;

}

int main(int argc, char* argv[]) {

	//arguments provided
    if (argc > 1)
    {
        //Check for help flag
        if (strcmp(argv[1], "/?") == 0 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            std::cout << "volctl (v1.3) - control system volume\n";
            std::cout << "Usage: volctl [volume]\n";
            std::cout << "If a volume level is provided (e.g., volctl 50), sets the system volume to that level (0-100).\n";
            std::cout << "If no argument is provided, toggles mute on/off\n";
            std::cout << "https://www.github.com/dmaccormac/volctl\n";
            return 0;
        }

        //set volume
        else {

			if (isMuted()) {
				if (!toggleMute()) {
					std::cerr << "Failed to unmute volume." << std::endl;
					return 1;
				}
			}

            int volume = atoi(argv[1]);
            if (SetVolume(volume)) {
                std::cout << "Volume set to " << volume << "%" << std::endl;
                return 0;
            }
            else {
                std::cerr << "Failed to set volume." << std::endl;
                return 1;
            }


        }

    }

	//no arguments, just toggle mute
	if (toggleMute()) {
		std::cout << "Volume mute toggled successfully." << std::endl;
		return 0;
	}
	else {
		std::cerr << "Failed to toggle volume mute." << std::endl;
		return 1;
	}

}