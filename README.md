<p align="center">
  <img src="https://user-images.githubusercontent.com/74720936/164480358-22f89315-fe38-46ba-a808-ddbcd1f8565c.png" width="350" height="400">
</p>

<h1 align="center">Oneiro Engine — powerful, fast and free engine for visual novels.</h1>

## Why Oneiro?

Oneiro is powerful and fast engine, because it's written in C++ and Vulkan API. For visual novel scripts Oneiro uses lua scripting language, because it's simple and fast language.

**Currently, Oneiro Engine doen't have all features, because it's under development.**

### Example:
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
                log::get("log")->info("Press left button!");
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

#

<h1 align="center">Made with ❤️</h1>
