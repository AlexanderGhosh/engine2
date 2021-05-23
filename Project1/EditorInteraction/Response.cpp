#include "Response.h"

EditorInteraction::Response::Response() : responseID(0), response(0), locationType(0), locationUID(0), payload({})
{
}

EditorInteraction::Response::Response(char* data) : Response()
{
	responseID = data[0];
	response = data[1];
	locationType = data[2];
	locationUID = Utils::convert<short>(&data[3]);
	std::memcpy(&payload, &data[4], PAYLOAD_SIZE);
}

void EditorInteraction::Response::cleanUp()
{
}

size_t EditorInteraction::Response::size()
{
	return PAYLOAD_SIZE + 5;
}

EditorInteraction::ResponseType EditorInteraction::Response::getCommandType() const
{
	return static_cast<ResponseType>(response);
}

EditorInteraction::LocationType EditorInteraction::Response::getLocationType() const
{
	return static_cast<LocationType>(locationType);
}

Short EditorInteraction::Response::getLocationUID() const
{
	return locationUID;
}

bool EditorInteraction::Response::isValid() const
{
	return response != 0;
}

void EditorInteraction::Response::setCommandType(const ResponseType& type)
{
	this->response = static_cast<byte>(type);
}

void EditorInteraction::Response::setLocationType(const LocationType& type)
{
	this->locationType = static_cast<byte>(type);
}

void EditorInteraction::Response::setLocationUID(Short uid)
{
	this->locationUID = uid;
}