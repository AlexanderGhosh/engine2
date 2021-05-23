#include "DataManager.h"
#include "../GameObject/GameObject.h"
#include "../Scene/GameScene.h"
#include "Command.h"
#include "Response.h"
#include "../Componets/ComponetBase.h"
#include "../SharedMemoryManagerCPP.h"
#include "../Rendering/Rendering.h"
#include "../Utils/ResourceLoader.h"

std::unordered_map<int, GameObject>  EditorInteraction::DataManager::gameObjects = { };
std::unordered_map<int, Component::ComponetBase*>  EditorInteraction::DataManager::components = { };
GameScene* EditorInteraction::DataManager::scene = nullptr;
int EditorInteraction::DataManager::UID = 0;

int EditorInteraction::DataManager::getUID()
{
	return UID++;
}
bool EditorInteraction::DataManager::createGameObject(const Command& command)
{
	const int uid = getUID();
	GameObject& obj = gameObjects[uid];
	Component::RenderMesh* rm = new Component::RenderMesh();

	rm->setModel(ResourceLoader::getModel("cube.obj"));
	obj.getLocalTransform()->Position = Utils::fill(command.getCommandID() + 1);

	obj.addComponet(rm);
	components[uid] = rm;
	scene->addObject(&obj);
	return true;
}

void EditorInteraction::DataManager::sendResponse(bool success, const Command& command)
{
	std::vector<char> data;
	data.resize(command.size() + 1);
	data[0] = 1; // specifyes as a response
	data[1] = command.getCommandID();
	data[2] = static_cast<char>(success ? ResponseType::Success : ResponseType::Fail);
	Response response = Response(&data[1]);
	SharedMemoryLocation& mem = scene->getSharedData();
	// clearBuffer();
	mem.writeToFile(&data[0], data.size() * sizeof(char), 0);
}

void EditorInteraction::DataManager::clearBuffer()
{
	auto& d = scene->getSharedData();
	std::vector<char> data;
	data.resize(d.getFileSize());
	d.writeToFile(&data[0], d.getFileSize(), 0);
}

void EditorInteraction::DataManager::executeCommand(const Command& command)
{
	bool success = false;
	switch (command.getCommandType())
	{
	case CommandType::Create:
		success = createGameObject(command);
		break;
	}
	sendResponse(success, command);
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