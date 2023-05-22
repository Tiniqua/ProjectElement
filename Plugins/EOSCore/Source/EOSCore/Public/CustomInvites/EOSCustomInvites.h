/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "EOSCoreModule.h"
#include "CustomInvites/EOSCustomInvitesTypes.h"
#include "EOSCustomInvites.generated.h"

/**
 * The Custom Invites Interface is designed to allow developers to have custom game Invite and Join operations driven by the Notification Service and supported by the Overlay (if desired).
 * All Custom Invites Interface calls take a handle of type EOS_HCustomInvites as the first parameter.
 * This handle can be retrieved from a EOS_HPlatform handle by using the EOS_Platform_GetCustomInvitesInterface function.
 *
 * @see EOS_Platform_GetCustomInvitesInterface
 */
UCLASS()
class EOSCORE_API UCoreCustomInvites : public UEOSCoreSubsystem
{
	GENERATED_BODY()
public:
	virtual void Deinitialize() override;
public:
	/**
	 * The Custom Invites Interface is designed to allow developers to have custom game Invite and Join operations driven by the Notification Service and supported by the Overlay (if desired).
	 * All Custom Invites Interface calls take a handle of type EOS_HCustomInvites as the first parameter.
	 * This handle can be retrieved from a EOS_HPlatform handle by using the EOS_Platform_GetCustomInvitesInterface function.
	 *
	 * @see EOS_Platform_GetCustomInvitesInterface
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Subsystems", meta = (WorldContext = "WorldContextObject"))
	static UCoreCustomInvites* GetCustomInvites(UObject* WorldContextObject);

public:
	/**
	 * Initializes a Custom Invite with a specified payload in preparation for it to be sent to another user or users.
	 *
	 * @param Options Structure containing information about the request.
	 *
	 * @return EOS_Success if the operation completes successfully
	 *         EOS_InvalidParameters if any of the options values are incorrect
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|CustomInvites", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_CustomInvites_SetCustomInvite"))
	static EOSResult EOSCustomInvitesSetCustomInvite(UObject* WorldContextObject, FEOSCustomInvitesSetCustomInviteOptions Options);

	/**
	 * Sends a Custom Invite that has previously been initialized via SetCustomInvite to a group of users.
	 *
	 * @param Options Structure containing information about the request.
	 * @param Callback A callback that is fired when the operation completes, either successfully or in error
	 *
	 * @return EOS_Success if the query completes successfully
	 *         EOS_InvalidParameters if any of the options values are incorrect
	 *         EOS_TooManyRequests if the number of allowed queries is exceeded
	 *         EOS_NotFound if SetCustomInvite has not been previously successfully called for this user
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|CustomInvites", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_CustomInvites_SendCustomInvite"))
	static void EOSCustomInvitesSendCustomInvite(UObject* WorldContextObject, FEOSCustomInvitesSendCustomInviteOptions Options, const FOnCustomInvitesOnSendCustomInviteCallback& Callback);

	/**
	 * Register to receive notifications when a Custom Invite for any logged in local users is received
	 * @note must call EOS_CustomInvites_RemoveNotifyCustomInviteReceived to remove the notification
	 *
	 * @param Options Structure containing information about the request.
	 * @param Callback A callback that is fired when a Custom Invite is received.
	 *
	 * @return handle representing the registered callback
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|CustomInvites", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_CustomInvites_AddNotifyCustomInviteReceived"))
	static FEOSNotificationId EOSCustomInvitesAddNotifyCustomInviteReceived(UObject* WorldContextObject, FEOSCustomInvitesAddNotifyCustomInviteReceivedOptions Options, const FOnCustomInvitesOnCustomInviteReceivedCallback& Callback);

	/**
	 * Unregister from receiving notifications when a Custom Invite for any logged in local users is received
	 *
	 * @param Id Handle representing the registered callback
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|CustomInvites", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_CustomInvites_RemoveNotifyCustomInviteReceived"))
	static void EOSCustomInvitesRemoveNotifyCustomInviteReceived(UObject* WorldContextObject, FEOSNotificationId Id);

	/**
	 * Register to receive notifications when a Custom Invite for any logged in local users is accepted via the Social Overlay
	 * @note must call EOS_CustomInvites_RemoveNotifyCustomInviteAccepted to remove the notification
	 *
	 * @param Options Structure containing information about the request.
	 * @param ClientData Arbitrary data that is passed back to you in the Callback.
	 * @param Callback A callback that is fired when a Custom Invite is accepted via the Social Overlay.
	 *
	 * @return handle representing the registered callback
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|CustomInvites", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_CustomInvites_AddNotifyCustomInviteAccepted"))
	static FEOSNotificationId EOSCustomInvitesAddNotifyCustomInviteAccepted(UObject* WorldContextObject, const FOnCustomInvitesOnCustomInviteAcceptedCallback& Callback);

	/**
	 * Unregister from receiving notifications when a Custom Invite for any logged in local users is accepted via the Social Overlay
	 *
	 * @param Id Handle representing the registered callback
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|CustomInvites", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_CustomInvites_RemoveNotifyCustomInviteAccepted"))
	static void EOSCustomInvitesRemoveNotifyCustomInviteAccepted(UObject* WorldContextObject, FEOSNotificationId Id);

	/**
	 * Register to receive notifications when a Custom Invite for any logged in local user is rejected via the Social Overlay
	 * Invites rejected in this way do not need to have FinalizeInvite called on them, it is called automatically internally by the SDK.
	 * @note must call EOS_CustomInvites_RemoveNotifyCustomInviteRejected to remove the notification
	 *
	 * @param Options Structure containing information about the request.
	 * @param Callback A callback that is fired when a Custom Invite is rejected via the Social Overlay.
	 *
	 * @return handle representing the registered callback
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|CustomInvites", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_CustomInvites_AddNotifyCustomInviteRejected"))
	FEOSNotificationId EOSCustomInvitesAddNotifyCustomInviteRejected(UObject* WorldContextObject, FEOSCustomInvitesAddNotifyCustomInviteRejectedOptions Options, const FOnCustomInvitesOnCustomInviteRejectedCallback& Callback);

	/**
	 * Unregister from receiving notifications when a Custom Invite for any logged in local user is rejected via the Social Overlay
	 *
	 * @param Id Handle representing the registered callback
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|CustomInvites", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_CustomInvites_RemoveNotifyCustomInviteRejected"))
	void EOSCustomInvitesRemoveNotifyCustomInviteRejected(UObject* WorldContextObject, FEOSNotificationId Id);

	/**
	 * Signal that the title has completed processing a received Custom Invite, and that it should be cleaned up internally and in the Overlay
	 *
	 * @param Options Structure containing information about the request.
	 *
	 * @return EOS_Success if the operation completes successfully
	 *         EOS_InvalidParameters if any of the option values are incorrect
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|CustomInvites", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_CustomInvites_FinalizeInvite"))
	static EOSResult EOSCustomInvitesFinalizeInvite(UObject* WorldContextObject, FEOSCustomInvitesFinalizeInviteOptions Options);

private:
	static void Internal_OnSendCustomInviteCallback(const EOS_CustomInvites_SendCustomInviteCallbackInfo* Data);

protected:
	TMap<FEOSNotificationId, FCustomInvitesCustomInviteReceivedCallback*> m_CustomInviteReceivedCallback;
	TMap<FEOSNotificationId, FCustomInvitesCustomInviteAcceptedCallback*> m_CustomInvitesOnCustomInviteAcceptedCallback;
	TMap<FEOSNotificationId, FCustomInvitesOnCustomInviteRejectedCallback*> m_CustomInvitesOnCustomInviteRejectedCallback;
};
