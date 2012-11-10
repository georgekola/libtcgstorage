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

#define BLOCK_SIZE 512
//TODO: fix handling of x=0
#define BYTES2BLOCKS(x) (((x - 1) >> 9) + 1)    // fit data to 512 blocks

static HANDLE diskHandle = NULL;

//TODO: move this function to tcgsparm
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
        CharToOem(errorText, errorText);
        printf(errorText);

        // release memory allocated by FormatMessage()
        LocalFree(errorText);
        errorText = NULL;
    }
}


static TCGS_Error_t TCGS_ATA_Open_win(char* device)
{
    if (NULL == device) {        
        //null pointer for parameter
        RETURN_ERROR(ERROR_BAD_PARAMETER, ERROR_INTERFACE);
    }

    diskHandle = CreateFile((TCHAR * )device,
                            GENERIC_WRITE | GENERIC_READ,
                            FILE_SHARE_WRITE | FILE_SHARE_READ,
                            NULL,
                            OPEN_EXISTING,
                            0,
                            NULL);

    if (diskHandle == INVALID_HANDLE_VALUE) {
        //TODO: remove debug output messages
        printf("Error opening device -- ");
        printErrorMessage(GetLastError());
        RETURN_ERROR(ERROR_OPENING_DEVICE, ERROR_INTERFACE);
    }

    // Device is opened    
    return ERROR_SUCCESS;
}

static TCGS_Error_t TCGS_ATA_Close_win(char* device)
{
    CloseHandle(diskHandle);
    diskHandle = NULL;
}


/*****************************************************************************
 * \brief Map command to ATA interface and send it to TPer.
 *
 * Depending on command direction command can either send data to device
 * or receive data from it.
 *
 * Device shall be opened with TCGS_ATA_Open_win before.
 *
 * @param[in]  inputCommandBlock      input command block
 * @param[in]  inputPayload           input payload. NULL for read command
 * @param[out] tperError              interface command error status
 * @param[out] outputPayload          output payload. NULL for write command
 *
 * \return ERROR_SUCCESS if interface command is successfully mapped to ATA
 * transport command that is sent to TPer. Returned response (if applicable) 
 * and error status code are returned by refference. Error code ERROR_INTERFACE
 * is returned otherwise
 *
 * \see TCGS_ATA_Open_win
 *****************************************************************************/
static TCGS_Error_t TCGS_ATA_IoCommand_win (
    TCGS_CommandBlock_t *inputCommandBlock,  void *inputPayload,
    TCGS_InterfaceError_t *tperError, void *outputPayload)
{
    uint8 *pAtaBuffer;
    #define pAtaPassThrough ((ATA_PASS_THROUGH_EX*)pAtaBuffer)
    ULONG ataBufferSize;
    DWORD BytesReturned;
    uint8 ataOpcode;
    TCGS_InterfaceParameters_t *parameters;
    bool  isDma = FALSE, isExtendedError = FALSE;

    if (NULL == diskHandle) {
        //disk is not opened
        RETURN_ERROR(ERROR_DEVICE_NOT_OPENED, ERROR_INTERFACE);
    }
    
    if (NULL == inputCommandBlock ||
        (inputCommandBlock->command == IF_SEND && inputPayload == NULL) ||
        (inputCommandBlock->command == IF_RECV && outputPayload == NULL))
    {
        //commandBlock and either inputPayload or outputPayload shall be specified
        RETURN_ERROR(ERROR_BAD_PARAMETER, ERROR_INTERFACE);
    }

    if (inputCommandBlock->protocolId < 1 || inputCommandBlock->protocolId > 6) {
        //protocols from 1-6 are supported by TCG standart
        RETURN_ERROR(ERROR_PROTOCOL_UNSUPPORTED, ERROR_INTERFACE);
    }

    //allocate one buffer for both input and output as the longest required
    if (inputCommandBlock->command == IF_SEND) {
        ataBufferSize = sizeof(ATA_PASS_THROUGH_EX) + inputCommandBlock->length;
    } else {
        //TODO: calculate size of buffer to receive (use max value from Properties?)
        ataBufferSize = sizeof(ATA_PASS_THROUGH_EX) + 512;
    }

    pAtaBuffer = (uint8*)malloc(sizeof(ATA_PASS_THROUGH_EX) + 512);
    memset(pAtaBuffer, 0, sizeof(ATA_PASS_THROUGH_EX));

    pAtaPassThrough->Length = sizeof(ATA_PASS_THROUGH_DIRECT);

    parameters = TCGS_ATA_GetParameters();
    if (TCGS_Interface_GetParameter(parameters, INTERFACE_PARAMETER_ATA_TRANSPORT_MODE) == ATA_TRANSPORT_DMA) {
        isDma = TRUE;
    }
    if (TCGS_Interface_GetParameter(parameters, INTERFACE_PARAMETER_ATA_EXTENDED_ERROR_FEATURE_SUPPORTED)) {
        isExtendedError = TRUE;
    }

    if (inputCommandBlock->command == IF_SEND) {
        pAtaPassThrough->AtaFlags = ATA_FLAGS_DATA_OUT;
        ataOpcode = isDma ? ATA_TRUSTED_SEND_DMA : ATA_TRUSTED_SEND;
        //set ATA command payload
        memmove(pAtaBuffer + sizeof(ATA_PASS_THROUGH_EX), inputPayload, inputCommandBlock->length);
    } else if (inputCommandBlock->command == IF_RECV) {
        pAtaPassThrough->AtaFlags = ATA_FLAGS_DATA_IN;
        ataOpcode = isDma ? ATA_TRUSTED_RECEIVE_DMA : ATA_TRUSTED_RECEIVE;
    } else {
        RETURN_ERROR(ERROR_COMMAND_UNSUPPORTED, ERROR_INTERFACE);
    }

    pAtaPassThrough->AtaFlags |= ATA_FLAGS_NO_MULTIPLE | ATA_FLAGS_DRDY_REQUIRED;
    pAtaPassThrough->DataBufferOffset = sizeof(ATA_PASS_THROUGH_EX);
    pAtaPassThrough->DataTransferLength = inputCommandBlock->length;

    pAtaPassThrough->TimeOutValue = 0xA0;   //TODO: ?????
    //pAtaPassThrough->CurrentTaskFile[0] = (uint8) inputCommandBlock->protocolId;
    //pAtaPassThrough->CurrentTaskFile[1] = (uint8) BYTES2BLOCKS(inputCommandBlock->length);
    //pAtaPassThrough->CurrentTaskFile[3] = inputCommandBlock->comId & 0xFF;
    //pAtaPassThrough->CurrentTaskFile[4] = (inputCommandBlock->comId >> 8) & 0xFF;
    //pAtaPassThrough->CurrentTaskFile[6] = ataOpcode;

    if (!DeviceIoControl(diskHandle,
                        IOCTL_ATA_PASS_THROUGH,
                        pAtaBuffer,
                        ataBufferSize,
                        pAtaBuffer,
                        ataBufferSize,
                        &BytesReturned,
                        NULL))
    {
        printErrorMessage(GetLastError());
        RETURN_ERROR(ERROR_IO_COMMAND, ERROR_INTERFACE);
    }

/*
Cmd.taskFileOut0().Error = PTE.CurrentTaskFile[0];
Cmd.taskFileOut0().Count = PTE.CurrentTaskFile[1];
Cmd.taskFileOut0().LBALow = PTE.CurrentTaskFile[2];
Cmd.taskFileOut0().LBAMid = PTE.CurrentTaskFile[3];
Cmd.taskFileOut0().LBAHigh = PTE.CurrentTaskFile[4];
Cmd.taskFileOut0().Device = PTE.CurrentTaskFile[5];
Cmd.taskFileOut0().Status = PTE.CurrentTaskFile[6];
*/

    if (pAtaPassThrough->CurrentTaskFile[0] == 0x00) {
        if (pAtaPassThrough->CurrentTaskFile[6] == 0x50) {
            *tperError = INTERFACE_ERROR_GOOD;
        } else {
            RETURN_ERROR(ERROR_INVALID_INTERFACE_STATUS, ERROR_TPER);
        }
    } else if (pAtaPassThrough->CurrentTaskFile[0] == 0x04) {
        if (pAtaPassThrough->CurrentTaskFile[6] == 0x51) {
            if (isExtendedError) {
                //TODO: check extended error status supported and map interface code
                *tperError = INTERFACE_ERROR_COMMON;
            } else {
                *tperError = INTERFACE_ERROR_COMMON;
            }
        } else {
            RETURN_ERROR(ERROR_INVALID_INTERFACE_STATUS, ERROR_TPER);
        }
    } else {
        RETURN_ERROR(ERROR_INVALID_INTERFACE_ERROR, ERROR_TPER);
    }

    if (inputCommandBlock->command == IF_RECV) {
        memmove(outputPayload, pAtaBuffer + sizeof(ATA_PASS_THROUGH_EX), inputCommandBlock->length);
    }
    
    return ERROR_SUCCESS;
}


TCGS_Error_t TCGS_ATA_SetDeviceParameter_win (char *name, uint32 value)
{
    return ERROR_INTERFACE;
}

static TCGS_Error_t TCGS_ATA_UpdateDeviceParameters_win (void)
{
    return ERROR_SUCCESS;
}

TCGS_InterfaceDescriptor_t TCGS_ATA_InterfaceDescriptor =
{
    INTERFACE_ATA,
    (TCGS_OpenCommand_t)&TCGS_ATA_Open_win,
    (TCGS_CloseCommand_t)&TCGS_ATA_Close_win,
    (TCGS_IoCommand_t)&TCGS_ATA_IoCommand_win,
    (TCGS_GetParameters_t)&TCGS_ATA_GetParameters,
    (TCGS_SetDeviceParameter_t)&TCGS_ATA_SetDeviceParameter_win,
    (TCGS_UpdateDeviceParameters_t)&TCGS_ATA_UpdateDeviceParameters_win,
};

#endif //(defined(_WIN32) || defined(_WIN64))
