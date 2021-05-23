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
        byte response;
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

        // setters
        void setCommandType(const ResponseType& type);
        void setLocationType(const LocationType& type);
        void setLocationUID(Short uid);
	};
}

