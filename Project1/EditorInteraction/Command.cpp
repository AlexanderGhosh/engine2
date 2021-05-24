#include "Command.h"

byte EditorInteraction::Command::commandID = 255;
EditorInteraction::Command::Command() : command(0), locationType(0), locationUID(0), payload({})
{
}

EditorInteraction::Command::Command(char* data) : Command()
{
	if (commandID == data[0])
		return;
	commandID = data[0];
	command = data[1];
	locationType = data[2];
	locationUID = Utils::convert<short>(&data[3]);
	std::memcpy(&payload, &data[5], PAYLOAD_SIZE);
}

void EditorInteraction::Command::cleanUp()
{
}

size_t EditorInteraction::Command::size()
{
	return PAYLOAD_SIZE + 5;
}

byte EditorInteraction::Command::getCommandID() const
{
	return commandID;
}

EditorInteraction::CommandType EditorInteraction::Command::getCommandType() const
{
	return static_cast<CommandType>(command);
}

EditorInteraction::LocationType EditorInteraction::Command::getLocationType() const
{
	return static_cast<LocationType>(locationType);
}

Short EditorInteraction::Command::getLocationUID() const
{
	return locationUID;
}

const std::array<byte, PAYLOAD_SIZE>& EditorInteraction::Command::getPayload() const
{
	return payload;
}

bool EditorInteraction::Command::isValid() const
{
	return command != 0;
}

void EditorInteraction::Command::setResponseType(const CommandType& type)
{
	this->command = static_cast<byte>(type);
}

void EditorInteraction::Command::setLocationType(const LocationType& type)
{
	this->locationType = static_cast<byte>(type);
}

void EditorInteraction::Command::setLocationUID(Short uid)
{
	this->locationUID = uid;
}

void EditorInteraction::Command::setPayload(const std::array<byte, PAYLOAD_SIZE>& payload)
{
	this->payload = payload;
}
