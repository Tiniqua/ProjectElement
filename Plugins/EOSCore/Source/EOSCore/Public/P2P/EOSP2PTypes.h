/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "eos_p2p_types.h"
#include "Core/EOSHelpers.h"
#include "EOSP2PTypes.generated.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		ENUMS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

/**
 * Type of established connection
 */
UENUM(BlueprintType)
enum class EEOSEConnectionEstablishedType : uint8
{
	/** The connection is brand new */
	EOS_CET_NewConnection = 0,
	/** The connection is reestablished (reconnection) */
	EOS_CET_Reconnection = 1
};

/**
 * Types of network connections.
 */
UENUM(BlueprintType)
enum class EEOSENetworkConnectionType : uint8
{
	/** There is no established connection */
	EOS_NCT_NoConnection = 0,
	/** A direct connection to the peer over the Internet or Local Network */
	EOS_NCT_DirectConnection = 1,
	/** A relayed connection using Epic-provided servers to the peer over the Internet */
	EOS_NCT_RelayedConnection = 2
};

/**
 * Setting for controlling whether relay servers are used.
 *
 * Please see the following EOS_ERelayControl value compatibility-chart to better understand how changing this value
 * can affect compatibility between clients with different settings.
 *
 * +------------------------------+---------------------+-------------------------------+---------------------+
 * |                              |   EOS_RC_NoRelays   |  EOS_RC_AllowRelays (Default) |  EOS_RC_ForceRelays |
 * +------------------------------+---------------------+-------------------------------+---------------------+
 * | EOS_RC_NoRelays              |  Compatible         |  Compatible                   |  Connection Failure |
 * | EOS_RC_AllowRelays (Default) |  Compatible         |  Compatible                   |  Compatible         |
 * | EOS_RC_ForceRelays           |  Connection Failure |  Compatible                   |  Compatible         |
 * +------------------------------+---------------------+-------------------------------+---------------------+
 */
UENUM(BlueprintType)
enum class EEOSERelayControl : uint8
{
	/** Peer connections will never attempt to use relay servers. Clients with restrictive NATs may not be able to connect to peers. */
	EOS_RC_NoRelays = 0,
	/** Peer connections will attempt to use relay servers, but only after direct connection attempts fail. This is the default value if not changed. */
	EOS_RC_AllowRelays = 1,
	/** Peer connections will only ever use relay servers. This will add latency to all connections, but will hide IP Addresses from peers. */
	EOS_RC_ForceRelays = 2
};

/**
 * Types of packet reliability.
 *
 * Ordered packets will only be ordered relative to other ordered packets. Reliable/unreliable and ordered/unordered communication
 * can be sent on the same Socket ID and Channel.
 */
UENUM(BlueprintType)
enum class EEOSEPacketReliability : uint8
{
	/** Packets will only be sent once and may be received out of order */
	EOS_PR_UnreliableUnordered = 0,
	/** Packets may be sent multiple times and may be received out of order */
	EOS_PR_ReliableUnordered = 1,
	/** Packets may be sent multiple times and will be received in order */
	EOS_PR_ReliableOrdered = 2
};

/**
 * Categories of NAT strictness.
 */
UENUM(BlueprintType)
enum class EEOSENATType : uint8
{
	/** NAT type either unknown (remote) or we are unable to determine it (local) */
	EOS_NAT_Unknown = 0,
	/** All peers can directly-connect to you */
	EOS_NAT_Open = 1,
	/** You can directly-connect to other Moderate and Open peers */
	EOS_NAT_Moderate = 2,
	/** You can only directly-connect to Open peers */
	EOS_NAT_Strict = 3
};

/**
 * Reasons why a P2P connection was closed
 */
UENUM(BlueprintType)
enum class EEOSEConnectionClosedReason : uint8
{
	/** The connection was closed for unknown reasons. This most notably happens during application shutdown. */
	EOS_CCR_Unknown = 0,
	/** The connection was at least locally accepted, but was closed by the local user via a call to EOS_P2P_CloseConnection / EOS_P2P_CloseConnections. */
	EOS_CCR_ClosedByLocalUser = 1,
	/** The connection was at least locally accepted, but was gracefully closed by the remote user via a call to EOS_P2P_CloseConnection / EOS_P2P_CloseConnections. */
	EOS_CCR_ClosedByPeer = 2,
	/** The connection was at least locally accepted, but was not remotely accepted in time. */
	EOS_CCR_TimedOut = 3,
	/** The connection was accepted, but the connection could not be created due to too many other existing connections */
	EOS_CCR_TooManyConnections = 4,
	/** The connection was accepted, The remote user sent an invalid message */
	EOS_CCR_InvalidMessage = 5,
	/** The connection was accepted, but the remote user sent us invalid data */
	EOS_CCR_InvalidData = 6,
	/** The connection was accepted, but we failed to ever establish a connection with the remote user due to connectivity issues. */
	EOS_CCR_ConnectionFailed = 7,
	/** The connection was accepted and established, but the peer silently went away. */
	EOS_CCR_ConnectionClosed = 8,
	/** The connection was locally accepted, but we failed to negotiate a connection with the remote user. This most commonly occurs if the local user goes offline or is logged-out during the connection process. */
	EOS_CCR_NegotiationFailed = 9,
	/** The connection was accepted, but there was an internal error occurred and the connection cannot be created or continue. */
	EOS_CCR_UnexpectedError = 10
};


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		STRUCTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

/**
 * P2P Socket ID
 *
 * The Socket ID contains an application-defined name for the connection between a local person and another peer.
 *
 * When a remote user receives a connection request from you, they will receive this information.  It can be important
 * to only accept connections with a known socket-name and/or from a known user, to prevent leaking of private
 * information, such as a user's IP address. Using the socket name as a secret key can help prevent such leaks. Shared
 * private data, like a private match's Session ID are good candidates for a socket name.
 */
USTRUCT(BlueprintType)
struct FEOSP2PSocketId
{
	GENERATED_BODY()
public:
	/** API Version. */	
	int32 ApiVersion;
public:
	/** A name for the connection. Must be a NULL-terminated string of between 1-32 alpha-numeric characters (A-Z, a-z, 0-9) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FString SocketName;
public:
	explicit FEOSP2PSocketId()
		: ApiVersion(EOS_P2P_SOCKETID_API_LATEST)
	{
	}

	FEOSP2PSocketId(const EOS_P2P_SocketId& Socket)
		: ApiVersion(EOS_P2P_SOCKETID_API_LATEST)
		, SocketName(Socket.SocketName)
	{
	}
};

/**
 * Structure containing information about the data being sent and to which player
 */
USTRUCT(BlueprintType)
struct FEOSP2PSendPacketOptions
{
	GENERATED_BODY()
public:
	/** Local User ID */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSProductUserId LocalUserId;
	/** The ID of the Peer you would like to send a packet to */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSProductUserId RemoteUserId;
	/** The socket ID for data you are sending in this packet */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSP2PSocketId SocketId;
	/** Channel associated with this data */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	int32 Channel;
	/** The size of the data to be sent to the RemoteUser */
	int32 DataLengthBytes;
	/** The data to be sent to the RemoteUser */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	TArray<uint8> Data;
	/** If false and we do not already have an established connection to the peer, this data will be dropped */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	bool bAllowDelayedDelivery;
	/** Setting to control the delivery reliability of this packet */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	EEOSEPacketReliability Reliability;
	/**
	 * If set to EOS_TRUE, EOS_P2P_SendPacket will not automatically establish a connection with the RemoteUserId and will require explicit calls to
	 * EOS_P2P_AcceptConnection first whenever the connection is closed. If set to EOS_FALSE, EOS_P2P_SendPacket will automatically accept and start
	 * the connection any time it is called and the connection is not already open.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	bool bDisableAutoAcceptConnection;
public:
	explicit FEOSP2PSendPacketOptions()
		: Channel(-1)
		, DataLengthBytes(0)
		, bAllowDelayedDelivery(false)
		, Reliability(EEOSEPacketReliability::EOS_PR_UnreliableUnordered)
		, bDisableAutoAcceptConnection(false)
	{
	}
};

/**
 * Structure containing information about who would like to receive a packet.
 */
USTRUCT(BlueprintType)
struct FEOSP2PGetNextReceivedPacketSizeOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** The user who is receiving the packet */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSProductUserId LocalUserId;
	/** An optional channel to request the data for. If NULL, we're retrieving the size of the next packet on any channel */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	int32 RequestedChannel;
public:
	explicit FEOSP2PGetNextReceivedPacketSizeOptions()
		: ApiVersion(EOS_P2P_GETNEXTRECEIVEDPACKETSIZE_API_LATEST)
		, RequestedChannel(-1)
	{
	}
};

/**
 * Structure containing information about who would like to receive a packet, and how much data can be stored safely.
 */
USTRUCT(BlueprintType)
struct FEOSP2PReceivePacketOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** The user who is receiving the packet */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSProductUserId LocalUserId;
	/** The maximum amount of data in bytes that can be safely copied to OutData in the function call */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	int32 MaxDataSizeBytes;
	/** An optional channel to request the data for. If NULL, we're retrieving the next packet on any channel */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	int32 RequestedChannel;
public:
	explicit FEOSP2PReceivePacketOptions()
		: ApiVersion(EOS_P2P_RECEIVEPACKET_API_LATEST)
		, MaxDataSizeBytes(4096)
		, RequestedChannel(-1)
	{
	}
};

/**
 * Structure containing information about who would like connection notifications, and about which socket.
 */
USTRUCT(BlueprintType)
struct FEOSP2PAddNotifyPeerConnectionRequestOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** The user who wishes to listen for incoming connection requests */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSProductUserId LocalUserId;
	/** (Optional) The socket ID to listen for, for incoming connection requests. If NULL, we listen for all requests */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSP2PSocketId SocketId;
public:
	explicit FEOSP2PAddNotifyPeerConnectionRequestOptions()
		: ApiVersion(EOS_P2P_ADDNOTIFYPEERCONNECTIONREQUEST_API_LATEST)
	{
	}
};

/**
 * Structure containing information about an incoming connection request.
 */
USTRUCT(BlueprintType)
struct FEOSP2POnIncomingConnectionRequestInfo
{
	GENERATED_BODY()
public:
	/** Client-specified data passed into EOS_Presence_AddNotifyOnPresenceChanged */
	void* ClientData;
	/** The local user who is being requested to open a P2P session with RemoteUserId */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSProductUserId LocalUserId;
	/** The remote user who requested a peer connection with the local user */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSProductUserId RemoteUserId;
	/** The ID of the socket the Remote User wishes to communicate on */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSP2PSocketId SocketId;
public:
	explicit FEOSP2POnIncomingConnectionRequestInfo()
		: ClientData(nullptr)
	{
	}

	FEOSP2POnIncomingConnectionRequestInfo(const EOS_P2P_OnIncomingConnectionRequestInfo& Data)
		: ClientData(Data.ClientData)
		, LocalUserId(Data.LocalUserId)
		, RemoteUserId(Data.RemoteUserId)
		, SocketId(*Data.SocketId)
	{
	}
};

/**
 * Structure containing information about an incoming connection request.
 */
USTRUCT(BlueprintType)
struct FEOSP2PAddNotifyPeerConnectionEstablishedOptions
{
	GENERATED_BODY()
public:
	/** The Product User ID of the local user who would like to receive notifications */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSProductUserId LocalUserId;
	/** The optional socket ID, used as a filter for established connections. If NULL, this function handler will be called for all sockets */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSP2PSocketId SocketId;
};

/**
 * Structure containing information about who would like notifications about interrupted connections, and for which socket.
 */
USTRUCT(BlueprintType)
struct FEOSP2PAddNotifyPeerConnectionInterruptedOptions
{
	GENERATED_BODY()
public:
	/** The Product User ID of the local user who would like notifications */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSProductUserId LocalUserId;
	/** An optional socket ID to filter interrupted connections on. If NULL, this function handler will be called for all interrupted connections */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSP2PSocketId SocketId;
};

/**
 * Structure containing information about who would like notifications about closed connections, and for which socket.
 */
USTRUCT(BlueprintType)
struct FEOSP2PAddNotifyPeerConnectionClosedOptions
{
	GENERATED_BODY()
public:
	/** The local user who would like notifications */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSProductUserId LocalUserId;
	/** (Optional) The socket ID to listen for to be closed. If NULL, this handler will be called for all closed connections */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSP2PSocketId SocketId;
};

/**
 * Structure containing information about an connection request that is being closed.
 */
USTRUCT(BlueprintType)
struct FEOSP2POnRemoteConnectionClosedInfo
{
	GENERATED_BODY()
public:
	/** Client-specified data passed into EOS_Presence_AddNotifyOnPresenceChanged */
	void* ClientData;
	/** The local user who is being notified of a connection being closed */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSProductUserId LocalUserId;
	/** The remote user who this connection was with */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSProductUserId RemoteUserId;
	/** The socket ID of the connection being closed */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSP2PSocketId SocketId;
	/** The reason the connection was closed (if known) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	EEOSEConnectionClosedReason Reason;
public:
	explicit FEOSP2POnRemoteConnectionClosedInfo()
		: ClientData(nullptr)
		, Reason(EEOSEConnectionClosedReason::EOS_CCR_Unknown)
	{
	}

	FEOSP2POnRemoteConnectionClosedInfo(const EOS_P2P_OnRemoteConnectionClosedInfo& data)
		: ClientData(data.ClientData)
		, LocalUserId(data.LocalUserId)
		, RemoteUserId(data.RemoteUserId)
		, SocketId(*data.SocketId)
		, Reason(static_cast<EEOSEConnectionClosedReason>(data.Reason))
	{
	}
};

/**
 * Structure containing information about who would like to accept a connection, and which connection.
 */
USTRUCT(BlueprintType)
struct FEOSP2PAcceptConnectionOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** The local user who is accepting any pending or future connections with RemoteUserId */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSProductUserId LocalUserId;
	/** The remote user who has either sent a connection request or will in the future */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSProductUserId RemoteUserId;
	/** The socket ID of the connection to accept on */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSP2PSocketId SocketId;
public:
	explicit FEOSP2PAcceptConnectionOptions()
		: ApiVersion(EOS_P2P_ACCEPTCONNECTION_API_LATEST)
	{
	}
};

/**
 * Structure containing information about who would like to close a connection, and which connection.
 */
USTRUCT(BlueprintType)
struct FEOSP2PCloseConnectionOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** The local user who would like to close a previously accepted connection (or decline a pending invite) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSProductUserId LocalUserId;
	/** The remote user to disconnect from (or to reject a pending invite from) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSProductUserId RemoteUserId;
	/** The socket ID of the connection to close (or optionally NULL to not accept any connection requests from the Remote User) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSP2PSocketId SocketId;
public:
	explicit FEOSP2PCloseConnectionOptions()
		: ApiVersion(EOS_P2P_CLOSECONNECTION_API_LATEST)
	{
	}
};

/**
 * Structure containing information about who would like to close connections, and by what socket ID
 */
USTRUCT(BlueprintType)
struct FEOSP2PCloseConnectionsOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** The local user who would like to close all connections that use a particular socket ID */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSProductUserId LocalUserId;
	/** The socket ID of the connections to close */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSP2PSocketId SocketId;
public:
	explicit FEOSP2PCloseConnectionsOptions()
		: ApiVersion(EOS_P2P_CLOSECONNECTIONS_API_LATEST)
	{
	}
};

/**
 * Structure containing information needed to query NAT-types
 */
USTRUCT(BlueprintType)
struct FEOSP2PQueryNATTypeOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	explicit FEOSP2PQueryNATTypeOptions()
		: ApiVersion(EOS_P2P_QUERYNATTYPE_API_LATEST)
	{
	}
};

/**
 * Structure containing information about the local network NAT type
 */
USTRUCT(BlueprintType)
struct FEOSP2POnQueryNATTypeCompleteInfo
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Result code for the operation. EOS_Success is returned for a successful query, other codes indicate an error */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	EOSResult ResultCode;
	/** Client-specified data passed into EOS_P2P_QueryNATType */
	void* ClientData;
	/** The queried NAT type */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	EEOSENATType NATType;
public:
	explicit FEOSP2POnQueryNATTypeCompleteInfo()
		: ApiVersion(EOS_P2P_CLOSECONNECTIONS_API_LATEST)
		, ResultCode(EOSResult::EOS_ServiceFailure)
		, ClientData(nullptr)
		, NATType(EEOSENATType::EOS_NAT_Unknown)
	{
	}

	FEOSP2POnQueryNATTypeCompleteInfo(const EOS_P2P_OnQueryNATTypeCompleteInfo& data)
		: ApiVersion(EOS_P2P_CLOSECONNECTIONS_API_LATEST)
		, ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		, ClientData(data.ClientData)
		, NATType(static_cast<EEOSENATType>(data.NATType))
	{
	}
};

/**
 * Structure containing information needed to get perviously queried NAT-types
 */
USTRUCT(BlueprintType)
struct FEOSP2PGetNATTypeOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	explicit FEOSP2PGetNATTypeOptions()
		: ApiVersion(EOS_P2P_GETNATTYPE_API_LATEST)
	{
	}
};

/**
 * Structure containing information about new relay configurations.
 */
USTRUCT(BlueprintType)
struct FEOSP2PSetRelayControlOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/**
	 * The requested level of relay servers for P2P connections. This setting is only applied to new P2P connections, or when existing P2P connections
	 * reconnect during a temporary connectivity outage. Peers with an incompatible setting to the local setting will not be able to connnect.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	EEOSERelayControl RelayControl;
public:
	explicit FEOSP2PSetRelayControlOptions()
		: ApiVersion(EOS_P2P_SETRELAYCONTROL_API_LATEST)
		, RelayControl(EEOSERelayControl::EOS_RC_AllowRelays)
	{
	}
};

/**
 * Structure containing information about getting the relay control setting.
 */
USTRUCT(BlueprintType)
struct FEOSP2PGetRelayControlOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	explicit FEOSP2PGetRelayControlOptions()
		: ApiVersion(EOS_P2P_GETRELAYCONTROL_API_LATEST)
	{
	}
};

/**
 * Structure containing information about new port range settings.
 */
USTRUCT(BlueprintType)
struct FEOSP2PSetPortRangeOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** The ideal port to use for P2P traffic. The default value is 7777. If set to 0, the OS will choose a port. If set to 0, MaxAdditionalPortsToTry must be set to 0. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	int32 Port;
	/**
	 * The maximum amount of additional ports to try if Port is unavailable. Ports will be tried from Port to Port + MaxAdditionalPortsToTry
	 * inclusive, until one is available or we run out of ports. If no ports are available, P2P connections will fail. The default value is 99.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	int32 MaxAdditionalPortsToTry;
public:
	explicit FEOSP2PSetPortRangeOptions()
		: ApiVersion(EOS_P2P_SETPORTRANGE_API_LATEST)
		, Port(7777)
		, MaxAdditionalPortsToTry(99)
	{
	}
};

/**
 * Structure containing information about getting the configured port range settings.
 */
USTRUCT(BlueprintType)
struct FEOSP2PGetPortRangeOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	explicit FEOSP2PGetPortRangeOptions()
		: ApiVersion(EOS_P2P_GETPORTRANGE_API_LATEST)
	{
	}
};

/**
* Structure containing information about new packet queue size settings.
*/
USTRUCT(BlueprintType)
struct FEOSP2PSetPacketQueueSizeOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** The ideal maximum amount of bytes the Incoming packet queue can consume */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FString IncomingPacketQueueMaxSizeBytes;
	/** The ideal maximum amount of bytes the Outgoing packet queue can consume */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FString OutgoingPacketQueueMaxSizeBytes;
public:
	explicit FEOSP2PSetPacketQueueSizeOptions()
		: ApiVersion(EOS_P2P_SETPACKETQUEUESIZE_API_LATEST)
	{
	}
};

/**
* Structure containing information needed to get the current packet queue information.
*/
USTRUCT(BlueprintType)
struct FEOSP2PGetPacketQueueInfoOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	explicit FEOSP2PGetPacketQueueInfoOptions()
		: ApiVersion(EOS_P2P_GETPACKETQUEUEINFO_API_LATEST)
	{
	}
};

/**
* Information related to the current state of the packet queues. It is possible for the current size
* to be larger than the maximum size if the maximum size changes or if the maximum queue size is
* set to EOS_P2P_MAX_QUEUE_SIZE_UNLIMITED.
*/
USTRUCT(BlueprintType)
struct FEOSP2PPacketQueueInfo
{
	GENERATED_BODY()
public:
	/** The maximum size in bytes of the incoming packet queue */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FString IncomingPacketQueueMaxSizeBytes;
	/** The current size in bytes of the incoming packet queue */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FString IncomingPacketQueueCurrentSizeBytes;
	/** The current number of queued packets in the incoming packet queue */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FString IncomingPacketQueueCurrentPacketCount;
	/** The maximum size in bytes of the outgoing packet queue */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FString OutgoingPacketQueueMaxSizeBytes;
	/** The current size in bytes of the outgoing packet queue */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FString OutgoingPacketQueueCurrentSizeBytes;
	/** The current amount of queued packets in the outgoing packet queue */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FString OutgoingPacketQueueCurrentPacketCount;
public:
	explicit FEOSP2PPacketQueueInfo()
	{
	}

	FEOSP2PPacketQueueInfo(const EOS_P2P_PacketQueueInfo& data)
		: IncomingPacketQueueMaxSizeBytes(LexToString(data.IncomingPacketQueueMaxSizeBytes))
		, IncomingPacketQueueCurrentSizeBytes(LexToString(data.IncomingPacketQueueCurrentSizeBytes))
		, IncomingPacketQueueCurrentPacketCount(LexToString(data.IncomingPacketQueueCurrentPacketCount))
		, OutgoingPacketQueueMaxSizeBytes(LexToString(data.OutgoingPacketQueueMaxSizeBytes))
		, OutgoingPacketQueueCurrentSizeBytes(LexToString(data.OutgoingPacketQueueCurrentSizeBytes))
		, OutgoingPacketQueueCurrentPacketCount(LexToString(data.OutgoingPacketQueueCurrentPacketCount))
	{
	}
};

/**
* Structure containing information about what version of the EOS_P2P_AddNotifyIncomingPacketQueueFull function is supported.
*/
USTRUCT(BlueprintType)
struct FEOSP2PAddNotifyIncomingPacketQueueFullOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	explicit FEOSP2PAddNotifyIncomingPacketQueueFullOptions()
		: ApiVersion(EOS_P2P_ADDNOTIFYINCOMINGPACKETQUEUEFULL_API_LATEST)
	{
	}
};

/**
* Structure containing information about the local network NAT type
*/
USTRUCT(BlueprintType)
struct FEOSP2POnIncomingPacketQueueFullInfo
{
	GENERATED_BODY()
public:
	/** Client-specified data passed into AddNotifyIncomingPacketQueueFull */
	void* ClientData;
	/** The maximum size in bytes the incoming packet queue is allowed to use */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FString PacketQueueMaxSizeBytes;
	/** The current size in bytes the incoming packet queue is currently using */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FString PacketQueueCurrentSizeBytes;
	/** The Product User ID of the local user who is receiving the packet that would overflow the queue */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSProductUserId OverflowPacketLocalUserId;
	/** The channel the incoming packet is for */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	int32 OverflowPacketChannel;
	/** The size in bytes of the incoming packet (and related metadata) that would overflow the queue */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	int32 OverflowPacketSizeBytes;
public:
	explicit FEOSP2POnIncomingPacketQueueFullInfo()
		: ClientData(nullptr)
		, OverflowPacketChannel(0)
		, OverflowPacketSizeBytes(0)
	{
	}

	FEOSP2POnIncomingPacketQueueFullInfo(const EOS_P2P_OnIncomingPacketQueueFullInfo& Data)
		: ClientData(Data.ClientData)
		, PacketQueueMaxSizeBytes(LexToString(Data.PacketQueueMaxSizeBytes))
		, PacketQueueCurrentSizeBytes(LexToString(Data.PacketQueueCurrentSizeBytes))
		, OverflowPacketLocalUserId(Data.OverflowPacketLocalUserId)
		, OverflowPacketChannel(Data.OverflowPacketChannel)
		, OverflowPacketSizeBytes(Data.OverflowPacketSizeBytes)
	{
	}
};

/**
 * Structure containing information about a connection being established
 */
USTRUCT(BlueprintType)
struct FEOSP2POnPeerConnectionEstablishedInfo
{
	GENERATED_BODY()
public:
	/** The Product User ID of the local user who is being notified of a connection being established */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSProductUserId LocalUserId;
	/** The Product User ID of the remote user who this connection was with */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSProductUserId RemoteUserId;
	/** The socket ID of the connection being established */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSP2PSocketId SocketId;
	/** Information if this is a new connection or reconnection */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	EEOSEConnectionEstablishedType ConnectionType;
	/** What type of network connection is being used for this connection */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	EEOSENetworkConnectionType NetworkType;
public:
	explicit FEOSP2POnPeerConnectionEstablishedInfo()
		: ConnectionType(EEOSEConnectionEstablishedType::EOS_CET_Reconnection)
		, NetworkType(EEOSENetworkConnectionType::EOS_NCT_NoConnection)
	{
	}

	FEOSP2POnPeerConnectionEstablishedInfo(const EOS_P2P_OnPeerConnectionEstablishedInfo& Data)
		: LocalUserId(Data.LocalUserId)
		, RemoteUserId(Data.RemoteUserId)
		, SocketId(*Data.SocketId)
		, ConnectionType(static_cast<EEOSEConnectionEstablishedType>(Data.ConnectionType))
		, NetworkType(static_cast<EEOSENetworkConnectionType>(Data.NetworkType))
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSP2POnPeerConnectionInterruptedInfo
{
	GENERATED_BODY()
public:
	void* ClientData;
	/** The local user who is being notified of a connection that was interrupted */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSProductUserId LocalUserId;
	/** The Product User ID of the remote user who this connection was with */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSProductUserId RemoteUserId;
	/** The socket ID of the connection that was interrupted */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "P2P")
	FEOSP2PSocketId SocketId;
public:
	explicit FEOSP2POnPeerConnectionInterruptedInfo()
		: ClientData(nullptr)
	{
	}

	FEOSP2POnPeerConnectionInterruptedInfo(const EOS_P2P_OnPeerConnectionInterruptedInfo& Data)
		: ClientData(Data.ClientData)
		, LocalUserId(Data.LocalUserId)
		, RemoteUserId(Data.RemoteUserId)
		, SocketId(*Data.SocketId)
	{
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		DELEGATES
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnP2PIncomingConnectionRequestCallback, const FEOSP2POnIncomingConnectionRequestInfo&, Data, bool, bWasSuccessful);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnP2PRemoteConnectionClosedCallback, const FEOSP2POnRemoteConnectionClosedInfo&, Data, bool, bWasSuccessful);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnP2PQueryNATTypeCompleteCallback, const FEOSP2POnQueryNATTypeCompleteInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnP2POnIncomingPacketQueueFullCallback, const FEOSP2POnIncomingPacketQueueFullInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnP2OnPeerConnectionEstablishedCallback, const FEOSP2POnPeerConnectionEstablishedInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnP2OnPeerConnectionInterruptedCallback, const FEOSP2POnPeerConnectionInterruptedInfo&, Data);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnP2PIncomingConnectionRequestCallbackDelegate, const FEOSP2POnIncomingConnectionRequestInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnP2PRemoteConnectionClosedCallbackDelegate, const FEOSP2POnRemoteConnectionClosedInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnP2PQueryNATTypeCompleteCallbackDelegate, const FEOSP2POnQueryNATTypeCompleteInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnP2POnPeerConnectionEstablishedCallbackDelegate, const FEOSP2POnPeerConnectionEstablishedInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnP2POnPeerConnectionInterruptedCallbackDelegate, const FEOSP2POnPeerConnectionInterruptedInfo&, Data);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		CALLBACK OBJECTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
struct FIncomingPacketQueueFullCallback
{
public:
	FIncomingPacketQueueFullCallback(const FOnP2POnIncomingPacketQueueFullCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FIncomingPacketQueueFullCallback()
	{
		m_Callback.Unbind();
	}

public:
	FOnP2POnIncomingPacketQueueFullCallback m_Callback;
};

struct FPeerConnectionInterruptedCallback
{
public:
	FPeerConnectionInterruptedCallback(const FOnP2OnPeerConnectionInterruptedCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FPeerConnectionInterruptedCallback()
	{
		m_Callback.Unbind();
	}

public:
	FOnP2OnPeerConnectionInterruptedCallback m_Callback;
};

struct FPeerConnectionEstablishedCallback
{
public:
	FPeerConnectionEstablishedCallback(const FOnP2OnPeerConnectionEstablishedCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FPeerConnectionEstablishedCallback()
	{
		m_Callback.Unbind();
	}

public:
	FOnP2OnPeerConnectionEstablishedCallback m_Callback;
};