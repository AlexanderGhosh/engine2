#include "DataManager.h"
#include "../GameObject/GameObject.h"
#include "../Scene/GameScene.h"
#include "Command.h"
#include "Response.h"
#include "../Componets/ComponetBase.h"
#include "../SharedMemoryManagerCPP.h"
#include "../Rendering/Rendering.h"
#include "../Utils/ResourceLoader.h"

std::unordered_map<short, GameObject>  EditorInteraction::DataManager::gameObjects = { };
std::unordered_map<short, Component::ComponetBase*>  EditorInteraction::DataManager::components = { };
GameScene* EditorInteraction::DataManager::scene = nullptr;
short EditorInteraction::DataManager::UID = 0;

short EditorInteraction::DataManager::getUID()
{
	return UID++ % 0xffff;
}
bool EditorInteraction::DataManager::createGameObject(CC command, Response& response)
{
	const short uid = getUID();
	GameObject& obj = gameObjects[uid];
	scene->addObject(&obj);

	response.setLocationUID(uid);
	response.setLocationType(LocationType::GameObject);
	return true;
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
	response.setLocationType(LocationType::RenderMesh);
	return true;
}

bool EditorInteraction::DataManager::createComponent(CC command, Response& response) {
	const auto& payload = command.getPayload();
	const byte& componetType = payload[0];
	switch (componetType)
	{
	case 0:
		return createRenderMesh(command, response);
	}

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

void EditorInteraction::DataManager::clearBuffer()
{
	auto& d = scene->getSharedData();
	std::vector<char> data;
	data.resize(d.getFileSize());
	d.writeToFile(&data[0], d.getFileSize(), 0);
}

bool EditorInteraction::DataManager::createCommand(CC command, Response& response)
{
	bool success = false;
	switch (command.getLocationType())
	{
	case LocationType::GameObject:
		success = createGameObject(command, response);
		break;
	case LocationType::RenderMesh:
		success = createRenderMesh(command, response);
		break;
	case LocationType::Components:
		success = createComponent(command, response);
	}
	return success;
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
		delete (*itt).second;
		itt = components.erase(itt);
	}
}