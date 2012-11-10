/////////////////////////////////////////////////////////////////////////////
/// tcgs_interface.h
///
/// transport interface mapper
///
/// (c) Artem Zankovich, 2012
/////////////////////////////////////////////////////////////////////////////

#ifndef _TCGS_INTERFACE_H
#define _TCGS_INTERFACE_H

#include "tcgs_types.h"
#include "tcgs_config.h"

typedef enum
{
    INTERFACE_ERROR_GOOD,
    INTERFACE_ERROR_INVALID_SECURITY_PROTOCOL_ID_PARAMATER,
    INTERFACE_ERROR_INVALID_TRANSFER_LENGTH_PARAMETER_ON_IF_SEND,
    INTERFACE_ERROR_OTHER_INVALID_COMMAND_PARAMETER,
    INTERFACE_ERROR_SYNCHRONOUS_PROTOCOL_VIOLATION,
    INTERFACE_ERROR_DATA_PROTECTION,
    INTERFACE_ERROR_COMMON,   //this error is used when SDR is disabled
} TCGS_InterfaceError_t;

typedef enum
{
    IF_SEND,
    IF_RECV,
    IF_LAST,    //special value that stores number of values in this enum
} TCGS_Command_t;

/*
 * \brief The abstracted command block of the interface commands
 *
 * This structure describes format of interface commands as defined in
 * TCG ARchitecture Core Specification (section 3.3. Interface Communications).
 * Further mapping to host interface to device is additionally required.
 */
typedef struct
{
    TCGS_Command_t command;         //Either IF-SEND or IF-RECV
    uint8          protocolId;      //Between 0x01 and 0x06
    uint32         length;          //The amount of data to be transferred, in bytes
    uint32         comId;           //The ComID to be used, for Protocol IDs 0x01, 0x02, 0x06
} TCGS_CommandBlock_t;

/*****************************************************************************
 * \brief Supported transport interfaces
 *
 * List of supported (now or in the future) transport interfaces
 * defined in SIIS specification
 *
 *****************************************************************************/
typedef enum
{
    INTERFACE_UNKNOWN,
#if defined(TCGS_INTERFACE_SCSI_SUPPORTED)
    INTERFACE_SCSI,
#endif //TCGS_INTERFACE_SCSI_SUPPORTED
#if defined(TCGS_INTERFACE_ATA_SUPPORTED)
    INTERFACE_ATA,
#endif //TCGS_INTERFACE_ATA_SUPPORTED
#if defined(TCGS_INTERFACE_NVM_EXPRESS_SUPPORTED)
    INTERFACE_NVM_EXPRESS,
#endif //TCGS_INTEFACE_VTPER_SUPPORTED
#if defined(TCGS_INTERFACE_VTPER_SUPPORTED)
    INTERFACE_VTPER
#endif //TCGS_INTEFACE_VTPER_SUPPORTED
} TCGS_Interface_t;

//structure contains association of interace type code with its printable name
typedef struct {
    TCGS_Interface_t type;
    char*            name;
} TCGS_InterfaceName_t;

/*****************************************************************************
 * \brief array of supported interfaces (type code + printable name). The last
 * element SHALL always be of type INTERFACE_UNKNOWN 
 */
extern TCGS_InterfaceName_t TCGS_InterfaceNames[];

/*****************************************************************************
 * \brief Get internal interface code by printable interface name
 *
 * @param[in]  name     printable interface name
 *
 * \return internal interface code
 *
 * \see TCGS_InterfaceNames
 *
 *****************************************************************************/
TCGS_Interface_t TCGS_Interface_GetCode(char *name);

/*****************************************************************************
 * Structures and functions to work with interface parameters. There are two
 * types of interface parameters -- configurations and device properties. 
 * First are used to configure work with interface (i.e. PIO or DMA mode
 * of trusted commands for ATA interface). Second reflect properties of device
 * received with device informational command (i.e. IDENTIFY DEVICE for ATA 
 * interface)
 *
 * \see TCGS_InterfaceParameters_t, 
 *****************************************************************************/

#define MAX_INTERFACE_PARAMETER_NAME_LENGTH 32

/*****************************************************************************
 * Single interface parameter. It is identified by name and contains some
 * value
 *
 * There are two types of interface parameters -- configurations and device
 * properties. First are used to configure work with interface (i.e. PIO or
 * DMA mode of ATA trusted commands for ATA interface). Second reflect
 * properties of device received with device informational command (i.e.
 * IDENTIFY DEVICE for ATA interface)
 *
 * \see TCGS_InterfaceParameters_t
 */
typedef struct
{
    char                            name[MAX_INTERFACE_PARAMETER_NAME_LENGTH + 1];
    uint32                          value;
} TCGS_IntefaceParameter_t;


/*****************************************************************************
 * List of parameters for concrete interface. 
 * 
 * Every interface defines and maintains its list of parameters. It initializes
 * configurations and device properties in interface function `init`. Device
 * properties may be updated then by call of interface function
 * `updateDeviceProperties`
 * 
 *
 * \see TCGS_InterfaceDescriptor_t, TCGS_IntefaceParameter_t
 */
typedef struct
{
    unsigned length;
    TCGS_IntefaceParameter_t *param;
} TCGS_InterfaceParameters_t;


/*****************************************************************************
 * \brief Set of functions that constitutes transport interface
 *****************************************************************************/
typedef TCGS_Error_t (*TCGS_OpenCommand_t) (char *device);

typedef TCGS_Error_t (*TCGS_CloseCommand_t) (void);

/*****************************************************************************
 * \brief Map TCG command to an interface one and send it to TPer.
 *
 * Depending on command direction command can either send data to device
 * or receive data from it.
 *
 * Device shall be opened with TCGS_OpenCommand_t command before.
 *
 * @param[in]  inputCommandBlock      input command block
 * @param[in]  inputPayload           input payload. NULL for read command
 * @param[out] tperError              interface command error status
 * @param[out] outputPayload          output payload. NULL for write command
 *
 * \return ERROR_SUCCESS if interface command is successfully mapped to
 * transport command that is sent to TPer. Returned response (if applicable) 
 * and error status code are returned by refference. Error code ERROR_INTERFACE
 * is returned otherwise
 *
 * \see TCGS_OpenCommand_t
 *****************************************************************************/

typedef TCGS_InterfaceError_t (*TCGS_IoCommand_t)
    (TCGS_CommandBlock_t *inputCommandBlock,  void *inputPayload,
     TCGS_InterfaceError_t *interfaceError, void *outputPayload);

typedef TCGS_InterfaceParameters_t*  (*TCGS_GetParameters_t) (void);

typedef TCGS_Error_t (*TCGS_SetDeviceParameter_t) (char *name, uint32 value);

typedef TCGS_Error_t (*TCGS_UpdateDeviceParameters_t) (void);

/*****************************************************************************
 * \brief Interface descriptor
 *
 * This structure contains pointers to transport-specific implementation of
 * transport functions. Don't use it directly, use TSGS_SetInterface function
 * instead
 *
 * \see TCGS_SetInterface
 *
 *****************************************************************************/
typedef struct
{
    TCGS_Interface_t     type;
    TCGS_OpenCommand_t   open;
    TCGS_CloseCommand_t  close;
    TCGS_IoCommand_t     send;
    TCGS_GetParameters_t getParameters;
    TCGS_SetDeviceParameter_t setDeviceParameter;
    TCGS_UpdateDeviceParameters_t updateDeviceParameters;
} TCGS_InterfaceDescriptor_t;

/*****************************************************************************
 * \brief Set new transport interface descriptor
 *
 * @param[in]  functions              interface descriptor
 *
 * \return None
 *
 *****************************************************************************/
void TCGS_Interface_SetDescriptor(TCGS_InterfaceDescriptor_t *descriptor);

/*****************************************************************************
 * \brief Get current interface descriptor
 *
 * \return interface descriptor
 *
 *****************************************************************************/
TCGS_InterfaceDescriptor_t* TCGS_Interface_GetDescriptor(void);

/*****************************************************************************
 * \brief Open device using current interface descriptor
 *
 * @param[in]  device                 device name
 *
 * \return error code
 *
 *****************************************************************************/
TCGS_Error_t TCGS_Interface_OpenDevice(char *device);


/*****************************************************************************
 * \brief Map command to current interface and send it to TPer. Return response (if provided)
 * and status.
 *
 * @param[in]  inputCommandBlock      input command block
 * @param[in]  inputPayload           input payload. NULL if command has no data
 * @param[out] tperError              interface command error status
 * @param[out] outputPayload          output payload
 *
 * \return ERROR_SUCCESS if interface command is successfully mapped to current transport
 * sent to TPer and the last returned response (error status code and payload). Error code
 * ERROR_INTERFACE is returned otherwise
 *
 *****************************************************************************/
TCGS_InterfaceError_t TCGS_Interface_IoCommand(
    TCGS_CommandBlock_t *inputCommandBlock,  void *inputPayload,
    TCGS_InterfaceError_t *interfaceError, void *outputPayload);

/*****************************************************************************
 * \brief Set transport-dependent parameter
 *
 * It depends on implementation of concrete transport protocol how to use
 * provided parameter
 *
 * @param[in]  name                   name of the parameter
 * @param[in]  value                  value of the parameter
 *
 * \return None
 *
 * \see TSGS_GetInterfaceParameter
 *
 *****************************************************************************/
void   TCGS_SetInterfaceParameter(char *name, uint32 value);

/*****************************************************************************
 * \brief Get device-controlled transport-dependent parameter
 *
 * It depends on implementation of concrete transport protocol how to set
 * provided parameter. For example, ATA interface implementation may set
 * parameters that specify value of Trusted Feature Set support flag
 * form Identify Device response
 *
 * @param[in]  name                   name of the parameter
 *
 * \return     uint32                 value of the parameter
 *
 * \see TSGS_SetInterfaceParameter
 *
 *****************************************************************************/
uint32 TCGS_GetInterfaceParameter(char *name);

#endif //TCGS_INTERFACE_H
