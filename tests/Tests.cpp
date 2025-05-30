#include <string>
#include <iostream>
#include "Tests.h"
#include "Goblin.h"
#include "World.h"
#include "../src/Events.h"


// Create tests that derive from Test like this
class TestEvents : public Test
{
  public:
    virtual TestResult Run() override
    {
        std::optional<std::string> message;
        EventStream<> eventStream;

        bool valid = true;
        valid &= !eventStream.Curr();
        valid &= !eventStream.Prev();
        valid &= !eventStream.OnStart();
        valid &= !eventStream.OnEnd();
        if (!valid)
            message = "failed after initialization";

        eventStream.SetCurr(true);
        valid &= eventStream.Curr();
        valid &= !eventStream.Prev();
        valid &= eventStream.OnStart();
        valid &= !eventStream.OnEnd();
        if (!valid)
            message = "failed at EventStream OnStart subtest";

        eventStream.UpdatePrev();

        valid &= eventStream.Curr();
        valid &= eventStream.Prev();
        valid &= !eventStream.OnStart();
        valid &= !eventStream.OnEnd();
        if (!valid)
            message = "failed at EventStream During subtest";

        eventStream.SetCurr(false);

        valid &= !eventStream.Curr();
        valid &= eventStream.Prev();
        valid &= !eventStream.OnStart();
        valid &= eventStream.OnEnd();
        if (!valid)
            message = "failed at EventStream OnEnd subtest";

        bool failed = !valid;

        return TestResult{failed, message};
    }
};

void Test::UnloadTests()
{
    for (auto& [id, test] : tests)
        delete test;
}

int Test::RunTests(std::vector<std::string> tests)
{
    for (auto& testId : tests)
    {
        if (Test::tests.contains(testId))
        {
            TestResult res = Test::tests[testId]->Run();
            if (res.failed)
                std::cout << "\033[31mTest '" << testId << "' failed!\n\033[0m";
            else
                std::cout << "\033[32mTest '" << testId << "' succeeded\n\033[0m";

            if (res.message)
                std::cout << testId << ": " << *res.message << '\n';
        }
        else
        {
            std::cout << std::string("Unknown test '") + testId + "'. Is it added to Test::tests?\n";
            return 1;
        }
    }

    return 0;
    };

int main(int argc, const char** argv)
{
    std::vector<std::string> tests = {};
    if (argc > 1)
    {
        if (std::string(argv[1]) == "all")
            for (auto& [id, val] : Test::tests)
                tests.push_back(id);
        else
            for (int i = 1; i < argc; i++)
                tests.push_back(argv[i]);
        int status = Test::RunTests(tests);
        if (status != 0)
            std::cout << "\033[31mTesting failed!\n\033[0m";
        else
            std::cout << "Testing finished\n";

        Test::UnloadTests();
        return status;
    }

    std::cout << "No test ID provided.\n";
    return 1;
};

class GoblinShoutSetupTest : public Test
{
  public:
    TestResult Run() override
    {
        ObjectContext context;
        context.messageLog = new MessageLog();
        context.world = new World(context.messageLog);

        Goblin goblin(context);
        auto* shout = dynamic_cast<Shout*>(goblin.GetAbilities()["shout"]);

        if (!shout)
            return {true, "Shout ability not initialized"};

        if (shout->damage.type != PHYSICAL)
            return {true, "Shout type is not PHYSICAL"};

        if (shout->radius != 2.5f)
            return {true, "Shout radius incorrect"};

        if (shout->damage.value != 4 && !goblin.GetStats().contains("strength"))
            return {true, "Shout damage incorrect"};

        return {false, "GoblinShoutSetupTest passed."};
    }
};

std::map<std::string, Test*> Test::tests = {
    {"events", new TestEvents()},
    {"goblin_shout_setup", new GoblinShoutSetupTest()},
};
