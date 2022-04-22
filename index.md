# Oneiro Engine — это мощный, быстрый и главное бесплатный движок для визуальных новелл, созданный разработчиками для разработчиков.
<p align="center">
  <img src="https://user-images.githubusercontent.com/74720936/164480358-22f89315-fe38-46ba-a808-ddbcd1f8565c.png" width="350" height="400">
</p>

[Main github repository](https://github.com/OneiroGames/Oneiro)

[Trello](https://trello.com/b/jfd8Urru/oneiro-engine)

[VK](https://vk.com/oneiroengine)

### Пример:
```cpp
#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Runtime/Application.hpp"


class SandBoxApp final : public oe::Runtime::Application
{
public:
    bool Init() override
    {
        oe::log::get("log")->info("Initializing...");
        return true;
    }

    bool Update() override
    {
        // Update logic
        return true;
    }

    void Shutdown() override
    {
        oe::log::get("log")->info("Closing...");
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
                Logger::get("log")->info("Press left button!");
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
