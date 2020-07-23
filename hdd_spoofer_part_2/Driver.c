#include <ntddk.h>
#include <wdm.h>


extern POBJECT_TYPE* IoDriverObjectType;

NTSYSAPI NTSTATUS NTAPI ObReferenceObjectByName(
    PUNICODE_STRING ObjectName,
    ULONG Attributes,
    PACCESS_STATE AccessState,
    ACCESS_MASK DesiredAccess,
    POBJECT_TYPE ObjectType,
    KPROCESSOR_MODE AccessMode,
    PVOID ParseContext OPTIONAL,
    PVOID* Object
);

char spoofed_by_jguo12[] = "SPOOFED-BY-JGUO12";

NTSTATUS DriverEntry(DRIVER_OBJECT* driverObject, PUNICODE_STRING registryPath)
{
	UNICODE_STRING storDriverName;
	DRIVER_OBJECT* storDriverObject = NULL;

	RtlInitUnicodeString(&storDriverName, L"\\Driver\\storahci");
	ObReferenceObjectByName(&storDriverName, OBJ_KERNEL_HANDLE, 0, GENERIC_ALL, *IoDriverObjectType, KernelMode, 0, &storDriverObject);

    DEVICE_OBJECT* storDeviceObject = storDriverObject->DeviceObject;

    while (storDeviceObject != NULL)
    {
        char* serialNumber = (char*)(storDeviceObject->DeviceExtension) + 0x68;    //serial number offset

        RtlCopyMemory(serialNumber, spoofed_by_jguo12, sizeof(spoofed_by_jguo12));

        storDeviceObject = storDeviceObject->NextDevice;
    }
}
