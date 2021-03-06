// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

#pragma once

#if !defined(_LEASELAYERIO_H_)
#define _LEASELAYERIO_H_

#include "leaselayerinc.h"


#define KSPIN_LOCK Common::RwLock
#define FAST_MUTEX Common::RwLock

//
// SHA1 hash code length
//
#define REMOTE_CERT_SIZE 12000

// Current lease driver version
#define LEASE_DRIVER_VERSION 101 // Version 1.0

//
// Base IOCTL code for the lease layer kernal driver.
//
#define IOCTL_LEASING_BASE 0x00000666
//
// Macro definition for defining IOCTL and FSCTL function control codes.  Note
// that function codes 0-2047 are reserved for Microsoft Corporation, and
// 2048-4095 are reserved for customers.
//

#define CTL_CODE( DeviceType, Function, Method, Access ) (                 \
    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
)
//
// Lease Agent IOCTL control codes.
//
#define IOCTL_LEASING_CREATE_LEASE_AGENT \
    CTL_CODE(IOCTL_LEASING_BASE, 0x0, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)
#define IOCTL_LEASING_CLOSE_LEASE_AGENT    \
    CTL_CODE(IOCTL_LEASING_BASE, 0x1, METHOD_BUFFERED, FILE_WRITE_DATA)
#define IOCTL_LEASING_BLOCK_LEASE_AGENT \
    CTL_CODE(IOCTL_LEASING_BASE, 0x99, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)
//
// Register Application IOCTL control codes.
//
#define IOCTL_CREATE_LEASING_APPLICATION    \
    CTL_CODE(IOCTL_LEASING_BASE, 0x2, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)
#define IOCTL_REGISTER_LEASING_APPLICATION    \
    CTL_CODE(IOCTL_LEASING_BASE, 0x3, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)
#define IOCTL_UNREGISTER_LEASING_APPLICATION    \
    CTL_CODE(IOCTL_LEASING_BASE, 0x4, METHOD_BUFFERED, FILE_WRITE_DATA)

//
// Lease IOCTL control codes.
//
#define IOCTL_ESTABLISH_LEASE    \
        CTL_CODE(IOCTL_LEASING_BASE, 0x6, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)
#define IOCTL_TERMINATE_LEASE    \
        CTL_CODE(IOCTL_LEASING_BASE, 0x7, METHOD_BUFFERED, FILE_WRITE_DATA)

//
// Get remote application expired IOCTL control code.
//
#define IOCTL_GET_LEASING_APPLICATION_EXPIRATION_TIME    \
    CTL_CODE(IOCTL_LEASING_BASE, 0x8, METHOD_BUFFERED, FILE_READ_DATA)

//
// Arbitration result IOCTL control code.
//
#define IOCTL_ARBITRATION_RESULT    \
    CTL_CODE(IOCTL_LEASING_BASE, 0x9, METHOD_BUFFERED, FILE_WRITE_DATA)

//
// Set global lease expiration time IOCTL control code.
//
#define IOCTL_SET_GLOBAL_LEASE_EXPIRATION_TIME    \
    CTL_CODE(IOCTL_LEASING_BASE, 0x10, METHOD_BUFFERED, FILE_WRITE_DATA)

//
// Remote Cert Verify result IOCTL control code.
//
#define IOCTL_REMOTE_CERT_VERIFY_RESULT    \
    CTL_CODE(IOCTL_LEASING_BASE, 0x11, METHOD_BUFFERED, FILE_WRITE_DATA)

//
// Update certificate IOCTL control code.
//
#define IOCTL_UPDATE_CERTIFICATE    \
    CTL_CODE(IOCTL_LEASING_BASE, 0x12, METHOD_BUFFERED, FILE_WRITE_DATA)

//
// Connection fault injection
//
#define IOCTL_BLOCK_LEASE_CONNECTION    \
    CTL_CODE(IOCTL_LEASING_BASE, 0x13, METHOD_BUFFERED, FILE_WRITE_DATA)

//
// Set security descriptor IOCTL control code.
//
#define IOCTL_SET_SECURITY_DESCRIPTOR    \
    CTL_CODE(IOCTL_LEASING_BASE, 0x14, METHOD_BUFFERED, FILE_WRITE_DATA)

//
// Update lease duration IOCTL control code.
//
#define IOCTL_UPDATE_LEASE_DURATION    \
        CTL_CODE(IOCTL_LEASING_BASE, 0x15, METHOD_BUFFERED, FILE_WRITE_DATA)

//
// Get current lease duration IOCTL control code.
//
#define IOCTL_QUERY_LEASE_DURATION    \
    CTL_CODE(IOCTL_LEASING_BASE, 0x16, METHOD_BUFFERED, FILE_READ_DATA)

//
// Change Transport behaviro for testing purpose
//
#define IOCTL_ADD_TRANSPORT_BEHAVIOR    \
    CTL_CODE(IOCTL_LEASING_BASE, 0x18, METHOD_BUFFERED, FILE_WRITE_DATA)

//
// Clear Transport behaviro for testing purpose
//
#define IOCTL_CLEAR_TRANSPORT_BEHAVIOR    \
    CTL_CODE(IOCTL_LEASING_BASE, 0x19, METHOD_BUFFERED, FILE_WRITE_DATA)

//
// Crash lease application
//
#define IOCTL_CRASH_LEASING_APPLICATION    \
    CTL_CODE(IOCTL_LEASING_BASE, 0x20, METHOD_BUFFERED, FILE_WRITE_DATA)

//
// Get current lease duration IOCTL control code.
//
#define IOCTL_GET_REMOTE_LEASE_EXPIRATION_TIME    \
    CTL_CODE(IOCTL_LEASING_BASE, 0x22, METHOD_BUFFERED, FILE_READ_DATA)

//
// Event completion type.
//
typedef enum _LEASE_LAYER_EVENT_TYPE {

    LEASING_APPLICATION_NONE,
    LEASING_APPLICATION_EXPIRED,
    LEASING_APPLICATION_LEASE_ESTABLISHED,
    REMOTE_LEASING_APPLICATION_EXPIRED,
    LEASING_APPLICATION_ARBITRATE,
    REMOTE_CERTIFICATE_VERIFY

} LEASE_LAYER_EVENT_TYPE, *PLEASE_LAYER_EVENT_TYPE;

#define EVENT_COUNT 5

//
// Event buffer (input and output). On input it contains just
// the lease agent and application handle. On output it is 
// populated with different information depending on the event type.
//
typedef struct _LEASE_LAYER_EVENT_INPUT_OUTPUT_BUFFER {

    //
    // Output.
    //
    LEASE_LAYER_EVENT_TYPE EventType;
    //
    // Output.
    //
    HANDLE LeaseHandle;
    //
    // Input.
    //
    HANDLE LeaseAgentHandle;
    //
    // Input and output.
    //
    HANDLE LeasingApplicationHandle;
    //
    // Output.
    //
    WCHAR RemoteLeasingApplicationIdentifier[MAX_PATH + 1];
    //
    // Output.
    //
    LONGLONG LeaseAgentInstance;
    //
    // Output.
    //
    LONGLONG RemoteLeaseAgentInstance;
    //
    // Output.
    //
   LONG RemoteTTL;
   LONG LocalTTL;
    //
    // Remote socket address.
    //
    TRANSPORT_LISTEN_ENDPOINT RemoteSocketAddress;
    //
    // Raw remote certificate chain size
    //
    DWORD cbCertificate;
    //
    // Raw remote certificate chain data
    //
    BYTE pbCertificate[REMOTE_CERT_SIZE];
    //
    // Pointer to the remote cert verify complete operation in kernel
    //
    PVOID remoteCertVerifyCompleteOp;
    //
    // Record the time when insert to the event queue
    //
    LARGE_INTEGER insertionTime;
    //
    // Lease protocol version of the remote side
    //
    USHORT remoteVersion;
    //
    // The instance for the lease where local node is the monitor
    //
    LONGLONG monitorLeaseInstance;
    //
    // The instance for the lease where local node is the subject
    //
    LONGLONG subjectLeaseInstance;
    //
    // The upper bound of the duration (ms) that the remote side could have started an arbitration
    //
    LONG remoteArbitrationDurationUpperBound;

} LEASE_LAYER_EVENT_INPUT_OUTPUT_BUFFER, *PLEASE_LAYER_EVENT_INPUT_OUTPUT_BUFFER;

//
// Create Lease Agent IOCTL input buffer.
//
typedef struct _CREATE_LEASE_AGENT_INPUT_BUFFER_DEVICE_IOCTL {

    //
    // Listening address of a lease agent.
    //
    TRANSPORT_LISTEN_ENDPOINT SocketAddress;
    //
    // Lease TTL in milliseconds.
    //
    LEASE_CONFIG_DURATIONS LeaseConfigDurations;
    //
    // Lease short arbitration timeout in milliseconds.
    //
    LONG LeaseSuspendDurationMilliseconds;
    //
    // Lease long arbitration timeout in milliseconds.
    //
    LONG ArbitrationDurationMilliseconds;
    //
    // Number of renew times in a lease duration.
    //
    LONG NumberOfLeaseRetry;
    //
    // Starting point of first retry in a lease duration.
    //
    LONG StartOfLeaseRetry;
    //
    // SecuritySettings
    //
    Transport::SecuritySettings const* SecuritySettings;
    //
    // User mode lease agent version for the input
    //
    LONG UserModeVersion;

} CREATE_LEASE_AGENT_INPUT_BUFFER_DEVICE_IOCTL, *PCREATE_LEASE_AGENT_INPUT_BUFFER_DEVICE_IOCTL;

//
// Lease Agent IOCTL buffer.
//
typedef struct _LEASE_AGENT_BUFFER_DEVICE_IOCTL {

    //
    // Lease agent handle (input or output).
    //
    HANDLE LeaseAgentHandle;
    //
    // Lease agent instance (output)
    //
    LONGLONG LeaseAgentInstance;
    //
    // Kernel mode lease agent version
    //
    LONG KernelModeVersion;

} LEASE_AGENT_BUFFER_DEVICE_IOCTL, *PLEASE_AGENT_BUFFER_DEVICE_IOCTL;

//
// Lease Agent IOCTL buffer.
//
typedef struct _CLOSE_LEASE_AGENT_BUFFER_DEVICE_IOCTL {

    //
    // Lease agent listening socket address (input).
    //
    TRANSPORT_LISTEN_ENDPOINT SocketAddress;

} CLOSE_LEASE_AGENT_BUFFER_DEVICE_IOCTL, *PCLOSE_LEASE_AGENT_BUFFER_DEVICE_IOCTL;

// Leasing application callbacks
typedef struct _OVERLAPPED_LEASE_LAYER_EXTENSION //: OVERLAPPED
{
    //
    // User mode and kernel mode.
    //LINUXTODO consider removing EventBuffer
    LEASE_LAYER_EVENT_INPUT_OUTPUT_BUFFER EventBuffer;
    //
    // User mode only.
    //
    LEASING_APPLICATION_EXPIRED_CALLBACK LeasingApplicationExpired;
    REMOTE_LEASING_APPLICATION_EXPIRED_CALLBACK RemoteLeasingApplicationExpired;
    LEASING_APPLICATION_ARBITRATE_CALLBACK LeasingApplicationArbitrate;
    LEASING_APPLICATION_LEASE_ESTABLISHED_CALLBACK LeasingApplicationLeaseEstablished;
    PVOID CallbackContext;

} OVERLAPPED_LEASE_LAYER_EXTENSION, *POVERLAPPED_LEASE_LAYER_EXTENSION;

//
// Add Leasing Application IOCTL input buffer.
//
typedef struct _CREATE_LEASING_APPLICATION_INPUT_BUFFER_DEVICE_IOCTL {

    //
    // Lease agent handle.
    //
    HANDLE LeaseAgentHandle;
    //
    // Unique identifier of the application.
    //
    WCHAR LeasingApplicationIdentifier[MAX_PATH + 1];
    //
    // Specifies if the application can participate in arbitration.
    //
    BOOLEAN IsArbitrationEnabled;
    //
    // Leasing application TTL
    //
    LONG LeasingAppExpiryTimeoutMilliseconds;
    //
    // Upper limit for renewal by indirec lease
    //
    LONG MaxIndirectLeaseTimeoutMilliseconds;

    OVERLAPPED_LEASE_LAYER_EXTENSION Callbacks;

} CREATE_LEASING_APPLICATION_INPUT_BUFFER_DEVICE_IOCTL, *PCREATE_LEASING_APPLICATION_INPUT_BUFFER_DEVICE_IOCTL;

//
// Delete Leasing Application IOCTL input buffer.
//
typedef struct _DELETE_LEASING_APPLICATION_INPUT_BUFFER_DEVICE_IOCTL {

    //
    // Unique identifier of the application.
    //
    WCHAR LeasingApplicationIdentifier[MAX_PATH + 1];

} DELETE_LEASING_APPLICATION_INPUT_BUFFER_DEVICE_IOCTL, *PDELETE_LEASING_APPLICATION_INPUT_BUFFER_DEVICE_IOCTL;

//
// Leasing Application IOCTL buffer.
//
typedef struct _LEASING_APPLICATION_BUFFER_DEVICE_IOCTL {

    //
    // Leasing aplication handle (input or output).
    //
    HANDLE LeasingApplicationHandle;
} LEASING_APPLICATION_BUFFER_DEVICE_IOCTL, *PLEASING_APPLICATION_BUFFER_DEVICE_IOCTL;

//
// Lease IOCTL buffer.
//
typedef struct _TERMINATE_LEASE_BUFFER_DEVICE_IOCTL {
    
    //
    // Leasing aplication handle.
    //
    HANDLE LeasingApplicationHandle;
    //
    // Lease handle
    //
    HANDLE LeaseHandle;
    //
    // Remote leasing application name.
    //
    WCHAR RemoteLeasingApplicationIdentifier[MAX_PATH + 1];

} TERMINATE_LEASE_BUFFER_DEVICE_IOCTL, *PTERMINATE_LEASE_BUFFER_DEVICE_IOCTL;

//
// Remove Leasing Application IOCTL input buffer.
//
typedef struct _REMOVE_LEASING_APPLICATION_INPUT_BUFFER_DEVICE_IOCTL {

    //
    // Lease agent handle.
    //
    HANDLE LeaseAgentHandle;
    //
    // Leasing application handle.
    //
    HANDLE LeasingApplicationHandle;

} REMOVE_LEASING_APPLICATION_INPUT_BUFFER_DEVICE_IOCTL, *PREMOVE_LEASING_APPLICATION_INPUT_BUFFER_DEVICE_IOCTL;

//
// Retrieve the next remote leasing application expired IOCTL input buffer.
//
typedef struct _REMOTE_LEASING_APPLICATION_EXPIRED_INPUT_BUFFER_DEVICE_IOCTL {

    //
    // Lease agent handle.
    //
    HANDLE LeaseAgentHandle;
    //
    // Leasing application handle.
    //
    HANDLE LeasingApplicationHandle;

} REMOTE_LEASING_APPLICATION_EXPIRED_INPUT_BUFFER_DEVICE_IOCTL, *PREMOTE_LEASING_APPLICATION_EXPIRED_INPUT_BUFFER_DEVICE_IOCTL;

//
// Establish lease IOCTL input buffer.
//
typedef struct _ESTABLISH_LEASE_INPUT_BUFFER_DEVICE_IOCTL {

    //
    // Leasing application handle.
    //
    HANDLE LeasingApplicationHandle;
    //
    // Remote leasing application name.
    //
    WCHAR RemoteLeasingApplicationIdentifier[MAX_PATH + 1];
    //
    // Remote listening address of a lease agent.
    //
    TRANSPORT_LISTEN_ENDPOINT RemoteSocketAddress;
    //
    // Expected remote lease agent instance
    //
    LONGLONG RemoteLeaseAgentInstance;
    //
    // Lease TTL type.
    //
    LEASE_DURATION_TYPE LeaseDurationType;

} ESTABLISH_LEASE_INPUT_BUFFER_DEVICE_IOCTL, *PESTABLISH_LEASE_INPUT_BUFFER_DEVICE_IOCTL;

//
// Establish lease IOCTL output buffer.
//
typedef struct _ESTABLISH_LEASE_OUTPUT_BUFFER_DEVICE_IOCTL {

    //
    // Lease handle (output).
    //
    HANDLE LeaseHandle;
    //
    // established flag (output)
    //
    BOOLEAN IsEstablished;

} ESTABLISH_LEASE_OUTPUT_BUFFER_DEVICE_IOCTL, *PESTABLISH_LEASE_OUTPUT_BUFFER_DEVICE_IOCTL;
//
// Arbitration result input buffer.
//
typedef struct _ARBITRATION_RESULT_INPUT_BUFFER {

    //
    // Leasing application handle.
    //
    HANDLE LeasingApplicationHandle;
    //
    // time to live for the local lease agent, currently the value is either 0, in which case
    // the local side has lost arbitration and must go down, or MAXLONG, in which case the
    // local side can continue to live.
    //
    LONG LocalTimeToLive;
    //
    // time to live for the remote lease agent, if this value is MAXLONG, the remote side can
    // not be declared as down
    //
    LONG RemoteTimeToLive;
    //
    // whether this is the delayed result or intermediate result
    //
    BOOLEAN IsDelayed;
    //
    // Lease agent instance.
    //
    LONGLONG LeaseAgentInstance;
    //
    // Remote lease agent instance.
    //
    LONGLONG RemoteLeaseAgentInstance;
    //
    // Remote listening address of a lease agent.
    //
    TRANSPORT_LISTEN_ENDPOINT RemoteSocketAddress;
    
} ARBITRATION_RESULT_INPUT_BUFFER, *PARBITRATION_RESULT_INPUT_BUFFER;

typedef struct _GET_LEASING_APPLICATION_EXPIRATION_INPUT_BUFFER {

    //
    // Leasing application handle.
    //
    HANDLE LeasingApplicationHandle;
    //
    // Request TTL.
    //
    LONG RequestTimeToLive;

} GET_LEASING_APPLICATION_EXPIRATION_INPUT_BUFFER, *PGET_LEASING_APPLICATION_EXPIRATION_INPUT_BUFFER;

typedef struct _GET_LEASING_APPLICATION_EXPIRATION_OUTPUT_BUFFER {

    //
    // Leasing application handle.
    //
    LONGLONG KernelSystemTime;
    //
    // Time to live of the global lease.
    //
    LONG TimeToLive;

} GET_LEASING_APPLICATION_EXPIRATION_OUTPUT_BUFFER, *PGET_LEASING_APPLICATION_EXPIRATION_OUTPUT_BUFFER;

typedef struct _GLOBAL_LEASE_EXPIRATION_INPUT_BUFFER {

    //
    // Leasing application handle.
    //
    HANDLE LeasingApplicationHandle;
    //
    // Global lease expire time.
    //
    LONGLONG LeaseExpireTime;

} GLOBAL_LEASE_EXPIRATION_INPUT_BUFFER, *PGLOBAL_LEASE_EXPIRATION_INPUT_BUFFER;

//
// Unregister Leasing Application IOCTL input buffer.
// This structure was added when the IsDelayed flag was introduced
//
typedef struct _UNREGISTER_LEASING_APPLICATION_INPUT_BUFFER_DEVICE_IOCTL {

    //
    // Leasing application handle.
    //
    HANDLE LeasingApplicationHandle;
    //
    // Specifies if the unregister should be delayed.
    //
    BOOLEAN IsDelayed;

} UNREGISTER_LEASING_APPLICATION_INPUT_BUFFER_DEVICE_IOCTL, *PUNREGISTER_LEASING_APPLICATION_INPUT_BUFFER_DEVICE_IOCTL;

//
// Remote certificate verify callback operation IOCTL buffer.
//
typedef struct _REMOTE_CERT_VERIFY_CALLBACK_OPERATION_INPUT_BUFFER {

    //
    // Leasing application handle.
    //
    HANDLE LeasingApplicationHandle;
    //
    // Handle to the operation (input or output).
    //
    PVOID CertVerifyOperation;
    //
    // Remote certificate verify result
    //
    LONG VerifyResult;

} REMOTE_CERT_VERIFY_CALLBACK_OPERATION_INPUT_BUFFER, *PREMOTE_CERT_VERIFY_CALLBACK_OPERATION_INPUT_BUFFER;


typedef struct _UPDATE_SECURITY_SETTINGS_INPUT_BUFFER
{
    HANDLE LeasingApplicationHandle;
    Transport::SecuritySettings const* SecuritySettings;
} UPDATE_SECURITY_SETTINGS_INPUT_BUFFER, *PUPDATE_SECURITY_SETTINGS_INPUT_BUFFER;

//
// Block lease connection IOCTL input buffer.
//
typedef struct _BLOCK_LEASE_CONNECTION_BUFFER_DEVICE_IOCTL {

    //
    // Lease agent listening socket address (input).
    //
    TRANSPORT_LISTEN_ENDPOINT LocalSocketAddress;
    //
    // Remote listening address of a lease agent.
    //
    TRANSPORT_LISTEN_ENDPOINT RemoteSocketAddress;
    //
    // Flag for blocking or unblocking
    //
    BOOLEAN IsBlocking;

} BLOCK_LEASE_CONNECTION_BUFFER_DEVICE_IOCTL, *PBLOCK_LEASE_CONNECTION_BUFFER_DEVICE_IOCTL;

//
// Set security descriptor IOCTL input buffer.
//
typedef struct _SET_SECURITY_DESCRIPTOR_INPUT_BUFFER
{
    HANDLE LeaseAgentHandle;
    
    LONG SecurityDescriptorSize;

    SECURITY_DESCRIPTOR SecurityDescriptor; // Security descriptor in self-relative format

} SET_SECURITY_DESCRIPTOR_INPUT_BUFFER, *PSET_SECURITY_DESCRIPTOR_INPUT_BUFFER;


//
// Update lease duration IOCTL input buffer.
//
typedef struct _UPDATE_LEASE_DURATION_INPUT_BUFFER
{
    // For kernel LeaseAgentContext
    HANDLE LeaseAgentHandle;

    // The new config value
    LEASE_CONFIG_DURATIONS UpdatedLeaseDurations;

} UPDATE_LEASE_DURATION_INPUT_BUFFER, *PUPDATE_LEASE_DURATION_INPUT_BUFFER;

//
// Remote lease expiration result input buffer
//
typedef struct _REMOTE_LEASE_EXPIRATION_RESULT_INPUT_BUFFER {

    //
    // Leasing application handle.
    //
    HANDLE LeasingApplicationHandle;
    //
    // Remote leasing application name.
    //
    _Inout_updates_z_(MAX_PATH + 1) WCHAR RemoteLeasingApplicationIdentifier[MAX_PATH + 1];
} REMOTE_LEASE_EXPIRATION_RESULT_INPUT_BUFFER, *PREMOTE_LEASE_EXPIRATION_RESULT_INPUT_BUFFER;

//
// Remote lease expiration result output buffer
//
typedef struct _REMOTE_LEASE_EXPIRATION_RESULT_OUTPUT_BUFFER {

    //
    // TTL on monitor side
    //
    LONG MonitorExpireTTL;

    //
    // TTL on subject side
    //
    LONG SubjectExpireTTL;

} REMOTE_LEASE_EXPIRATION_RESULT_OUTPUT_BUFFER, *PREMOTE_LEASE_EXPIRATION_RESULT_OUTPUT_BUFFER;

//
// Request context space. Used to manage request completion/cacellation.
//
typedef struct _REQUEST_CONTEXT {

    _REQUEST_CONTEXT() : IsCanceled(FALSE), IsCompleted(FALSE) {}
    //
    // TRUE if the request has been canceled.
    //
    BOOLEAN IsCanceled;
    //
    // TRUE if the request has been completed.
    //
    BOOLEAN IsCompleted;
    //
    // Request concurrency control.
    //
    KSPIN_LOCK Lock;

} REQUEST_CONTEXT, *PREQUEST_CONTEXT;

struct WdfRequest
{
    WdfRequest() : input(nullptr), inputLength(0), output(nullptr), outputCapacity(0), outputLength(0) {}
    WdfRequest(void* inbuf, DWORD inbufLen,void* outBuf, DWORD outBufLen)
        : input(inbuf), inputLength(inbufLen), output(outBuf), outputCapacity(outBufLen), outputLength(0)
    {
    }

    REQUEST_CONTEXT requestContext;
    void* input;
    size_t inputLength;    
    void* output;
    size_t outputCapacity;
    size_t outputLength;
};

typedef std::shared_ptr<WdfRequest> WDFREQUEST;

NTSTATUS DeviceIoControl(
  _In_        DWORD        dwIoControlCode,
  _In_opt_    LPVOID       lpInBuffer,
  _In_        DWORD        nInBufferSize,
  _Out_opt_   LPVOID       lpOutBuffer,
  _In_        DWORD        nOutBufferSize,
  _Out_opt_   LPDWORD      lpBytesReturned);

NTSTATUS
WdfRequestRetrieveInputBuffer(
    _In_ WDFREQUEST & Request,
    _In_ size_t MinimumRequiredLength,
    _Outptr_result_bytebuffer_(*Length) PVOID* Buffer,
    _Out_opt_ size_t* Length);

NTSTATUS
WdfRequestRetrieveOutputBuffer(
    _In_ WDFREQUEST & Request,
    _In_ size_t MinimumRequiredSize,
    _Outptr_result_bytebuffer_(*Length) PVOID* Buffer,
    _Out_opt_ size_t* Length);

BOOL IsLeaseLayerInitialized(VOID);
VOID InitializeLeaseLayer(VOID);

// void ClearLeasingApplicationTTLFileMap(HANDLE handle);

#endif  // _LEASELAYERIO_H_
