#pragma once
#include <unordered_map>
#include "../Utils/General.h"

class GameScene;
class GameObject;
namespace Component {
	class ComponetBase;
};
namespace EditorInteraction {
	class Command;
	class DataManager
	{
	private:
		static GameScene* scene;
		static int UID;
		static std::unordered_map<int, GameObject> gameObjects;
		static std::unordered_map<int, Component::ComponetBase*> components;
		static int getUID();
		static bool createGameObject(const Command& command);
		static void sendResponse(bool success, const Command& command);
		static void clearBuffer();
	public:
		static void cleanUp();
		static void executeCommand(const Command& command);
		static void setScene(GameScene* scene);
	};
};