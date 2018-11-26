#pragma once

#include <CrySystem/ICryPlugin.h>
#include <CryGame/IGameFramework.h>
#include <CryEntitySystem/IEntityClass.h>
#include <CryNetwork/INetwork.h>

class CPheromoneManager;
class CCryFirstPersonPlayerComponent;
class CCameraController;
class CEnvironmentController;
class CUIController;
class CHiveComponent;
class CDialogueManager;
class CRewindManager;
class CTimelineManager;
class COptimizationManager;
class CGameflowManager;
class CPlayerComponent;

#include "Build.h"

#define ONCEPERFRAMCHECK										\
static float fTime = 0;											\
if (fTime == gEnv->pTimer->GetFrameStartTime().GetSeconds())	\
	return;														\
else															\
	fTime = gEnv->pTimer->GetFrameStartTime().GetSeconds();		\

// The entry-point of the application
// An instance of CGamePlugin is automatically created when the library is loaded
// We then construct the local player entity and CCryFirstPersonPlayerComponent instance when OnClientConnectionReceived is first called.
class CGamePlugin 
	: public ICryPlugin
	, public ISystemEventListener
	, public INetworkedClientListener
{
public:
	CRYINTERFACE_SIMPLE(ICryPlugin)
	CRYGENERATE_SINGLETONCLASS_GUID(CGamePlugin, "Game_Blank", "{FC9BD884-49DE-4494-9D64-191734BBB7E3}"_cry_guid)

	virtual ~CGamePlugin();

	static CGamePlugin *gGamePlugin;
	
	// Managers
	CEnvironmentController *m_pEnvironmentController;
	CUIController *m_pUIController;
	CDialogueManager *m_pDialogueManager;
	CRewindManager *m_pRewindManager;
	CTimelineManager *m_pTimelineManager;
	COptimizationManager *m_pOptimizationManager;
	CPheromoneManager *m_pPheromoneManager;
	CGameflowManager *m_pGameflowManager;
	//~Managers

	CHiveComponent *m_pHive = nullptr;

	// Player Functions
	CPlayerComponent* GetPlayerComponent() { return m_pPlayerComponent; }
	//void InitializePlayer(int channelId, bool bIsReset);
	//~Player Functions

	// Camera
	CCameraController *m_pCameraController;
	void InitializeCamera();
	IEntity *m_pCameraEntity;
	//~Camera

	// Misc
#ifdef BUILD_DEBUG
	float m_TimeScale = 1;
#endif // ~BUILD_DEBUG
	//~Mics

	// Misc Functions
	void TestXml();
	//~Misc Functions

	void InitializeSystems();
	void PostInit();
	void OnLevelLoaded();
	void OnGameplayReady();

	// ICryPlugin
	virtual const char* GetName() const override { return "GamePlugin"; }
	virtual const char* GetCategory() const override { return "Game"; }
	virtual bool Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams) override;
	virtual void OnPluginUpdate(EPluginUpdateType updateType) override;
	// ~ICryPlugin

	// ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;
	// ~ISystemEventListener

	// INetworkedClientListener
	// Sent to the local client on disconnect
	virtual void OnLocalClientDisconnected(EDisconnectionCause cause, const char* description) override {}

	// Sent to the server when a new client has started connecting
	// Return false to disallow the connection
	virtual bool OnClientConnectionReceived(int channelId, bool bIsReset) override;
	// Sent to the server when a new client has finished connecting and is ready for gameplay
	// Return false to disallow the connection and kick the player
	virtual bool OnClientReadyForGameplay(int channelId, bool bIsReset) override;
	// Sent to the server when a client is disconnected
	virtual void OnClientDisconnected(int channelId, EDisconnectionCause cause, const char* description, bool bKeepClient) override;
	// Sent to the server when a client is timing out (no packets for X seconds)
	// Return true to allow disconnection, otherwise false to keep client.
	virtual bool OnClientTimingOut(int channelId, EDisconnectionCause cause, const char* description) override { return true; }
	// ~INetworkedClientListener

protected:
	// Managers
	void InitializeAudio();
	void InitializeEnvironment();
	void InitializeUI();
	void InitializeDialogueManager();
	void InitializeRewindManager();
	void InitializeTimelineManager();
	void InitializeOptimizationManager();
	void InitializePheromoneManager();
	void InitializeGameflowManager();
	//~Managers

	CPlayerComponent *m_pPlayerComponent = nullptr;
	void InitializePlayerComponent();

	// Map containing player components, key is the channel id received in OnClientConnectionReceived
	//std::unordered_map<int, EntityId> m_players;
};