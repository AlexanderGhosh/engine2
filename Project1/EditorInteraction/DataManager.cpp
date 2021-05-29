#include "DataManager.h"
#include "../GameObject/GameObject.h"
#include "../Scene/GameScene.h"
#include "Command.h"
#include "Response.h"
#include "../Componets/ComponetBase.h"
#include "../SharedMemoryManagerCPP.h"
#include "../Rendering/Rendering.h"
#include "../Utils/ResourceLoader.h"
#include "../Componets/Camera.h"

std::unordered_map<short, GameObject>  EditorInteraction::DataManager::gameObjects = { };
std::unordered_map<short, Component::ComponetBase*>  EditorInteraction::DataManager::components = { };
GameScene* EditorInteraction::DataManager::scene = nullptr;
short EditorInteraction::DataManager::UID = 0;
std::vector<short> EditorInteraction::DataManager::transformUIDs = { };

short EditorInteraction::DataManager::getUID()
{
	return UID++ % 0xffff;
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
	}
	return success;
}

bool EditorInteraction::DataManager::createGameObject(CC command, Response& response)
{
	short uid = getUID();
	GameObject& obj = gameObjects[uid];
	scene->addObject(&obj);

	uid = getUID();
	components[uid] = obj.getLocalTransform(); 
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
	}
	return false;
}

bool EditorInteraction::DataManager::createRenderMesh(CC command, Response& response)
{
	const short uid = getUID();
	const short parentUID = command.getLocationUID();
	GameObject& parentObject = gameObjects[parentUID];
	auto& payload = command.getPayload();

	const short modelUID = Utils::convert<short>(&payload[2]);
	const bool isTransparent = payload[5];
	const byte materialCount = payload[7];
	const byte sizeOfMaterialUID = payload[8];

	int ptr = 8;
	for (byte i = 0; i < materialCount; i++) {
		const short materialUID = Utils::convert<short>(&payload[ptr]);
		ptr += sizeOfMaterialUID;
	}

	Component::RenderMesh* rm = DBG_NEW Component::RenderMesh();
	rm->setModel(ResourceLoader::getModel("cube.obj"));
	rm->setTransparent(isTransparent);

	parentObject.addComponet(rm);
	components[uid] = rm;

	response.setLocationUID(uid);
	response.setLocationType(LocationType::Components);
	return true;
}

bool EditorInteraction::DataManager::createCamera(CMD_RES)
{
	const short uid = getUID();
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
	glm::quat rot(eular);

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
		success = editTransform(command, response, Utils::ptrCast<Component::Transform>(component));
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
	auto* mainCamera = scene->getMainCamera();
	auto& payload = command.getPayload();
	const byte functionIndex = payload[0];
	float xOff = Utils::convert<float>(&payload[1]);
	float yOff = Utils::convert<float>(&payload[5]);

	Utils::log(glm::to_string(glm::vec2(xOff, yOff)), "\r");
	// xOff = -1;
	// yOff = 0;
	mainCamera->processMouseMovement(xOff, yOff);
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
