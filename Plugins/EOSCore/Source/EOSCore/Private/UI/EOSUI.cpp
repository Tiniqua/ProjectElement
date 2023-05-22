/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "UI/EOSUI.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLogging.h"

void UCoreUI::Deinitialize()
{
	LogVerbose("");

	for (auto Element: m_DisplaySettingsUpdatedCallbacks)
	{
		EOS_UI_RemoveNotifyDisplaySettingsUpdated(GetUiHandle(this), Element.Key);
	}

	m_DisplaySettingsUpdatedCallbacks.Empty();
}

UCoreUI* UCoreUI::GetUI(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (UWorld* World = WorldContextObject->GetWorld())
		{
			if (UGameInstance* GameInstance = World->GetGameInstance())
			{
				return GameInstance->GetSubsystem<UCoreUI>();
			}
		}
	}
	return nullptr;
}

bool UCoreUI::EOSUIGetFriendsExclusiveInput(UObject* WorldContextObject, FEOSUIGetFriendsExclusiveInputOptions Options)
{
	LogVerbose("");

	bool bResult = false;

	if (GetUiHandle(WorldContextObject))
	{
		EOS_UI_GetFriendsExclusiveInputOptions UIOptions = {};
		UIOptions.ApiVersion = EOS_UI_GETFRIENDSEXCLUSIVEINPUT_API_LATEST;
		UIOptions.LocalUserId = Options.LocalUserId;

		bResult = (EOS_UI_GetFriendsExclusiveInput(GetUiHandle(WorldContextObject), &UIOptions) > 0);
	}

	return bResult;
}

void UCoreUI::EOSUIShowBlockPlayer(UObject* WorldContextObject, FEOSUIShowBlockPlayerOptions Options, const FOnShowBlockPlayerCallback& Callback)
{
	LogVerbose("");

	if (GetUiHandle(WorldContextObject))
	{
		EOS_UI_ShowBlockPlayerOptions UIOptions = {};
		UIOptions.ApiVersion = EOS_UI_SHOWBLOCKPLAYER_API_LATEST;
		UIOptions.LocalUserId = Options.LocalUserId;
		UIOptions.TargetUserId = Options.TargetUserId;
		
		FShowBlockPlayerCallback* CallbackObj = new FShowBlockPlayerCallback(Callback);
		EOS_UI_ShowBlockPlayer(GetUiHandle(WorldContextObject), &UIOptions, CallbackObj, Internal_OnShowBlockPlayerCallback);
	}
}

void UCoreUI::EOSUIShowReportPlayer(UObject* WorldContextObject, FEOSUIShowReportPlayerOptions Options, const FOnShowReportPlayerCallback& Callback)
{
	LogVerbose("");

	if (GetUiHandle(WorldContextObject))
	{
		EOS_UI_ShowReportPlayerOptions UIOptions = {};
		UIOptions.ApiVersion = EOS_UI_SHOWREPORTPLAYER_API_LATEST;
		UIOptions.LocalUserId = Options.LocalUserId;
		UIOptions.TargetUserId = Options.TargetUserId;
		
		FShowReportPlayerCallback* CallbackObj = new FShowReportPlayerCallback(Callback);
		EOS_UI_ShowReportPlayer(GetUiHandle(WorldContextObject), &UIOptions, CallbackObj, Internal_OnShowReportPlayerCallback);
	}
}

EOSResult UCoreUI::EOSUIPauseSocialOverlay(UObject* WorldContextObject, FEOSUIPauseSocialOverlayOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetUiHandle(WorldContextObject))
	{
		EOS_UI_PauseSocialOverlayOptions UIOptions = {};
		UIOptions.ApiVersion = EOS_UI_PAUSESOCIALOVERLAY_API_LATEST;
		UIOptions.bIsPaused = Options.bIsPaused;

		Result = EOSHelpers::ToEOSCoreResult(EOS_UI_PauseSocialOverlay(GetUiHandle(WorldContextObject), &UIOptions));
	}

	return Result;
}

bool UCoreUI::EOSUIIsSocialOverlayPaused(UObject* WorldContextObject, FEOSUIIsSocialOverlayPausedOptions Options)
{
	LogVerbose("");

	bool bResult = false;

	if (GetUiHandle(WorldContextObject))
	{
		EOS_UI_IsSocialOverlayPausedOptions UIOptions = {};
		UIOptions.ApiVersion = EOS_UI_ISSOCIALOVERLAYPAUSED_API_LATEST;

		bResult = (EOS_UI_IsSocialOverlayPaused(GetUiHandle(WorldContextObject), &UIOptions) > 0);
	}

	return bResult;
}

void UCoreUI::EOSUIShowFriends(UObject* WorldContextObject, FEOSUIShowFriendsOptions Options, const FOnShowFriendsCallback& Callback)
{
	LogVerbose("");

	if (GetUiHandle(WorldContextObject))
	{
		EOS_UI_ShowFriendsOptions UIOptions = {};
		UIOptions.ApiVersion = EOS_UI_SHOWFRIENDS_API_LATEST;
		UIOptions.LocalUserId = Options.LocalUserId;
		
		FShowFriendsCallback* CallbackObj = new FShowFriendsCallback(Callback);
		EOS_UI_ShowFriends(GetUiHandle(WorldContextObject), &UIOptions, CallbackObj, Internal_OnShowFriendsCallback);
	}
}

void UCoreUI::EOSUIHideFriends(UObject* WorldContextObject, FEOSUIHideFriendsOptions Options, const FOnHideFriendsCallback& Callback)
{
	LogVerbose("");

	if (GetUiHandle(WorldContextObject))
	{
		EOS_UI_HideFriendsOptions UIOptions = {};
		UIOptions.ApiVersion = EOS_UI_HIDEFRIENDS_API_LATEST;
		UIOptions.LocalUserId = Options.LocalUserId;
		
		FHideFriendsCallback* CallbackObj = new FHideFriendsCallback(Callback);
		EOS_UI_HideFriends(GetUiHandle(WorldContextObject), &UIOptions, CallbackObj, Internal_OnHideFriendsCallback);
	}
}

bool UCoreUI::EOSUIGetFriendsVisible(UObject* WorldContextObject, FEOSUIGetFriendsVisibleOptions Options)
{
	LogVerbose("");

	bool bResult = false;

	if (GetUiHandle(WorldContextObject))
	{
		EOS_UI_GetFriendsVisibleOptions UIOptions = {};
		UIOptions.ApiVersion = Options.ApiVersion;
		UIOptions.LocalUserId = Options.LocalUserId;

		bResult = (EOS_UI_GetFriendsVisible(GetUiHandle(WorldContextObject), &UIOptions) > 0);
	}

	return bResult;
}

EOSResult UCoreUI::EOSUISetToggleFriendsKey(UObject* WorldContextObject, FEOSUISetToggleFriendsKeyOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetUiHandle(WorldContextObject))
	{
		EOS_UI_SetToggleFriendsKeyOptions UIOptions = {};
		UIOptions.ApiVersion = Options.ApiVersion;

		EOS_UI_EKeyCombination Combination = EOS_UI_EKeyCombination::EOS_UIK_None;
		int32 KeyCombination = Options.keyCombination;

		if ((KeyCombination & static_cast<int32>(EOS_UI_EKeyCombination::EOS_UIK_Shift)) != 0)
		{
			Combination = Combination | EOS_UI_EKeyCombination::EOS_UIK_Shift;
			KeyCombination -= static_cast<int32>(EOS_UI_EKeyCombination::EOS_UIK_Shift);
		}

		if ((KeyCombination & static_cast<int32>(EOS_UI_EKeyCombination::EOS_UIK_Control)) != 0)
		{
			Combination = Combination | EOS_UI_EKeyCombination::EOS_UIK_Control;
			KeyCombination -= static_cast<int32>(EOS_UI_EKeyCombination::EOS_UIK_Control);
		}

		if ((KeyCombination & static_cast<int32>(EOS_UI_EKeyCombination::EOS_UIK_Alt)) != 0)
		{
			Combination = Combination | EOS_UI_EKeyCombination::EOS_UIK_Alt;
			KeyCombination -= static_cast<int32>(EOS_UI_EKeyCombination::EOS_UIK_Alt);
		}

		if ((KeyCombination & static_cast<int32>(EOS_UI_EKeyCombination::EOS_UIK_Meta)) != 0)
		{
			Combination = Combination | EOS_UI_EKeyCombination::EOS_UIK_Meta;
			KeyCombination -= static_cast<int32>(EOS_UI_EKeyCombination::EOS_UIK_Meta);
		}

		if (KeyCombination > 0)
		{
			Combination |= static_cast<EOS_UI_EKeyCombination>(KeyCombination);
			KeyCombination -= static_cast<int32>(static_cast<EOS_UI_EKeyCombination>(KeyCombination));
		}

		UIOptions.KeyCombination = Combination;

		Result = EOSHelpers::ToEOSCoreResult(EOS_UI_SetToggleFriendsKey(GetUiHandle(WorldContextObject), &UIOptions));
	}

	return Result;
}

int32 UCoreUI::EOSUIGetToggleFriendsKey(UObject* WorldContextObject, FEOSUIGetToggleFriendsKeyOptions Options)
{
	LogVerbose("");

	int32 Result = 0;

	if (GetUiHandle(WorldContextObject))
	{
		EOS_UI_GetToggleFriendsKeyOptions UIOptions = {};
		UIOptions.ApiVersion = Options.ApiVersion;

		Result = static_cast<int32>(EOS_UI_GetToggleFriendsKey(GetUiHandle(WorldContextObject), &UIOptions));
	}

	return Result;
}

bool UCoreUI::EOSUIIsValidKeyCombination(UObject* WorldContextObject, int32 KeyCombination)
{
	LogVerbose("");

	bool bResult = false;

	if (GetUiHandle(WorldContextObject))
	{
		EOS_UI_EKeyCombination Combination = EOS_UI_EKeyCombination::EOS_UIK_None;

		if ((KeyCombination & static_cast<int32>(EOS_UI_EKeyCombination::EOS_UIK_Shift)) != 0)
		{
			Combination = Combination | EOS_UI_EKeyCombination::EOS_UIK_Shift;
			KeyCombination -= static_cast<int32>(EOS_UI_EKeyCombination::EOS_UIK_Shift);
		}

		if ((KeyCombination & static_cast<int32>(EOS_UI_EKeyCombination::EOS_UIK_Control)) != 0)
		{
			Combination = Combination | EOS_UI_EKeyCombination::EOS_UIK_Control;
			KeyCombination -= static_cast<int32>(EOS_UI_EKeyCombination::EOS_UIK_Control);
		}

		if ((KeyCombination & static_cast<int32>(EOS_UI_EKeyCombination::EOS_UIK_Alt)) != 0)
		{
			Combination = Combination | EOS_UI_EKeyCombination::EOS_UIK_Alt;
			KeyCombination -= static_cast<int32>(EOS_UI_EKeyCombination::EOS_UIK_Alt);
		}

		if ((KeyCombination & static_cast<int32>(EOS_UI_EKeyCombination::EOS_UIK_Meta)) != 0)
		{
			Combination = Combination | EOS_UI_EKeyCombination::EOS_UIK_Meta;
			KeyCombination -= static_cast<int32>(EOS_UI_EKeyCombination::EOS_UIK_Meta);
		}

		if (KeyCombination > 0)
		{
			Combination |= static_cast<EOS_UI_EKeyCombination>(KeyCombination);
			KeyCombination -= static_cast<int32>(static_cast<EOS_UI_EKeyCombination>(KeyCombination));
		}

		bResult = (EOS_UI_IsValidKeyCombination(GetUiHandle(WorldContextObject), Combination) > 0);
	}

	return bResult;
}

EOSResult UCoreUI::EOSUISetDisplayPreference(UObject* WorldContextObject, FEOSUISetDisplayPreferenceOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetUiHandle(WorldContextObject))
	{
		EOS_UI_SetDisplayPreferenceOptions UIOptions = {};
		UIOptions.ApiVersion = Options.ApiVersion;
		UIOptions.NotificationLocation = static_cast<EOS_UI_ENotificationLocation>(Options.NotificationLocation);

		Result = EOSHelpers::ToEOSCoreResult(EOS_UI_SetDisplayPreference(GetUiHandle(WorldContextObject), &UIOptions));
	}

	return Result;
}

EEOSUIENotificationLocation UCoreUI::EOSUIGetNotificationLocationPreference(UObject* WorldContextObject)
{
	LogVerbose("");

	EEOSUIENotificationLocation Result = EEOSUIENotificationLocation::EOS_UNL_BottomLeft;

	if (GetUiHandle(WorldContextObject))
	{
		Result = static_cast<EEOSUIENotificationLocation>(EOS_UI_GetNotificationLocationPreference(GetUiHandle(WorldContextObject)));
	}

	return Result;
}

EOSResult UCoreUI::EOSUIAcknowledgeEventId(UObject* WorldContextObject, FEOSUIAcknowledgeEventIdOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetUiHandle(WorldContextObject))
	{
		EOS_UI_AcknowledgeEventIdOptions UIOptions = {};
		UIOptions.ApiVersion = Options.ApiVersion;
		UIOptions.UiEventId = Options.UiEventId;
		UIOptions.Result = static_cast<EOS_EResult>(Options.Result);

		Result = EOSHelpers::ToEOSCoreResult(EOS_UI_AcknowledgeEventId(GetUiHandle(WorldContextObject), &UIOptions));
	}

	return Result;
}

FEOSNotificationId UCoreUI::EOSUIAddNotifyDisplaySettingsUpdated(UObject* WorldContextObject, const FEOSUIAddNotifyDisplaySettingsUpdatedOptions Options, const FOnDisplaySettingsUpdatedCallback& Callback)
{
	LogVerbose("");

	EOS_UI_AddNotifyDisplaySettingsUpdatedOptions UIOptions = {};
	UIOptions.ApiVersion = Options.ApiVersion;

	FDisplaySettingsUpdatedCallback* CallbackObj = new FDisplaySettingsUpdatedCallback(Callback);

	EOS_NotificationId Id = EOS_UI_AddNotifyDisplaySettingsUpdated(GetUiHandle(WorldContextObject), &UIOptions, CallbackObj, ([](const EOS_UI_OnDisplaySettingsUpdatedCallbackInfo* data)
	{
		const FDisplaySettingsUpdatedCallback* CallbackData = static_cast<FDisplaySettingsUpdatedCallback*>(data->ClientData);
		CallbackData->m_Callback.ExecuteIfBound(*data);
	}));

	
	GetUI(WorldContextObject)->m_DisplaySettingsUpdatedCallbacks.Add(Id, CallbackObj);

	return Id;
}

void UCoreUI::EOSUIRemoveNotifyDisplaySettingsUpdated(UObject* WorldContextObject, FEOSNotificationId Id)
{
	LogVerbose("");

	EOS_UI_RemoveNotifyDisplaySettingsUpdated(GetUiHandle(WorldContextObject), Id);
	GetUI(WorldContextObject)->m_DisplaySettingsUpdatedCallbacks.Remove(Id);
}

void UCoreUI::Internal_OnShowFriendsCallback(const EOS_UI_ShowFriendsCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FShowFriendsCallback* CallbackObj = static_cast<FShowFriendsCallback*>(Data->ClientData);
	check(CallbackObj);

	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreUI::Internal_OnHideFriendsCallback(const EOS_UI_HideFriendsCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FHideFriendsCallback* CallbackObj = static_cast<FHideFriendsCallback*>(Data->ClientData);
	check(CallbackObj);

	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreUI::Internal_OnShowBlockPlayerCallback(const EOS_UI_OnShowBlockPlayerCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FShowBlockPlayerCallback* CallbackObj = static_cast<FShowBlockPlayerCallback*>(Data->ClientData);
	check(CallbackObj);

	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}

void UCoreUI::Internal_OnShowReportPlayerCallback(const EOS_UI_OnShowReportPlayerCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FShowReportPlayerCallback* CallbackObj = static_cast<FShowReportPlayerCallback*>(Data->ClientData);
	check(CallbackObj);

	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}
