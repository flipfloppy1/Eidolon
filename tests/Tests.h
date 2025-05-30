#include <map>
#include <vector>
#include <string>
#include <optional>

struct TestResult
{
    bool failed;
    std::optional<std::string> message;
};

class Test
{
  public:
    static std::map<std::string, Test*> tests;
    static int RunTests(std::vector<std::string> tests);
    static void UnloadTests();
    virtual ~Test() = default;
    virtual TestResult Run() = 0;
};
