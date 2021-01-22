#include "Driver.h"
	
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING regPath) {
	UNREFERENCED_PARAMETER(regPath);
	CTL_CODE(0x1337, 0x800, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA);
	UNICODE_STRING symbolic, devicename;
	RtlInitUnicodeString(&symbolic, L"\\Device\\kabaite");
	RtlInitUnicodeString(&symbolic, L"\\DosDevices\\kabaite");
	IoCreateDevice(pDriver, NULL, &devicename, FILE_DEVICE_NULL, FILE_DEVICE_SECURE_OPEN, FALSE, &Device);
	IoCreateSymbolicLink(&symbolic, &devicename);
	pDriver->DriverUnload = DriverUnload;
	pDriver->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
	return STATUS_SUCCESS;
}

void DriverUnload(PDRIVER_OBJECT pDriver) {
	UNICODE_STRING symbolic;
	RtlInitUnicodeString(&symbolic, L"\\DosDevices\\kabaite");
	IoDeleteDevice(&Device);
	IoDeleteSymbolicLink(&symbolic);
	UNREFERENCED_PARAMETER(pDriver);
}

NTSTATUS DispatchCreate(PDRIVER_OBJECT pDriver, IRP* irp) {
	IO_STACK_LOCATION	*pIOLoc = IoGetCurrentIrpStackLocation(irp);
	UNICODE_STRING		path;
	OBJECT_ATTRIBUTES	objAtr;
	IO_STATUS_BLOCK		IoStatus;
	PHANDLE				hFile;
	NTSTATUS			status;
	RtlInitUnicodeString(&path, L"\\DosDevices\\C:\\teste.txt");
	InitializeObjectAttributes(&objAtr, &path, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, NULL);
	status = ZwCreateFile(hFile, GENERIC_ALL, &objAtr, &IoStatus, NULL, pIOLoc->Parameters.Create.FileAttributes, pIOLoc->Parameters.Create.ShareAccess,FILE_CREATE, FILE_NON_DIRECTORY_FILE, NULL, pIOLoc->Parameters.Create.EaLength);
	if (FAILED(status)) {
		return status;
	}
	status = ZwWriteFile(hFile, NULL, NULL, NULL, &IoStatus, irp->AssociatedIrp.SystemBuffer, pIOLoc->Parameters.DeviceIoControl.InputBufferLength, NULL, NULL);
	if (FAILED(status)) {
		return status;
	}
	ObCloseHandle(hFile, KernelMode);
	IoCompleteRequest(irp, NULL);
}