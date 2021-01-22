#include <iostream>
#include <Windows.h>

int main(){
	CTL_CODE(0x1337, 0x800, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA);
	HANDLE	hDevice = CreateFileA("\\DosDevices\\kabaite", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	const char *buff = "Hello from userland";
	DeviceIoControl(hDevice, 0x800, &buff, sizeof(buff), NULL, NULL, NULL, NULL);
	CloseHandle(hDevice);
}
