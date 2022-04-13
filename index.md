# Oneiro Engine — это мощный, быстрый и главное бесплатный движок для визуальных новелл, созданный разработчиками для разработчиков.

[Main github repository](https://github.com/OneiroGames/Oneiro)

[Trello](https://trello.com/b/jfd8Urru/oneiro-engine)

### Пример:
```cpp
#include "Oneiro/Runtime/Application.hpp"
#include "Oneiro/Core/Logger.hpp"

class SandBoxApp final : public oe::Runtime::Application
{
public:
    bool Init() override
    {
        oe::Logger::Get("log")->PrintMessage("Initializing...");
        return true;
    }

    bool Update() override
    {
        // Update logic
        return true;
    }

    void Shutdown() override
    {
        oe::Logger::Get("log")->PrintMessage("Closing...");
    }

    void HandleKey(oe::Input::Key key, oe::Input::Action action) override
    {
        using namespace oe;
        if (action == Input::Action::PRESS)
        {
            switch (key)
            {
            case Input::Key::ESC:
                Stop(); break;
            default: break;
            }
        }
    }

    void HandleButton(oe::Input::Button button, oe::Input::Action action) override
    {
        using namespace oe;
        if (action == Input::Action::PRESS)
        {
            switch (button)
            {
            case Input::Button::LEFT:
                Logger::Get("log")->PrintMessage("Press left button!");
                break;
            default:
                break;
            }
        }
    }
};

namespace oe::Runtime
{
    std::shared_ptr<Application> CreateApplication(int argc, char* argv[])
    {
        return std::make_shared<SandBoxApp>();
    }
}
```
