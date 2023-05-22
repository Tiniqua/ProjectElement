/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "CustomInvites/EOSCustomInvites.h"
#include "Core/EOSCorePluginPrivatePCH.h"
#include "Core/EOSCoreLogging.h"

void UCoreCustomInvites::Deinitialize()
{
	LogVerbose("");

	for (auto& Element : m_CustomInviteReceivedCallback)
	{
		EOS_CustomInvites_RemoveNotifyCustomInviteReceived(GetCustomInvitesHandle(this), Element.Key);
	}

	for (auto& Element : m_CustomInvitesOnCustomInviteAcceptedCallback)
	{
		EOS_CustomInvites_RemoveNotifyCustomInviteAccepted(GetCustomInvitesHandle(this), Element.Key);
	}
	
	m_CustomInviteReceivedCallback.Empty();
	m_CustomInvitesOnCustomInviteAcceptedCallback.Empty();
}

UCoreCustomInvites* UCoreCustomInvites::GetCustomInvites(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (UWorld* World = WorldContextObject->GetWorld())
		{
			if (UGameInstance* GameInstance = World->GetGameInstance())
			{
				return GameInstance->GetSubsystem<UCoreCustomInvites>();
			}
		}
	}
	return nullptr;
}

FEOSNotificationId UCoreCustomInvites::EOSCustomInvitesAddNotifyCustomInviteRejected(UObject* WorldContextObject, FEOSCustomInvitesAddNotifyCustomInviteRejectedOptions Options, const FOnCustomInvitesOnCustomInviteRejectedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId NotificationId = {};

	if (GetCustomInvitesHandle(WorldContextObject))
	{
		EOS_CustomInvites_AddNotifyCustomInviteRejectedOptions CustomInvitesOptions = {  };
		CustomInvitesOptions.ApiVersion = EOS_CUSTOMINVITES_ADDNOTIFYCUSTOMINVITEREJECTED_API_LATEST;

		FCustomInvitesOnCustomInviteRejectedCallback* CallbackObj = new FCustomInvitesOnCustomInviteRejectedCallback(Callback);
		NotificationId = EOS_CustomInvites_AddNotifyCustomInviteRejected(GetCustomInvitesHandle(WorldContextObject), &CustomInvitesOptions, CallbackObj, [](const EOS_CustomInvites_CustomInviteRejectedCallbackInfo* Data)
		{
			const FCustomInvitesOnCustomInviteRejectedCallback* CallbackObj = static_cast<FCustomInvitesOnCustomInviteRejectedCallback*>(Data->ClientData);
			check(CallbackObj);
			if (CallbackObj)
			{
				CallbackObj->m_Callback.ExecuteIfBound(*Data);
			}
		});
		GetCustomInvites(WorldContextObject)->m_CustomInvitesOnCustomInviteRejectedCallback.Add(NotificationId, CallbackObj);
	}
	
	return NotificationId;
}

void UCoreCustomInvites::EOSCustomInvitesRemoveNotifyCustomInviteRejected(UObject* WorldContextObject, FEOSNotificationId Id)
{
	LogVerbose("");

	if (GetAchievementsHandle(WorldContextObject))
	{
		EOS_CustomInvites_RemoveNotifyCustomInviteReceived(GetCustomInvitesHandle(WorldContextObject), Id);
		GetCustomInvites(WorldContextObject)->m_CustomInvitesOnCustomInviteRejectedCallback.Remove(Id);
	}
}

EOSResult UCoreCustomInvites::EOSCustomInvitesSetCustomInvite(UObject* WorldContextObject, FEOSCustomInvitesSetCustomInviteOptions Options)
{
	LogVerbose("");

	EOSResult Result = EOSResult::EOS_ServiceFailure;
	
	if (GetCustomInvitesHandle(WorldContextObject))
	{
		const FTCHARToUTF8 Payload(*Options.Payload);
		
		EOS_CustomInvites_SetCustomInviteOptions CustomInvitesOptions = { };
		CustomInvitesOptions.ApiVersion = EOS_CUSTOMINVITES_SETCUSTOMINVITE_API_LATEST;
		CustomInvitesOptions.LocalUserId = Options.LocalUserId;
		CustomInvitesOptions.Payload = Payload.Get();

		Result = EOSHelpers::ToEOSCoreResult(EOS_CustomInvites_SetCustomInvite(GetCustomInvitesHandle(WorldContextObject), &CustomInvitesOptions));
	}

	return Result;
}

void UCoreCustomInvites::EOSCustomInvitesSendCustomInvite(UObject* WorldContextObject, FEOSCustomInvitesSendCustomInviteOptions Options, const FOnCustomInvitesOnSendCustomInviteCallback& Callback)
{
	LogVerbose("");

	if (GetCustomInvitesHandle(WorldContextObject))
	{
		FCustomInvitesSendCustomInvite CustomInvitesOptions = { Options.LocalUserId, Options.TargetUserIds };
		CustomInvitesOptions.ApiVersion = EOS_CUSTOMINVITES_SETCUSTOMINVITE_API_LATEST;

		FCustomInvitesSendCustomInviteCallback* CallbackObj = new FCustomInvitesSendCustomInviteCallback(Callback);
		EOS_CustomInvites_SendCustomInvite(GetCustomInvitesHandle(WorldContextObject), &CustomInvitesOptions, CallbackObj, Internal_OnSendCustomInviteCallback);
	}
}

FEOSNotificationId UCoreCustomInvites::EOSCustomInvitesAddNotifyCustomInviteReceived(UObject* WorldContextObject, FEOSCustomInvitesAddNotifyCustomInviteReceivedOptions Options, const FOnCustomInvitesOnCustomInviteReceivedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId NotificationId = {};

	if (GetCustomInvitesHandle(WorldContextObject))
	{
		EOS_CustomInvites_AddNotifyCustomInviteReceivedOptions CustomInvitesOptions = {  };
		CustomInvitesOptions.ApiVersion = EOS_CUSTOMINVITES_ADDNOTIFYCUSTOMINVITERECEIVED_API_LATEST;

		FCustomInvitesCustomInviteReceivedCallback* CallbackObj = new FCustomInvitesCustomInviteReceivedCallback(Callback);
		NotificationId = EOS_CustomInvites_AddNotifyCustomInviteReceived(GetCustomInvitesHandle(WorldContextObject), &CustomInvitesOptions, CallbackObj, [](const EOS_CustomInvites_OnCustomInviteReceivedCallbackInfo* Data)
		{
			const FCustomInvitesCustomInviteReceivedCallback* CallbackObj = static_cast<FCustomInvitesCustomInviteReceivedCallback*>(Data->ClientData);
			check(CallbackObj);
			if (CallbackObj)
			{
				CallbackObj->m_Callback.ExecuteIfBound(*Data);
			}
		});
		GetCustomInvites(WorldContextObject)->m_CustomInviteReceivedCallback.Add(NotificationId, CallbackObj);
	}
	
	return NotificationId;
}

void UCoreCustomInvites::EOSCustomInvitesRemoveNotifyCustomInviteReceived(UObject* WorldContextObject, FEOSNotificationId Id)
{
	LogVerbose("");

	if (GetAchievementsHandle(WorldContextObject))
	{
		EOS_CustomInvites_RemoveNotifyCustomInviteReceived(GetCustomInvitesHandle(WorldContextObject), Id);
		GetCustomInvites(WorldContextObject)->m_CustomInviteReceivedCallback.Remove(Id);
	}
}

FEOSNotificationId UCoreCustomInvites::EOSCustomInvitesAddNotifyCustomInviteAccepted(UObject* WorldContextObject, const FOnCustomInvitesOnCustomInviteAcceptedCallback& Callback)
{
	LogVerbose("");

	FEOSNotificationId NotificationId = {};

	if (GetCustomInvitesHandle(WorldContextObject))
	{
		EOS_CustomInvites_AddNotifyCustomInviteAcceptedOptions CustomInvitesOptions = {  };
		CustomInvitesOptions.ApiVersion = EOS_CUSTOMINVITES_ADDNOTIFYCUSTOMINVITEACCEPTED_API_LATEST;

		FCustomInvitesCustomInviteAcceptedCallback* CallbackObj = new FCustomInvitesCustomInviteAcceptedCallback(Callback);
		NotificationId = EOS_CustomInvites_AddNotifyCustomInviteAccepted(GetCustomInvitesHandle(WorldContextObject), &CustomInvitesOptions, CallbackObj, [](const EOS_CustomInvites_OnCustomInviteAcceptedCallbackInfo* Data)
		{
			const FCustomInvitesCustomInviteAcceptedCallback* CallbackObj = static_cast<FCustomInvitesCustomInviteAcceptedCallback*>(Data->ClientData);
			check(CallbackObj);
			if (CallbackObj)
			{
				CallbackObj->m_Callback.ExecuteIfBound(*Data);
			}
		});
		GetCustomInvites(WorldContextObject)->m_CustomInvitesOnCustomInviteAcceptedCallback.Add(NotificationId, CallbackObj);
	}
	
	return NotificationId;
}

void UCoreCustomInvites::EOSCustomInvitesRemoveNotifyCustomInviteAccepted(UObject* WorldContextObject, FEOSNotificationId Id)
{
	LogVerbose("");

	if (GetAchievementsHandle(WorldContextObject))
	{
		EOS_CustomInvites_RemoveNotifyCustomInviteAccepted(GetCustomInvitesHandle(WorldContextObject), Id);
		GetCustomInvites(WorldContextObject)->m_CustomInvitesOnCustomInviteAcceptedCallback.Remove(Id);
	}
}

EOSResult UCoreCustomInvites::EOSCustomInvitesFinalizeInvite(UObject* WorldContextObject, FEOSCustomInvitesFinalizeInviteOptions Options)
{
	LogVerbose("");
	
	EOSResult Result = EOSResult::EOS_ServiceFailure;

	if (GetCustomInvitesHandle(WorldContextObject))
	{
		EOS_CustomInvites_FinalizeInviteOptions CustomInvitesOptions = { };
		CustomInvitesOptions.ApiVersion = EOS_CUSTOMINVITES_FINALIZEINVITE_API_LATEST;
		CustomInvitesOptions.TargetUserId = Options.TargetUserId;
		CustomInvitesOptions.LocalUserId = Options.LocalUserId;
		CustomInvitesOptions.CustomInviteId = new char[512];
		FCStringAnsi::Strncpy(const_cast<char*>(CustomInvitesOptions.CustomInviteId), TCHAR_TO_UTF8(*Options.CustomInviteId), 512);

		CustomInvitesOptions.ProcessingResult = static_cast<EOS_EResult>(Options.ProcessingResult);

		Result = EOSHelpers::ToEOSCoreResult(EOS_CustomInvites_FinalizeInvite(GetCustomInvitesHandle(WorldContextObject), &CustomInvitesOptions));
	}
	
	return Result;
}

void UCoreCustomInvites::Internal_OnSendCustomInviteCallback(const EOS_CustomInvites_SendCustomInviteCallbackInfo* Data)
{
	LogVerbose("%s", *FString(EOS_EResult_ToString(Data->ResultCode)));

	const FCustomInvitesSendCustomInviteCallback* CallbackObj = static_cast<FCustomInvitesSendCustomInviteCallback*>(Data->ClientData);
	check(CallbackObj);
	if (CallbackObj)
	{
		CallbackObj->m_Callback.ExecuteIfBound(*Data);
		delete CallbackObj;
	}
}
