#pragma once
#include "sdk/public/steam/isteamuserstats.h"

static ISteamUserStats* Steam_UserState;
static bool Steam_Test = false;
static int Steam_Appid = 1893580;

static bool Achievement_first[] = { true,true,true,true };
static string Achievement_names[] = { "XYKS", "鼻祖", "至高的神","至高至强至善的魔王"};
static string Achievement_Apinames[] = { "ACHIEVEMENT_1_0", "ACHIEVEMENT_1_1", "ACHIEVEMENT_1_2","ACHIEVEMENT_1_3"};
//解锁成就
void UnlockAchievement(string _apiname);
static void InitSteamApi()
{
	if (Steam_Test)
	{
		if (SteamAPI_RestartAppIfNecessary(Steam_Appid))//k_uAppIdInvalid
		{
			Print::Debug("非使用steam进行启动");
			//glfwSetWindowShouldClose(Window, true);
			abort();
			return;
		}
	}
	if (!SteamAPI_Init())//初始化SteamAPI
	{
		Print::Debug("初始化steamAPI失败");
		return;
	}
	Steam_UserState = SteamUserStats();
	UnlockAchievement(Achievement_Apinames[0]);
	SteamAPI_Shutdown();
}

enum EAchievements
{
	ACH_WIN_ONE_GAME = 0,
	ACH_WIN_100_GAMES = 1,
	ACH_HEAVY_FIRE = 2,
	ACH_TRAVEL_FAR_ACCUM = 3,
	ACH_TRAVEL_FAR_SINGLE = 4,
};

struct Achievement_t
{
	EAchievements m_eAchievementID;
	const char* m_pchAchievementID;//API名称
	char m_rgchName[128];
	char m_rgchDescription[256];
	bool m_bAchieved;
	int m_iIconImage;
};

//bool SetAchievement(const char* pchName) = 0;
//-----------------------------------------------------------------------------
// Purpose: Unlock this achievement
//-----------------------------------------------------------------------------
void UnlockAchievement(Achievement_t& achievement)
{
	achievement.m_bAchieved = true;

	// the icon may change once it's unlocked
	//图标解锁后可能会发生变化
	achievement.m_iIconImage = 0;

	// mark it down
	//记下来
	Steam_UserState->SetAchievement(achievement.m_pchAchievementID);

	// Store stats end of frame
	// 存储帧结束时的统计信息
	//m_bStoreStats = true;
}

//解锁成就
void UnlockAchievement(string _apiname)
{
	const char* _name = _apiname.c_str();
	Steam_UserState->SetAchievement(_name);
}
void RealUnlockAchievement(string _apiname)
{
	if (!SteamAPI_Init())//初始化SteamAPI
	{
		Print::Debug("初始化steamAPI失败");
		return;
	}
	Steam_UserState = SteamUserStats();
	UnlockAchievement(_apiname);
	SteamAPI_Shutdown();
}

long long Get_Plant_Score()
{
	return Plant_Score - 629;
}
void Set_Plant_Score(long long _i)
{
	Plant_Score = _i + 629;
	if (Get_Plant_Score() > 999999)
	{
		if (Get_Plant_Score() > 99999999 && Achievement_first[2] == true)
		{
			RealUnlockAchievement(Achievement_Apinames[2]);
			Achievement_first[2] = false;

		}
		else if (Get_Plant_Score() > 9999999999 && Achievement_first[3] == true)
		{
			RealUnlockAchievement(Achievement_Apinames[3]);
			Achievement_first[3] = false;
		}
		else
		{
			if (Achievement_first[1] == true)
			{
				RealUnlockAchievement(Achievement_Apinames[1]);
				Achievement_first[1] = false;
			}
		}
	}
}

void End_SteamAPI()
{
	SteamAPI_Shutdown();
}


//static int InRealMain(const char* pchCmdLine, HINSTANCE hInstance, int nCmdShow)
//{
//
//	if (SteamAPI_RestartAppIfNecessary(Steam_Appid))//k_uAppIdInvalid
//	{
//		// if Steam is not running or the game wasn't started through Steam, SteamAPI_RestartAppIfNecessary starts the 
//		// local Steam client and also launches this game again.
//
//		// Once you get a public Steam AppID assigned for this game, you need to replace k_uAppIdInvalid with it and
//		// removed steam_appid.txt from the game depot.
//		//如果Steam没有运行或者游戏不是通过Steam启动的，SteamAPI_RestartApp（如果需要）会启动游戏
//		//本地Steam客户端，并再次推出该游戏。
//
//		//一旦你为这个游戏分配了一个公共的Steam应用ID，你需要用它替换k_uAppIdInvalid，然后
//		//已移除蒸汽。来自游戏仓库的txt。
//		return EXIT_FAILURE;
//	}
//
//
//	// Init Steam CEG
//	if (!Steamworks_InitCEGLibrary())
//	{
//		OutputDebugString((LPCWSTR)"Steamworks_InitCEGLibrary() failed\n");
//		Alert("Fatal Error", "Steam must be running to play this game (InitDrmLibrary() failed).\n");
//		return EXIT_FAILURE;
//	}
//
//	// Initialize SteamAPI, if this fails we bail out since we depend on Steam for lots of stuff.
//	// You don't necessarily have to though if you write your code to check whether all the Steam
//	// interfaces are NULL before using them and provide alternate paths when they are unavailable.
//	//
//	// This will also load the in-game steam overlay dll into your process.  That dll is normally
//	// injected by steam when it launches games, but by calling this you cause it to always load,
//	// even when not launched via steam.
//	//初始化SteamAPI，如果失败了，我们就退出，因为我们在很多事情上都依赖Steam。
//	//不过，如果您编写代码来检查所有蒸汽
//	//接口在使用前为空，在不可用时提供备用路径。
//	//
//	//这也会将游戏中的steam overlay dll加载到您的进程中。该dll通常是
//	//当它启动游戏时注入蒸汽，但通过调用它，你会使它始终加载，
//	//即使不是通过蒸汽发射。
//	if (!SteamAPI_Init())//初始化SteamAPI
//	{
//		OutputDebugString((LPCWSTR)"SteamAPI_Init() failed\n");
//		Alert("Fatal Error", "Steam must be running to play this game (SteamAPI_Init() failed).\n");
//		return EXIT_FAILURE;
//	}
//
//	//设置调试处理程序 set our debug handler
//	SteamClient()->SetWarningMessageHook(&SteamAPIDebugTextHook);
//
//	// Ensure that the user has logged into Steam. This will always return true if the game is launched
//	// from Steam, but if Steam is at the login prompt when you run your game from the debugger, it
//	// will return false.
//	//确保用户已登录Steam。如果游戏启动，这将始终返回真值
//	//来自Steam，但如果Steam在从调试器运行游戏时出现登录提示，则
//	//将返回false。
//	if (!SteamUser()->BLoggedOn())
//	{
//		OutputDebugString((LPCWSTR)"Steam user is not logged in\n");
//		Alert("Fatal Error", "Steam user must be logged in to play this game (SteamUser()->BLoggedOn() returned false).\n");
//		return EXIT_FAILURE;
//	}
//
//	const char* pchServerAddress, * pchLobbyID;
//	if (!ParseCommandLine(pchCmdLine, &pchServerAddress, &pchLobbyID))
//	{
//		// no connect string on process command line. If app was launched via a Steam URL, the extra command line parameters in that URL
//		// get be retrieved with GetLaunchCommandLine. This way an attacker can't put malicious parameters in the process command line
//		// which might allow much more functionality then indented.
//		//进程命令行上没有连接字符串。如果应用程序是通过Steam URL启动的，则该URL中的额外命令行参数
//		// //可以使用GetLaunchCommandLine检索get。这样，攻击者就不能在进程命令行中输入恶意参数
//		//这可能会允许更多的功能，然后缩进。
//
//		char szCommandLine[1024] = {};
//
//		if (SteamApps()->GetLaunchCommandLine(szCommandLine, sizeof(szCommandLine)) > 0)
//		{
//			ParseCommandLine(szCommandLine, &pchServerAddress, &pchLobbyID);
//		}
//	}
//
//	// do a DRM self check
//	Steamworks_SelfCheck();
//
//	// Construct a new instance of the game engine 
//	// bugbug jmccaskey - make screen resolution dynamic, maybe take it on command line?
//	//构建游戏引擎的新实例
//	//bugbug jmccaskey-使屏幕分辨率动态化，或者在命令行上使用它？
//	IGameEngine* pGameEngine =
//#if defined(_WIN32)
//		new CGameEngineWin32(hInstance, nCmdShow, 1024, 768);
//#elif defined(OSX)
//		CreateGameEngineOSX();
//#elif defined(SDL)
//		CreateGameEngineSDL();
//#else
//#error	Need CreateGameEngine()
//#endif
//
//		if (!SteamInput()->Init(false))
//		{
//			OutputDebugString((LPCWSTR)"SteamInput()->Init failed.\n");
//			Alert("Fatal Error", "SteamInput()->Init failed.\n");
//			return EXIT_FAILURE;
//		}
//	char rgchCWD[1024];
//	if (!_getcwd(rgchCWD, sizeof(rgchCWD)))
//	{
//		strcpy(rgchCWD, ".");
//	}
//
//	char rgchFullPath[1024];
//#if defined(_WIN32)
//	_snprintf(rgchFullPath, sizeof(rgchFullPath), "%s\\%s", rgchCWD, "steam_input_manifest.vdf");
//#elif defined(OSX)
//	// hack for now, because we do not have utility functions available for finding the resource path
//	// alternatively we could disable the SteamController init on OS X
//	_snprintf(rgchFullPath, sizeof(rgchFullPath), "%s/steamworksexample.app/Contents/Resources/%s", rgchCWD, "steam_input_manifest.vdf");
//#else
//	_snprintf(rgchFullPath, sizeof(rgchFullPath), "%s/%s", rgchCWD, "steam_input_manifest.vdf");
//#endif
//
//	SteamInput()->SetInputActionManifestFilePath(rgchFullPath);
//
//	// This call will block and run until the game exits
//	RunGameLoop(pGameEngine, pchServerAddress, pchLobbyID);
//
//	// Shutdown the SteamAPI
//	SteamAPI_Shutdown();
//
//	// Shutdown Steam CEG
//	Steamworks_TermCEGLibrary();
//
//	// exit
//	return EXIT_SUCCESS;
//}