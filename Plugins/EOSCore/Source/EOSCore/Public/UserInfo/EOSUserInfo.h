/**
* Copyright (C) 2017-2022 | eelDev AB
*
 official EOSCore Documentation: https://eeldev.com
*/

#pragma once

#include "CoreMinimal.h"
#include "EOSCoreModule.h"
#include "eos_userinfo_types.h"
#include "UserInfo/EOSUserInfoTypes.h"
#include "EOSUserInfo.generated.h"

/**
 * The UserInfo Interface is used to receive user information for Epic account IDs from the backend services and to retrieve that information once it is cached.
 * All UserInfo Interface calls take a handle of type EOS_HUserInfo as the first parameter.
 * This handle can be retrieved from a EOS_HPlatform handle by using the EOS_Platform_GetUserInfoInterface function.
 *
 * NOTE: At this time, this feature is only available for products that are part of the Epic Games store.
 *
 * @see EOS_Platform_GetUserInfoInterface
 */

UCLASS()
class EOSCORE_API UCoreUserInfo : public UEOSCoreSubsystem
{
	GENERATED_BODY()
public:
	/**
	 * The UserInfo Interface is used to receive user information for Epic account IDs from the backend services and to retrieve that information once it is cached.
	 * All UserInfo Interface calls take a handle of type EOS_HUserInfo as the first parameter.
	 * This handle can be retrieved from a EOS_HPlatform handle by using the EOS_Platform_GetUserInfoInterface function.
	 *
	 * NOTE: At this time, this feature is only available for products that are part of the Epic Games store.
	 *
	 * @see EOS_Platform_GetUserInfoInterface
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EOSCore|Subsystems", meta = (WorldContext = "WorldContextObject"))
	static UCoreUserInfo* GetUserInfo(UObject* WorldContextObject);

public:
	/**
	* EOS_UserInfo_QueryUserInfo is used to start an asynchronous query to retrieve information, such as display name, about another account.
	* Once the callback has been fired with a successful ResultCode, it is possible to call EOS_UserInfo_CopyUserInfo to receive an EOS_UserInfo containing the available information.
	*
	* @param Options structure containing the input parameters
	* @param Callback a callback that is fired when the async operation completes, either successfully or in error
	*
	* @see EOS_UserInfo
	* @see EOS_UserInfo_CopyUserInfo
	* @see EOS_UserInfo_QueryUserInfoOptions
	* @see EOS_UserInfo_OnQueryUserInfoCallback
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|UserInfo", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_UserInfo_QueryUserInfo"))
	void EOSUserInfoQueryUserInfo(UObject* WorldContextObject, FEOSUserInfoQueryUserInfoOptions Options, const FOnQueryUserInfoCallback& Callback);

	/**
	* EOS_UserInfo_QueryUserInfoByDisplayName is used to start an asynchronous query to retrieve user information by display name. This can be useful for getting the EOS_EpicAccountId for a display name.
	* Once the callback has been fired with a successful ResultCode, it is possible to call EOS_UserInfo_CopyUserInfo to receive an EOS_UserInfo containing the available information.
	*
	* @param Options structure containing the input parameters
	* @param Callback a callback that is fired when the async operation completes, either successfully or in error
	*
	* @see EOS_UserInfo
	* @see EOS_UserInfo_CopyUserInfo
	* @see EOS_UserInfo_QueryUserInfoByDisplayNameOptions
	* @see EOS_UserInfo_OnQueryUserInfoByDisplayNameCallback
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|UserInfo", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_UserInfo_QueryUserInfoByDisplayName"))
	void EOSUserInfoQueryUserInfoByDisplayName(UObject* WorldContextObject, FEOSUserInfoQueryUserInfoByDisplayNameOptions Options, const FOnQueryUserInfoByDisplayNameCallback& Callback);

	/**
	* EOS_UserInfo_QueryUserInfoByExternalAccount is used to start an asynchronous query to retrieve user information by external accounts.
	* This can be useful for getting the EOS_EpicAccountId for external accounts.
	* Once the callback has been fired with a successful ResultCode, it is possible to call CopyUserInfo to receive an EOS_UserInfo containing the available information.
	*
	* @param Options structure containing the input parameters
	* @param Callback a callback that is fired when the async operation completes, either successfully or in error
	*
	* @see EOS_UserInfo
	* @see EOS_UserInfo_QueryUserInfoByExternalAccountOptions
	* @see EOS_UserInfo_OnQueryUserInfoByExternalAccountCallback
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|UserInfo", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_UserInfo_QueryUserInfoByExternalAccount"))
	void EOSUserInfoQueryUserInfoByExternalAccount(UObject* WorldContextObject, FEOSUserInfoQueryUserInfoByExternalAccountOptions Options, const FOnQueryUserInfoByExternalAccountCallback& Callback);

	/**
	* EOS_UserInfo_CopyUserInfo is used to immediately retrieve a copy of user information based on an Epic Online Services Account ID, cached by a previous call to EOS_UserInfo_QueryUserInfo.
	* If the call returns an EOS_Success result, the out parameter, OutUserInfo, must be passed to EOS_UserInfo_Release to release the memory associated with it.
	*
	* @param Options structure containing the input parameters
	* @param OutUserInfo out parameter used to receive the EOS_UserInfo structure.
	*
	* @return EOS_Success if the information is available and passed out in OutUserInfo
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_IncompatibleVersion if the API version passed in is incorrect
	*         EOS_NotFound if the user info is not locally cached. The information must have been previously cached by a call to EOS_UserInfo_QueryUserInfo
	*
	* @see EOS_UserInfo
	* @see EOS_UserInfo_CopyUserInfoOptions
	* @see EOS_UserInfo_Release
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|UserInfo", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_UserInfo_CopyUserInfo"))
	static EOSResult EOSUserInfoCopyUserInfo(UObject* WorldContextObject, FEOSUserInfoCopyUserInfoOptions Options, FEOSUserInfo& OutUserInfo);

	/**
	* Fetch the number of external user infos that are cached locally.
	*
	* @param Options The options associated with retrieving the external user info count
	*
	* @see EOS_UserInfo_CopyExternalUserInfoByIndex
	*
	* @return The number of external user infos, or 0 if there is an error
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|UserInfo", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_UserInfo_GetExternalUserInfoCount"))
	static int32 EOSUserInfoGetExternalUserInfoCount(UObject* WorldContextObject, FEOSUserInfoGetExternalUserInfoCountOptions Options);

	/**
	* Fetches an external user info from a given index.
	*
	* @param Options Structure containing the index being accessed
	* @param OutExternalUserInfo The external user info. If it exists and is valid, use EOS_UserInfo_ExternalUserInfo_Release when finished
	*
	* @see EOS_UserInfo_ExternalUserInfo_Release
	*
	* @return EOS_Success if the information is available and passed out in OutExternalUserInfo
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_NotFound if the external user info is not found
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|UserInfo", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_UserInfo_CopyExternalUserInfoByIndex"))
	static EOSResult EOSUserInfoCopyExternalUserInfoByIndex(UObject* WorldContextObject, FEOSUserInfoCopyExternalUserInfoByIndexOptions Options, FEOSUserInfoExternalUserInfo& OutExternalUserInfo);

	/**
	* Fetches an external user info for a given external account type.
	*
	* @param Options Structure containing the account type being accessed
	* @param OutExternalUserInfo The external user info. If it exists and is valid, use EOS_UserInfo_ExternalUserInfo_Release when finished
	*
	* @see EOS_UserInfo_ExternalUserInfo_Release
	*
	* @return EOS_Success if the information is available and passed out in OutExternalUserInfo
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_NotFound if the external user info is not found
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|UserInfo", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_UserInfo_CopyExternalUserInfoByAccountType"))
	static EOSResult EOSUserInfoCopyExternalUserInfoByAccountType(UObject* WorldContextObject, FEOSUserInfoCopyExternalUserInfoByAccountTypeOptions Options, FEOSUserInfoExternalUserInfo& OutExternalUserInfo);

	/**
	* Fetches an external user info for a given external account ID.
	*
	* @param Options Structure containing the account ID being accessed
	* @param OutExternalUserInfo The external user info. If it exists and is valid, use EOS_UserInfo_ExternalUserInfo_Release when finished
	*
	* @see EOS_UserInfo_ExternalUserInfo_Release
	*
	* @return EOS_Success if the information is available and passed out in OutExternalUserInfo
	*         EOS_InvalidParameters if you pass a null pointer for the out parameter
	*         EOS_NotFound if the external user info is not found
	*/
	UFUNCTION(BlueprintCallable, Category = "EOSCore|UserInfo", meta = (WorldContext = "WorldContextObject", DisplayName = "EOS_UserInfo_CopyExternalUserInfoByAccountId"))
	static EOSResult EOSUserInfoCopyExternalUserInfoByAccountId(UObject* WorldContextObject, FEOSUserInfoCopyExternalUserInfoByAccountIdOptions Options, FEOSUserInfoExternalUserInfo& OutExternalUserInfo);
private:
	static void Internal_OnQueryUserInfoCallback(const EOS_UserInfo_QueryUserInfoCallbackInfo* Data);
	static void Internal_OnQueryUserInfoByDisplayNameCallback(const EOS_UserInfo_QueryUserInfoByDisplayNameCallbackInfo* Data);
	static void Internal_OnQueryUserInfoByExternalAccountCallback(const EOS_UserInfo_QueryUserInfoByExternalAccountCallbackInfo* Data);
};
