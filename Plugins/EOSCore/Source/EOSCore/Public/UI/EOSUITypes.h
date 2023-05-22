/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Core/EOSHelpers.h"
#include "eos_ui_types.h"
#include "EOSUITypes.generated.h"

class UCoreUI;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		ENUMS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

UENUM(BlueprintType)
enum class EEOSUIENotificationLocation : uint8
{
	EOS_UNL_TopLeft,
	EOS_UNL_TopRight,
	EOS_UNL_BottomLeft,
	EOS_UNL_BottomRight
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		STRUCTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

/**
 * Input parameters for the EOS_UI_ShowFriends Function.
 */
USTRUCT(BlueprintType)
struct FEOSUIShowFriendsOptions
{
	GENERATED_BODY()
public:
	/** Account ID of the user whose friend list is being shown. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	FEOSEpicAccountId LocalUserId;
public:
	explicit FEOSUIShowFriendsOptions()
	{
	}
};

/**
 * Output parameters for the EOS_UI_ShowFriends function.
 */
USTRUCT(BlueprintType)
struct FEOSUIShowFriendsCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	EOSResult ResultCode;
	/** Context that was passed into EOS_UI_ShowFriends */
	void* ClientData;
	/** Account ID of the user whose friend list is being shown. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	FEOSEpicAccountId LocalUserId;
public:
	explicit FEOSUIShowFriendsCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
	{
	}

	FEOSUIShowFriendsCallbackInfo(const EOS_UI_ShowFriendsCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
	{
	}
};

/**
 * Input parameters for the EOS_UI_HideFriends Function.
 */
USTRUCT(BlueprintType)
struct FEOSUIHideFriendsOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Account ID of the user whose friend list is being shown. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	FEOSEpicAccountId LocalUserId;
public:
	explicit FEOSUIHideFriendsOptions()
		: ApiVersion(EOS_UI_HIDEFRIENDS_API_LATEST)
	{
	}

	FEOSUIHideFriendsOptions(const EOS_UI_HideFriendsOptions& data)
		: ApiVersion(EOS_UI_HIDEFRIENDS_API_LATEST)
		  , LocalUserId(data.LocalUserId)
	{
	}
};

/**
 * Output parameters for the EOS_UI_HideFriends function.
 */
USTRUCT(BlueprintType)
struct FEOSUIHideFriendsCallbackInfo
{
	GENERATED_BODY()
public:
	/** Result code for the operation. EOS_Success is returned for a successful operation, otherwise one of the error codes is returned. See eos_common.h */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	EOSResult ResultCode;
	/** Context that was passed into EOS_UI_HideFriends */
	void* ClientData;
	/** Account ID of the user whose friend list is being shown. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	FEOSEpicAccountId LocalUserId;
public:
	explicit FEOSUIHideFriendsCallbackInfo()
		: ResultCode(EOSResult::EOS_ServiceFailure)
	{
	}

	FEOSUIHideFriendsCallbackInfo(const EOS_UI_HideFriendsCallbackInfo& data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(data.ResultCode))
		  , ClientData(data.ClientData)
		  , LocalUserId(data.LocalUserId)
	{
	}
};

/**
 * Input parameters for the EOS_UI_GetFriendsVisible Function.
 */
USTRUCT(BlueprintType)
struct FEOSUIGetFriendsVisibleOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Account ID of the user whose overlay is being updated. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	FEOSEpicAccountId LocalUserId;
public:
	explicit FEOSUIGetFriendsVisibleOptions()
		: ApiVersion(EOS_UI_GETFRIENDSVISIBLE_API_LATEST)
	{
	}

	FEOSUIGetFriendsVisibleOptions(const EOS_UI_GetFriendsVisibleOptions& data)
		: ApiVersion(EOS_UI_GETFRIENDSVISIBLE_API_LATEST)
		  , LocalUserId(data.LocalUserId)
	{
	}
};

/**
 * Input parameters for the EOS_UI_GetFriendsVisible Function.
 */
USTRUCT(BlueprintType)
struct FEOSUIGetFriendsExclusiveInputOptions
{
	GENERATED_BODY()
public:
	/** The Epic Account ID of the user whose overlay is being checked. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	FEOSEpicAccountId LocalUserId;
public:
	FEOSUIGetFriendsExclusiveInputOptions()
	{
	}
};

/**
 * Input parameters for the EOS_UI_SetToggleFriendsKey Function.
 */
USTRUCT(BlueprintType)
struct FEOSUISetToggleFriendsKeyOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/**
	 * The new key combination which will be used to toggle the friends overlay.
	 * The combination can be any set of modifiers and one key.
	 * A value of EOS_UIK_None will cause the key to revert to the default.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	int32 keyCombination;
public:
	explicit FEOSUISetToggleFriendsKeyOptions()
		: ApiVersion(EOS_UI_SETTOGGLEFRIENDSKEY_API_LATEST)
		  , keyCombination(0)
	{
	}

	FEOSUISetToggleFriendsKeyOptions(const EOS_UI_SetToggleFriendsKeyOptions& data)
		: ApiVersion(EOS_UI_SETTOGGLEFRIENDSKEY_API_LATEST)
	{
	}
};

/**
 * Input parameters for the EOS_UI_GetToggleFriendsKey Function.
 */
USTRUCT(BlueprintType)
struct FEOSUIGetToggleFriendsKeyOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	explicit FEOSUIGetToggleFriendsKeyOptions()
		: ApiVersion(EOS_UI_GETTOGGLEFRIENDSKEY_API_LATEST)
	{
	}

	FEOSUIGetToggleFriendsKeyOptions(const EOS_UI_GetToggleFriendsKeyOptions& data)
		: ApiVersion(EOS_UI_GETTOGGLEFRIENDSKEY_API_LATEST)
	{
	}
};

/**
 * Input parameters for the EOS_UI_SetDisplayPreference function.
 */
USTRUCT(BlueprintType)
struct FEOSUISetDisplayPreferenceOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** Preference for notification pop-up locations. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	EEOSUIENotificationLocation NotificationLocation;
public:
	explicit FEOSUISetDisplayPreferenceOptions()
		: ApiVersion(EOS_UI_SETDISPLAYPREFERENCE_API_LATEST)
		  , NotificationLocation(EEOSUIENotificationLocation::EOS_UNL_TopRight)
	{
	}

	FEOSUISetDisplayPreferenceOptions(const EOS_UI_SetDisplayPreferenceOptions& data)
		: ApiVersion(EOS_UI_SETDISPLAYPREFERENCE_API_LATEST)
		  , NotificationLocation(static_cast<EEOSUIENotificationLocation>(data.NotificationLocation))
	{
	}
};

/**
 * Input parameters for the EOS_UI_AcknowledgeEventId.
 */
USTRUCT(BlueprintType)
struct FEOSUIAcknowledgeEventIdOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	/** The ID being acknowledged. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	FEOSUIEventId UiEventId;
	/**
	 * The result to use for the acknowledgment.
	 * When acknowledging EOS_Presence_JoinGameAcceptedCallbackInfo this should be the
	 * result code from the JoinSession call.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	EOSResult Result;
public:
	explicit FEOSUIAcknowledgeEventIdOptions()
		: ApiVersion(EOS_UI_ACKNOWLEDGECORRELATIONID_API_LATEST)
		  , Result(EOSResult::EOS_ServiceFailure)
	{
	}

	FEOSUIAcknowledgeEventIdOptions(const EOS_UI_AcknowledgeEventIdOptions& data)
		: ApiVersion(EOS_UI_ACKNOWLEDGECORRELATIONID_API_LATEST)
		  , UiEventId(data.UiEventId)
		  , Result(EOSHelpers::ToEOSCoreResult(data.Result))
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSUIAddNotifyDisplaySettingsUpdatedOptions
{
	GENERATED_BODY()
public:
	/** API Version. */
	int32 ApiVersion;
public:
	explicit FEOSUIAddNotifyDisplaySettingsUpdatedOptions()
		: ApiVersion(1)
	{
	}
};

USTRUCT(BlueprintType)
struct FEOSUIOnDisplaySettingsUpdatedCallbackInfo
{
	GENERATED_BODY()
public:
	/** Context that was passed into EOS_UI_AddNotifyDisplaySettingsUpdated */
	void* ClientData;
	/** True when any portion of the overlay is visible. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	bool bIsVisible;
	/**
	 * True when the overlay has switched to exclusive input mode.
	 * While in exclusive input mode, no keyboard or mouse input will be sent to the game.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	bool bIsExclusiveInput;
public:
	explicit FEOSUIOnDisplaySettingsUpdatedCallbackInfo()
		: ClientData(nullptr)
		  , bIsVisible(false)
		  , bIsExclusiveInput(false)
	{
	}

	FEOSUIOnDisplaySettingsUpdatedCallbackInfo(const EOS_UI_OnDisplaySettingsUpdatedCallbackInfo& data)
		: ClientData(data.ClientData)
		  , bIsVisible(data.bIsVisible > 0)
		  , bIsExclusiveInput(data.bIsExclusiveInput > 0)
	{
	}
};

/**
 * Parameters for the EOS_UI_ShowBlockPlayer function.
 */
USTRUCT(BlueprintType)
struct FEOSUIShowBlockPlayerOptions
{
	GENERATED_BODY()
public:
	/** The Epic Online Services Account ID of the user who is requesting the Block. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	FEOSEpicAccountId LocalUserId;
	/** The Epic Online Services Account ID of the user whose is being Blocked. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	FEOSEpicAccountId TargetUserId;
public:
	FEOSUIShowBlockPlayerOptions()
	{
	}
};

/**
 * Output parameters for the EOS_UI_ShowBlockPlayer function.
 */
USTRUCT(BlueprintType)
struct FEOSUIOnShowBlockPlayerCallbackInfo
{
	GENERATED_BODY()
public:
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	EOSResult ResultCode;
	/** Context that was passed into EOS_UI_ShowBlockPlayer */
	void* ClientData;
	/** The Epic Online Services Account ID of the user who requested the block. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	FEOSEpicAccountId LocalUserId;
	/** The Epic Online Services Account ID of the user who was to be blocked. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	FEOSEpicAccountId TargetUserId;
public:
	FEOSUIOnShowBlockPlayerCallbackInfo()
		: ResultCode(EOSResult::MAX)
		  , ClientData(nullptr)
	{
	}

	FEOSUIOnShowBlockPlayerCallbackInfo(const EOS_UI_OnShowBlockPlayerCallbackInfo& Data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(Data.ResultCode))
		  , ClientData(Data.ClientData)
		  , LocalUserId(Data.LocalUserId)
		  , TargetUserId(Data.TargetUserId)
	{
	}
};

/**
 * Parameters for the EOS_UI_ShowReportPlayer function.
 */
USTRUCT(BlueprintType)
struct FEOSUIShowReportPlayerOptions
{
	GENERATED_BODY()
public:
	/** The Epic Online Services Account ID of the user who is requesting the Report. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	FEOSEpicAccountId LocalUserId;
	/** The Epic Online Services Account ID of the user whose is being Reported. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	FEOSEpicAccountId TargetUserId;
public:
	FEOSUIShowReportPlayerOptions()
	{
	}
};

/**
 * Output parameters for the EOS_UI_ShowReportPlayer function.
 */
USTRUCT(BlueprintType)
struct FEOSUIOnShowReportPlayerCallbackInfo
{
	GENERATED_BODY()
public:
	/** The EOS_EResult code for the operation. EOS_Success indicates that the operation succeeded; other codes indicate errors. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	EOSResult ResultCode;
	/** Context that was passed into EOS_UI_ShowReportPlayer */
	void* ClientData;
	/** The Epic Online Services Account ID of the user who requested the Report. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	FEOSEpicAccountId LocalUserId;
	/** The Epic Online Services Account ID of the user who was to be Reported. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	FEOSEpicAccountId TargetUserId;
public:
	FEOSUIOnShowReportPlayerCallbackInfo()
		: ResultCode(EOSResult::MAX)
		  , ClientData(nullptr)
	{
	}

	FEOSUIOnShowReportPlayerCallbackInfo(const EOS_UI_OnShowReportPlayerCallbackInfo& Data)
		: ResultCode(EOSHelpers::ToEOSCoreResult(Data.ResultCode))
		  , ClientData(Data.ClientData)
		  , LocalUserId(Data.LocalUserId)
		  , TargetUserId(Data.TargetUserId)
	{
	}
};

/**
 * Input parameters for the EOS_UI_PauseSocialOverlay function.
 */
USTRUCT(BlueprintType)
struct FEOSUIPauseSocialOverlayOptions
{
	GENERATED_BODY()
public:
	/** The desired bIsPaused state of the overlay.  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	bool bIsPaused;
public:
	FEOSUIPauseSocialOverlayOptions()
		: bIsPaused(false)
	{
	}
};

/**
 * Input parameters for the EOS_UI_IsSocialOverlayPaused function.
 */
USTRUCT(BlueprintType)
struct FEOSUIIsSocialOverlayPausedOptions
{
	GENERATED_BODY()
public:
	FEOSUIIsSocialOverlayPausedOptions()
	{
	}
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		DELEGATES
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnShowFriendsCallback, const FEOSUIShowFriendsCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnHideFriendsCallback, const FEOSUIHideFriendsCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnDisplaySettingsUpdatedCallback, const FEOSUIOnDisplaySettingsUpdatedCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnShowBlockPlayerCallback, const FEOSUIOnShowBlockPlayerCallbackInfo&, Data);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnShowReportPlayerCallback, const FEOSUIOnShowReportPlayerCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShowFriendsCallbackDelegate, const FEOSUIShowFriendsCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHideFriendsCallbackDelegate, const FEOSUIHideFriendsCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDisplaySettingsUpdatedCallbackDelegate, const FEOSUIOnDisplaySettingsUpdatedCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShowBlockPlayerCallbackDelegate, const FEOSUIOnShowBlockPlayerCallbackInfo&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShowReportPlayerCallbackDelegate, const FEOSUIOnShowReportPlayerCallbackInfo&, Data);


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//		CALLBACK OBJECTS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

struct FShowFriendsCallback
{
public:
	FOnShowFriendsCallback m_Callback;
public:
	FShowFriendsCallback(const FOnShowFriendsCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FShowFriendsCallback()
	{
		m_Callback.Unbind();
	}
};

struct FHideFriendsCallback
{
public:
	FOnHideFriendsCallback m_Callback;
public:
	FHideFriendsCallback(const FOnHideFriendsCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FHideFriendsCallback()
	{
		m_Callback.Unbind();
	}
};

struct FShowBlockPlayerCallback
{
public:
	FOnShowBlockPlayerCallback m_Callback;
public:
	FShowBlockPlayerCallback(const FOnShowBlockPlayerCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FShowBlockPlayerCallback()
	{
		m_Callback.Unbind();
	}
};

struct FShowReportPlayerCallback
{
public:
	FOnShowReportPlayerCallback m_Callback;
public:
	FShowReportPlayerCallback(const FOnShowReportPlayerCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FShowReportPlayerCallback()
	{
		m_Callback.Unbind();
	}
};

struct FDisplaySettingsUpdatedCallback
{
public:
	FDisplaySettingsUpdatedCallback(const FOnDisplaySettingsUpdatedCallback& callback)
		: m_Callback(callback)
	{
	}

	virtual ~FDisplaySettingsUpdatedCallback()
	{
		m_Callback.Unbind();
	}

public:
	FOnDisplaySettingsUpdatedCallback m_Callback;
};
