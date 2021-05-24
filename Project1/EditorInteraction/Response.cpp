#include "Response.h"

EditorInteraction::Response::Response() : responseID(0), responseType(0), locationType(0), locationUID(0), payload({})
{
}

EditorInteraction::Response::Response(char* data) : Response()
{
	responseID = data[0];
	responseType = data[1];
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
	return static_cast<ResponseType>(responseType);
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
	return responseType != 0;
}

std::vector<char> EditorInteraction::Response::getData() const
{
	std::vector<char> res;
	res.reserve(size()); 
	Utils::addToByteArray(res, responseID);
	Utils::addToByteArray(res, responseType);
	Utils::addToByteArray(res, locationType);
	Utils::addToByteArray(res, locationUID);
	res.insert(res.begin() + 5, payload.begin(), payload.end());
	return res;
}

void EditorInteraction::Response::setResponseID(const byte& id)
{
	responseID = id;
}

void EditorInteraction::Response::setResponseType(const ResponseType& type)
{
	this->responseType = static_cast<byte>(type);
}

void EditorInteraction::Response::setLocationType(const LocationType& type)
{
	this->locationType = static_cast<byte>(type);
}

void EditorInteraction::Response::setLocationUID(Short uid)
{
	this->locationUID = uid;
}