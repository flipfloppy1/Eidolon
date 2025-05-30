#pragma once
#include "CreatureObject.h"
#include <string>
#include <variant>
#include <vector>

class MessageLog
{
  public:
    MessageLog() { consumedReader = 0; }

    void Push(std::string message)
    {
        messages.push_back(message);
    }

    void Push(CreatureObject* subject, std::variant<Usage*,Ability*> action, GameObject* object)
    {
        Push(subject, action, object, nullptr);
    }

    void Push(CreatureObject* subject, std::variant<Usage*,Ability*> action, GameObject* object, GameObject* with)
    {
        if (subject->IsPlayer())
            messages.push_back(ProcessPlayerMessage(subject, action, object, with));
        else
            messages.push_back(ProcessCreatureMessage(subject, action, object, with));
    }

    bool NewMessage()
    {
        return consumedReader < messages.size();
    }

    std::string ConsumeMessage()
    {
        if (consumedReader < messages.size())
            return messages[consumedReader++];

        return "";
    }

  private:
    std::string ProcessPlayerMessage(CreatureObject* subject, std::variant<Usage*, Ability*> action, GameObject* object, GameObject* with)
    {
        std::string message;

        if (action.index() == 0)
        {
            Usage* usage = std::get<Usage*>(action);
        }
        else
        {
            Ability* ability = std::get<Ability*>(action);
        }
        return message;
    }
    std::string ProcessCreatureMessage(CreatureObject* subject, std::variant<Usage*, Ability*> action, GameObject* object, GameObject* with)
    {
        std::string message;



        return message;
    }
    std::vector<std::string> messages;
    int consumedReader;
};
