#pragma once
#include "../Utils/General.h"

namespace EditorInteraction {
    enum class LocationType : byte;
	enum class ResponseType : byte {
		ERROR, Success, Fail
	};
	class Response
	{
    private:
        byte responseID;
        byte responseType;
        byte locationType;
        short locationUID;
        std::array<byte, PAYLOAD_SIZE> payload;
    public:
        Response();
        Response(char* data);
        void cleanUp();
        static size_t size();

        // getters
        ResponseType getCommandType() const;
        LocationType getLocationType() const;
        Short getLocationUID() const;
        bool isValid() const;
        std::vector<char> getData() const;

        // setters
        void setResponseID(const byte& id);
        void setResponseType(const ResponseType& type);
        void setLocationType(const LocationType& type);
        void setLocationUID(Short uid);
	};
}

