/////////////////////////////////////////////////////////////////////////////
/// tcgsparm.c
///
/// tcgsparm utility
///
/// (c) Artem Zankovich, 2012
//////////////////////////////////////////////////////////////////////////////

#include "libtcgstorage.h"
#if defined(TCGS_INTERFACE_ATA_SUPPORTED)
#include "tcgs_interface_ata.h"
#endif //defined(TCGS_INTERFACE_ATA_SUPPORTED)
#if defined(TCGS_INTERFACE_VTPER_SUPPORTED)
#include "tcgs_interface_vtper.h"
#endif //defined(TCGS_INTERFACE_VTPER_SUPPORTED)

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//parsed command line arguments
typedef struct
{
  char *startedAs;
  char *interface;
  char *device;
  char *command;
  bool showHelp;
} Arguments_t;

void showUsage(Arguments_t *args)
{
    printf(
        "TCG Storage params -- get trusted drive parameters. (c) Artem Zankovich, 2012\n"\
        "%s [-h] [-i #type [-d #name] -c #command]\n"\
        "  -h, --help       show this help message\n"\
        "  -i, --interface  interface type. List supported interface types when omitted\n"\
        "  -d, --device     device name. List available devices for specified interface when omitted\n"\
        "  -c, --command    TCG Storage command to send on device\n",
        args->startedAs
    );
}

void listDevices(TCGS_Interface_t interface, Arguments_t *args)
{
    switch (interface)
    {
    case INTERFACE_UNKNOWN:
        return;
    case INTERFACE_ATA:
        printf("Devices list for interface: %s\n", args->interface);
        break;
    }
    listDevicesImpl();
}

void listInterfaces(Arguments_t *args)
{
    TCGS_InterfaceName_t* interface;
    
    printf("List of supported device interfaces:\n");
    
    for (interface = &TCGS_InterfaceNames[0]; interface->type != INTERFACE_UNKNOWN; interface++) {
        printf("    %s\n", interface->name);
    }
    
    if (interface == &TCGS_InterfaceNames[0]) {
        printf("(empty)");
    }
}

//parse command line arguments
Arguments_t *decodeArguments(int argc, char* argv[])
{
    static Arguments_t arguments;
    
    memset(&arguments, 0, sizeof(Arguments_t));
    arguments.startedAs = argv[0];

    while (TRUE) {
        static struct option long_options[] = {
            {"interface", required_argument, 0, 'i'},
            {"device",    required_argument, 0, 'd'},
            {"command",   required_argument, 0, 'c'},
            {"help",      no_argument      , 0, 'h'},
            {0, 0, 0, 0}};
           
        /* getopt_long stores the option index here. */
        int option_index = 0;

        int c = getopt_long (argc, argv, "i:d:c:h", long_options, &option_index);
        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c) {
        case 'i':
            arguments.interface = optarg;
            break;

        case 'd':
            if (arguments.interface == NULL) {
                fprintf(stderr, "%s: error -- specify interface type before selecting device\n", argv[0]);
                listInterfaces(&arguments);
                exit(1);
            }
            arguments.device = optarg;
            break;

        case 'c':
            arguments.command = optarg;
            break;
        case 'h':
            arguments.showHelp = TRUE;
            break;
        }
    }

    if (optind < argc) {
        fprintf (stderr, "%s: warning -- only options are supported\n", argv[0]);
    }

    return &arguments;
}


TCGS_Error_t openDevice(TCGS_Interface_t interface, Arguments_t *args)
{
    switch (interface)
    {
#if defined(TCGS_INTERFACE_ATA_SUPPORTED)
    case INTERFACE_ATA:
        TCGS_InitHost(&TCGS_ATA_InterfaceDescriptor);
        if (TCGS_Interface_OpenDevice(args->device) != ERROR_SUCCESS) {
            printf("Error opening device\n");
            exit(-1);
        }
        break;
#endif //defined(TCGS_INTERFACE_ATA_SUPPORTED)

#if defined(TCGS_INTERFACE_SCSI_SUPPORTED)
    case INTERFACE_SCSI:
        fprintf(stderr, "SCSI devices are not supproted at this moment\n");
        return ERROR_INTERFACE;
#endif //defined(TCGS_INTERFACE_SCSI_SUPPORTED)

#if defined(TCGS_INTERFACE_NVM_EXPRESS_SUPPORTED)
    case INTERFACE_NVM_EXPRESS:
        fprintf(stderr, "NVM Express devices are not supproted at this moment\n");
        return ERROR_INTERFACE;
#endif //defined(TCGS_INTERFACE_NVM_EXPRESS_SUPPORTED)

#if defined(TCGS_INTERFACE_VTPER_SUPPORTED)
    case INTERFACE_VTPER:
        TCGS_InitHost(&TCGS_VTper_InterfaceDescriptor);
        break;
#endif //defined(TCGS_INTERFACE_VTPER_SUPPORTED)
    }

    return ERROR_SUCCESS;
}

void processCommand(Arguments_t *args)
{
    printf("Processing command: %s\n", args->command);
    if (TCGS_Level0Discovery() != ERROR_SUCCESS) {
        printf("Error processing command\n");
        exit(0);
    }
}

//utility entry point
int main(int argc, char* argv[])
{
    Arguments_t *args;
    TCGS_Interface_t interface;
    args = decodeArguments(argc, argv);

    if (args->showHelp || argc < 2) {
        showUsage(args);
        exit(0);
    }

    if (args->interface == NULL) {
        fprintf(stderr, "%s: error -- specify interface type of target device (--interface)\n", argv[0]);
        listInterfaces(args);
        exit(1); 
    }

    interface = TCGS_Interface_GetCode(args->interface) ; 
    if (interface == INTERFACE_UNKNOWN) {
        fprintf(stderr, "%s: error -- invalid interface: %s\n", argv[0], args->interface);
        listInterfaces(args);
        exit(1);
    }

    if (args->device != NULL || interface == INTERFACE_VTPER) {
        if (openDevice(interface, args) != ERROR_SUCCESS) {
            fprintf(stderr, "%s: error -- invalid device: %s\n", argv[0], args->device);
            listDevices(interface, args);
            exit(1);
        }
        
        processCommand(args);
        exit(0);
    } else {
        fprintf(stderr, "%s: error -- no device specified for interface: %s\n", argv[0], args->interface);
        listDevices(interface, args);
        exit(1);
    }
    
    if (args->command != NULL) {
        if (args->device == NULL) {
            fprintf(stderr, "%s: error -- specify target device name (--device)\n", argv[0]);
            listDevices(interface, args);
            exit(1); 
        }
        processCommand(args);
        exit(0);
    }

    
    TCGS_DestroyHost();
    
    return 0;
}

