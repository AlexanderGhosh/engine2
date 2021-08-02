#include "DataManager.h"
#include "../GameObject/GameObject.h"
#include "../Scene/GameScene.h"
#include "Command.h"
#include "Response.h"
#include "ResourceItem.h"
#include "../Componets/ComponetBase.h"
#include "../SharedMemoryManagerCPP.h"
#include "../Rendering/Rendering.h"
#include "../Utils/ResourceLoader.h"
#include "../Componets/Camera.h"
#include "../Componets/OrbitCamera.h"

std::unordered_map<short, GameObject>  EditorInteraction::DataManager::gameObjects = { };
std::unordered_map<short, Component::ComponetBase*>  EditorInteraction::DataManager::components = { };
std::unordered_map<short, EditorInteraction::ResourceItem> EditorInteraction::DataManager::resources = { };
GameScene* EditorInteraction::DataManager::scene = nullptr;
short EditorInteraction::DataManager::objUID = 0;
short EditorInteraction::DataManager::compUID = 0;
short EditorInteraction::DataManager::resUID = 0;
std::vector<short> EditorInteraction::DataManager::transformUIDs = { };

short EditorInteraction::DataManager::getGameObjUID()
{
	return objUID++ % 0xffff;
}
short EditorInteraction::DataManager::getCompUID()
{
	return compUID++ % 0xffff;
}
short EditorInteraction::DataManager::getResUID()
{
	return resUID++ % 0xffff;
}

bool EditorInteraction::DataManager::resizeWindow(CC command, Response& response)
{
	const auto& payload = command.getPayload();
	short width = Utils::convert<short>(&payload[0]);
	byte ar_x = Utils::convert<short>(&payload[2]);
	byte ar_y = Utils::convert<short>(&payload[3]);
	glm::svec2 ar = glm::svec2(static_cast<short>(ar_x), static_cast<short>(ar_y));
	scene->reSize(ar, width);
	return true;
}

bool EditorInteraction::DataManager::createCommand(CC command, Response& response)
{
	bool success = false;
	switch (command.getLocationType())
	{
	case LocationType::GameObject:
		success = createGameObject(command, response);
		break;
	case LocationType::Components:
		success = createComponent(command, response);
		break;
	case LocationType::Resource:
		success = createResource(command, response);
		break;
	}
	return success;
}

bool EditorInteraction::DataManager::createGameObject(CC command, Response& response)
{
	short uid = getGameObjUID();
	GameObject& obj = gameObjects[uid];
	scene->addObject(&obj);

	uid = getCompUID();
	components[uid] = obj.getLocalTransform(); 
	if (!components[uid])
	{
		bool t = false;
	}
	transformUIDs.push_back(uid);

	response.setLocationUID(uid);
	response.setLocationType(LocationType::GameObject);
	return true;
}

bool EditorInteraction::DataManager::createComponent(CC command, Response& response) {
	const auto& payload = command.getPayload();
	const byte& componetType = payload[0];
	switch (componetType)
	{
	case 0: // transform
		break;
	case 1:
		return createRenderMesh(command, response);
		break;
	case 2:
		return createCamera(command, response);
		break;
	case 4:
		return createOrbitCamera(command, response);
		break;
	}
	return false;
}

bool EditorInteraction::DataManager::createRenderMesh(CC command, Response& response)
{
	const short uid = getCompUID();
	const short parentUID = command.getLocationUID();
	GameObject& parentObject = gameObjects[parentUID];
	auto& payload = command.getPayload();

	const short modelUID = Utils::convert<short>(&payload[2]);
	// Utils::log(std::to_string(modelUID));
	const bool isTransparent = payload[5];
	const byte materialCount = payload[7];
	const byte sizeOfMaterialUID = payload[8];

	int ptr = 8;
	for (byte i = 0; i < materialCount; i++) {
		const short materialUID = Utils::convert<short>(&payload[ptr]);
		ptr += sizeOfMaterialUID;
	}

	Component::RenderMesh* rm = DBG_NEW Component::RenderMesh();

	ResourceItem item = resources[modelUID];
	if (item.getType() != ResourceTypes::Model) {
		rm->setModel(ResourceLoader::getModel("cube.dae"));
		rm->setTransparent(0);
		if (item.getType() == ResourceTypes::ERROR) {
			resources.erase(modelUID);
		}
	}
	else {
		Primative::Model model = ResourceLoader::getModel(item.getName());
		rm->setModel(model);
		rm->setTransparent(isTransparent);
	}

	parentObject.addComponet(rm);
	components[uid] = rm;

	response.setLocationUID(uid);
	response.setLocationType(LocationType::Components);
	return true;
}

bool EditorInteraction::DataManager::createCamera(CMD_RES)
{
	const short uid = getCompUID();
	const short parentUID = command.getLocationUID();
	GameObject& parentObject = gameObjects[parentUID];
	auto& payload = command.getPayload();

	const bool isMain = payload[2];

	Component::Camera* cam = DBG_NEW Component::Camera();
	parentObject.addComponet(cam);

	components[uid] = cam;

	if (isMain) {
		scene->setMainCamera(cam);
	}

	response.setLocationUID(uid);
	response.setLocationType(LocationType::Components);
	return true;
}

bool EditorInteraction::DataManager::createOrbitCamera(CMD_RES)
{
	const short uid = getCompUID();
	const short parentUID = command.getLocationUID();
	GameObject& parentObject = gameObjects[parentUID];
	auto& payload = command.getPayload();

	const bool isMain = payload[2];

	Component::OrbitCamera* cam = DBG_NEW Component::OrbitCamera();
	parentObject.addComponet(cam);

	components[uid] = cam;

	if (isMain) {
		scene->setMainCamera(cam);
	}

	response.setLocationUID(uid);
	response.setLocationType(LocationType::Components);
	return true;
}

bool EditorInteraction::DataManager::createResource(CC command, Response& response)
{
	bool success = false;

	const auto& payload = command.getPayload();
	const byte& nameLen = payload[1];

	std::vector<char> nameData;
	nameData.resize(nameLen);

	memcpy(nameData.data(), &payload[2], nameLen);

	const std::string name(nameData.begin(), nameData.end());

	const byte& resType = payload[0];

	switch (resType)
	{
	case 0:
		success = createModel(command, response, name);
		break;
	case 1:
		success = createTexture(command, response, name);
		break;
	case 2:
		success = createSound(command, response, name);
		break;
	}
	return success;
}

bool EditorInteraction::DataManager::createModel(CC command, Response& response, String name)
{
	ResourceLoader::createModel(name);
	ResourceItem res = ResourceItem(name, ResourceTypes::Model);
	resources[getResUID()] = res;
	return true;
}

bool EditorInteraction::DataManager::createTexture(CC command, Response& response, String name)
{
	ResourceLoader::loadTexture(name, TextureType::AlbedoMap);
	ResourceItem res = ResourceItem(name, ResourceTypes::Texture);
	resources[getResUID()] = res;
	return true;
}

bool EditorInteraction::DataManager::createSound(CC command, Response& response, String name)
{
	return false;
}

void EditorInteraction::DataManager::sendResponse(bool success, CC command, Response& response)
{
	std::vector<char> data;
	data.resize(response.size() + 1);
	data[0] = 1; // specifyes as a response

	response.setResponseID(command.getCommandID());
	response.setResponseType(success ? ResponseType::Success : ResponseType::Fail);
	std::vector<char> responseData = response.getData();
	data.insert(data.begin() + 1, responseData.begin(), responseData.end());

	SharedMemoryLocation& mem = scene->getSharedData();
	mem.writeToFile(&data[0], data.size() * sizeof(char), 0);
}

bool EditorInteraction::DataManager::editCommand(CC command, Response& response)
{
	bool success = false;
	switch (command.getLocationType())
	{
	case LocationType::Components:
		success = editComponent(command, response);
	}
	return success;
}

bool EditorInteraction::DataManager::editTransform(CC command, Response& response, Component::Transform* component)
{
	/*const short parentUID = command.getLocationUID();
	GameObject& parent = gameObjects[parentUID];
	Component::Transform* transform = parent.getLocalTransform();
	auto& payload = command.getPayload();

	const byte indexOfParam = payload[0];
	const byte sizeOfParam = payload[1];

	glm::vec3 newData(0);
	newData.x = Utils::convert<float>(&payload[2]);
	newData.y = Utils::convert<float>(&payload[6]);
	newData.z = Utils::convert<float>(&payload[10]);

	switch (indexOfParam)
	{
	case 0:
		transform->Position = newData;
		break;
	case 2:
		transform->Scale = newData;
		break;
	case 3:
		transform->Rotation = glm::quat(newData);
		break;
	}
	response.setLocationUID(response.getLocationUID());
	response.setLocationType(LocationType::Components);*/

	const auto& payload = command.getPayload();
	const byte& typeCode = payload[0];
	glm::vec3 pos = Utils::convert<glm::vec3>(&payload[2]);
	glm::vec3 scale = Utils::convert<glm::vec3>(&payload[15]);
	glm::vec3 eular = Utils::convert<glm::vec3>(&payload[28]);
	glm::quat rot(glm::radians(eular));

	Utils::log("New Pos: " + glm::to_string(pos));

	component->Position = pos;
	component->Scale = scale;
	component->Rotation = glm::normalize(rot);

	return true;
}

bool EditorInteraction::DataManager::editComponent(CC command, Response& response)
{
	bool success = false;
	Component::ComponetBase* component = components[command.getLocationUID()];
	if (!component) {
		components.erase(command.getLocationUID());
	}
	else {
		switch (component->getType())
		{
		case Component::Type::Transform:
			success = editTransform(command, response, Utils::ptrCast<Component::Transform>(component));
			break;
		}
	}
	return success;
}

bool EditorInteraction::DataManager::callFunction(CC command, Response& response)
{
	auto* mainCamera = Utils::ptrCast<Component::OrbitCamera>(scene->getMainCamera());
	auto& payload = command.getPayload();
	const byte functionIndex = payload[0];
	float xOff = Utils::convert<float>(&payload[1]);
	float yOff = Utils::convert<float>(&payload[5]);
	bool move = Utils::convert<bool>(&payload[9]);

	// Utils::log(glm::to_string(glm::vec2(xOff, yOff)), "\r");
	// xOff = -1;
	// yOff = 0;
	mainCamera->processMouseMovement(xOff, yOff, move);
	return true;
}

void EditorInteraction::DataManager::clearBuffer()
{
	auto& d = scene->getSharedData();
	std::vector<char> data;
	data.resize(d.getFileSize());
	d.writeToFile(&data[0], d.getFileSize(), 0);
}

void EditorInteraction::DataManager::executeCommand(CC command)
{
	bool success = false;
	Response response;
	switch (command.getCommandType())
	{
	case CommandType::Create:
		success = createCommand(command, response);
		break;
	case CommandType::Edit:
		success = editCommand(command, response);
		break;
	case CommandType::CallFunction:
		success = callFunction(command, response);
		break;
	case CommandType::ResizeWindow:
		success = resizeWindow(command, response);
		break;
	}
	sendResponse(success, command, response);
	std::string msg = "";
	for (auto o : gameObjects) {
		msg += "GO: " + std::to_string(o.first) + "\n";
	}
	for (auto o : components) {
		msg += "COMP: " + std::to_string(o.first) + "\n";
	}
	for (auto o : resources) {
		msg += "RES: " + std::to_string(o.first) + "\n";
	}
	Utils::writeToFile("C:\\Users\\AGWDW\\Desktop\\log.txt", msg);
}

void EditorInteraction::DataManager::setScene(GameScene* scene)
{
	EditorInteraction::DataManager::scene = scene;
}

void EditorInteraction::DataManager::cleanUp()
{
	for (auto itt = components.begin(); itt != components.end();) {
		if (Utils::contains(transformUIDs, (*itt).first)) {

		}
		else if ((*itt).second) {
			delete (*itt).second;
		}
		itt = components.erase(itt);
	}
}
