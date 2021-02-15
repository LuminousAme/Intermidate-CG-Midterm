//Intermdiate CG Midterm by Ame Gilham #10071352 for INFR 2350
//main.cpp - The source file that runs the program

//include titan features
#include "Titan/Application.h"
//include the scene class
#include "MidtermScene.h"
//set the namespace being used
using namespace Titan;

//asset setup function
void PrepareAssetLoading();

int main() {
	//initliaze OTTER's base logging system
	Logger::Init(); 
	 //initliaze titan's application
	TTN_Application::Init("Intermidate Computer Graphics Midterm - INFR 2350 - Ame Gilham 100741352", 1920, 1080);
	//initliaze imgui for titan
	TTN_Application::InitImgui();

	//make sure the cursor is not locked so that ImGui controls can be used
	TTN_Application::TTN_Input::SetCursorLocked(false);

	//set up the assets to be loaded
	PrepareAssetLoading();

	//load all of the assets
	TTN_AssetSystem::LoadSetNow(0);
	
	//create and initliaze the scene
	MidtermScene* scene = new MidtermScene;
	scene->InitScene();
	//and add it to the application
	TTN_Application::scenes.push_back(scene);

	//while the application is running, main loop
	while (!TTN_Application::GetIsClosing()) {
		//update the scenes and render them to the screen
		TTN_Application::Update();
	}

	//clean up all the application data as the application is now ending
	TTN_Application::Closing();

	//when the application has ended, exit the program with no errors
	return 0;
} 

void PrepareAssetLoading() {
	TTN_AssetSystem::AddSkyboxToBeLoaded("sky", "textures/skybox/night.jpg");
	TTN_AssetSystem::AddMeshToBeLoaded("skybox mesh", "models/SkyboxMesh.obj");
	TTN_AssetSystem::AddDefaultShaderToBeLoaded("skybox shader", TTN_DefaultShaders::VERT_SKYBOX, TTN_DefaultShaders::FRAG_SKYBOX);

	TTN_AssetSystem::AddTexture2DToBeLoaded("ship text", "textures/Gunship texture.png");
	TTN_AssetSystem::AddMeshToBeLoaded("ship mesh", "models/SpaceShip.obj");
	TTN_AssetSystem::AddTexture2DToBeLoaded("landing pad text", "textures/Landing pad text.png");
	TTN_AssetSystem::AddMeshToBeLoaded("landing pad mesh", "models/LandingPad.obj");
	TTN_AssetSystem::AddTexture2DToBeLoaded("lighthouse text", "textures/light post text.png");
	TTN_AssetSystem::AddMeshToBeLoaded("lighthouse mesh", "models/LightHouse.obj");
	TTN_AssetSystem::AddDefaultShaderToBeLoaded("basic shader", TTN_DefaultShaders::VERT_NO_COLOR, TTN_DefaultShaders::FRAG_BLINN_PHONG_ALBEDO_ONLY);
}