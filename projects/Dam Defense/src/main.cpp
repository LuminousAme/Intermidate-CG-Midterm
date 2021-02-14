//Dam Defense, by Atlas X Games
//main.cpp, the source file that runs the game

//import required titan features
#include "Titan/Application.h"
//include the other headers in dam defense
#include "Game/Game.h"
#include "Launch/SplashCard.h"
#include "Launch/LoadingScene.h"
#include "Menu/MainMenu.h"
#include "Game/PauseMenu.h"
#include "Game/GameOverMenu.h"
#include "Game/GameWinMenu.h"

using namespace Titan;

//asset setup function
void PrepareAssetLoading();

//main function, runs the program
int main() { 
	Logger::Init(); //initliaze otter's base logging system
	TTN_Application::Init("Dam Defense", 1920, 1080); //initliaze titan's application

	//data to track loading progress
	bool set1Loaded = false;
	bool set2Loaded = false;

	//reference to the audio engine (used to pause game audio while the game isn't running)
	TTN_AudioEngine& audioEngine = TTN_AudioEngine::Instance();

	//lock the cursor while focused in the application window
	TTN_Application::TTN_Input::SetCursorLocked(false);

	//prepare the assets
	PrepareAssetLoading();

	//load set 0 assets
	TTN_AssetSystem::LoadSetNow(0);

	//create the scenes
	SplashCard* splash = new SplashCard;
	LoadingScene* loadingScreen = new LoadingScene;
	Game* gameScene = new Game;
	GameUI* gameSceneUI = new GameUI;
	MainMenu* titleScreen = new MainMenu;
	MainMenuUI* titleScreenUI = new MainMenuUI;
	PauseMenu* paused = new PauseMenu;
	GameOverMenu* gameOver = new GameOverMenu;
	GameOverMenuUI* gameOverUI = new GameOverMenuUI;
	GameWinMenu* gameWin = new GameWinMenu;
	GameWinMenuUI* gameWinUI = new GameWinMenuUI;

	//initliaze them
	splash->InitScene();
	loadingScreen->InitScene();
	loadingScreen->SetShouldRender(false);
	gameScene->SetShouldRender(false);
	gameSceneUI->SetShouldRender(false);
	titleScreen->SetShouldRender(false);
	titleScreenUI->SetShouldRender(false);
	gameOver->SetShouldRender(false);
	gameOverUI->SetShouldRender(false);
	paused->SetShouldRender(false);
	gameWin->SetShouldRender(false);
	gameWinUI->SetShouldRender(false);

	//add them to the application
	TTN_Application::scenes.push_back(splash);
	TTN_Application::scenes.push_back(loadingScreen);
	TTN_Application::scenes.push_back(gameScene);
	TTN_Application::scenes.push_back(gameSceneUI);
	TTN_Application::scenes.push_back(paused);
	TTN_Application::scenes.push_back(titleScreen);
	TTN_Application::scenes.push_back(titleScreenUI);
	TTN_Application::scenes.push_back(gameOver);
	TTN_Application::scenes.push_back(gameOverUI);
	TTN_Application::scenes.push_back(gameWin);
	TTN_Application::scenes.push_back(gameWinUI);

	// init's the configs and contexts for imgui
	TTN_Application::InitImgui();
	bool firstTime = false;
	//while the application is running
	while (!TTN_Application::GetIsClosing()) {
		
		//check if the splash card is done playing
		if (splash->GetShouldRender() && splash->GetTotalSceneTime() > 4.0f) {
			//if it is move to the loading screen
			splash->SetShouldRender(false);
			loadingScreen->SetShouldRender(true);
			//and start up the queue to load the main menu assets in
			TTN_AssetSystem::LoadSetInBackground(1);
			TTN_AssetSystem::LoadSetInBackground(2);
			TTN_AssetSystem::LoadSetInBackground(3);
		}

		//check if the loading is done
		if (loadingScreen->GetShouldRender() && set1Loaded) {
			//if it is, go to the main menu
			loadingScreen->SetShouldRender(false);
			titleScreen->InitScene();
			titleScreen->SetShouldRender(true);
			titleScreenUI->InitScene();
			titleScreenUI->SetShouldRender(true);
			
		}

		//check if the loading is done and the menu should be going to the game
		if (titleScreenUI->GetShouldRender() && titleScreenUI->GetShouldPlay() && set2Loaded && (!firstTime) ) {
			//if it is, go to the game
			titleScreen->SetShouldRender(false);
			titleScreenUI->SetShouldRender(false);
			titleScreenUI->SetShouldPlay(false);
			TTN_Application::TTN_Input::SetCursorLocked(true);
			gameScene->InitScene();
			gameSceneUI->InitScene();
			paused->InitScene();
			gameOver->InitScene();
			gameOverUI->InitScene();
			gameWin->InitScene();
			gameWinUI->InitScene();
			gameOver->SetShouldRender(false);
			gameOverUI->SetShouldRender(false);
			gameWin->SetShouldRender(false);
			gameWinUI->SetShouldRender(false);
			gameScene->SetShouldRender(true);
			gameSceneUI->SetShouldRender(true);
			firstTime = true;
		}

		//for going back to the main menu in game over
		if (titleScreenUI->GetShouldRender() && titleScreenUI->GetShouldPlay() && set2Loaded && (firstTime)) {
			//if it is, go to the game
			titleScreen->SetShouldRender(false);
			titleScreenUI->SetShouldRender(false);
			titleScreenUI->SetShouldPlay(false);
			TTN_Application::TTN_Input::SetCursorLocked(true);
			gameOver->SetShouldRender(false);
			gameOverUI->SetShouldRender(false);
			gameWin->SetShouldRender(false);
			gameWinUI->SetShouldRender(false);
			gameScene->SetShouldRender(true);
			gameSceneUI->SetShouldRender(true);
			gameScene->RestartData();
		}

		//check if the game should quit
		if (titleScreenUI->GetShouldQuit() || paused->GetShouldQuit() || gameOverUI->GetShouldQuit() || gameWinUI->GetShouldQuit()) {
			TTN_Application::Quit();
		}

		//pause menu rendering
		//if the player has paused but the menu hasn't appeared yet
		if (gameScene->GetShouldRender() && !paused->GetShouldRender() && gameScene->GetPaused()) {
			TTN_Application::TTN_Input::SetCursorLocked(false);
			paused->SetShouldResume(false);
			paused->SetShouldRender(true);
		}
		//if the menu has appeared but the player has unpaused with the esc key
		else if (gameScene->GetShouldRender() && paused->GetShouldRender() && !gameScene->GetPaused()) {
			TTN_Application::TTN_Input::SetCursorLocked(true);
			paused->SetShouldResume(false);
			paused->SetShouldRender(false);
		}
		//if the menu has appeared and the player has unpaused from the menu button
		else if (gameScene->GetShouldRender() && paused->GetShouldRender() && paused->GetShouldResume()) {
			TTN_Application::TTN_Input::SetCursorLocked(true);
			gameScene->SetGameIsPaused(false);
			gameScene->SetPaused(false);
			paused->SetShouldResume(false);
			paused->SetShouldRender(false);
		}

		//if the game is over
		if (gameScene->GetGameIsOver()) {
			TTN_Application::TTN_Input::SetCursorLocked(false);
			gameScene->SetGameIsOver(false);
			gameScene->SetShouldRender(false);
			gameSceneUI->SetShouldRender(false);
			paused->SetShouldRender(false);
			gameScene->SetGameIsPaused(true);
			gameOver->SetShouldRender(true);
			gameOverUI->SetShouldRender(true);
			gameOverUI->SetShouldMenu(false);
			audioEngine.GetBus("Music").SetPaused(true);
			audioEngine.GetBus("SFX").SetPaused(true);
		}

		//if game over should render and restart
		if (gameOverUI->GetShouldRender() && gameOverUI->GetShouldPlay() && gameOver->GetShouldRender()) {
			gameOver->SetShouldRender(false);
			gameOverUI->SetShouldRender(false);
			gameOverUI->SetShouldPlay(false);
			gameOverUI->SetShouldMenu(false);
			TTN_Application::TTN_Input::SetCursorLocked(true);
			gameScene->SetGameIsPaused(false);
			gameScene->SetShouldRender(true);
			gameSceneUI->SetShouldRender(true);
			gameScene->SetGameIsOver(false);
			gameScene->RestartData();
		}
		//game over go to menu
		if (gameOverUI->GetShouldRender() && gameOverUI->GetShouldMenu() && gameOver->GetShouldRender()) {
			gameOver->SetShouldRender(false);
			gameOverUI->SetShouldRender(false);
			gameOverUI->SetShouldMenu(false);
			gameOverUI->SetShouldPlay(false);
			TTN_Application::TTN_Input::SetCursorLocked(false);
			titleScreen->SetShouldRender(true);
			titleScreenUI->SetShouldRender(true);
		}

		//if player wins game
		if (gameScene->GetGameWin()) {
			TTN_Application::TTN_Input::SetCursorLocked(false);
			gameScene->SetGameWin(false);
			gameScene->SetShouldRender(false);
			gameSceneUI->SetShouldRender(false);
			paused->SetShouldRender(false);
			gameScene->SetGameIsPaused(true);
			gameWin->SetShouldRender(true);
			gameWinUI->SetShouldRender(true);
			gameWinUI->SetShouldMenu(false);
			audioEngine.GetBus("Music").SetPaused(true);
			audioEngine.GetBus("SFX").SetPaused(true);
		}

		//if game win and they want to play again
		if (gameWinUI->GetShouldRender() && gameWinUI->GetShouldPlay() && gameWin->GetShouldRender()) {
			gameWin->SetShouldRender(false);
			gameWinUI->SetShouldRender(false);
			gameWinUI->SetShouldPlay(false);
			gameWinUI->SetShouldMenu(false);
			TTN_Application::TTN_Input::SetCursorLocked(true);
			gameScene->SetGameIsPaused(false);
			gameScene->SetShouldRender(true);
			gameSceneUI->SetShouldRender(true);
			gameScene->SetGameIsOver(false);
			gameScene->RestartData();
		}

		if (gameWinUI->GetShouldRender() && gameWinUI->GetShouldMenu() && gameWin->GetShouldRender()) {
			gameWin->SetShouldRender(false);
			gameWinUI->SetShouldRender(false);
			gameWinUI->SetShouldMenu(false);
			gameWinUI->SetShouldPlay(false);
			TTN_Application::TTN_Input::SetCursorLocked(false);
			titleScreen->SetShouldRender(true);
			titleScreenUI->SetShouldRender(true);
		}


		if (!set1Loaded && TTN_AssetSystem::GetSetLoaded(1) && TTN_AssetSystem::GetCurrentSet() == 1)
			set1Loaded = true;
		if (!set2Loaded && TTN_AssetSystem::GetSetLoaded(2) && TTN_AssetSystem::GetCurrentSet() == 2)
			set2Loaded = true;
		

		//update the scenes and render the screen
		TTN_Application::Update();
	}
	
	//clean up all the application data
	TTN_Application::Closing();

	//when the application has ended, exit the program with no errors
	return 0; 
} 

void PrepareAssetLoading() {
	//Set 0 assets that get loaded right as the program begins after Titan and Logger init 
	TTN_AssetSystem::AddTexture2DToBeLoaded("BG", "textures/Background.png", 0); //dark grey background for splash card, loading screen and pause menu
	TTN_AssetSystem::AddTexture2DToBeLoaded("AtlasXLogo", "textures/Atlas X Games Logo.png", 0); //team logo for splash card
	TTN_AssetSystem::AddTexture2DToBeLoaded("Loading-Text", "textures/text/loading.png", 0); //loading text for loading screen
	TTN_AssetSystem::AddTexture2DToBeLoaded("Loading-Circle", "textures/loading-circle.png", 0); //circle to rotate while loading

	//Set 1 assets to be loaded while the splash card and loading screen play
	TTN_AssetSystem::AddMeshToBeLoaded("Skybox mesh", "models/SkyboxMesh.obj", 1); //mesh for the skybox
	TTN_AssetSystem::AddSkyboxToBeLoaded("Skybox texture", "textures/skybox/sky.png", 1); //texture for the skybox
	TTN_AssetSystem::AddMeshToBeLoaded("Dam mesh", "models/Dam.obj", 1); //mesh for the dam 
	TTN_AssetSystem::AddTexture2DToBeLoaded("Dam texture", "textures/Dam.png", 1); //texture for the dam
	TTN_AssetSystem::AddMorphAnimationMeshesToBeLoaded("Cannon mesh", "models/cannon/cannon", 7, 1); //mesh for the cannon
	TTN_AssetSystem::AddTexture2DToBeLoaded("Cannon texture", "textures/metal.png", 1); //texture for the cannon
	TTN_AssetSystem::AddMeshToBeLoaded("Flamethrower mesh", "models/Flamethrower.obj", 1); //mesh for the flamethrowers
	TTN_AssetSystem::AddTexture2DToBeLoaded("Flamethrower texture", "textures/FlamethrowerTexture.png", 1); //texture for the flamethrower
	TTN_AssetSystem::AddMeshToBeLoaded("Terrain plane", "models/terrainPlain.obj", 1); //large plane with lots of subdivisions for the terrain and water
	TTN_AssetSystem::AddTexture2DToBeLoaded("Terrain height map", "textures/Game Map Long 2.jpg", 1); //height map for the terrain
	TTN_AssetSystem::AddTexture2DToBeLoaded("Sand texture", "textures/SandTexture.jpg", 1); //sand texture
	TTN_AssetSystem::AddTexture2DToBeLoaded("Rock texture", "textures/RockTexture.jpg", 1); //rock texture
	TTN_AssetSystem::AddTexture2DToBeLoaded("Grass texture", "textures/GrassTexture.jpg", 1); //grass texture
	TTN_AssetSystem::AddTexture2DToBeLoaded("Water texture", "textures/water.png", 1); //water texture
	TTN_AssetSystem::AddDefaultShaderToBeLoaded("Basic textured shader", TTN_DefaultShaders::VERT_NO_COLOR, TTN_DefaultShaders::FRAG_BLINN_PHONG_ALBEDO_ONLY, 1);
	TTN_AssetSystem::AddDefaultShaderToBeLoaded("Skybox shader", TTN_DefaultShaders::VERT_SKYBOX, TTN_DefaultShaders::FRAG_SKYBOX, 1);
	TTN_AssetSystem::AddDefaultShaderToBeLoaded("Animated textured shader", TTN_DefaultShaders::VERT_MORPH_ANIMATION_NO_COLOR, TTN_DefaultShaders::FRAG_BLINN_PHONG_ALBEDO_ONLY, 1);
	TTN_AssetSystem::AddShaderToBeLoaded("Terrain shader", "shaders/terrain_vert.glsl", "shaders/terrain_frag.glsl", 1);
	TTN_AssetSystem::AddShaderToBeLoaded("Water shader", "shaders/water_vert.glsl", "shaders/water_frag.glsl", 1);
	
	TTN_AssetSystem::AddLUTTobeLoaded("Warm LUT", "Warm_LUT.cube",  1);
	TTN_AssetSystem::AddLUTTobeLoaded("Cool LUT", "Cool_LUT.cube",  1);
	TTN_AssetSystem::AddLUTTobeLoaded("Custom LUT", "Custom_LUT.cube",  1);

	TTN_AssetSystem::AddTexture2DToBeLoaded("blue ramp", "textures/ramps/blue ramp.png");
	TTN_AssetSystem::AddTexture2DToBeLoaded("Normal Map", "textures/terrain normal map.png");

	//TTN_AssetSystem::AddTexture2DToBeLoaded("Health Bar", "textures/health.png", 1); //full health bar
	TTN_AssetSystem::AddTexture2DToBeLoaded("Health Bar", "textures/healthbar.png", 1); //health bar hud empty
	TTN_AssetSystem::AddTexture2DToBeLoaded("Health Bar Dam", "textures/healthDam.png", 1); //health  hud 



	TTN_AssetSystem::AddTexture2DToBeLoaded("Button Base", "textures/Button_1.png", 1); //button when not being hovered over
	TTN_AssetSystem::AddTexture2DToBeLoaded("Button Hovering", "textures/Button_2.png", 1); //button when being hovered over
	TTN_AssetSystem::AddTexture2DToBeLoaded("Play-Text", "textures/text/play.png", 1); //rendered text of word Play
	TTN_AssetSystem::AddTexture2DToBeLoaded("Arcade-Text", "textures/text/Arcade.png", 1); //rendered text of word Arcade
	TTN_AssetSystem::AddTexture2DToBeLoaded("Options-Text", "textures/text/Options.png", 1); //rendered text of word Options
	TTN_AssetSystem::AddTexture2DToBeLoaded("Game Over", "textures/text/Game over.png", 1); //rendered text of words game over
	TTN_AssetSystem::AddTexture2DToBeLoaded("You Win", "textures/text/You win.png", 1); //rendered text of words you win!
	TTN_AssetSystem::AddTexture2DToBeLoaded("Score", "textures/text/Score.png", 1); //rendered text of word Score:
	TTN_AssetSystem::AddTexture2DToBeLoaded("Play Again", "textures/text/Play again.png", 1); //rendered text of words Play again
	TTN_AssetSystem::AddTexture2DToBeLoaded("Quit-Text", "textures/text/Quit.png", 1); //rendered text of word Quit
	TTN_AssetSystem::AddTexture2DToBeLoaded("Main Menu", "textures/text/Main Menu.png", 1); //rendered text of word main menu

	TTN_AssetSystem::AddTexture2DToBeLoaded("Game logo", "textures/Dam Defense logo.png", 1); //logo for the game
	TTN_AssetSystem::AddMeshToBeLoaded("Sphere", "models/IcoSphereMesh.obj", 1);

	//set 2, the game (excluding things already loaded into set 1)
	for(int i = 0; i < 10; i++)
		TTN_AssetSystem::AddTexture2DToBeLoaded(std::to_string(i) + "-Text", "textures/text/" + std::to_string(i) + ".png", 2); //numbers for health and score
	for (int i = 1; i < 4; i++) {
		TTN_AssetSystem::AddMeshToBeLoaded("Boat " + std::to_string(i), "models/Boat " + std::to_string(i) + ".obj", 2); //enemy boat meshes
		TTN_AssetSystem::AddTexture2DToBeLoaded("Boat texture " + std::to_string(i), "textures/Boat " + std::to_string(i) + " Texture.png", 2); //enemy boat textures 
	}
	TTN_AssetSystem::AddMorphAnimationMeshesToBeLoaded("Bird mesh", "models/bird/bird", 2, 2); //bird mesh
	TTN_AssetSystem::AddTexture2DToBeLoaded("Bird texture", "textures/BirdTexture.png", 2); //bird texture
	TTN_AssetSystem::AddTexture2DToBeLoaded("Paused-Text", "textures/text/Paused.png", 2); //rendered text of the word paused
	TTN_AssetSystem::AddTexture2DToBeLoaded("Resume-Text", "textures/text/Resume.png", 2); //rendered text of the word resume
	TTN_AssetSystem::AddTexture2DToBeLoaded("Score-Text", "textures/text/Score.png", 2); //rendered text of the word Score

	//set 3, win/lose screen
	//TTN_AssetSystem::AddTexture2DToBeLoaded("You Win-Text", "textures/text/You win.png", 3); //rendered text of the pharse "You Win!" 
	//TTN_AssetSystem::AddTexture2DToBeLoaded("Game Over-Text", "textures/text/Game over.png", 3); //rendered text of the phrase "Game Over..." 
	//TTN_AssetSystem::AddTexture2DToBeLoaded("Play Again-Text", "textures/text/Play again.png", 3); //rendered text of the phrase "Play Again" 
}