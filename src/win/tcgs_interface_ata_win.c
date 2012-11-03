/////////////////////////////////////////////////////////////////////////////
/// tcgs_interface_ata.c
///
/// Implementation of ATA interface for MS Windows
///
/// (c) Artem Zankovich, 2012
/////////////////////////////////////////////////////////////////////////////

#if (defined(_WIN32) || defined(_WIN64))

#include "../tcgs_interface.h"
#include "../tcgs_interface_ata.h"
#include "../tcgs_types.h"

#include <stdio.h>
#include <windows.h>
//Mingw32 has not ported ATA_PASS_THROUGH_DIRECT from Windows DDK. Project includes file from Mingw64
#include "ntddscsi.h"


ATA_PASS_THROUGH_DIRECT gAtaIoCommand;

TCGS_IntefaceParameter_t parameter[] =
{
	{"ata.transport_mode", (uint32)ATA_TRANSPORT_DMA},
};

TCGS_IntefaceParameters_t parameters[] =
{
	sizeof(parameter) / sizeof(parameter[0]),
	parameter
};

#define BLOCK_SIZE 512
//TODO: fix handling of x=0
#define BYTES2BLOCKS(x) (((x - 1) >> 9) + 1)    // fit data to 512 blocks

static HANDLE fileHandle = NULL;

static void printErrorMessage(DWORD errorCode)
{
    LPTSTR errorText = NULL;

    FormatMessage(
       // use system message tables to retrieve error text
       FORMAT_MESSAGE_FROM_SYSTEM
       // allocate buffer on local heap for error text
       |FORMAT_MESSAGE_ALLOCATE_BUFFER
       // Important! will fail otherwise, since we're not 
       // (and CANNOT) pass insertion parameters
       |FORMAT_MESSAGE_IGNORE_INSERTS,  
       NULL,    // unused with FORMAT_MESSAGE_FROM_SYSTEM
       errorCode,
       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
       (LPTSTR)&errorText,  // output 
       0, // minimum size for output buffer
       NULL); 

    if ( NULL != errorText )
    {
       printf(errorText);

       // release memory allocated by FormatMessage()
       LocalFree(errorText);
       errorText = NULL;
    }
}


static TCGS_Error_t TCGS_ATA_Open(char* device)
{
    HANDLE fileHandle = NULL;
    ATA_PASS_THROUGH_DIRECT * lpOutBuffer = &gAtaIoCommand;
    ULONG bufferSize = 512;
    DWORD* nOutBufferSize = &bufferSize;
    DWORD BytesReturned;
    DWORD lastError;
    uint8 ataOpcode;

    if (NULL == device) {        
        return ERROR_INTERFACE;
    }

    fileHandle = CreateFile((TCHAR * )device,
                            GENERIC_WRITE | GENERIC_READ,
                            FILE_SHARE_WRITE | FILE_SHARE_READ,
                            NULL,
                            OPEN_EXISTING,
                            0,
                            NULL);

    if (fileHandle == INVALID_HANDLE_VALUE) {        
        printf("Error opening device -- ");
        printErrorMessage(GetLastError());
        return ERROR_INTERFACE;
    }

    // Device is opened    
    return ERROR_SUCCESS;
}

/*****************************************************************************
 * \brief Map command to ATA interface and send it to TPer. Return response and status.
 *
 * @param[in]  inputCommandBlock      input command block
 * @param[in]  inputPayload           input payload. NULL if command has no data
 * @param[out] tperError              interface command error status
 * @param[out] outputPayload          output payload
 *
 * \return ERROR_SUCCESS if interface command is successfully mapped to ATA transport
 * sent to TPer and the last returned response (error status code and payload). Error code
 * ERROR_INTERFACE is returned otherwise
 *
 *****************************************************************************/
static TCGS_Error_t TCGS_ATA_Send(
    TCGS_CommandBlock_t *inputCommandBlock,  void *inputPayload,
    TCGS_InterfaceError_t *tperError, void *outputPayload)
{
	*tperError = INTERFACE_ERROR_GOOD;

	/*HANDLE fileHandle = NULL;
    ATA_PASS_THROUGH_DIRECT * lpOutBuffer = &gAtaIoCommand;
    ULONG bufferSize = 512;
    DWORD* nOutBufferSize = &bufferSize;
	DWORD BytesReturned;
	DWORD lastError;
    uint8 ataOpcode;

    if (NULL == device || NULL == inputCommandBlock) {
        return TRUSTED_IF_ERROR_BAD_PARAMETER;
    }

    if (command->CommandType != TRUSTED_STANDARD_TCG) {
        // We don't know to handle standards other TCG. yet. TBD
        return TRUSTED_IF_ERROR_UNSUPPORTED_STANDARD;
    }


    if (device->ifType != TRUSTED_INTERFACE_ATA) {
        // We don't know to handle other types. Yet. TBD
        return TRUSTED_IF_ERROR_UNKNOWN_DEVICE_TYPE;
    }


	fileHandle = CreateFile((TCHAR * )device->name,
		                    GENERIC_WRITE | GENERIC_READ,
		                    FILE_SHARE_WRITE | FILE_SHARE_READ,
		                    NULL,
		                    OPEN_EXISTING,
		                    0,
		                    NULL);

	if (fileHandle == INVALID_HANDLE_VALUE) {
		return TRUSTED_IF_ERROR_CANT_OPEN_DEVICE;

	}

    // Device is opened

	// Clear ATA I/O command buffer
	memset(lpOutBuffer, 0, sizeof(ATA_PASS_THROUGH_DIRECT));

	lpOutBuffer->AtaFlags = (ATA_FLAGS_NO_MULTIPLE | ATA_FLAGS_DRDY_REQUIRED);
    lpOutBuffer->DataBuffer = inputPayload;
	lpOutBuffer->DataTransferLength = BYTES2BLOCKS(inputCommandBlock->length) * BLOCK_SIZE;		// it includes extra data next to passed data

    if (inputCommandBlock->command == IF_SEND) {
        lpOutBuffer->AtaFlags |= ATA_FLAGS_DATA_OUT;
        //TODO: get POI or DMA from properties
        ataOpcode = 0x5E;
    } else if (inputCommandBlock->command == IF_RECV) {
        lpOutBuffer->AtaFlags |= ATA_FLAGS_DATA_IN;
        //TODO: get POI or DMA from properties
        ataOpcode = 0x5C;
    } else {
    	return ERROR_INTERFACE;
    }

	lpOutBuffer->Length = sizeof(ATA_PASS_THROUGH_DIRECT);
	lpOutBuffer->TimeOutValue = 0xA0;	//TODO: ?????
	lpOutBuffer->CurrentTaskFile[0] = (uint8) inputCommandBlock->protocolId;
	lpOutBuffer->CurrentTaskFile[1] = (uint8) BYTES2BLOCKS(inputCommandBlock->length);
    lpOutBuffer->CurrentTaskFile[3] = (inputCommandBlock->comId & 0xFF);
	lpOutBuffer->CurrentTaskFile[4] = (inputCommandBlock->comId >> 8);
	lpOutBuffer->CurrentTaskFile[6] = ataOpcode;

	if (DeviceIoControl(fileHandle,
                        IOCTL_ATA_PASS_THROUGH_DIRECT,
                        lpOutBuffer,
                        *nOutBufferSize,
                        lpOutBuffer,
                        *nOutBufferSize,
                        &BytesReturned,
                        NULL) == 0)
    {
		lastError = GetLastError();
	} else {
		lastError = 0;
	}

    CloseHandle (fileHandle);

    //TODO: map to interface error
    *tperError = lastError;


    if (lastError == 0x3E6 ) {
        command->OSError = TRUSTED_IF_ERROR_NO_ACCESS_RIGHTS;
#if (WIN_XP_PRIVILIGED_ACCESS_WORKAROUND_ENABLED)
        if (--loopCounter) {
            goto repeatAttempt;
        } else
#endif // (WIN_XP_PRIVILIGED_ACCESS_GLITCH>0)
        return command->OSError;
    }

	if (lpOutBuffer->CurrentTaskFile[0] != 0) {
		*nOutBufferSize = 0;
        command->OSError = TRUSTED_IF_ERROR_ATA_ERROR;
        return command->OSError;
	}


    return TRUSTED_IF_OK;*/
    
	return ERROR_SUCCESS;
}

static void TCGS_ATA_SetParameter(char *name, uint32 value)
{
	return;
}

static uint32 TCGS_ATA_GetParameter(char *name)
{
	return 0;
}

TCGS_InterfaceDescriptor_t TCGS_ATA_InterfaceDescriptor =
{
	INTERFACE_ATA,
	(TCGS_OpenCommand_t)&TCGS_ATA_Open,
	(TCGS_IoCommand_t)&TCGS_ATA_Send,
	(TCGS_SetParameterCommand_t)&TCGS_ATA_SetParameter,
	(TCGS_GetParameterCommand_t)&TCGS_ATA_GetParameter,
};

#endif //(defined(_WIN32) || defined(_WIN64))
