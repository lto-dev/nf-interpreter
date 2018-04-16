//
// Copyright (c) 2017 The nanoFramework project contributors
// Portions Copyright (c) Microsoft Corporation.  All rights reserved.
// See LICENSE file in the project root for full license information.
//

#include "sockets_lwip.h"

//--//

Sockets_LWIP_Driver g_Sockets_LWIP_Driver;

// HAL_COMPLETION Sockets_LWIP_Driver::s_DebuggerTimeoutCompletion;

// static HAL_CONTINUATION MulticastResponseContinuation;

// int32_t g_DebuggerPort_SslCtx_Handle = -1;

SOCK_SOCKET SOCK_socket( int family, int type, int protocol ) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return Sockets_LWIP_Driver::Socket( family, type, protocol, FALSE );
}
int SOCK_bind( SOCK_SOCKET socket, const struct SOCK_sockaddr* address, int addressLen  ) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return HAL_SOCK_bind( socket, address, addressLen  );
}
int SOCK_connect(SOCK_SOCKET socket, const struct SOCK_sockaddr* address, int addressLen) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return Sockets_LWIP_Driver::Connect(socket, address, addressLen);
}
int SOCK_send(SOCK_SOCKET socket, const char* buf, int len, int flags) 
{ 
    NATIVE_PROFILE_PAL_COM();    
    return Sockets_LWIP_Driver::Send(socket, buf, len, flags);
}
int SOCK_recv(SOCK_SOCKET socket, char* buf, int len, int flags)
{ 
    NATIVE_PROFILE_PAL_COM();
    return Sockets_LWIP_Driver::Recv(socket, buf, len, flags);
}
int SOCK_close(SOCK_SOCKET socket)
{ 
    NATIVE_PROFILE_PAL_COM();
    return Sockets_LWIP_Driver::Close(socket);
}
int SOCK_listen( SOCK_SOCKET socket, int backlog )
{ 
    NATIVE_PROFILE_PAL_COM();
    return Sockets_LWIP_Driver::Listen( socket, backlog );
}
SOCK_SOCKET SOCK_accept( SOCK_SOCKET socket, struct SOCK_sockaddr* address, int* addressLen )
{ 
    NATIVE_PROFILE_PAL_COM();
    return Sockets_LWIP_Driver::Accept( socket, address, addressLen, FALSE );
}
int SOCK_shutdown( SOCK_SOCKET socket, int how )
{ 
    NATIVE_PROFILE_PAL_COM();
    return HAL_SOCK_shutdown( socket, how );
}
int SOCK_getaddrinfo( const char* nodename, char* servname, const struct SOCK_addrinfo* hints, struct SOCK_addrinfo** res )
{ 
    NATIVE_PROFILE_PAL_COM();
    return HAL_SOCK_getaddrinfo( nodename, servname, hints, res );
}
void SOCK_freeaddrinfo( struct SOCK_addrinfo* ai )
{ 
    NATIVE_PROFILE_PAL_COM();
    HAL_SOCK_freeaddrinfo( ai );
}
int SOCK_ioctl( SOCK_SOCKET socket, int cmd, int* data )
{ 
    NATIVE_PROFILE_PAL_COM();
    return HAL_SOCK_ioctl( socket, cmd, data );
}
int SOCK_getlasterror()
{ 
    NATIVE_PROFILE_PAL_COM();
    return HAL_SOCK_getlasterror();
}
int SOCK_select( int nfds, SOCK_fd_set* readfds, SOCK_fd_set* writefds, SOCK_fd_set* except, const struct SOCK_timeval* timeout )
{ 
    NATIVE_PROFILE_PAL_COM();
    return Sockets_LWIP_Driver::Select( nfds, readfds, writefds, except, timeout );
}
int SOCK_setsockopt( SOCK_SOCKET socket, int level, int optname, const char* optval, int  optlen )
{ 
    NATIVE_PROFILE_PAL_COM();
    return HAL_SOCK_setsockopt( socket, level, optname, optval, optlen );
}
int SOCK_getsockopt( SOCK_SOCKET socket, int level, int optname, char* optval, int* optlen )
{ 
    NATIVE_PROFILE_PAL_COM();
    return HAL_SOCK_getsockopt( socket, level, optname, optval, optlen );
}
int SOCK_getpeername( SOCK_SOCKET socket, struct SOCK_sockaddr* name, int* namelen )
{ 
    NATIVE_PROFILE_PAL_COM();
    return HAL_SOCK_getpeername( socket, name, namelen );
}
int SOCK_getsockname( SOCK_SOCKET socket, struct SOCK_sockaddr* name, int* namelen )
{ 
    NATIVE_PROFILE_PAL_COM();
    return HAL_SOCK_getsockname( socket, name, namelen );
}
int SOCK_recvfrom( SOCK_SOCKET s, char* buf, int len, int flags, struct SOCK_sockaddr* from, int* fromlen )
{ 
    NATIVE_PROFILE_PAL_COM();
    return Sockets_LWIP_Driver::RecvFrom( s, buf, len, flags, from, fromlen );
}
int SOCK_sendto( SOCK_SOCKET s, const char* buf, int len, int flags, const struct SOCK_sockaddr* to, int tolen )
{ 
    NATIVE_PROFILE_PAL_COM();
    return Sockets_LWIP_Driver::SendTo( s, buf, len, flags, to, tolen );
}

bool Network_Initialize()
{
    NATIVE_PROFILE_PAL_COM();
    return Sockets_LWIP_Driver::Initialize( );
}

bool Network_Uninitialize()
{
    NATIVE_PROFILE_PAL_COM();

    return Sockets_LWIP_Driver::Uninitialize( );
}

void SOCKETS_CloseConnections()
{
    NATIVE_PROFILE_PAL_COM();
    Sockets_LWIP_Driver::CloseConnections(FALSE);
}


uint32_t SOCK_CONFIGURATION_GetAdapterCount()
{
    NATIVE_PROFILE_PAL_COM();
    return HAL_SOCK_CONFIGURATION_GetAdapterCount();
}

HRESULT SOCK_CONFIGURATION_LoadAdapterConfiguration( uint32_t interfaceIndex, HAL_Configuration_NetworkInterface* config )
{
    NATIVE_PROFILE_PAL_COM();
    return HAL_SOCK_CONFIGURATION_LoadAdapterConfiguration(interfaceIndex, config);
}

HRESULT SOCK_CONFIGURATION_UpdateAdapterConfiguration( uint32_t interfaceIndex, uint32_t updateFlags, HAL_Configuration_NetworkInterface* config )
{
    NATIVE_PROFILE_PAL_COM();
    HRESULT hr = S_OK;
    bool fDbg = FALSE;

    if(interfaceIndex >= NETWORK_INTERFACE_COUNT) 
    {
        return CLR_E_INVALID_PARAMETER;
    }

    const uint32_t c_reInitFlag = SOCK_NETWORKCONFIGURATION_UPDATE_DHCP       | 
                                SOCK_NETWORKCONFIGURATION_UPDATE_DHCP_RENEW | 
                                SOCK_NETWORKCONFIGURATION_UPDATE_MAC;

    const uint32_t c_uninitFlag = c_reInitFlag | SOCK_NETWORKCONFIGURATION_UPDATE_DHCP_RELEASE;

    if(0 != (updateFlags & c_uninitFlag))
    {
        fDbg = SOCKETS_DbgUninitialize(COM_SOCKET_DBG);
    }

    hr = HAL_SOCK_CONFIGURATION_UpdateAdapterConfiguration(interfaceIndex, updateFlags, config);

    if(SUCCEEDED(hr))
    {
        Sockets_LWIP_Driver::SaveConfig(interfaceIndex, config);
    }
    else
    {
        // restore the network configuration
        // FIXME replace with config manager get config block
        HAL_SOCK_CONFIGURATION_UpdateAdapterConfiguration(interfaceIndex, updateFlags, g_TargetConfiguration.NetworkInterfaceConfigs->Configs[interfaceIndex]);
    }

    if(0 != (updateFlags & c_reInitFlag))
    {
        if(fDbg) SOCKETS_DbgInitialize(COM_SOCKET_DBG);
    }

    return hr;
}

HRESULT SOCK_CONFIGURATION_LoadConfiguration( uint32_t interfaceIndex, HAL_Configuration_NetworkInterface* config )
{
    NATIVE_PROFILE_PAL_COM();
    HRESULT hr = S_OK;

    if(interfaceIndex >= NETWORK_INTERFACE_COUNT || config == NULL) 
    {
        return CLR_E_INVALID_PARAMETER;
    }

    // load current DCHP settings
    hr = SOCK_CONFIGURATION_LoadAdapterConfiguration(interfaceIndex, config);

    return hr;
}

HRESULT SOCK_CONFIGURATION_LoadWirelessConfiguration( uint32_t interfaceIndex, SOCK_WirelessConfiguration* wirelessConfig )
{
    NATIVE_PROFILE_PAL_COM();

    if (interfaceIndex >= WIRELESS_INTERFACE_COUNT)
    {
        return CLR_E_INVALID_PARAMETER;
    }

    Sockets_LWIP_Driver::ApplyWirelessConfig();

    /// Hal version is given a chance if it wants to override stored predifned values.

    if (HAL_SOCK_CONFIGURATION_LoadWirelessConfiguration(interfaceIndex, wirelessConfig) != S_OK)
    {
        memcpy( wirelessConfig, &g_WirelessConfig.WirelessInterfaces[interfaceIndex], sizeof(SOCK_WirelessConfiguration) );
    }

    return S_OK;
}

HRESULT SOCK_CONFIGURATION_UpdateWirelessConfiguration( uint32_t interfaceIndex, SOCK_WirelessConfiguration* wirelessConfig )
{
    NATIVE_PROFILE_PAL_COM();

    if (interfaceIndex >= WIRELESS_INTERFACE_COUNT)
    {
        return CLR_E_INVALID_PARAMETER;
    }

    memcpy( &g_WirelessConfig.WirelessInterfaces[interfaceIndex], wirelessConfig, sizeof(SOCK_WirelessConfiguration) );

    return S_OK;
}

HRESULT SOCK_CONFIGURATION_SaveAllWirelessConfigurations( )
{
//FIXME    HAL_CONFIG_BLOCK::UpdateBlockWithName(g_WirelessConfig.GetDriverName(), &g_WirelessConfig, sizeof(g_WirelessConfig), TRUE);

    return S_OK;
}


#define SOCKET_SHUTDOWN_READ         0
#define SOCKET_SHUTDOWN_WRITE        1
#define SOCKET_SHUTDOWN_READ_WRITE   2

#define ISSET_SOCKET_FLAG(x,y) ((y) == ((y) & (x).m_flags))
#define SET_SOCKET_FLAG(x,y)   (x).m_flags |= (y)
#define CLEAR_SOCKET_FLAG(x,y) (x).m_flags &= ~(y)

//-----------------------------------------------------------------------------
//
//  CloseConnections - close all connections with the option of leaving the debugger sockets open.
//  debugger sockets are left open on CLR reboot so that communication with the debugger is not
//  lost.
//
//-----------------------------------------------------------------------------
void Sockets_LWIP_Driver::CloseConnections(bool fCloseDbg)
{
    NATIVE_PROFILE_PAL_COM();

    int32_t cnt = g_Sockets_LWIP_Driver.m_cntSockets;
    int32_t idx = 0;

    // round one - close all SSL sockets
    for( int32_t i=cnt-1; i>=0; i-- )
    {
        struct SocketRegisterMap& entry = g_Sockets_LWIP_Driver.m_socketHandles[i];
        
        if(entry.m_socket  != SOCK_SOCKET_ERROR && 
           entry.m_sslData != NULL)
        {
            SSL_CloseSocket(entry.m_socket);
        }
    }

    // round two - close all non-SSL sockets
    cnt = g_Sockets_LWIP_Driver.m_cntSockets;

    for( int32_t i=0; i<cnt; i++ )
    {
        struct SocketRegisterMap& entry = g_Sockets_LWIP_Driver.m_socketHandles[i];
        
        if(entry.m_socket != SOCK_SOCKET_ERROR)
        {
            if(fCloseDbg || !ISSET_SOCKET_FLAG(entry, SocketRegisterMap::c_DebugSocket))
            {
                // use the HAL method so we don't unregister the socket since we handle that here
                HAL_SOCK_close(entry.m_socket);
                
                g_Sockets_LWIP_Driver.m_socketHandles[i].m_socket  = SOCK_SOCKET_ERROR;
                g_Sockets_LWIP_Driver.m_socketHandles[i].m_flags   = 0;
                g_Sockets_LWIP_Driver.m_socketHandles[i].m_sslData = NULL;
                g_Sockets_LWIP_Driver.m_cntSockets--;
            }
            else if(i > 0)
            {
                memcpy( &g_Sockets_LWIP_Driver.m_socketHandles[i], 
                        &g_Sockets_LWIP_Driver.m_socketHandles[idx++], 
                        sizeof(g_Sockets_LWIP_Driver.m_socketHandles[i]) );
            }
        }
    }
}

//--//

SOCK_SOCKET Sockets_LWIP_Driver::Socket( int32_t family, int32_t type, int32_t protocol, bool fDebug )
{
    NATIVE_PROFILE_PAL_COM();

    int ret = HAL_SOCK_socket(family, type, protocol);

    if (ret != SOCK_SOCKET_ERROR)
    {
        RegisterSocket(ret, (protocol != SOCK_IPPROTO_TCP), fDebug);
    }
    return ret;
}
int Sockets_LWIP_Driver::Connect(SOCK_SOCKET socket, const struct SOCK_sockaddr* address, int32_t addressLen)
{
    NATIVE_PROFILE_PAL_COM();

    return HAL_SOCK_connect(socket, address, addressLen);
}
int Sockets_LWIP_Driver::Send(SOCK_SOCKET socket, const char* buf, int32_t len, int32_t flags) 
{
    NATIVE_PROFILE_PAL_COM();

    return HAL_SOCK_send(socket, buf, len, flags);
}
int Sockets_LWIP_Driver::Recv(SOCK_SOCKET socket, char* buf, int32_t len, int32_t flags)
{
    NATIVE_PROFILE_PAL_COM();

    return HAL_SOCK_recv(socket, buf, len, flags);
}

int Sockets_LWIP_Driver::Shutdown(SOCK_SOCKET sock, int32_t how)
{
    NATIVE_PROFILE_PAL_COM();

    return HAL_SOCK_shutdown(sock, how);
}

//-----------------------------------------------------------------------------
//
// Close - The close method marks a socket to be closed by the select thread.  Close is handled
// in this way because (at least for one implementation) the select method can not be set for a 
// closed socket.  Therfore in the select thread the socket is closed.
//
//-----------------------------------------------------------------------------
int Sockets_LWIP_Driver::Close(SOCK_SOCKET sock)
{
    NATIVE_PROFILE_PAL_COM();
    UnregisterSocket(sock);
    return HAL_SOCK_close(sock);
}
int Sockets_LWIP_Driver::Listen( SOCK_SOCKET socket, int32_t backlog )
{
    NATIVE_PROFILE_PAL_COM();

    return HAL_SOCK_listen(socket, backlog);
}
SOCK_SOCKET Sockets_LWIP_Driver::Accept( SOCK_SOCKET socket, struct SOCK_sockaddr* address, int* addressLen, bool fDebug )
{
    NATIVE_PROFILE_PAL_COM();

    int ret = HAL_SOCK_accept(socket, address, addressLen);

    if (ret != SOCK_SOCKET_ERROR)
    {
        RegisterSocket(ret, TRUE, fDebug);
    }    

    return ret;
}    

int Sockets_LWIP_Driver::Select( int32_t nfds, SOCK_fd_set* readfds, SOCK_fd_set* writefds, SOCK_fd_set* exceptfds, const SOCK_timeval* timeout )
{
    NATIVE_PROFILE_PAL_COM();

    return HAL_SOCK_select(nfds, readfds, writefds, exceptfds, timeout);
}

int Sockets_LWIP_Driver::RecvFrom( SOCK_SOCKET s, char* buf, int32_t len, int32_t flags, struct SOCK_sockaddr* from, int* fromlen )
{
    NATIVE_PROFILE_PAL_COM();

    return HAL_SOCK_recvfrom(s, buf, len, flags, from, fromlen);
}

int Sockets_LWIP_Driver::SendTo( SOCK_SOCKET s, const char* buf, int32_t len, int32_t flags, const struct SOCK_sockaddr* to, int32_t tolen )
{
    NATIVE_PROFILE_PAL_COM();
    
    return HAL_SOCK_sendto(s, buf, len, flags, to, tolen);
}



void Sockets_LWIP_Driver::SaveConfig(int32_t index, HAL_Configuration_NetworkInterface *cfg)
{
    NATIVE_PROFILE_PAL_COM();
    if(index >= NETWORK_INTERFACE_COUNT) return;

    if(cfg) 
    {
        // FIXME replace with call to save config block
        memcpy( g_TargetConfiguration.NetworkInterfaceConfigs->Configs[index], cfg, sizeof(HAL_Configuration_NetworkInterface) );
    }
    
//FIXME    HAL_CONFIG_BLOCK::UpdateBlockWithName(g_NetworkConfig.GetDriverName(), &g_NetworkConfig, sizeof(g_NetworkConfig), TRUE);
}

void Sockets_LWIP_Driver::ApplyConfig()
{
    NATIVE_PROFILE_PAL_COM();
// FIXME
    // if(!HAL_CONFIG_BLOCK::ApplyConfig( g_NetworkConfig.GetDriverName(), &g_NetworkConfig, sizeof(g_NetworkConfig) ))
    // {
    //     // save to the dynamic config section so that MFDeploy will be able to get the configuration.
    //     SaveConfig(0, NULL);            
    // }
}

void Sockets_LWIP_Driver::ApplyWirelessConfig()
{
    NATIVE_PROFILE_PAL_COM();

    if(!s_wirelessInitialized)
    {
        //FIXME
        // if(!HAL_CONFIG_BLOCK::ApplyConfig( g_WirelessConfig.GetDriverName(), &g_WirelessConfig, sizeof(g_WirelessConfig) ))
        // {
        //     SaveWirelessConfig(0, NULL);
        // }
        s_wirelessInitialized = TRUE;
    }
}

void Sockets_LWIP_Driver::SaveWirelessConfig(int32_t index, HAL_Configuration_NetworkInterface *cfg)
{
    NATIVE_PROFILE_PAL_COM();
    if(index >= WIRELESS_INTERFACE_COUNT) return;
    
    if(cfg) 
    {
        memcpy( &g_WirelessConfig.WirelessInterfaces[index], cfg, sizeof(SOCK_WirelessConfiguration) );
    }
    
//FIXME    HAL_CONFIG_BLOCK::UpdateBlockWithName(g_WirelessConfig.GetDriverName(), &g_WirelessConfig, sizeof(g_WirelessConfig), TRUE);    
}


    
bool Sockets_LWIP_Driver::Initialize()
{
    NATIVE_PROFILE_PAL_COM();
    SOCKET_CHECK_ENTER(); 

    if(!s_initialized)
    {
        g_Sockets_LWIP_Driver.m_fShuttingDown = FALSE;

        g_Sockets_LWIP_Driver.m_cntSockets = 0;
        
        for( int32_t i=0; i<SOCKETS_MAX_COUNT; i++ )
        {
            g_Sockets_LWIP_Driver.m_socketHandles[i].m_socket  = SOCK_SOCKET_ERROR;
            g_Sockets_LWIP_Driver.m_socketHandles[i].m_flags   = 0;
            g_Sockets_LWIP_Driver.m_socketHandles[i].m_sslData = NULL;
        }


//        ApplyConfig();        
//        ApplyWirelessConfig();
        
        SOCKET_CHECK_bool( HAL_SOCK_Initialize() );

//        SSL_Initialize();

        s_initialized = TRUE;
    }
     
    
    SOCKET_CHECK_EXIT_bool();
}

// Dummy methods in case Debbuger is not installed
__nfweak void Sockets_LWIP_Driver::Debugger_Abort() {}
__nfweak void Sockets_LWIP_Driver::Debugger_Uninitialize() {}

bool Sockets_LWIP_Driver::Uninitialize( )
{
    NATIVE_PROFILE_PAL_COM();
    bool ret = TRUE;
   
    if(s_initialized)
    {
        Debugger_Abort();

       // close all connections (including debugger sockets)
        CloseConnections(TRUE);

        SSL_Uninitialize();
    
        Debugger_Uninitialize();
 
        ret = HAL_SOCK_Uninitialize();

        s_initialized          = FALSE;
        s_wirelessInitialized  = FALSE;
        s_discoveryInitialized = FALSE;
    }
   
    
    return ret;
}



//-----------------------------------------------------------------------------
//
//  RegisterSocket - socket tracking.  This method is used to track sockets
//    opened by this driver.  It does not include the fake socket.  Register
//    should be called after a socket is opened (via socket or accept methods).
//    The selectable parameter should only be true if the socket is in the 
//    listening or connected state
//
//-----------------------------------------------------------------------------
void Sockets_LWIP_Driver::RegisterSocket( SOCK_SOCKET sock, bool selectable, bool fDebug )
{
    NATIVE_PROFILE_PAL_COM();
    if(sock == SOCK_SOCKET_ERROR)
    {
        ASSERT(FALSE);
        return;
    }

    if(g_Sockets_LWIP_Driver.m_cntSockets >= SOCKETS_MAX_COUNT) return;

    GLOBAL_LOCK_SOCKETS(lock);

    g_Sockets_LWIP_Driver.m_socketHandles[g_Sockets_LWIP_Driver.m_cntSockets].m_socket  = sock;
    g_Sockets_LWIP_Driver.m_socketHandles[g_Sockets_LWIP_Driver.m_cntSockets].m_flags   = 0;
    g_Sockets_LWIP_Driver.m_socketHandles[g_Sockets_LWIP_Driver.m_cntSockets].m_sslData = NULL;

    if(fDebug) SET_SOCKET_FLAG(g_Sockets_LWIP_Driver.m_socketHandles[g_Sockets_LWIP_Driver.m_cntSockets], SocketRegisterMap::c_DebugSocket);

    g_Sockets_LWIP_Driver.m_cntSockets++;
}

//-----------------------------------------------------------------------------
//
//  UnregisterSocket - No longer track a given socket for clean up and selection.
//    This method should only be called immediately before closing down a socket.
//
//-----------------------------------------------------------------------------
void Sockets_LWIP_Driver::UnregisterSocket( SOCK_SOCKET sock )
{
    int32_t index= -1;
    
    NATIVE_PROFILE_PAL_COM();
    GLOBAL_LOCK_SOCKETS(lock);

    g_Sockets_LWIP_Driver.GetSocketSslData(sock, index);

    if (index == -1) return;

    g_Sockets_LWIP_Driver.m_cntSockets--;

    if(index != g_Sockets_LWIP_Driver.m_cntSockets)
    {
        memcpy( &g_Sockets_LWIP_Driver.m_socketHandles[index], 
                &g_Sockets_LWIP_Driver.m_socketHandles[g_Sockets_LWIP_Driver.m_cntSockets], 
                sizeof(g_Sockets_LWIP_Driver.m_socketHandles[index]) );
    }

    g_Sockets_LWIP_Driver.m_socketHandles[g_Sockets_LWIP_Driver.m_cntSockets].m_socket = SOCK_SOCKET_ERROR;
    g_Sockets_LWIP_Driver.m_socketHandles[g_Sockets_LWIP_Driver.m_cntSockets].m_flags  = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// weak implementations of the functions (to be replaced with _strong_ implementations if and when required) //
// C++ version
__nfweak void debug_printf( const char* format, ... )
{
    return;
}

bool Sockets_LWIP_Driver::s_initialized=FALSE;
bool Sockets_LWIP_Driver::s_wirelessInitialized=FALSE;
bool Sockets_LWIP_Driver::s_discoveryInitialized=FALSE;


