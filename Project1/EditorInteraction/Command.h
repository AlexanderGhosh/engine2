#pragma once
#include "../Utils/General.h"

namespace EditorInteraction {
    enum class CommandType : byte
    {
        ERROR,
        Create
    };
    enum class LocationType : byte
    {
        GameObject
    };
    class Command
    {
    private:
        static byte commandID;
        byte command;
        byte locationType;
        short locationUID;
        std::array<byte, PAYLOAD_SIZE> payload;
    public:
        Command();
        Command(char* data);
        void cleanUp();
        static size_t size();

        // getters
        byte getCommandID() const;
        CommandType getCommandType() const;
        LocationType getLocationType() const;
        Short getLocationUID() const;
        const std::array<byte, PAYLOAD_SIZE>& getPayload() const;
        bool isValid() const;

        // setters
        void setCommandType(const CommandType& type);
        void setLocationType(const LocationType& type);
        void setLocationUID(Short uid);
        void setPayload(const std::array<byte, PAYLOAD_SIZE>& payload);
    };
}

