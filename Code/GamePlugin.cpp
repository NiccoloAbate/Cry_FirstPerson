#include "StdAfx.h"
#include "GamePlugin.h"

#include "Game\CameraController.h"
#include "Game\EnvironmentController.h"
#include "Components\Environment\Rain.h"
#include "Components\Hive\Pheromone.h"
#include "Utils\ConsoleCommands.h"
#include "Game\AudioController.h"
#include "Game\UIController.h"
#include "Dialogue\DialogueManager.h"
#include "Game\RewindManager.h"
#include "Timeline\TimelineManager.h"
#include "Optimization\OptimizationManager.h"
#include "Gameflow\GameflowManager.h"
#include "Components\Player\Player.h"
#include "Components\Player\PlayerExtension.h"
#include "Components\SpawnPoint.h"
#include "Components\Characters\Character.h"
#include "Utils\Initialization\DeferredInitialization.h"

#include <CrySchematyc/Env/IEnvRegistry.h>
#include <CrySchematyc/Env/EnvPackage.h>
#include <CrySchematyc/Utils/SharedString.h>

#include <IGameObjectSystem.h>
#include <IGameObject.h>

// Included only once per DLL module.
#include <CryCore/Platform/platform_impl.inl>


CGamePlugin* CGamePlugin::gGamePlugin = nullptr;

CGamePlugin::~CGamePlugin()
{
	// Remove any registered listeners before 'this' becomes invalid
	gEnv->pGameFramework->RemoveNetworkedClientListener(*this);
	gEnv->pSystem->GetISystemEventDispatcher()->RemoveListener(this);

	if (gEnv->pSchematyc)
	{
		gEnv->pSchematyc->GetEnvRegistry().DeregisterPackage(CGamePlugin::GetCID());
	}

	delete m_pCameraController;
	delete m_pEnvironmentController;
	delete m_pPheromoneManager;
}

void CGamePlugin::InitializeEnvironment()
{
	m_pEnvironmentController = new CEnvironmentController;
	m_pEnvironmentController->Initialize();

	m_pEnvironmentController->SetDiffuseMultiplier(.2);
	gEnv->pConsole->ExecuteString("r_VolumetricClouds 1");
	gEnv->pConsole->ExecuteString("e_VolumetricFog 1");
}

void CGamePlugin::InitializeUI()
{
	m_pUIController = new CUIController;
	m_pUIController->Initialize();
}

void CGamePlugin::InitializeDialogueManager()
{
	m_pDialogueManager = new CDialogueManager;
	m_pDialogueManager->Initialize();
	TestXml();
}

void CGamePlugin::InitializeRewindManager()
{
	m_pRewindManager = new CRewindManager;
	m_pRewindManager->Initialize();
}

void CGamePlugin::InitializeTimelineManager()
{
	m_pTimelineManager = new CTimelineManager;
	m_pTimelineManager->Initialize();
}

void CGamePlugin::InitializeOptimizationManager()
{
	m_pOptimizationManager = new COptimizationManager;
	m_pOptimizationManager->Initialize();
}

void CGamePlugin::InitializeAudio()
{
	//CAudioController::PlayMusic("Game 03");
}

void CGamePlugin::InitializePheromoneManager()
{
	m_pPheromoneManager = new CPheromoneManager();
}

void CGamePlugin::InitializeGameflowManager()
{
	m_pGameflowManager = new CGameflowManager;
	m_pGameflowManager->Initialize();
}

void CGamePlugin::InitializePlayerComponent()
{
	SEntitySpawnParams PlayerSpawnParams;
	PlayerSpawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->GetDefaultClass();
	PlayerSpawnParams.sName = "PlayerComponent";
	PlayerSpawnParams.nFlags |= ENTITY_FLAG_NEVER_NETWORK_STATIC;

	if (IEntity* pEntity = gEnv->pEntitySystem->SpawnEntity(PlayerSpawnParams))
	{
		if (!gEnv->IsEditor())
			CSpawnPointComponent::FindFirstSpawnPoint()->SpawnEntity(pEntity);
		m_pPlayerComponent = pEntity->CreateComponent<CPlayerComponent>();
	}
}

/*
// Connection received from a client, create a player entity and component
SEntitySpawnParams spawnParams;
spawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->GetDefaultClass();
spawnParams.sName = "Player";
spawnParams.nFlags |= ENTITY_FLAG_NEVER_NETWORK_STATIC;

// Set local player details
if (m_players.size() == 0 && !gEnv->IsDedicated())
{
spawnParams.id = LOCAL_PLAYER_ENTITY_ID;
spawnParams.nFlags |= ENTITY_FLAG_LOCAL_PLAYER;
}

// Spawn the player entity
if (IEntity* pPlayerEntity = gEnv->pEntitySystem->SpawnEntity(spawnParams))
{
// Set the local player entity channel id, and bind it to the network so that it can support Multiplayer contexts
pPlayerEntity->GetNetEntity()->SetChannelId(channelId);
pPlayerEntity->GetNetEntity()->BindToNetwork();

// Create the player component instance
CCryFirstPersonPlayerComponent* pPlayer = pPlayerEntity->GetOrCreateComponentClass<CCryFirstPersonPlayerComponent>();
//Sets camera component
pPlayer->SetCamera(m_pCameraEntity);
pPlayer->SetInactiveChar();

// Push the component into our map, with the channel id as the key
m_players.emplace(std::make_pair(channelId, pPlayerEntity->GetId()));
m_pPlayers.push_back(pPlayerEntity);
}

SEntitySpawnParams spawnParams_2;
spawnParams_2.pClass = gEnv->pEntitySystem->GetClassRegistry()->GetDefaultClass();
spawnParams_2.sName = "Player";
spawnParams_2.nFlags |= ENTITY_FLAG_NEVER_NETWORK_STATIC;
//spawnParams_2.id = LOCAL_PLAYER_ENTITY_ID;
//spawnParams_2.nFlags |= ENTITY_FLAG_LOCAL_PLAYER;

if (IEntity* pPlayerEntity_2 = gEnv->pEntitySystem->SpawnEntity(spawnParams_2))
{
// Set the local player entity channel id, and bind it to the network so that it can support Multiplayer contexts
pPlayerEntity_2->GetNetEntity()->SetChannelId(channelId);
pPlayerEntity_2->GetNetEntity()->BindToNetwork();

// Create the player component instance
CCryFirstPersonPlayerComponent* pPlayer = pPlayerEntity_2->GetOrCreateComponentClass<CCryFirstPersonPlayerComponent>();
//Add the one true camera
pPlayer->SetCamera(m_pCameraEntity);
pPlayer->SetActiveChar();
// Push the component into our map, with the channel id as the key
//m_players.emplace(std::make_pair(channelId, pPlayerEntity_2->GetId()));
m_pPlayers.push_back(pPlayerEntity_2);
m_PlayerIndex = m_pPlayers.size() - 1;
}
*/

IEntity* CGamePlugin::GetPlayerExtensionEntity()
{
	return GetPlayerComponent()->GetPlayerExtension()->GetEntity();
}

void CGamePlugin::InitializeCamera()
{
	SEntitySpawnParams spawnParams_Cam;
	spawnParams_Cam.pClass = gEnv->pEntitySystem->GetClassRegistry()->GetDefaultClass();
	spawnParams_Cam.sName = "Camera";
	spawnParams_Cam.nFlags |= ENTITY_FLAG_NEVER_NETWORK_STATIC;

	if (m_pCameraEntity = gEnv->pEntitySystem->SpawnEntity(spawnParams_Cam))
	{
		Cry::DefaultComponents::CCameraComponent *pCamComponent = m_pCameraEntity->GetOrCreateComponent<Cry::DefaultComponents::CCameraComponent>();
	}

	m_pCameraController = new CCameraController;
	m_pCameraController->m_pCameraEntity = m_pCameraEntity;
}

void CGamePlugin::TestXml()
{

	XmlNodeRef Behaviors = gEnv->pSystem->LoadXmlFromFile("Saves/Behaviors");
	if (!Behaviors)
		Behaviors = gEnv->pSystem->CreateXmlNode("Behaviors");

	XmlNodeRef B1 = Behaviors->findChild("Behavior1");
	if (!B1)
		B1 = Behaviors->newChild("Behavior1");

	XmlNodeRef H1 = B1->findChild("History");
	if (!H1)
		H1 = B1->newChild("History");

	float AvScore1 = 0;
	if (H1->haveAttr("AverageScore"))
		H1->getAttr("AverageScore", AvScore1);
	else
		H1->setAttr("AverageScore", AvScore1);

	Behaviors->saveToFile("Saves/Behaviors");

}

void CGamePlugin::InitializeSystems()
{
	m_pPheromoneManager = new CPheromoneManager();
}

void CGamePlugin::PostInit()
{
	InitializeUI();

	// Don't need to load the map in editor
	if (!gEnv->IsEditor())
	{
		gEnv->pConsole->ExecuteString("map example", false, true);
	}
}

#include <IEffectSystem.h>
void CGamePlugin::OnLevelLoaded()
{
	SetUpdateFlags(EUpdateType_Update);
	
	m_pUIController->OnLevelLoaded();
	InitializeDialogueManager();
	InitializeEnvironment();
	InitializeAudio();

	if (!gEnv->IsEditor()) {
		gEnv->pConsole->ExecuteString("r_Width 1920");
		gEnv->pConsole->ExecuteString("r_Height 1080");
	}

	

	//auto pEffectSystem = gEnv->pGameFramework->GetIEffectSystem();
	//pEffectSystem->Activate(pEffectSystem->GetEffectId("FrostEffect"));

}

void CGamePlugin::OnGameplayReady()
{
	InitializeGameflowManager();
	m_pUIController->OnGameplayStart();

	TRIGGER_DEFERREDINITIALIZATION_CHECKPOINT();
}

void CC_Fullscreen(CC_Args pArgs)
{
	CC_ARGSCOUNTCHECK(1)
	if (gEnv->IsEditor())
		return;
	bool bFullScreen;
	FromString(bFullScreen, pArgs->GetArg(1));
	if (bFullScreen)
	{
		gEnv->pConsole->ExecuteString("r_Width 1920");
		gEnv->pConsole->ExecuteString("r_Height 1080");
		gEnv->pConsole->ExecuteString("r_Fullscreen 1");
	}
	else
	{
		gEnv->pConsole->ExecuteString("r_Fullscreen 0");
	}
}
ADDCONSOLECOMMAND("FullScreen", CC_Fullscreen)

void CC_Reload(CC_Args pArgs)
{
	if (gEnv->IsEditor())
		return;
	gEnv->pConsole->ExecuteString("map example");
}
ADDCONSOLECOMMAND("Reload", CC_Reload) // doesn't work yet; have to reset game code, such as pheromone manager

bool CGamePlugin::Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams)
{
	gGamePlugin = this;
	// Register for engine system events, in our case we need ESYSTEM_EVENT_GAME_POST_INIT to load the map
	gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this, "CGamePlugin");
	// Listen for client connection events, in order to create the local player
	gEnv->pGameFramework->AddNetworkedClientListener(*this);

	InitializeOptimizationManager();
	InitializePheromoneManager();
	InitializeRewindManager();
	InitializeTimelineManager();

	return true;
}

void CGamePlugin::OnPluginUpdate(EPluginUpdateType updateType)
{
	switch (updateType)
	{
	default:
		break;
	case EUpdateType_Update:
	{
		float fFrameTime = gEnv->pTimer->GetFrameTime();
		m_pOptimizationManager->Update();
		if(m_pGameflowManager)
			m_pGameflowManager->Update(fFrameTime);
		m_pTimelineManager->Update(fFrameTime);
		m_pUIController->Update(fFrameTime);
	}
	break;
	}
}

void CGamePlugin::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	switch (event)
	{
	case ESYSTEM_EVENT_REGISTER_SCHEMATYC_ENV:
	{
		// Register all components that belong to this plug-in
		auto staticAutoRegisterLambda = [](Schematyc::IEnvRegistrar& registrar)
		{
			// Call all static callback registered with the CRY_STATIC_AUTO_REGISTER_WITH_PARAM
			Detail::CStaticAutoRegistrar<Schematyc::IEnvRegistrar&>::InvokeStaticCallbacks(registrar);
		};

		if (gEnv->pSchematyc)
		{
			gEnv->pSchematyc->GetEnvRegistry().RegisterPackage(
				stl::make_unique<Schematyc::CEnvPackage>(
					CGamePlugin::GetCID(),
					"EntityComponents",
					"Crytek GmbH",
					"Components",
					staticAutoRegisterLambda
					)
			);
		}
	}
	break;
		// Called when the game framework has initialized and we are ready for game logic to start
	case ESYSTEM_EVENT_GAME_POST_INIT:
	{
		PostInit();
	}
	break;
	case ESYSTEM_EVENT_LEVEL_LOAD_END:
	{
		OnLevelLoaded();
	}
	break;
	}
}

bool CGamePlugin::OnClientConnectionReceived(int channelId, bool bIsReset)
{
	InitializeCamera();
	InitializePlayerComponent();

	return true;
}

/*
// Revive players when the network reports that the client is connected and ready for gameplay
auto it = m_players.find(channelId);
if (it != m_players.end())
{
if (IEntity* pPlayerEntity = gEnv->pEntitySystem->GetEntity(it->second))
{
if (CCryFirstPersonPlayerComponent* pPlayer = pPlayerEntity->GetComponent<CCryFirstPersonPlayerComponent>())
{
pPlayer->Revive();
}
}
}
*/

bool CGamePlugin::OnClientReadyForGameplay(int channelId, bool bIsReset)
{
	
	OnGameplayReady();

	return true;
}

/*
// Client disconnected, remove the entity and from map
auto it = m_players.find(channelId);
if (it != m_players.end())
{
gEnv->pEntitySystem->RemoveEntity(it->second);

m_players.erase(it);
}
*/

void CGamePlugin::OnClientDisconnected(int channelId, EDisconnectionCause cause, const char* description, bool bKeepClient)
{
	
}

CRYREGISTER_SINGLETON_CLASS(CGamePlugin)
