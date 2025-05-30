#include "CreatureObject.h"

class Human : public CreatureObject
{
    public:
    Human(ObjectContext context): CreatureObject(context) { kind = "human"; }

    bool Do(Ability* ability) override
    {
        if (Talk* talkData = dynamic_cast<Talk*>(ability))
        {
            return true;
        }
        return false;
    }
    private:
};
