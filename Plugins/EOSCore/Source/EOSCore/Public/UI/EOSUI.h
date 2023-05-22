/**
* Copyright (C) 2017-2022 | eelDev AB
*
 official EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "EOSCoreModule.h"
#include "eos_ui_types.h"
#include "UI/EOSUITypes.h"
#include "EOSUI.generated.h"

/**
* The UI Interface is used to access the Social Overlay UI.  Each UI component will have a function for
* opening it.  All UI Interface calls take a handle of type EOS_HUI as the first parameter.
* This handle can be retrieved from an EOS_HPlatform handle by using the EOS_Platform_GetUIInterface function.
*
* @see EOS_Platform_GetUIInterface
*/

UCLASS()
class EOSCORE_API UCoreUI : public UEOSCoreSubsystem
{
	GENERATED_BODY()
public:
	virtual void Deinitialize() override;
public:
	/**
	 * The UI Interface is used to access the overlay UI.  Each UI component will have a function for
	 * opening it.  All UI Interface calls take a handle of type EOS_HUI as the first parameter.
	 * This handle can be retrieved from a EOS_HPlatform handle by using the EOS_Platform_GetUIInterface function.
	 *
	 * NOTE: At this time, this feature is only available for products that are part of the Epic Games store.
	 *
	 * @see EOS_Platform_GetUIInterface
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Subsystems", meta = (WorldContext = "WorldContextObject"))
	static UCoreUI* GetUI(UObject* WorldContextObject);

public:
	/**
	* Opens the Social Overlay with a request to show the friends list.
	*
	* @param Options Structure containing the Epic Online Services Account ID of the friends list to show.
	* @param Callback A callback that is fired when the request to show the friends list has been sent to the Social Overlay, or on an error.
	*
	* @return EOS_Success If the Social Overlay has been notified about the request.
	*         EOS_InvalidParameters If any of the options are incorrect.
	*         EOS_NotConfigured If the Social Overlay is not properly configured.
	*         EOS_NoChange If the Social Overlay is already visible.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|UI", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_UI_ShowFriends"))
	void EOSUIShowFriends(UObject* WorldContextObject, FEOSUIShowFriendsOptions Options, const FOnShowFriendsCallback& Callback);

	/**
	* Hides the active Social Overlay.
	*
	* @param Options Structure containing the Epic Online Services Account ID of the browser to close.
	* @param Callback A callback that is fired when the request to hide the friends list has been processed, or on an error.
	*
	* @return EOS_Success If the Social Overlay has been notified about the request.
	*         EOS_InvalidParameters If any of the options are incorrect.
	*         EOS_NotConfigured If the Social Overlay is not properly configured.
	*         EOS_NoChange If the Social Overlay is already hidden.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|UI", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_UI_HideFriends"))
	void EOSUIHideFriends(UObject* WorldContextObject, FEOSUIHideFriendsOptions Options, const FOnHideFriendsCallback& Callback);

	/**
	* Gets the friends overlay visibility.
	*
	* @param Options Structure containing the Epic Online Services Account ID of the friends Social Overlay owner.
	*
	* @return EOS_TRUE If the overlay is visible.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|UI", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_UI_GetFriendsVisible"))
	static bool EOSUIGetFriendsVisible(UObject* WorldContextObject, FEOSUIGetFriendsVisibleOptions Options);

	/**
	 * Gets the friends overlay exclusive input state.
	 *
	 * @param Options Structure containing the Epic Account ID of the friends Social Overlay owner.
	 *
	 * @return EOS_TRUE If the overlay has exclusive input.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|UI", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_UI_GetFriendsExclusiveInput"))
	static bool EOSUIGetFriendsExclusiveInput(UObject* WorldContextObject, FEOSUIGetFriendsExclusiveInputOptions Options);

	/**
	* Updates the current Toggle Friends Key.  This key can be used by the user to toggle the friends
	* overlay when available. The default value represents `Shift + F3` as `((int32_t)EOS_UIK_Shift | (int32_t)EOS_UIK_F3)`.
	* The provided key should satisfy EOS_UI_IsValidKeyCombination. The value EOS_UIK_None is specially handled
	* by resetting the key binding to the system default.
	*
	* @param Options Structure containing the key combination to use.
	*
	* @return EOS_Success If the overlay has been notified about the request.
	*         EOS_InvalidParameters If any of the options are incorrect.
	*         EOS_NotConfigured If the overlay is not properly configured.
	*         EOS_NoChange If the key combination did not change.
	*
	* @see EOS_UI_IsValidKeyCombination
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|UI", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_UI_SetToggleFriendsKey"))
	static EOSResult EOSUISetToggleFriendsKey(UObject* WorldContextObject, FEOSUISetToggleFriendsKeyOptions Options);

	/**
	* Returns the current Toggle Friends Key.  This key can be used by the user to toggle the friends
	* overlay when available. The default value represents `Shift + F3` as `((int32_t)EOS_UIK_Shift | (int32_t)EOS_UIK_F3)`.
	*
	* @param Options Structure containing any options that are needed to retrieve the key.
	* @return A valid key combination which represent a single key with zero or more modifier keys.
	*         EOS_UIK_None will be returned if any error occurs.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|UI", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_UI_GetToggleFriendsKey"))
	static int32 EOSUIGetToggleFriendsKey(UObject* WorldContextObject, FEOSUIGetToggleFriendsKeyOptions Options);

	/**
	* Determine if a key combination is valid. A key combinations must have a single key and at least one modifier.
	* The single key must be one of the following: F1 through F12, Space, Backspace, Escape, or Tab.
	* The modifier key must be one or more of the following: Shift, Control, or Alt.
	*
	* @param KeyCombination The key to test.
	* @return  EOS_TRUE if the provided key combination is valid.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|UI", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_UI_IsValidKeyCombination"))
	static bool EOSUIIsValidKeyCombination(UObject* WorldContextObject, int32 KeyCombination);

	/**
	* Define any preferences for any display settings.
	*
	* @param Options Structure containing any options that are needed to set
	* 
	* @return EOS_Success If the overlay has been notified about the request.
	*         EOS_InvalidParameters If any of the options are incorrect.
	*         EOS_NotConfigured If the overlay is not properly configured.
	*         EOS_NoChange If the preferences did not change.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|UI", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_UI_SetDisplayPreference"))
	static EOSResult EOSUISetDisplayPreference(UObject* WorldContextObject, FEOSUISetDisplayPreferenceOptions Options);

	/**
	* Returns the current notification location display preference.
	* @return The current notification location display preference.
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|UI", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_UI_GetNotificationLocationPreference"))
	static EEOSUIENotificationLocation EOSUIGetNotificationLocationPreference(UObject* WorldContextObject);

	/**
	* Lets the SDK know that the given UI event ID has been acknowledged and should be released.
	*
	* @return An EOS_EResult is returned to indicate success or an error.
	*
	* EOS_Success is returned if the UI event ID has been acknowledged.
	* EOS_NotFound is returned if the UI event ID does not exist.
	*
	* @see EOS_Presence_JoinGameAcceptedCallbackInfo
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|UI", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_UI_AcknowledgeEventId"))
	static EOSResult EOSUIAcknowledgeEventId(UObject* WorldContextObject, FEOSUIAcknowledgeEventIdOptions Options);

	/**
	 * Requests that the Social Overlay open and display the "Block User" flow for the specified user.
	 *
	 * @param Callback A callback that is fired when the user exits the Block UI.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|UI", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_UI_ShowBlockPlayer"))
	void EOSUIShowBlockPlayer(UObject* WorldContextObject, FEOSUIShowBlockPlayerOptions Options, const FOnShowBlockPlayerCallback& Callback);

	/**
	 * Requests that the Social Overlay open and display the "Report User" flow for the specified user.
	 *
	 * @param Callback A callback that is fired when the user exits the Report UI.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|UI", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_UI_ShowReportPlayer"))
	void EOSUIShowReportPlayer(UObject* WorldContextObject, FEOSUIShowReportPlayerOptions Options, const FOnShowReportPlayerCallback& Callback);

	/**
	 * Sets the bIsPaused state of the overlay.
	 * While true then all notifications will be delayed until after the bIsPaused is false again.
	 * While true then the key and button events will not toggle the overlay.
	 * If the Overlay was visible before being paused then it will be hidden.
	 * If it is known that the Overlay should now be visible after being paused then it will be shown.
	 *
	 * @return EOS_Success If the overlay has been notified about the request.
	 *         EOS_IncompatibleVersion if the API version passed in is incorrect.
	 *         EOS_InvalidParameters If any of the options are incorrect.
	 *         EOS_NotConfigured If the overlay is not properly configured.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|UI", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_UI_PauseSocialOverlay"))
	EOSResult EOSUIPauseSocialOverlay(UObject* WorldContextObject, FEOSUIPauseSocialOverlayOptions Options);

	/**
	 * Gets the bIsPaused state of the overlay as set by any previous calls to EOS_UI_PauseSocialOverlay().
	 *
	 * @return EOS_TRUE If the overlay is paused.
	 *
	 * @see EOS_UI_PauseSocialOverlay
	 */
	UFUNCTION(BlueprintCallable, Category = "EOSCore|UI", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_UI_IsSocialOverlayPaused"))
	bool EOSUIIsSocialOverlayPaused(UObject* WorldContextObject, FEOSUIIsSocialOverlayPausedOptions Options);

	/**
	* Register to receive notifications when the overlay display settings are updated.
	* Newly registered handlers will always be called the next tick with the current state.
	* @note must call RemoveNotifyDisplaySettingsUpdated to remove the notification.
	*
	* @param Options Structure containing information about the request.
	* @param Callback A callback that is fired when the overlay display settings are updated.
	*
	* @return handle representing the registered callback
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|UI", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_UI_AddNotifyDisplaySettingsUpdated"))
	static FEOSNotificationId EOSUIAddNotifyDisplaySettingsUpdated(UObject* WorldContextObject, const FEOSUIAddNotifyDisplaySettingsUpdatedOptions Options, const FOnDisplaySettingsUpdatedCallback& Callback);

	/**
	* Unregister from receiving notifications when the overlay display settings are updated.
	*
	* @param Id Handle representing the registered callback
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|UI", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_UI_RemoveNotifyDisplaySettingsUpdated"))
	static void EOSUIRemoveNotifyDisplaySettingsUpdated(UObject* WorldContextObject, FEOSNotificationId Id);
private:
	static void Internal_OnShowFriendsCallback(const EOS_UI_ShowFriendsCallbackInfo* Data);
	static void Internal_OnHideFriendsCallback(const EOS_UI_HideFriendsCallbackInfo* Data);
	static void Internal_OnShowBlockPlayerCallback(const EOS_UI_OnShowBlockPlayerCallbackInfo* Data);
	static void Internal_OnShowReportPlayerCallback(const EOS_UI_OnShowReportPlayerCallbackInfo* Data);
protected:
	TMap<FEOSNotificationId, FDisplaySettingsUpdatedCallback*> m_DisplaySettingsUpdatedCallbacks;
};
