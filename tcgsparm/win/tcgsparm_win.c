/////////////////////////////////////////////////////////////////////////////
/// tcgsparm_win.c
///
/// platform-specific routines for tcgsparm utility (WIN32)
///
/// (c) Artem Zankovich, 2012
//////////////////////////////////////////////////////////////////////////////

#include "tcgsparm_platform.h"
#include "tcgs_types.h"

#include <windows.h>
#include "stdio.h"

static uint64 getDiskSize(const HANDLE handle)
{
    uint64 disk_size = 0;
    GET_LENGTH_INFORMATION buf;
    DISK_GEOMETRY_EX geometry_ex;
    DWORD i;
    DWORD gotbytes;

    if (DeviceIoControl(handle, IOCTL_DISK_GET_LENGTH_INFO, NULL, 0, &buf, sizeof(buf), &i, NULL))
    {
        disk_size = (uint64)buf.Length.QuadPart;
        return disk_size;
    }

    if (DeviceIoControl(handle, IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, NULL, 0,
	                    &geometry_ex, sizeof(geometry_ex), &gotbytes, NULL))
    {
        disk_size=(uint64)geometry_ex.DiskSize.QuadPart;
    }

    return disk_size;
} 


static char* getDiskName(const HANDLE handle)
{
    DWORD  cbBytesReturned = 0;
    STORAGE_PROPERTY_QUERY query;
    STORAGE_DESCRIPTOR_HEADER storageDescriptorHeader;
    char *diskName;

    memset((void*)&query, 0, sizeof (query));
    query.PropertyId = StorageDeviceProperty;
    query.QueryType = PropertyStandardQuery;
    if (!DeviceIoControl (
    	handle, IOCTL_STORAGE_QUERY_PROPERTY, &query, sizeof(query),
    	&storageDescriptorHeader, sizeof(storageDescriptorHeader),
    	&cbBytesReturned, NULL))
    {
    	return NULL;
    }
    
    STORAGE_DEVICE_DESCRIPTOR *spStorageDescriptor=(STORAGE_DEVICE_DESCRIPTOR *)malloc(storageDescriptorHeader.Size);

    if (!DeviceIoControl(handle, IOCTL_STORAGE_QUERY_PROPERTY,
        &query, sizeof(query),
        spStorageDescriptor, storageDescriptorHeader.Size,
        &cbBytesReturned, NULL))
    {
        free(spStorageDescriptor);
        return NULL;
    }
    
    const unsigned int offsetProduct = spStorageDescriptor->ProductIdOffset;
    const unsigned int offsetVendor  = spStorageDescriptor->VendorIdOffset;
    unsigned int lenProduct = 0;
    unsigned int lenVendor  = 0;
    char* buffer=(char*)spStorageDescriptor;

    buffer[(cbBytesReturned < sizeof(buffer) ? cbBytesReturned : sizeof(buffer)-1)]='\0';
    if (offsetVendor > 0)
        lenVendor = strlen(&buffer[offsetVendor]);
    if (offsetProduct > 0)
        lenProduct = strlen(&buffer[offsetProduct]);

    if (lenVendor >0 && lenProduct > 0) {
        diskName = (char*) malloc (lenVendor+1+lenProduct+1);
        sprintf(diskName, "%s %s", &buffer[offsetVendor], &buffer[offsetProduct]);
    } else if (lenProduct > 0) {
        diskName = (char*) malloc (lenProduct+1);
        sprintf(diskName, "%s", &buffer[offsetProduct]);
    } else {
        diskName = "(unknown)";
    }

    free(spStorageDescriptor);
    return diskName;
}


void listDevicesAta()
{
    const char* devicePathPattern = "\\\\.\\PhysicalDrive%u";
    char devicePath[] = "\\\\.\\PhysicalDrive00";
    HANDLE handle = INVALID_HANDLE_VALUE;
    char *diskName;
    int i;

    for (i = 0; i < 16; i++) {
        sprintf(devicePath, devicePathPattern, i);
        
        handle = CreateFile(devicePath, FILE_READ_DATA, (FILE_SHARE_WRITE | FILE_SHARE_READ),
                            NULL, OPEN_EXISTING, 0, NULL);
        if (handle == INVALID_HANDLE_VALUE) {
            //Windows returns ERROR_FILE_NOT_FOUND error if device is not installed
            if (GetLastError() == ERROR_ACCESS_DENIED) {
                printf("%s -- access denied\n", devicePath);
            }
            continue;
        }
        
        diskName = getDiskName(handle);
        //TODO: list mounted drives
        printf("%s - (%s, %d MB)\n", devicePath, diskName, getDiskSize(handle) / 1024 / 1024);

        if (diskName)
            free(diskName);
        CloseHandle(handle);
    }    

    return;
}


