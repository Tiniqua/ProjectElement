/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "eos_custominvites_types.h"
#include "Core/EOSTypes.h"
#include "Core/EOSHelpers.h"
#include "EOSCustomInvitesTypes.generated.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		STRUCTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

USTRUCT(BlueprintType)
struct FEOSCustomInvitesSetCustomInviteOptions
{
	GENERATED_BODY()
public:
	/** Local user creating / sending a Custom Invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId LocalUserId;
	/** String payload for the Custom Invite (must be less than EOS_CUSTOMINVITES_MAX_PAYLOAD_LENGTH) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FString Payload;
public:
	explicit FEOSCustomInvitesSetCustomInviteOptions() = default;
};

USTRUCT(BlueprintType)
struct FEOSCustomInvitesSendCustomInviteOptions
{
	GENERATED_BODY()
public:
	/** Local user sending a CustomInvite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId LocalUserId;
	/** Users to whom the invites should be sent */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	TArray<FEOSProductUserId> TargetUserIds;
public:
	explicit FEOSCustomInvitesSendCustomInviteOptions() = default;
};

USTRUCT(BlueprintType)
struct FEOSCustomInvitesAddNotifyCustomInviteReceivedOptions
{
	GENERATED_BODY()
public:
	explicit FEOSCustomInvitesAddNotifyCustomInviteReceivedOptions() = default;
};

/**
 * Output parameters for the EOS_CustomInvites_OnCustomInviteReceivedCallback Function.
 */
USTRUCT(BlueprintType)
struct FEOSCustomInvitesOnCustomInviteReceivedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Context that was passed into EOS_CustomInvites_AddNotifyCustomInviteReceived */
	void* ClientData;
	/** User that sent this custom invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId TargetUserId;
	/** Recipient Local user id */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId LocalUserId;
	/** Id of the received Custom Invite*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FString CustomInviteId;
	/** Payload of the received Custom Invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FString Payload;
public:
	explicit FEOSCustomInvitesOnCustomInviteReceivedCallbackInfo()
		: ClientData(nullptr)
	{
	}

	FEOSCustomInvitesOnCustomInviteReceivedCallbackInfo(const EOS_CustomInvites_OnCustomInviteReceivedCallbackInfo& data)
		: ClientData(data.ClientData)
		, TargetUserId(data.TargetUserId)
		, LocalUserId(data.LocalUserId)
	{
		const FUTF8ToTCHAR CustomInviteIdChar(data.CustomInviteId);
		CustomInviteId = CustomInviteIdChar.Get();

		const FUTF8ToTCHAR PayloadChar(data.Payload);
		Payload = PayloadChar.Get();
	}
};

USTRUCT(BlueprintType)
struct FEOSCustomInvitesAddNotifyCustomInviteAcceptedOptions
{
	GENERATED_BODY()
public:
	explicit FEOSCustomInvitesAddNotifyCustomInviteAcceptedOptions() = default;
};

/**
 * Output parameters for the EOS_CustomInvites_OnCustomInviteReceivedCallback Function.
 */
USTRUCT(BlueprintType)
struct FEOSCustomInvitesOnCustomInviteAcceptedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Context that was passed into EOS_CustomInvites_AddNotifyCustomInviteAccepted */
	void* ClientData;
	/** User that sent this custom invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId TargetUserId;
	/** Recipient Local user id */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId LocalUserId;
	/** Id of the received Custom Invite*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FString CustomInviteId;
	/** Payload of the received Custom Invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FString Payload;
public:
	explicit FEOSCustomInvitesOnCustomInviteAcceptedCallbackInfo()
		: ClientData(nullptr)
	{
	}

	FEOSCustomInvitesOnCustomInviteAcceptedCallbackInfo(const EOS_CustomInvites_OnCustomInviteAcceptedCallbackInfo& data)
		: ClientData(data.ClientData)
		, TargetUserId(data.TargetUserId)
		, LocalUserId(data.LocalUserId)
	{
		const FUTF8ToTCHAR CustomInviteIdChar(data.CustomInviteId);
		CustomInviteId = CustomInviteIdChar.Get();

		const FUTF8ToTCHAR PayloadChar(data.Payload);
		Payload = PayloadChar.Get();
	}
};

USTRUCT(BlueprintType)
struct FEOSCustomInvitesAddNotifyCustomInviteRejectedOptions
{
	GENERATED_BODY()
public:
	explicit FEOSCustomInvitesAddNotifyCustomInviteRejectedOptions() = default;
};

/**
 * Output parameters for the EOS_CustomInvites_CustomInviteRejectedCallback Function.
 */
USTRUCT(BlueprintType)
struct FEOSCustomInvitesCustomInviteRejectedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Context that was passed into EOS_CustomInvites_AddNotifyCustomInviteAccepted */
	void* ClientData;
	/** User that sent the custom invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId TargetUserId;
	/** Recipient Local user id */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId LocalUserId;
	/** Id of the rejected Custom Invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FString CustomInviteId;
	/** Payload of the rejected Custom Invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FString Payload;
public:
	explicit FEOSCustomInvitesCustomInviteRejectedCallbackInfo()
		: ClientData(nullptr)
	{
	}

	FEOSCustomInvitesCustomInviteRejectedCallbackInfo(const EOS_CustomInvites_CustomInviteRejectedCallbackInfo& data)
		: ClientData(data.ClientData)
		, TargetUserId(data.TargetUserId)
		, LocalUserId(data.LocalUserId)
	{
		const FUTF8ToTCHAR CustomInviteIdChar(data.CustomInviteId);
		CustomInviteId = CustomInviteIdChar.Get();

		const FUTF8ToTCHAR PayloadChar(data.Payload);
		Payload = PayloadChar.Get();
	}
};

/**
 * Output parameters for the EOS_CustomInvites_SendCustomInvite Function. These parameters are received through the callback provided to EOS_CustomInvites_SendCustomInvite
 */
USTRUCT(BlueprintType)
struct FEOSCustomInvitesOnSendCustomInviteCallbackInfo
{
	GENERATED_BODY()
public:
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	EOSResult ResultCode;
	/** Context that was passed into EOS_CustomInvites_SendCustomInvite */
	void* ClientData;
	/** Local user sending a CustomInvite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId LocalUserId;
	/** Users to whom the invites were successfully sent (can be different than original call if an invite for same Payload was previously sent) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	TArray<FEOSProductUserId> TargetUserIds;
public:
	explicit FEOSCustomInvitesOnSendCustomInviteCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
		, ClientData(nullptr)
	{
	}

	FEOSCustomInvitesOnSendCustomInviteCallbackInfo(const EOS_CustomInvites_SendCustomInviteCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		, ClientData(data.ClientData)
		, LocalUserId(data.LocalUserId)
	{
		for (uint32_t i=0; i<data.TargetUserIdsCount; i++)
		{
			TargetUserIds.Add(data.TargetUserIds[i]);
		}
	}
};

USTRUCT(BlueprintType)
struct FEOSCustomInvitesFinalizeInviteOptions
{
	GENERATED_BODY()
public:
	/** User that sent the custom invite */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId TargetUserId;
	/** Recipient Local user id */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FEOSProductUserId LocalUserId;
	/** Id of the Custom Invite accepted */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	FString CustomInviteId;
	/** Result of the Processing operation, transmitted to Social Overlay if applicable */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomInvites")
	EOSResult ProcessingResult;
public:
	FEOSCustomInvitesFinalizeInviteOptions()
		: ProcessingResult(EOSResult::MAX)
	{}
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCustomInvitesOnSendCustomInviteCallback, const FEOSCustomInvitesOnSendCustomInviteCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCustomInvitesOnCustomInviteReceivedCallback, const FEOSCustomInvitesOnCustomInviteReceivedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCustomInvitesOnCustomInviteAcceptedCallback, const FEOSCustomInvitesOnCustomInviteAcceptedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCustomInvitesOnCustomInviteRejectedCallback, const FEOSCustomInvitesCustomInviteRejectedCallbackInfo&, Data);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		CALLBACK OBJECTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
struct FCustomInvitesSendCustomInviteCallback
{
public:
	FOnCustomInvitesOnSendCustomInviteCallback m_Callback;
public:
	FCustomInvitesSendCustomInviteCallback(const FOnCustomInvitesOnSendCustomInviteCallback& callback)
		: m_Callback(callback)
	{
	}
	virtual ~FCustomInvitesSendCustomInviteCallback()
	{
		m_Callback.Unbind();
	}
};

struct FCustomInvitesCustomInviteReceivedCallback
{
public:
	FOnCustomInvitesOnCustomInviteReceivedCallback m_Callback;
public:
	FCustomInvitesCustomInviteReceivedCallback(const FOnCustomInvitesOnCustomInviteReceivedCallback& callback)
		: m_Callback(callback)
	{
	}
	virtual ~FCustomInvitesCustomInviteReceivedCallback()
	{
		m_Callback.Unbind();
	}
};

struct FCustomInvitesCustomInviteAcceptedCallback
{
public:
	FOnCustomInvitesOnCustomInviteAcceptedCallback m_Callback;
public:
	FCustomInvitesCustomInviteAcceptedCallback(const FOnCustomInvitesOnCustomInviteAcceptedCallback& callback)
		: m_Callback(callback)
	{
	}
	virtual ~FCustomInvitesCustomInviteAcceptedCallback()
	{
		m_Callback.Unbind();
	}
};

struct FCustomInvitesOnCustomInviteRejectedCallback
{
public:
	FOnCustomInvitesOnCustomInviteRejectedCallback m_Callback;
public:
	FCustomInvitesOnCustomInviteRejectedCallback(const FOnCustomInvitesOnCustomInviteRejectedCallback& callback)
		: m_Callback(callback)
	{
	}
	virtual ~FCustomInvitesOnCustomInviteRejectedCallback()
	{
		m_Callback.Unbind();
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		Operations
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
struct FCustomInvitesSendCustomInvite : public EOS_CustomInvites_SendCustomInviteOptions
{
	TArray<EOS_ProductUserId> PointerArray;
	
	FCustomInvitesSendCustomInvite(EOS_ProductUserId InLocalUserId, TArray<FEOSProductUserId>& InTargetUserIds)
		: EOS_CustomInvites_SendCustomInviteOptions()
		, PointerArray(MoveTemp(InTargetUserIds))
	{
		ApiVersion = EOS_CUSTOMINVITES_SETCUSTOMINVITE_API_LATEST;
		LocalUserId = InLocalUserId;

		TargetUserIds = PointerArray.GetData();
		TargetUserIdsCount = PointerArray.Num();
	}
};
