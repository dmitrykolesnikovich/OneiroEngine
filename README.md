<p align="center">
  <img src="https://user-images.githubusercontent.com/74720936/164480358-22f89315-fe38-46ba-a808-ddbcd1f8565c.png" width="350" height="400">
</p>

<h1 align="center">Oneiro Engine — powerful, fast and free engine for visual novels.</h1>

## Why Oneiro?

Oneiro is powerful and fast engine, because it's written in C++ and Vulkan API. For visual novel scripts Oneiro uses lua scripting language, because it's simple and fast language.

**Currently, Oneiro Engine doen't have all features, because it's under development.**

### Example:
CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.21)
project(SandBox)

set(CMAKE_CXX_STANDARD 20)

add_executable(${CMAKE_PROJECT_NAME} Source/Main.cpp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../Engine/ Oneiro)
```
Main.cpp
```cpp
#include "Oneiro/Runtime/Application.hpp"
#include "Oneiro/VisualNovel/VNCore.hpp"
#include "Oneiro/Lua/LuaFile.hpp"

namespace SandBox
{
    class Application : public oe::Runtime::Application
    {
    public:
        bool Init() override
        {
            mFile.LoadFile("Script.lua", false);
            oe::VisualNovel::Init(&mFile);
            return true;
        }

        bool Update() override
        {
            oe::VisualNovel::Update();
            return true;
        }

        void HandleButton(oe::Input::Button button, oe::Input::Action action) override
        {
            if (button == oe::Input::Button::LEFT && action == oe::Input::Action::PRESS)
                oe::VisualNovel::NextStep();
        }

        void Shutdown() override {}
    private:
        oe::Lua::File mFile;
    };
}

namespace oe::Runtime
{
    std::shared_ptr<Application> CreateApplication(int, char* [])
    {
        return std::make_shared<SandBox::Application>();
    }
}
```
Script.lua
```lua
-- creates characters, sprites, and other means
me = Character("Me")
-- second parameter is aspect ratio for sprite
-- for normal resizing sprite to window size you need keep aspect ratio
local sprite = Sprite2D("sprite.png", true)
-- but in background you don't need to enabling aspect ratio
local background = Sprite2D("background.jpg", false)
local music = Audio("music.mp3")

-- you must to set the var name that was passed to the label ctor
-- because for call to label function engine need know name of variable
start = Label("start")
-- entry point in label is main function
-- you can use colon when you call or define function
function start:main()
    background:show()
    sprite:show()
    music:play()
    me "Hello World!"
    music:stop()
    sprite:hide()
    background:hide()

    -- jump to another registered label
    jump "goodbye"
    -- if you need you can call function as default
    -- jump("goodbye")
end

goodbye = Label("goodbye")
function goodbye:main()
    me "Goodbye World!"
end
```

### Example output:
![image](https://user-images.githubusercontent.com/74720936/168714699-10d9a150-e706-4898-a3c3-32a1a5c44c75.png)

[Video](https://youtu.be/unQJ0THmIyg)

#

<h1 align="center">Made with ❤️</h1>
