//Intermdiate CG Midterm by Ame Gilham #10071352 for INFR 2350
//MidtermScene.h - The source file for the class that represents the scene being played

//include the class
#include "MidtermScene.h"
//include required features from titan
#include "Titan/Utilities/Interpolation.h"

//default constructor
MidtermScene::MidtermScene()
	: TTN_Scene(glm::vec3(1.0f), 0.0f)
{
}

//sets up the scene
void MidtermScene::InitScene()
{
	//setup basic camera control data
	rotAmmount = glm::vec2(0.0f, 0.0f);
	mousePos = glm::vec2(0.0f, 0.0f);
	facing = glm::vec3(0.0f, 0.0f, 1.0f);
	m_deltaTime = 0.0f;
	
	//setup bloom effect
	m_bloom = TTN_BloomEffect::Create();
	glm::ivec2 windowSize = TTN_Backend::GetWindowSize();
	m_bloom->Init(windowSize.x, windowSize.y);
	m_bloom->SetShouldApply(false);
	m_PostProcessingEffects.push_back(m_bloom);

	//create materials and add them to the vector of materials
	//skybox
	TTN_Material::smatptr skyboxMat = TTN_Material::Create();
	skyboxMat->SetSkybox(TTN_AssetSystem::GetSkybox("sky")); //note we don't add the skybox to the list of mats because we aren't changing lighting on it
	//ship
	TTN_Material::smatptr shipMat = TTN_Material::Create();
	shipMat->SetAlbedo(TTN_AssetSystem::GetTexture2D("ship text"));
	m_materials.push_back(shipMat);
	//landing pad
	TTN_Material::smatptr landingPadMat = TTN_Material::Create();
	landingPadMat->SetAlbedo(TTN_AssetSystem::GetTexture2D("landing pad text"));
	m_materials.push_back(landingPadMat);
	//lighthouse
	TTN_Material::smatptr lightHouseMat = TTN_Material::Create();
	lightHouseMat->SetAlbedo(TTN_AssetSystem::GetTexture2D("lighthouse text"));
	m_materials.push_back(lightHouseMat);
	//ground
	TTN_Material::smatptr groundMat = TTN_Material::Create();
	groundMat->SetAlbedo(TTN_AssetSystem::GetTexture2D("ground text"));
	m_materials.push_back(groundMat);
	groundMat->GetAlbedo()->SetHoriWrapMode(Texture_Wrap_Mode::Repeat);
	groundMat->GetAlbedo()->SetVertWrapMode(Texture_Wrap_Mode::Repeat);

	//setup entities
	//entity for the camera
	{
		//create an entity in the scene for the camera
		m_camera = CreateEntity();
		SetCamEntity(m_camera);
		Attach<TTN_Transform>(m_camera);
		Attach<TTN_Camera>(m_camera);
		auto& camTrans = Get<TTN_Transform>(m_camera);
		camTrans.SetPos(glm::vec3(0.0f, 5.0f, -10.0f));
		camTrans.SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
		camTrans.LookAlong(glm::vec3(0.0, 0.0, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		Get<TTN_Camera>(m_camera).CalcPerspective(60.0f, 1.78f, 0.01f, 1000.f);
		Get<TTN_Camera>(m_camera).View();
	}

	//entity for the skybox
	{
		skybox = CreateEntity();

		//setup a mesh renderer for the skybox
		TTN_Renderer skyboxRenderer = TTN_Renderer(TTN_AssetSystem::GetMesh("skybox mesh"), TTN_AssetSystem::GetShader("skybox shader"));

		skyboxRenderer.SetMat(skyboxMat);
		skyboxRenderer.SetRenderLayer(100);
		//attach that renderer to the entity
		AttachCopy<TTN_Renderer>(skybox, skyboxRenderer);

		//setup a transform for the skybox
		TTN_Transform skyboxTrans = TTN_Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
		//attach that transform to the entity
		AttachCopy<TTN_Transform>(skybox, skyboxTrans);
	}

	//entity for the ship
	{
		ship = CreateEntity();

		//setup a mesh renderer for the ship
		TTN_Renderer shipRenderer = TTN_Renderer(TTN_AssetSystem::GetMesh("ship mesh"), TTN_AssetSystem::GetShader("basic shader"));
		shipRenderer.SetMat(shipMat);
		//attach that renderer to the entity
		AttachCopy<TTN_Renderer>(ship, shipRenderer);

		//setup a transform for the ship
		TTN_Transform shipTrans = TTN_Transform(glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
		//attach that transform to the entity
		AttachCopy<TTN_Transform>(ship, shipTrans);
	}

	//entities for the lights that play from the ship's engines
	for(int i = 0; i < 2; i++) {
		m_Lights.push_back(CreateEntity());

		//set up a trasnform for the light
		TTN_Transform lightTrans = TTN_Transform();
		float factor = (i == 0) ? -1.0f : 1.0f;
		lightTrans.SetPos(glm::vec3(factor * 1.446f, -0.361f, -7.006f));
		lightTrans.SetScale(glm::vec3(0.1f));
		//attach that transform to the light entity
		AttachCopy<TTN_Transform>(m_Lights[i], lightTrans);

		//set up a light component for the light
		TTN_Light lightLight = TTN_Light(glm::vec3(0.8f, 0.2f, 0.0f), 0.3f, 3.5f, 0.3f, 0.0f, 0.3f);
		//attach that light to the light entity
		AttachCopy<TTN_Light>(m_Lights[i], lightLight);

		//make the light a child of the ship
		Get<TTN_Transform>(m_Lights[i]).SetParent(&Get<TTN_Transform>(ship), &ship);
	}

	//entity for the landingpad 
	{
		landingPad = CreateEntity();

		//setup a mesh renderer for the landing pad
		TTN_Renderer padRenderer = TTN_Renderer(TTN_AssetSystem::GetMesh("landing pad mesh"), TTN_AssetSystem::GetShader("basic shader"));
		padRenderer.SetMat(landingPadMat);
		//attach that renderer to the entity
		AttachCopy<TTN_Renderer>(landingPad, padRenderer);

		//setup a transform for the landing pad 
		TTN_Transform padTrans = TTN_Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
		//attach that transform to the entity
		AttachCopy<TTN_Transform>(landingPad, padTrans);
	}

	//entity for the lighthouse 
	{
		lightHouse = CreateEntity();

		//setup a mesh renderer for the light house
		TTN_Renderer houseRenderer = TTN_Renderer(TTN_AssetSystem::GetMesh("lighthouse mesh"), TTN_AssetSystem::GetShader("basic shader"));
		houseRenderer.SetMat(lightHouseMat);
		//attach that renderer to the entity
		AttachCopy<TTN_Renderer>(lightHouse, houseRenderer);

		//setup a transform for the light house
		TTN_Transform houseTrans = TTN_Transform(glm::vec3(12.5f, 5.0f, 3.0f), glm::vec3(0.0f), glm::vec3(1.0f));
		//attach that transform to the entity
		AttachCopy<TTN_Transform>(lightHouse, houseTrans);
	}

	//entity for the lighthouse's light
	{
		m_Lights.push_back(CreateEntity());

		//set up a trasnform for the light
		TTN_Transform lightTrans = TTN_Transform();
		lightTrans.SetPos(glm::vec3(12.5f, 15.0f, 3.0f));
		lightTrans.SetScale(glm::vec3(0.1f));
		//attach that transform to the light entity
		AttachCopy<TTN_Transform>(m_Lights[m_Lights.size() - 1], lightTrans);

		//set up a light component for the light
		TTN_Light lightLight = TTN_Light(glm::vec3(1.0f, 1.0f, 1.0f), 2.0f, 6.5f, 0.3f, 0.0f, 0.3f);
		//attach that light to the light entity
		AttachCopy<TTN_Light>(m_Lights[m_Lights.size() - 1], lightLight);
	}

	//entity for the ground plain
	{
		ground = CreateEntity();

		//setup a mesh renderer for the ground
		TTN_Renderer groundRenderer = TTN_Renderer(TTN_AssetSystem::GetMesh("plane"), TTN_AssetSystem::GetShader("basic shader"));
		groundRenderer.SetMat(groundMat);
		//attach that renderer to the entity
		AttachCopy<TTN_Renderer>(ground, groundRenderer);

		//setup a transform for the ground
		TTN_Transform groundTrans = TTN_Transform(glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(0.0f), glm::vec3(100.0f));
		//attach that transform to the entity
		AttachCopy<TTN_Transform>(ground, groundTrans);
	}
}

//updates the scene each frame
void MidtermScene::Update(float deltaTime)
{
	//copy the change in time over so the input checks can use them
	m_deltaTime = deltaTime;

	//have the ship fly up from the pad, fly off, come back, land, and repeat
	//add to it's time tracker
	timeSinceSegmentStart += deltaTime;
	//if a segment of it's path has ended
	if (timeSinceSegmentStart >= segmentLenghts[currentPath]) {
		//reset the timer
		timeSinceSegmentStart = 0.0f;
		//and move to the next path
		currentPath++;
		//reseting the thing if it has landed again
		if (currentPath > 4) currentPath = 0;
	}

	//if the ship is currently landed
	if (currentPath == 0) {
		Get<TTN_Transform>(ship).SetPos(glm::vec3(0.0f, 3.0f, 0.0f));
	}
	//if the ship is taking off
	if (currentPath == 1) {
		Get<TTN_Transform>(ship).SetPos(TTN_Interpolation::Lerp(glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.0f, 20.0f, 0.0f), 
			std::clamp(timeSinceSegmentStart / segmentLenghts[1], 0.0f, 1.0f)));
	}
	//if the ship is flying off
	if (currentPath == 2) {
		Get<TTN_Transform>(ship).SetPos(TTN_Interpolation::Lerp(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(0.0f, 500.0f, 1000.0f),
			SlowStart(std::clamp(timeSinceSegmentStart / segmentLenghts[2], 0.0f, 1.0f))));
	}
	//if the ship is flying back
	if (currentPath == 3) {
		Get<TTN_Transform>(ship).SetPos(TTN_Interpolation::Lerp(glm::vec3(0.0f, 500.0f, -1000.0f), glm::vec3(0.0f, 20.0f, 0.0f),
			FastStart(std::clamp(timeSinceSegmentStart / segmentLenghts[3], 0.0f, 1.0f))));
	}
	//if the ship is landing
	if (currentPath == 4) {
		Get<TTN_Transform>(ship).SetPos(TTN_Interpolation::Lerp(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(0.0f, 3.0f, 0.0f),
			std::clamp(timeSinceSegmentStart / segmentLenghts[4], 0.0f, 1.0f)));
	}

	//call the imgui update function
	ImGui();

	//don't forget to call the base scene's update
	TTN_Scene::Update(deltaTime);
}

void MidtermScene::KeyChecks()
{
	//move in the direction the camera is facing
	if (TTN_Application::TTN_Input::GetKey(TTN_KeyCode::W)) {
		Get<TTN_Transform>(m_camera).SetPos(Get<TTN_Transform>(m_camera).GetPos() + facing * 20.0f * m_deltaTime);
	}

	//move backwards from the direction the camera is facing
	if (TTN_Application::TTN_Input::GetKey(TTN_KeyCode::S)) {
		Get<TTN_Transform>(m_camera).SetPos(Get<TTN_Transform>(m_camera).GetPos() + facing * -20.0f * m_deltaTime);
	}

	//move to the left of the direction the camera is facing
	if (TTN_Application::TTN_Input::GetKey(TTN_KeyCode::A)) {
		glm::vec3 right = glm::vec3(glm::toMat4(glm::quat(glm::radians(glm::vec3(90.0f, 90.0f, 90.0f))))
			* glm::vec4(facing, 1.0f));;
		right.y = 0.0f;
		Get<TTN_Transform>(m_camera).SetPos(Get<TTN_Transform>(m_camera).GetPos() + right * 20.0f * m_deltaTime);
	}

	//move to the right of the direction the camera is facing
	if (TTN_Application::TTN_Input::GetKey(TTN_KeyCode::D)) {
		glm::vec3 right = glm::vec3(glm::toMat4(glm::quat(glm::radians(glm::vec3(90.0f, 90.0f, 90.0f))))
			* glm::vec4(facing, 1.0f));;
		right.y = 0.0f;
		Get<TTN_Transform>(m_camera).SetPos(Get<TTN_Transform>(m_camera).GetPos() + right * -20.0f * m_deltaTime);
	}
}

void MidtermScene::MouseButtonChecks()
{
	//get the mouse position
	glm::vec2 tempMousePos = TTN_Application::TTN_Input::GetMousePosition();

	if (TTN_Application::TTN_Input::GetMouseButton(TTN_MouseButton::Right)) {
		//figure out how much the cannon and camera should be rotated
		rotAmmount += (tempMousePos - mousePos) * 25.0f * m_deltaTime;

		//this is needed to make the rotating by mouse work
		if (rotAmmount.x > 360.0f) rotAmmount.x = 360.0f;
		else if (rotAmmount.x < 0.0f) rotAmmount.x = 0.0f;
		if (rotAmmount.y > 85.0f) rotAmmount.y = 85.0f;
		else if (rotAmmount.y < -85.0f) rotAmmount.y = -85.0f;

		//reset the rotation
		Get<TTN_Transform>(m_camera).LookAlong(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//and rotate it by the ammount it should be rotated
		Get<TTN_Transform>(m_camera).RotateFixed(glm::vec3(rotAmmount.y, -rotAmmount.x, 0.0f));
		//clear the direction the player is facing, and rotate it to face the same along
		facing = glm::vec3(0.0f, 0.0f, 1.0f);
		facing = glm::vec3(glm::toMat4(glm::quat(glm::radians(glm::vec3(rotAmmount.y, -rotAmmount.x, 0.0f))))
			* glm::vec4(facing, 1.0f));
		facing = glm::normalize(facing);
	}

	mousePos = tempMousePos;
}

void MidtermScene::ImGui()
{
	//begin the imgui tab
	ImGui::Begin("Editor");

	//light control
	if (ImGui::CollapsingHeader("Light Controls")) {
		ImGui::Text("Maximum number of lights: 16");

		//scene level lighting
		float sceneAmbientLight[3], sceneAmbientStr;
		sceneAmbientLight[0] = GetSceneAmbientColor().r;
		sceneAmbientLight[1] = GetSceneAmbientColor().g;
		sceneAmbientLight[2] = GetSceneAmbientColor().b;
		sceneAmbientStr = GetSceneAmbientLightStrength();

		//scene level ambient strenght
		if (ImGui::SliderFloat("Scene level ambient strenght", &sceneAmbientStr, 0.0f, 1.0f)) {
			SetSceneAmbientLightStrength(sceneAmbientStr);
		}

		//scene level ambient color
		if (ImGui::ColorPicker3("Scene level ambient color", sceneAmbientLight)) {
			SetSceneAmbientColor(glm::vec3(sceneAmbientLight[0], sceneAmbientLight[1], sceneAmbientLight[2]));
		}

		//loop through all the lights
		int i = 0;
		std::vector<entt::entity>::iterator it = m_Lights.begin();
		while (it != m_Lights.end()) {
			//make temp floats for their data
			float color[3], pos[3], ambientStr, specularStr, attenConst, attenLine, attenQuad;
			TTN_Light& tempLightRef = Get<TTN_Light>(*it);
			TTN_Transform& tempLightTransRef = Get<TTN_Transform>(*it);
			color[0] = tempLightRef.GetColor().r;
			color[1] = tempLightRef.GetColor().g;
			color[2] = tempLightRef.GetColor().b;
			pos[0] = tempLightTransRef.GetPos().x;
			pos[1] = tempLightTransRef.GetPos().y;
			pos[2] = tempLightTransRef.GetPos().z;
			ambientStr = tempLightRef.GetAmbientStrength();
			specularStr = tempLightRef.GetSpecularStrength();
			attenConst = tempLightRef.GetConstantAttenuation();
			attenLine = tempLightRef.GetLinearAttenuation();
			attenQuad = tempLightRef.GetQuadraticAttenuation();

			//position
			std::string tempPosString = "Light " + std::to_string(i) + " Position";
			if (ImGui::SliderFloat3(tempPosString.c_str(), pos, -20.0f, 20.0f)) {
				tempLightTransRef.SetPos(glm::vec3(pos[0], pos[1], pos[2]));
			}

			//color
			std::string tempColorString = "Light " + std::to_string(i) + " Color";
			if (ImGui::ColorPicker3(tempColorString.c_str(), color)) {
				tempLightRef.SetColor(glm::vec3(color[0], color[1], color[2]));
			}

			//strenghts
			std::string tempAmbientStrString = "Light " + std::to_string(i) + " Ambient strenght";
			if (ImGui::SliderFloat(tempAmbientStrString.c_str(), &ambientStr, 0.0f, 10.0f)) {
				tempLightRef.SetAmbientStrength(ambientStr);
			}

			std::string tempSpecularStrString = "Light " + std::to_string(i) + " Specular strenght";
			if (ImGui::SliderFloat(tempSpecularStrString.c_str(), &specularStr, 0.0f, 10.0f)) {
				tempLightRef.SetSpecularStrength(specularStr);
			}

			//attenutaition
			std::string tempAttenConst = "Light " + std::to_string(i) + " Constant Attenuation";
			if (ImGui::SliderFloat(tempAttenConst.c_str(), &attenConst, 0.0f, 100.0f)) {
				tempLightRef.SetConstantAttenuation(attenConst);
			}

			std::string tempAttenLine = "Light " + std::to_string(i) + " Linear Attenuation";
			if (ImGui::SliderFloat(tempAttenLine.c_str(), &attenLine, 0.0f, 100.0f)) {
				tempLightRef.SetLinearAttenuation(attenLine);
			}

			std::string tempAttenQuad = "Light " + std::to_string(i) + " Quadratic Attenuation";
			if (ImGui::SliderFloat(tempAttenQuad.c_str(), &attenQuad, 0.0f, 100.0f)) {
				tempLightRef.SetQuadraticAttenuation(attenQuad);
			}

			std::string tempButton = "Remove Light " + std::to_string(i);
			if (ImGui::Button(tempButton.c_str())) {
				DeleteEntity(*it);
				it = m_Lights.erase(it);
			}

			i++;
			it++;
		}

		//if there are less than 16 lights, give a button that allows the user to add a new light
		if (i < 15) {
			if (ImGui::Button("Add New Light")) {
				m_Lights.push_back(CreateEntity());

				TTN_Transform newTrans = TTN_Transform();
				TTN_Light newLight = TTN_Light();

				AttachCopy(m_Lights[m_Lights.size() - 1], newTrans);
				AttachCopy(m_Lights[m_Lights.size() - 1], newLight);
			}
		}
	}

	//the control for all the assignment requirements
	if (ImGui::CollapsingHeader("Effect Controls")) {
		//Lighting controls 

		//No ligthing
		if (ImGui::Checkbox("No Lighting", &m_noLighting)) {
			//set no lighting to true
			m_noLighting = true;
			//change all the other lighting settings to false
			m_ambientOnly = false;
			m_specularOnly = false;
			m_ambientAndSpecular = false;
			m_ambientSpecularAndBloom = false;

			//set that data in the materials
			for (int i = 0; i < m_materials.size(); i++) {
				m_materials[i]->SetHasAmbient(false);
				m_materials[i]->SetHasSpecular(false);
				m_bloom->SetShouldApply(false);
			}
		}

		//Ambient only
		if (ImGui::Checkbox("Ambient Lighting Only", &m_ambientOnly)) {
			//set ambient only to true
			m_ambientOnly = true;
			//change all the other lighting settings to false
			m_noLighting = false;
			m_specularOnly = false;
			m_ambientAndSpecular = false;
			m_ambientSpecularAndBloom = false;

			//set that data in the materials
			for (int i = 0; i < m_materials.size(); i++) {
				m_materials[i]->SetHasAmbient(true);
				m_materials[i]->SetHasSpecular(false);
				m_bloom->SetShouldApply(false);
			}
		}

		//Specular only
		if (ImGui::Checkbox("Specular Lighting Only", &m_specularOnly)) {
			//set Specular only to true
			m_specularOnly = true;
			//change all the other lighting settings to false
			m_noLighting = false;
			m_ambientOnly = false;
			m_ambientAndSpecular = false;
			m_ambientSpecularAndBloom = false;

			//set that data in the materials
			for (int i = 0; i < m_materials.size(); i++) {
				m_materials[i]->SetHasAmbient(false);
				m_materials[i]->SetHasSpecular(true);
				m_bloom->SetShouldApply(false);
			}
		}

		//Ambient and specular
		if (ImGui::Checkbox("Ambient and Specular Lighting", &m_ambientAndSpecular)) {
			//set ambient and specular to true
			m_ambientAndSpecular = true;
			//change all the other lighting settings to false
			m_noLighting = false;
			m_ambientOnly = false;
			m_specularOnly = false;
			m_ambientSpecularAndBloom = false;

			//set that data in the materials
			for (int i = 0; i < m_materials.size(); i++) {
				m_materials[i]->SetHasAmbient(true);
				m_materials[i]->SetHasSpecular(true);
				m_bloom->SetShouldApply(false);
			}
		}

		//Ambient, specular, and bloom
		if (ImGui::Checkbox("Ambient and Specular Lighting With Bloom ", &m_ambientSpecularAndBloom)) {
			//set ambient, specular, and bloom to true
			m_ambientSpecularAndBloom = true;
			//change all the other lighting settings to false
			m_noLighting = false;
			m_ambientOnly = false;
			m_specularOnly = false;
			m_ambientAndSpecular = false;

			//set that data in the materials
			for (int i = 0; i < m_materials.size(); i++) {
				m_materials[i]->SetHasAmbient(true);
				m_materials[i]->SetHasSpecular(true);
				m_bloom->SetShouldApply(true);
			}
		}

		//texture controls
		if (ImGui::Checkbox("Use Textures", &m_useTextures)) {
			for (int i = 0; i < m_materials.size(); i++) {
				m_materials[i]->SetUseAlbedo(m_useTextures);
			}
		}

		//bloom controls

		//threshold value
		float threshold = m_bloom->GetThreshold();
		if (ImGui::SliderFloat("Bloom Threshold", &threshold, 0.0f, 1.0f)) {
			m_bloom->SetThreshold(threshold);
		}

		//radius value
		float radius = m_bloom->GetRadius();
		if (ImGui::SliderFloat("Bloom blur radius", &radius, 0.1f, 10.0f)) {
			m_bloom->SetRadius(radius);
		}

		//number of passes
		int numOfPasses = m_bloom->GetNumOfPasses();
		if (ImGui::SliderInt("Bloom number of Blur passes", &numOfPasses, 1, 25)) {
			m_bloom->SetNumOfPasses(numOfPasses);
		}

		//downsizing scale of the blur framebuffers
		int downScale = m_bloom->GetBlurDownScale();
		if (ImGui::SliderInt("Bloom downward scale of blur buffers", &downScale, 1, 32)) {
			m_bloom->SetBlurDownScale(downScale);
		}
	}

	//end the ImGui tab
	ImGui::End();
}