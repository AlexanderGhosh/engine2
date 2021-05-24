#pragma once
#include <unordered_map>
#include "../Utils/General.h"

class GameScene;
class GameObject;
#define CC const Command&
#define CMD_RES const Command& command, Response& response
namespace Component {
	class ComponetBase;
};
namespace EditorInteraction {
	class Command;
	class Response;
	class DataManager
	{
	private:
		static GameScene* scene;
		static short UID;
		static std::unordered_map<short, GameObject> gameObjects;
		static std::unordered_map<short, Component::ComponetBase*> components;
		static short getUID();
		static bool createGameObject(CMD_RES);
		static bool createRenderMesh(CMD_RES);
		static bool createComponent(CMD_RES);
		static void sendResponse(bool success, CMD_RES);
		static void clearBuffer();
		static bool createCommand(CMD_RES);
	public:
		static void cleanUp();
		static void executeCommand(CC command);
		static void setScene(GameScene* scene);
	};
};