/**
* Copyright (C) 2017-2022 | eelDev AB
*
* EOSCore Documentation: https://eeldev.com
*/

#include "OnlineSubsystemEOSCore.h"
#include "OnlineIdentityEOSCore.h"
#include "OnlineSessionEOSCore.h"
#include "OnlineStatsEOSCore.h"
#include "OnlineLeaderboardsEOSCore.h"
#include "OnlineAchievementsEOSCore.h"
#include "OnlineStoreEOSCore.h"
#include "OnlineUserEOSCore.h"
#include "SocketSubsystemEOSCore.h"
#include "OnlineSubsystemModuleEOSCore.h"
#include "OnlineSubsystemEOSCorePrivatePCH.h"

#if PLATFORM_ANDROID
#include "Android/eos_android.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"

extern JavaVM* GJavaVM;
#endif

#if PLATFORM_SWITCH
#include <Switch/eos_Switch.h>
#endif

#if PLATFORM_IOS
#include <eos_ios.h>

static FString PlatformCreateCacheDir(const FString &ArtifactName, const FString &EOSSettingsCacheDir)
{
	NSString *BundleIdentifier = [[NSBundle mainBundle] bundleIdentifier];
	NSString *CacheDirectory = NSTemporaryDirectory();									// Potentially use NSCachesDirectory
	CacheDirectory = [CacheDirectory stringByAppendingPathComponent:BundleIdentifier];

	FTCHARToUTF8 TCArtifactName(*ArtifactName);
	NSString *NSArtifactName = [NSString stringWithUTF8String:TCArtifactName.Get()];
	CacheDirectory = [CacheDirectory stringByAppendingPathComponent:NSArtifactName];

	FTCHARToUTF8 TCEOSSettingsCacheDir(*EOSSettingsCacheDir);
	NSString *NSEOSSettingsCacheDir = [NSString stringWithUTF8String:TCArtifactName.Get()];
	CacheDirectory = [CacheDirectory stringByAppendingPathComponent:NSEOSSettingsCacheDir];

	const char *CStrCacheDirectory = [CacheDirectory UTF8String];

	UE_LOG(LogTemp, Warning, TEXT("IOS CacheDir: %s"), *FString(UTF8_TO_TCHAR(CStrCacheDirectory)));
	return FString(UTF8_TO_TCHAR(CStrCacheDirectory));
}
#endif

#if PLATFORM_WINDOWS
#include "Windows/eos_Windows.h"
#endif

#if WITH_EDITOR
#include "Editor.h"
#endif
#include "EOSCoreSettings.h"
#include "OnlineVoiceEOSCore.h"
#include "HAL/FileManager.h"

DECLARE_CYCLE_STAT(TEXT("Tick"), STAT_EOS_Tick, STATGROUP_EOS);

#define LOCTEXT_NAMESPACE "EOS"

FOnEOSCoreLog FOnlineSubsystemEOSCore::s_OnEOSCoreLog;

typedef struct _InternalData
{
	int32_t ApiVersion;
	const char* BackendEnvironment;
} InternalData;

void EOS_CALL EOSFree(void* Ptr)
{
	FMemory::Free(Ptr);
}

void* EOS_CALL EOSMalloc(size_t Size, size_t Alignment)
{
	return FMemory::Malloc(Size, Alignment);
}

void* EOS_CALL EOSRealloc(void* Ptr, size_t Size, size_t Alignment)
{
	return FMemory::Realloc(Ptr, Size, Alignment);
}

void EOS_CALL EOSLog(const EOS_LogMessage* InMsg)
{
	if (GLog == nullptr)
	{
		return;
	}

	FOnlineSubsystemEOSCore::s_OnEOSCoreLog.ExecuteIfBound(InMsg);

	switch (InMsg->Level)
	{
	case EOS_ELogLevel::EOS_LOG_Fatal:
		{
			LogError("%s: %s", ANSI_TO_TCHAR(InMsg->Category), ANSI_TO_TCHAR(InMsg->Message));
			UE_LOG_ONLINE(Fatal, TEXT("EOSSDK-%s: %s"), ANSI_TO_TCHAR(InMsg->Category), ANSI_TO_TCHAR(InMsg->Message));
			break;
		}
	case EOS_ELogLevel::EOS_LOG_Error:
		{
			LogError("%s: %s", ANSI_TO_TCHAR(InMsg->Category), ANSI_TO_TCHAR(InMsg->Message));
			UE_LOG_ONLINE(Error, TEXT("EOSSDK-%s: %s"), ANSI_TO_TCHAR(InMsg->Category), ANSI_TO_TCHAR(InMsg->Message));
			break;
		}
	case EOS_ELogLevel::EOS_LOG_Warning:
		{
			LogWarning("%s: %s", ANSI_TO_TCHAR(InMsg->Category), ANSI_TO_TCHAR(InMsg->Message));
			UE_LOG_ONLINE(Warning, TEXT("EOSSDK-%s: %s"), ANSI_TO_TCHAR(InMsg->Category), ANSI_TO_TCHAR(InMsg->Message));
			break;
		}
	case EOS_ELogLevel::EOS_LOG_Verbose:
		{
			LogVerbose("%s: %s", ANSI_TO_TCHAR(InMsg->Category), ANSI_TO_TCHAR(InMsg->Message));
			UE_LOG_ONLINE(Verbose, TEXT("EOSSDK-%s: %s"), ANSI_TO_TCHAR(InMsg->Category), ANSI_TO_TCHAR(InMsg->Message));
			break;
		}
	case EOS_ELogLevel::EOS_LOG_VeryVerbose:
		{
			LogVeryVerbose("%s: %s", ANSI_TO_TCHAR(InMsg->Category), ANSI_TO_TCHAR(InMsg->Message));
			UE_LOG_ONLINE(VeryVerbose, TEXT("EOSSDK-%s: %s"), ANSI_TO_TCHAR(InMsg->Category), ANSI_TO_TCHAR(InMsg->Message));
			break;
		}
	case EOS_ELogLevel::EOS_LOG_Info:
	default:
		{
			UE_LOG_ONLINE(Log, TEXT("EOSSDK-%s: %s"), ANSI_TO_TCHAR(InMsg->Category), ANSI_TO_TCHAR(InMsg->Message));
			break;
		}
	}
}

#define EOS_ENCRYPTION_KEY_MAX_LENGTH 64
#define EOS_ENCRYPTION_KEY_MAX_BUFFER_LEN (EOS_ENCRYPTION_KEY_MAX_LENGTH + 1)

struct FEOSPlatformOptions : public EOS_Platform_Options
{
public:
	FEOSPlatformOptions()
		: EOS_Platform_Options()
	{
		ApiVersion = EOS_PLATFORM_OPTIONS_API_LATEST;
		ProductId = ProductIdAnsi;
		SandboxId = SandboxIdAnsi;
		DeploymentId = DeploymentIdAnsi;
		ClientCredentials.ClientId = ClientIdAnsi;
		ClientCredentials.ClientSecret = ClientSecretAnsi;
		CacheDirectory = CacheDirectoryAnsi;
		EncryptionKey = EncryptionKeyAnsi;
	}

	char ClientIdAnsi[EOSCORE_OSS_STRING_BUFFER_LENGTH];
	char ClientSecretAnsi[EOSCORE_OSS_STRING_BUFFER_LENGTH];
	char ProductIdAnsi[EOSCORE_OSS_STRING_BUFFER_LENGTH];
	char SandboxIdAnsi[EOSCORE_OSS_STRING_BUFFER_LENGTH];
	char DeploymentIdAnsi[EOSCORE_OSS_STRING_BUFFER_LENGTH];
	char CacheDirectoryAnsi[EOSCORE_OSS_STRING_BUFFER_LENGTH];
	char EncryptionKeyAnsi[EOS_ENCRYPTION_KEY_MAX_BUFFER_LEN];
};

static char GProductNameAnsi[EOSCORE_PRODUCTNAME_MAX_BUFFER_LEN];
static char GProductVersionAnsi[EOSCORE_PRODUCTVERSION_MAX_BUFFER_LEN];
FString ProductName;
FString ProductVersion;

EOS_PlatformHandle* GEOSPlatformHandle = NULL;

bool FOnlineSubsystemEOSCore::ModuleInit()
{
	EOS_InitializeOptions SDKOptions = {};
	SDKOptions.ApiVersion = EOS_INITIALIZE_API_LATEST;
	ProductName = FApp::GetProjectName();
	FCStringAnsi::Strncpy(GProductNameAnsi, TCHAR_TO_UTF8(*ProductName), EOSCORE_PRODUCTNAME_MAX_BUFFER_LEN);
	SDKOptions.ProductName = GProductNameAnsi;
	ProductVersion = FNetworkVersion::GetProjectVersion();

	if (ProductVersion.IsEmpty())
	{
		ProductVersion = TEXT("Unknown");
	}

	FCStringAnsi::Strncpy(GProductVersionAnsi, TCHAR_TO_UTF8(*ProductVersion), EOSCORE_PRODUCTVERSION_MAX_BUFFER_LEN);
	SDKOptions.ProductVersion = GProductVersionAnsi;
	SDKOptions.AllocateMemoryFunction = &EOSMalloc;
	SDKOptions.ReallocateMemoryFunction = &EOSRealloc;
	SDKOptions.ReleaseMemoryFunction = &EOSFree;

#if PLATFORM_ANDROID
	static EOS_Android_InitializeOptions AndroidOptions = { 0 };
	AndroidOptions.ApiVersion = EOS_ANDROID_INITIALIZEOPTIONS_API_LATEST;

	AndroidOptions.Reserved = nullptr;
	AndroidOptions.OptionalInternalDirectory = nullptr;
	AndroidOptions.OptionalExternalDirectory = nullptr;
	
	SDKOptions.SystemInitializeOptions = &AndroidOptions;
#endif

	EOS_EResult InitResult = EOS_Initialize(&SDKOptions);
	if (InitResult != EOS_EResult::EOS_Success)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore: failed to initialize the EOS SDK with result code (%s)"), ANSI_TO_TCHAR(EOS_EResult_ToString(InitResult)));
		return false;
	}
#if !UE_BUILD_SHIPPING
	InitResult = EOS_Logging_SetCallback(&EOSLog);
	if (InitResult != EOS_EResult::EOS_Success)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore: failed to init logging with result code %s"), ANSI_TO_TCHAR(EOS_EResult_ToString(InitResult)));
	}
	EOS_Logging_SetLogLevel(EOS_ELogCategory::EOS_LC_ALL_CATEGORIES, EOS_ELogLevel::EOS_LOG_VeryVerbose);
#endif

	if (IsRunningGame() || IsRunningDedicatedServer())
	{
		GEOSPlatformHandle = FOnlineSubsystemEOSCore::PlatformCreate();
	}

	return true;
}

EOS_PlatformHandle* FOnlineSubsystemEOSCore::PlatformCreate()
{
	FEOSPlatformOptions PlatformOptions;
	PlatformOptions.Reserved = nullptr;

	EOS_Platform_RTCOptions RtcOptions = {};
	RtcOptions.ApiVersion = EOS_PLATFORM_RTCOPTIONS_API_LATEST;
	RtcOptions.PlatformSpecificOptions = nullptr;
	
	FString CacheDir;

	if (FString(FPlatformProcess::UserDir()).Len() > 0)
	{
		CacheDir = FPlatformProcess::UserDir() / FString("CacheDir");
	}
	else
	{
		LogWarning("CacheDir: %s invalid, attempting to use Project Saved Dir..", *CacheDir);

		CacheDir = FPaths::ProjectSavedDir() / FString("CacheDir");
	}

	FString OverrideCacheDir;
	GConfig->GetString(INI_SECTION, TEXT("OverrideCacheDir"), OverrideCacheDir, GEngineIni);

	if (OverrideCacheDir.Len() > 0)
	{
		LogVerbose("Overriding CacheDir: %s", *OverrideCacheDir);

		CacheDir = OverrideCacheDir;
	}

#if PLATFORM_IOS
	CacheDir = PlatformCreateCacheDir(FString("Test"), FString("Test123"));
#endif

#if PLATFORM_ANDROID
	extern FString GInternalFilePath;
	CacheDir = GInternalFilePath;
	//CacheDir = IFileManager::Get().ConvertToAbsolutePathForExternalAppForWrite(*GInternalFilePath);
	
//	JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
//	
//	auto EnvClass = NewScopedJavaObject(Env, Env->FindClass("android/os/Environment"));
//	jmethodID getExternalStorageDir = Env->GetStaticMethodID(*EnvClass, "getExternalStorageDirectory", "()Ljava/io/File;");
//	auto externalStoragePath = NewScopedJavaObject(Env, Env->CallStaticObjectMethod(*EnvClass, getExternalStorageDir, nullptr));
//	jmethodID getFilePath = Env->GetMethodID(Env->FindClass("java/io/File"), "getPath", "()Ljava/lang/String;");
//	
//	CacheDir = FJavaHelper::FStringFromLocalRef(Env, (jstring)Env->CallObjectMethod(*externalStoragePath, getFilePath, nullptr));
	
//	extern FString GFilePathBase;
//	CacheDir = GFilePathBase + FString("/UE4Game/") + FApp::GetProjectName() + "/" + FApp::GetProjectName() + FString("/");
#endif

	CacheDir = IFileManager::Get().ConvertToAbsolutePathForExternalAppForWrite(*CacheDir);
	LogVerbose("Using CacheDir: %s", *CacheDir);

	FCStringAnsi::Strncpy(PlatformOptions.CacheDirectoryAnsi, TCHAR_TO_UTF8(*CacheDir), EOSCORE_OSS_STRING_BUFFER_LENGTH);

	//PlatformOptions.bIsServer = IsRunningDedicatedServer() ? EOS_TRUE : EOS_FALSE;

	bool bDisableEOSOverlay = false;
	GConfig->GetBool(INI_SECTION, TEXT("bDisableEOSOverlay"), bDisableEOSOverlay, GEngineIni);

	if (bDisableEOSOverlay)
	{
		PlatformOptions.Flags = EOS_PF_DISABLE_OVERLAY;
	}
	else
	{
		uint64 OverlayFlags = 0;
		PlatformOptions.Flags = IsRunningGame() ? OverlayFlags : EOS_PF_DISABLE_OVERLAY;
	}

	FString ConfigName;
	FParse::Value(FCommandLine::Get(), TEXT("EOSConfig="), ConfigName);

	FEOSConfig EOSConfig;
	if (!UEOSCoreSettings::GetEOSConfig(ConfigName, EOSConfig))
	{
		UE_LOG_ONLINE(Error, TEXT("PlatformCreate() failed to find configuration: (%s)"), *ConfigName);

		return nullptr;
	}

	FCStringAnsi::Strncpy(PlatformOptions.ClientIdAnsi, TCHAR_TO_UTF8(*EOSConfig.ClientId), EOSCORE_OSS_STRING_BUFFER_LENGTH);
	FCStringAnsi::Strncpy(PlatformOptions.ClientSecretAnsi, TCHAR_TO_UTF8(*EOSConfig.ClientSecret), EOSCORE_OSS_STRING_BUFFER_LENGTH);
	FCStringAnsi::Strncpy(PlatformOptions.ProductIdAnsi, TCHAR_TO_UTF8(*EOSConfig.ProductId), EOSCORE_OSS_STRING_BUFFER_LENGTH);
	FCStringAnsi::Strncpy(PlatformOptions.SandboxIdAnsi, TCHAR_TO_UTF8(*EOSConfig.SandboxId), EOSCORE_OSS_STRING_BUFFER_LENGTH);
	FCStringAnsi::Strncpy(PlatformOptions.DeploymentIdAnsi, TCHAR_TO_UTF8(*EOSConfig.DeploymentId), EOSCORE_OSS_STRING_BUFFER_LENGTH);
	FCStringAnsi::Strncpy(PlatformOptions.EncryptionKeyAnsi, TCHAR_TO_UTF8(*EOSConfig.EncryptionKey), EOS_ENCRYPTION_KEY_MAX_BUFFER_LEN);

	FString Path;

#if PLATFORM_WINDOWS
	Path = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*FPaths::Combine(FPaths::EngineDir(), TEXT("Binaries/ThirdParty/Windows/XAudio2_9"), PLATFORM_64BITS ? TEXT("x64") : TEXT("x86"), TEXT("xaudio2_9redist.dll")));
	
	EOS_Windows_RTCOptions Windows_RTCOptions;
	Windows_RTCOptions.ApiVersion = EOS_WINDOWS_RTCOPTIONS_API_LATEST;
	Windows_RTCOptions.XAudio29DllPath = new char[512];
	FCStringAnsi::Strncpy(const_cast<char*>(Windows_RTCOptions.XAudio29DllPath), TCHAR_TO_UTF8(*Path), 512);
	RtcOptions.PlatformSpecificOptions = &Windows_RTCOptions;
#endif
	PlatformOptions.RTCOptions = &RtcOptions;
	
	if (EOSConfig.bEnableRTC == false)
	{
		PlatformOptions.RTCOptions = nullptr;
		LogVerbose("RTC disabled");
	}
	else
	{
		LogVerbose("RTC enabled (%s)", *Path);
	}

	if (EOSConfig.ProductId.Len() == 0)
	{
		LogError("EOSCore missing ProductId in DefaultEngine.ini");
	}

	if (EOSConfig.SandboxId.Len() == 0)
	{
		LogError("EOSCore missing SandboxId in DefaultEngine.ini");
	}

	if (EOSConfig.DeploymentId.Len() == 0)
	{
		LogError("EOSCore missing DeploymentId in DefaultEngine.ini");
	}

	if (EOSConfig.ClientId.Len() == 0)
	{
		LogError("EOSCore missing ClientId in DefaultEngine.ini");
	}

	if (EOSConfig.ClientSecret.Len() == 0)
	{
		LogError("EOSCore missing ClientSecret in DefaultEngine.ini");
	}

	if (EOSConfig.EncryptionKey.Len() == 0)
	{
		LogError("EOSCore missing EncryptKey in DefaultEngine.ini");
	}

	int32 TickBudgetInMilliseconds = 0;
	GConfig->GetInt(INI_SECTION, TEXT("TickBudgetInMilliseconds"), TickBudgetInMilliseconds, GEngineIni);

	PlatformOptions.TickBudgetInMilliseconds = TickBudgetInMilliseconds;

	if (!PlatformOptions.IntegratedPlatformOptionsContainerHandle)
	{
		EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainerOptions CreateIntegratedPlatformOptionsContainerOptions = {};
		CreateIntegratedPlatformOptionsContainerOptions.ApiVersion = EOS_INTEGRATEDPLATFORM_CREATEINTEGRATEDPLATFORMOPTIONSCONTAINER_API_LATEST;

		EOS_EResult Result = EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer(&CreateIntegratedPlatformOptionsContainerOptions, &PlatformOptions.IntegratedPlatformOptionsContainerHandle);
		if (Result != EOS_EResult::EOS_Success)
		{
			LogError("Failed to create integrated platform options container", ANSI_TO_TCHAR(EOS_EResult_ToString(Result)));
		}
	}

	EOS_PlatformHandle* EOSPlatformHandle = EOS_Platform_Create(&PlatformOptions);

	if (PlatformOptions.IntegratedPlatformOptionsContainerHandle)
	{
		EOS_IntegratedPlatformOptionsContainer_Release(PlatformOptions.IntegratedPlatformOptionsContainerHandle);
	}

	if (EOSPlatformHandle == nullptr)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore::PlatformCreate() failed to init EOS platform"));
		return nullptr;
	}

	

	return EOSPlatformHandle;
}

bool FOnlineSubsystemEOSCore::IsEnabled() const
{
	bool bEnableSubsystem = FOnlineSubsystemImpl::IsEnabled();

	if (bEnableSubsystem)
	{
		if (!GConfig->GetBool(TEXT("/Script/EOSCore.EOSCoreSettings"), TEXT("bEnabled"), bEnableSubsystem, GEngineIni))
		{
			GConfig->GetBool(TEXT("EOSCore"), TEXT("bEnabled"), bEnableSubsystem, GEngineIni);
		}
	}

	/*
		if (bEnableSubsystem)
		{
	#if UE_EDITOR
		bEnableSubsystem = FParse::Param(FCommandLine::Get(), TEXT("MultiprocessOSS"));
	#endif
		}*/

	return bEnableSubsystem;
}

bool FOnlineSubsystemEOSCore::Init()
{
	bWasLaunchedByEGS = FParse::Param(FCommandLine::Get(), TEXT("EpicPortal"));

	if (IsRunningGame() || IsRunningDedicatedServer())
	{
		m_EOSPlatformHandle = GEOSPlatformHandle;
	}
	else
	{
		m_EOSPlatformHandle = FOnlineSubsystemEOSCore::PlatformCreate();
	}

	if (m_EOSPlatformHandle == nullptr)
	{
		return false;
	}

	m_AuthHandle = EOS_Platform_GetAuthInterface(m_EOSPlatformHandle);
	if (m_AuthHandle == nullptr)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore: couldn't get auth handle"));
		return false;
	}
	m_UserInfoHandle = EOS_Platform_GetUserInfoInterface(m_EOSPlatformHandle);
	if (m_UserInfoHandle == nullptr)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore: couldn't get user info handle"));
		return false;
	}
	m_FriendsHandle = EOS_Platform_GetFriendsInterface(m_EOSPlatformHandle);
	if (m_FriendsHandle == nullptr)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore: couldn't get friends handle"));
		return false;
	}
	m_PresenceHandle = EOS_Platform_GetPresenceInterface(m_EOSPlatformHandle);
	if (m_PresenceHandle == nullptr)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore: couldn't get presence handle"));
		return false;
	}
	m_ConnectHandle = EOS_Platform_GetConnectInterface(m_EOSPlatformHandle);
	if (m_ConnectHandle == nullptr)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore: couldn't get connect handle"));
		return false;
	}
	m_SessionsHandle = EOS_Platform_GetSessionsInterface(m_EOSPlatformHandle);
	if (m_SessionsHandle == nullptr)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore: couldn't get sessions handle"));
		return false;
	}
	m_StatsHandle = EOS_Platform_GetStatsInterface(m_EOSPlatformHandle);
	if (m_StatsHandle == nullptr)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore: couldn't get stats handle"));
		return false;
	}
	m_LeaderboardsHandle = EOS_Platform_GetLeaderboardsInterface(m_EOSPlatformHandle);
	if (m_LeaderboardsHandle == nullptr)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore: couldn't get leaderboards handle"));
		return false;
	}
	m_MetricsHandle = EOS_Platform_GetMetricsInterface(m_EOSPlatformHandle);
	if (m_MetricsHandle == nullptr)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore: couldn't get metrics handle"));
		return false;
	}
	m_AchievementsHandle = EOS_Platform_GetAchievementsInterface(m_EOSPlatformHandle);
	if (m_AchievementsHandle == nullptr)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore: couldn't get achievements handle"));
		return false;
	}
	m_P2PHandle = EOS_Platform_GetP2PInterface(m_EOSPlatformHandle);
	if (m_P2PHandle == nullptr)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore: couldn't get p2p handle"));
		return false;
	}
	m_LobbyHandle = EOS_Platform_GetLobbyInterface(m_EOSPlatformHandle);
	if (m_LobbyHandle == nullptr)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore: couldn't get lobby handle"));
		return false;
	}
	m_PlayerDataStorageHandle = EOS_Platform_GetPlayerDataStorageInterface(m_EOSPlatformHandle);
	if (m_PlayerDataStorageHandle == nullptr)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore: couldn't get player data storage handle"));
		return false;
	}
	m_TitleStorageHandle = EOS_Platform_GetTitleStorageInterface(m_EOSPlatformHandle);
	if (m_TitleStorageHandle == nullptr)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore: couldn't get title storage handle"));
		return false;
	}
	m_UiHandle = EOS_Platform_GetUIInterface(m_EOSPlatformHandle);
	if (m_UiHandle == nullptr)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore: couldn't get ui handle"));
		return false;
	}
	m_ModsHandle = EOS_Platform_GetModsInterface(m_EOSPlatformHandle);
	if (m_ModsHandle == nullptr)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore: couldn't get mods handle"));
	}
	m_ReportsHandle = EOS_Platform_GetReportsInterface(m_EOSPlatformHandle);
	if (m_ReportsHandle == nullptr)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore: couldn't get reports handle"));
	}
	m_SanctionsHandle = EOS_Platform_GetSanctionsInterface(m_EOSPlatformHandle);
	if (m_SanctionsHandle == nullptr)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore: couldn't get sanctions handle"));
	}
	m_AntiCheatClientHandle = EOS_Platform_GetAntiCheatClientInterface(m_EOSPlatformHandle);
	if (m_AntiCheatClientHandle == nullptr)
	{
		UE_LOG_ONLINE(Log, TEXT("FOnlineSubsystemEOSCore: couldn't get anticheatclient handle"));
	}
	m_AntiCheatServerHandle = EOS_Platform_GetAntiCheatServerInterface(m_EOSPlatformHandle);
	if (m_AntiCheatServerHandle == nullptr)
	{
		UE_LOG_ONLINE(Log, TEXT("FOnlineSubsystemEOSCore: couldn't get anticheatserver handle"));
	}

	m_HRTCHandle = EOS_Platform_GetRTCInterface(m_EOSPlatformHandle);
	if (m_HRTCHandle == nullptr)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore: couldn't get anticheatserver handle"));
	}

	m_HRTCAdminHandle = EOS_Platform_GetRTCAdminInterface(m_EOSPlatformHandle);
	if (m_HRTCAdminHandle == nullptr)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore: couldn't get anticheatserver handle"));
	}

	m_EcomHandle = EOS_Platform_GetEcomInterface(m_EOSPlatformHandle);

	if (m_EcomHandle == nullptr)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore: couldn't get ecom handle"));
		return false;
	}

	m_ProgressionSnapshotHandle = EOS_Platform_GetProgressionSnapshotInterface(m_EOSPlatformHandle);

	if (m_ProgressionSnapshotHandle == nullptr)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore: couldn't get progression snapshot handle"));
		return false;
	}

	m_CustomInvitesHandle = EOS_Platform_GetCustomInvitesInterface(m_EOSPlatformHandle);

	if (m_CustomInvitesHandle == nullptr)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore: couldn't get custom invites handle"));
		return false;
	}

	if (bWasLaunchedByEGS)
	{
		m_StoreInterfacePtr = MakeShareable(new FOnlineStoreEOSCore(this));
	}

	m_VoiceInterface = MakeShareable(new FOnlineVoiceImplEOSCore(this));

	//if (m_VoiceInterface)
	//{
	//	m_VoiceInterface->Init();
	//}
	
	m_SocketSubsystem = MakeShareable(new FSocketSubsystemEOSCore(this));

	FString ErrorMessage;
	m_SocketSubsystem->Init(ErrorMessage);

	FCStringAnsi::Strncpy(m_ProductNameAnsi, GProductNameAnsi, EOSCORE_PRODUCTNAME_MAX_BUFFER_LEN);
	FCStringAnsi::Strncpy(m_ProductVersionAnsi, GProductVersionAnsi, EOSCORE_PRODUCTVERSION_MAX_BUFFER_LEN);

	m_IdentityInterfacePtr = MakeShareable(new FOnlineIdentityEOSCore(this));
	m_SessionInterfacePtr = MakeShareable(new FOnlineSessionEOSCore(this));
	m_SessionInterfacePtr->Init(TCHAR_TO_UTF8(*(ProductName + TEXT("_") + ProductVersion)));
	m_StatsInterfacePtr = MakeShareable(new FOnlineStatsEOSCore(this));
	m_LeaderboardsInterfacePtr = MakeShareable(new FOnlineLeaderboardsEOSCore(this));
	m_AchievementsInterfacePtr = MakeShareable(new FOnlineAchievementsEOSCore(this));

	EOS_Platform_SetApplicationStatus(m_EOSPlatformHandle, EOS_EApplicationStatus::EOS_AS_Foreground);
	EOS_Platform_SetNetworkStatus(m_EOSPlatformHandle, EOS_ENetworkStatus::EOS_NS_Online);

	StartTicker();

#if !UE_SERVER
	FString AuthType;
	FParse::Value(FCommandLine::Get(), TEXT("AUTH_TYPE="), AuthType);

	if (AuthType.Len() > 0)
	{
		m_IdentityInterfacePtr->AutoLogin(0);
	}
#endif

	return true;
}

bool FOnlineSubsystemEOSCore::Shutdown()
{
	StopTicker();

	UE_LOG_ONLINE(Verbose, TEXT("FOnlineSubsystemEOSCore::Shutdown()"));

	FOnlineSubsystemImpl::Shutdown();

#if !WITH_EDITOR
	EOS_EResult ShutdownResult = EOS_Shutdown();
	if (ShutdownResult != EOS_EResult::EOS_Success)
	{
		UE_LOG_ONLINE(Warning, TEXT("FOnlineSubsystemEOSCore: failed to shutdown the EOS SDK with result code (%d)"), (int32)ShutdownResult);
	}
#endif

	if (m_VoiceInterface.IsValid() && bVoiceInterfaceInitialized)
	{
		m_VoiceInterface->Shutdown();
	}

#define DESTRUCT_INTERFACE(Interface) \
	if (Interface.IsValid()) \
	{ \
		ensure(Interface.IsUnique()); \
		Interface = nullptr; \
	}

	DESTRUCT_INTERFACE(m_IdentityInterfacePtr);
	DESTRUCT_INTERFACE(m_SessionInterfacePtr);
	DESTRUCT_INTERFACE(m_StatsInterfacePtr);
	DESTRUCT_INTERFACE(m_LeaderboardsInterfacePtr);
	DESTRUCT_INTERFACE(m_AchievementsInterfacePtr);
	DESTRUCT_INTERFACE(m_StoreInterfacePtr);
	DESTRUCT_INTERFACE(m_VoiceInterface);

#undef DESTRUCT_INTERFACE

	DestroyEOSSocketSubsystem();

	m_EOSPlatformHandle = nullptr;
	m_AuthHandle = nullptr;
	m_FriendsHandle = nullptr;
	m_UserInfoHandle = nullptr;
	m_PresenceHandle = nullptr;
	m_ConnectHandle = nullptr;
	m_SessionsHandle = nullptr;
	m_StatsHandle = nullptr;
	m_LeaderboardsHandle = nullptr;
	m_MetricsHandle = nullptr;
	m_AchievementsHandle = nullptr;
	m_P2PHandle = nullptr;
	m_EcomHandle = nullptr;
	m_LobbyHandle = nullptr;
	m_PlayerDataStorageHandle = nullptr;
	m_TitleStorageHandle = nullptr;
	m_UiHandle = nullptr;
	m_ModsHandle = nullptr;
	m_ReportsHandle = nullptr;
	m_SanctionsHandle = nullptr;
	m_AntiCheatClientHandle = nullptr;
	m_AntiCheatServerHandle = nullptr;
	m_KWSHandle = nullptr;
	m_HRTCHandle = nullptr;
	m_HRTCAdminHandle = nullptr;
	m_ProgressionSnapshotHandle = nullptr;
	m_CustomInvitesHandle = nullptr;

	return true;
}

bool FOnlineSubsystemEOSCore::Tick(float DeltaTime)
{
	if (!bTickerStarted)
	{
		return true;
	}

	{
		FScopeCycleCounter Scope(GET_STATID(STAT_EOS_Tick), true);
		EOS_Platform_Tick(m_EOSPlatformHandle);
	}

	if (!FOnlineSubsystemImpl::Tick(DeltaTime))
	{
		return false;
	}

	if (m_SessionInterfacePtr)
	{
		m_SessionInterfacePtr->Tick(DeltaTime);
	}

	if (m_IdentityInterfacePtr)
	{
		m_IdentityInterfacePtr->Tick(DeltaTime);
	}

	if (m_VoiceInterface.IsValid() && bVoiceInterfaceInitialized)
	{
		m_VoiceInterface->Tick(DeltaTime);
	}

	return true;
}

bool FOnlineSubsystemEOSCore::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	if (FOnlineSubsystemImpl::Exec(InWorld, Cmd, Ar))
	{
		return true;
	}

	bool bWasHandled = false;

	if (FParse::Command(&Cmd, TEXT("EOS")))
	{
		if (m_StoreInterfacePtr != nullptr && FParse::Command(&Cmd, TEXT("ECOM")))
		{
			bWasHandled = m_StoreInterfacePtr->HandleEcomExec(InWorld, Cmd, Ar);
		}
	}

	return bWasHandled;
}

FString FOnlineSubsystemEOSCore::GetAppId() const
{
	return TEXT("");
}

FText FOnlineSubsystemEOSCore::GetOnlineServiceName() const
{
	return NSLOCTEXT("OnlineSubsystemEOSCore", "OnlineServiceName", "EOSCore");
}

IOnlineSessionPtr FOnlineSubsystemEOSCore::GetSessionInterface() const
{
	return m_SessionInterfacePtr;
}

IOnlineFriendsPtr FOnlineSubsystemEOSCore::GetFriendsInterface() const
{
	return nullptr;
}

IOnlineSharedCloudPtr FOnlineSubsystemEOSCore::GetSharedCloudInterface() const
{
	UE_LOG_ONLINE(Error, TEXT("Shared Cloud Interface Requested"));
	return nullptr;
}

IOnlineUserCloudPtr FOnlineSubsystemEOSCore::GetUserCloudInterface() const
{
	UE_LOG_ONLINE(Error, TEXT("User Cloud Interface Requested"));
	return nullptr;
}

IOnlineEntitlementsPtr FOnlineSubsystemEOSCore::GetEntitlementsInterface() const
{
	UE_LOG_ONLINE(Error, TEXT("Entitlements Interface Requested"));
	return nullptr;
};

IOnlineLeaderboardsPtr FOnlineSubsystemEOSCore::GetLeaderboardsInterface() const
{
	return m_LeaderboardsInterfacePtr;
}

IOnlineVoicePtr FOnlineSubsystemEOSCore::GetVoiceInterface() const
{
	if (m_VoiceInterface.IsValid() && !bVoiceInterfaceInitialized)
	{	
		if (!m_VoiceInterface->Init())
		{
			m_VoiceInterface = nullptr;
		}

		bVoiceInterfaceInitialized = true;
	}

	return m_VoiceInterface;
}

IOnlineExternalUIPtr FOnlineSubsystemEOSCore::GetExternalUIInterface() const
{
	return nullptr;
}

IOnlineIdentityPtr FOnlineSubsystemEOSCore::GetIdentityInterface() const
{
	return m_IdentityInterfacePtr;
}

IOnlineTitleFilePtr FOnlineSubsystemEOSCore::GetTitleFileInterface() const
{
	UE_LOG_ONLINE(Error, TEXT("Title File Interface Requested"));
	return nullptr;
}

IOnlineStoreV2Ptr FOnlineSubsystemEOSCore::GetStoreV2Interface() const
{
	return m_StoreInterfacePtr;
}

IOnlinePurchasePtr FOnlineSubsystemEOSCore::GetPurchaseInterface() const
{
	return m_StoreInterfacePtr;
}

IOnlineAchievementsPtr FOnlineSubsystemEOSCore::GetAchievementsInterface() const
{
	return m_AchievementsInterfacePtr;
}

IOnlineUserPtr FOnlineSubsystemEOSCore::GetUserInterface() const
{
	return m_UserInterfacePtr;
}

IOnlinePresencePtr FOnlineSubsystemEOSCore::GetPresenceInterface() const
{
	return nullptr;
}

IOnlineStatsPtr FOnlineSubsystemEOSCore::GetStatsInterface() const
{
	return m_StatsInterfacePtr;
}

#undef LOCTEXT_NAMESPACE
