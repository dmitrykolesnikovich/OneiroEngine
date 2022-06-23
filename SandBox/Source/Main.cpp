//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include <filesystem>

#include "Oneiro/Runtime/Application.hpp"
#include "Oneiro/Runtime/Engine.hpp"
#include "Oneiro/World/Components.hpp"
#include "Oneiro/World/Entity.hpp"
#include "Oneiro/World/WorldManager.hpp"

namespace SandBox
{
    class Application final : public oe::Runtime::Application
    {
        using oe::Runtime::Application::Application;
    public:
        bool Init() override
        {
            using namespace oe;
                
            constexpr auto vertexShaderSrc = R"(
                #version 330 core
                layout (location = 0) in vec4 aColor;
				layout (location = 1) in vec3 aPos;
				layout (location = 2) in vec2 aNormal;
				layout (location = 3) in vec2 aTexCoords;
                uniform mat4 uView;
                uniform mat4 uProjection;
                uniform mat4 uModel;
				out vec4 Color;
				out vec2 TexCoords;
                void main()
                {
                    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
					TexCoords = aTexCoords;
					Color = aColor;
                }
            )";

            constexpr auto fragmentShaderSrc = R"(
                #version 330 core
                out vec4 FragColor;
				uniform sampler2D uTexture;
				in vec4 Color;
				in vec2 TexCoords;
                void main()
                {
					if (TexCoords.x == 0.0 && TexCoords.y == 0.0)
						FragColor = Color;
					else
						FragColor = pow(texture(uTexture, TexCoords), vec4(1.0/2.2));
                }
            )";

            mShader.LoadShaderSrc<gl::VERTEX_SHADER>(vertexShaderSrc);
            mShader.LoadShaderSrc<gl::FRAGMENT_SHADER>(fragmentShaderSrc);
            mShader.CreateProgram();

            SetMode(Input::CURSOR, Input::CURSOR_DISABLED);

            gl::CullFace(gl::BACK);
            gl::FrontFace(gl::CCW);

            if (exists(std::filesystem::path("main.oeworld")))
            {
                const auto& worldManager = Core::Root::GetWorldManager();
                return worldManager->LoadWorld("main.oeworld");
            }

            const auto& world = Core::Root::GetWorldManager()->GetWorld();
            auto player = world->CreateEntity("Player");
            player.AddComponent<CameraComponent>();
            player.AddComponent<MainCameraComponent>();

            world->CreateEntity("Backpack").AddComponent<ModelComponent>().Model->Load("Assets/Models/backpack/backpack.obj");
            world->CreateEntity("Cube").AddComponent<ModelComponent>().Model->Load("Assets/Models/cube.fbx");

            return true;
        }

        bool Update(float deltaTime) override
        {
            using namespace oe;
            const auto& world = Core::Root::GetWorldManager()->GetWorld();

        	const auto playerEntity = world->GetEntity("Player");
            const auto backpackEntity = world->GetEntity("Backpack");
            const auto cubeEntity = world->GetEntity("Cube");

        	auto& mainCamera = playerEntity.GetComponent<MainCameraComponent>();

        	auto& backpackTransform = backpackEntity.GetComponent<TransformComponent>();
            auto& playerTransform = playerEntity.GetComponent<TransformComponent>();
            auto& cubeTransform = cubeEntity.GetComponent<TransformComponent>();

        	if (IsKey(Input::Action::PRESS, Input::Key::W))
                mainCamera.UpdateForward(deltaTime);
            if (IsKey(Input::Action::PRESS, Input::Key::S))
                mainCamera.UpdateBackward(deltaTime);
            if (IsKey(Input::Action::PRESS, Input::Key::D))
                mainCamera.UpdateRight(deltaTime);
            if (IsKey(Input::Action::PRESS, Input::Key::A))
                mainCamera.UpdateLeft(deltaTime);

            backpackTransform.Rotation += glm::vec3(deltaTime);
            playerTransform.Translation = mainCamera.Position;
            cubeTransform.Translation = glm::vec3(5.5f, 1.0f, 0.5f);
            cubeTransform.Rotation.x = 90.0f;

        	mShader.Use();
            mShader.SetUniform("uModel", backpackTransform.GetTransform());
            mShader.SetUniform("uView", mainCamera.GetViewMatrix());
            mShader.SetUniform("uProjection", playerEntity.GetComponent<CameraComponent>().GetPerspectiveProjection());
            backpackEntity.GetComponent<ModelComponent>().Model->Draw();

            mShader.Use();
            mShader.SetUniform("uModel", cubeTransform.GetTransform());
            Core::Root::GetWorldManager()->GetWorld()->GetEntity("Cube").GetComponent<ModelComponent>().Model->Draw();

            return true;
        }

        void MousePos(double xPos, double yPos) override
        {
            auto& mainCamera = oe::Core::Root::GetWorldManager()->GetWorld()->GetEntity("Player").GetComponent<oe::MainCameraComponent>();
            mainCamera.UpdateMouse(static_cast<float>(xPos), static_cast<float>(yPos));
        }

        void Shutdown() override
        {
            oe::Core::Root::GetWorldManager()->SaveWorld("main.oeworld", "Main");
        }

    private:
        oe::Renderer::GL::Shader mShader{};
    };
}

namespace oe::Runtime
{
    std::shared_ptr<Application> CreateApplication(int, char* [])
    {
        return std::make_shared<SandBox::Application>("SandBox", 1280, 720);
    }
}
