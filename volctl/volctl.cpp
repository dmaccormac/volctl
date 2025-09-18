// program: volctl.cpp
// description: command line tool to control system volume on Windows
// author: Dan MacCormac <dmaccormac@gmail.com>
// modified: 2025-09-17

#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <iostream>



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

int main(int argc, char* argv[]) {

    // show help message
    if (argc > 1)
    {
        if (strcmp(argv[1], "/?") == 0 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            std::cout << "volctl (v1.2) - utility to toggle system volume mute\n";
            std::cout << "https://www.github.com/dmaccormac/volctl\n";
            return 0;
        }

    }


	if (toggleMute()) {
		std::cout << "Volume mute toggled successfully." << std::endl;
		return 0;
	}
	else {
		std::cerr << "Failed to toggle volume mute." << std::endl;
		return 1;
	}

}