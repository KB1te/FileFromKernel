#include <ntddk.h>
#include <wdf.h>
#include <initguid.h>
#include "trace.h"
#include <ntifs.h>
#include <ntimage.h>

DRIVER_UNLOAD		DriverUnload;
DRIVER_INITIALIZE	DriverEntry;
DEVICE_OBJECT		Device;
DRIVER_DISPATCH		DispatchCreate;
