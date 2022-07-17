//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/World/World.hpp"
#include "Oneiro/Core/Random.hpp"
#include "Oneiro/World/Entity.hpp"

#include "yaml-cpp/yaml.h"

#include <filesystem>
#include <fstream>

// ReSharper disable CppInconsistentNaming
namespace YAML
{
    template <> struct convert<glm::vec3>
    {
        static Node encode(const glm::vec3& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template <> struct convert<glm::vec2>
    {
        static Node encode(const glm::vec2& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec2& rhs)
        {
            if (!node.IsSequence() || node.size() != 2)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };

    template <> struct convert<glm::vec4>
    {
        static Node encode(const glm::vec4& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };
} // namespace YAML

// ReSharper restore CppInconsistentNaming

namespace
{
    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }

    void SaveEntity(YAML::Emitter& out, oe::World::Entity entity)
    {
        out << YAML::BeginMap; // Begin Entity

        out << YAML::Key << "Entity" << YAML::Value << oe::Core::Random::DiceUuid();

        // Entities always have Tag and Transform components
        {
            // Begin TagComponent
            out << YAML::Key << "TagComponent";
            out << YAML::BeginMap;

            auto& tag = entity.GetComponent<oe::TagComponent>().Tag;
            out << YAML::Key << "Tag" << YAML::Value << tag;

            out << YAML::EndMap;
        } // End TagComponent

        {
            // Begin TransformComponent
            out << YAML::Key << "TransformComponent";
            out << YAML::BeginMap;

            const auto& tc = entity.GetComponent<oe::TransformComponent>();
            out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
            out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
            out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

            out << YAML::EndMap;
        } // End TransformComponent

        if (entity.HasComponent<oe::MainCameraComponent>())
        {
            // Begin MainCameraComponent

            out << YAML::Key << "MainCameraComponent";
            out << YAML::BeginMap;

            const auto& mc = entity.GetComponent<oe::MainCameraComponent>();

            out << YAML::Key << "Position" << YAML::Value << mc.Position;
            out << YAML::Key << "Front" << YAML::Value << mc.Front;
            out << YAML::Key << "Up" << YAML::Value << mc.Up;
            out << YAML::Key << "WorldUp" << YAML::Value << mc.WorldUp;
            out << YAML::Key << "Right" << YAML::Value << mc.Right;

            out << YAML::Key << "Yaw" << YAML::Value << mc.Yaw;
            out << YAML::Key << "Pitch" << YAML::Value << mc.Pitch;
            out << YAML::Key << "MovementSpeed" << YAML::Value << mc.MovementSpeed;
            out << YAML::Key << "MouseSensitivity" << YAML::Value << mc.MouseSensitivity;

            out << YAML::Key << "OrthoNear" << YAML::Value << mc.OrthoNear;
            out << YAML::Key << "OrthoFar" << YAML::Value << mc.OrthoFar;

            out << YAML::Key << "PerspectiveNear" << YAML::Value << mc.PerspectiveNear;
            out << YAML::Key << "PerspectiveFar" << YAML::Value << mc.PerspectiveFar;
            out << YAML::Key << "Fov" << YAML::Value << mc.Fov;

            out << YAML::EndMap;
        } // End MainCameraComponent

        if (entity.HasComponent<oe::ModelComponent>())
        {
            // Begin ModelComponent
            out << YAML::Key << "ModelComponent";
            out << YAML::BeginMap;

            const auto& model = entity.GetComponent<oe::ModelComponent>().Model;

            if (model->IsNeed2SaveVertices())
            {
                const auto& vertices = model->GetVertices();
                const size_t verticesCount = vertices.size();
                out << YAML::Key << "Vertices";
                out << YAML::BeginMap;
                for (size_t i{}; i < verticesCount; ++i)
                {
                    const auto& vertex = vertices[i];
                    out << YAML::Key << i;
                    out << YAML::BeginMap;
                    out << YAML::Key << "Color" << vertex.Color;
                    out << YAML::Key << "Position" << vertex.Position;
                    out << YAML::Key << "Normal" << vertex.Normal;
                    out << YAML::Key << "TexCoords" << vertex.TexCoords;
                    out << YAML::EndMap;
                }
                out << YAML::EndMap;
            }
            else
                out << YAML::Key << "Path" << model->GetPath();

            out << YAML::EndMap;
        } // End ModelComponent

        out << YAML::EndMap; // End Entity
    }
} // namespace

namespace oe::World
{
    World::World(const std::string& name, const std::string& path) : mData({name, path})
    {
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
                uniform vec3 uColor;
                void main()
                {
                    vec4 texture = texture(uTexture, TexCoords);
                    if (Color != vec4(0.0) && texture.rgb == vec3(0.0))
                        FragColor = Color;
                    else
                        FragColor = pow(texture, vec4(1.0/2.2));
                }
            )";

        constexpr auto spriteVertShaderSrc = R"(
                #version 330 core
                layout (location = 0) in vec3 aPos;
                uniform mat4 uView;
                uniform mat4 uProjection;
                out vec2 TexCoords;
                uniform float uAR;
                uniform bool uKeepAspectRatio;
                void main()
                {
                    vec2 scale = uKeepAspectRatio ? vec2(uAR > 1 ? 1 / uAR : 1, uAR < 1 ? uAR : 1) : vec2(1.0);
                    TexCoords = aPos.xy;
                    gl_Position = uProjection * uView * vec4(aPos.xy * scale, 0.0, 1.0);
                }
            )";

        constexpr auto spriteFragShaderSrc = R"(
                #version 330 core
                out vec4 FragColor;
                uniform sampler2D uTexture;
                uniform bool uUseTextureAlpha;
                uniform float uTextureAlpha;
                in vec2 TexCoords;
                void main()
                {
                    vec4 Texture = texture2D(uTexture, TexCoords);
                    if (Texture.a < 0.35)
                            discard;
                    if (uTextureAlpha <= Texture.a)
                            Texture.a = uTextureAlpha;
                        FragColor = pow(vec4(Texture.rgba), vec4(1.0/2.2));
                }
            )";

        mMainShader.LoadShaderSrc<gl::VERTEX_SHADER>(vertexShaderSrc);
        mMainShader.LoadShaderSrc<gl::FRAGMENT_SHADER>(fragmentShaderSrc);
        mMainShader.CreateProgram();

        mSprite2DShader.LoadShaderSrc<gl::VERTEX_SHADER>(spriteVertShaderSrc);
        mSprite2DShader.LoadShaderSrc<gl::FRAGMENT_SHADER>(spriteFragShaderSrc);
        mSprite2DShader.CreateProgram();
    }

    World::~World() = default;

    Entity World::CreateEntity(const std::string& name)
    {
        Entity entity = {mRegistry.create(), this};
        entity.AddComponent<TagComponent>(name.empty() ? "Entity" : name);
        entity.AddComponent<TransformComponent>();
        return entity;
    }

    void World::DestroyEntity(Entity entity)
    {
        mRegistry.destroy(entity);
    }

    void World::DestroyAllEntities()
    {
        mRegistry.clear();
    }

    bool World::IsExists(const std::string& path)
    {
        return exists(std::filesystem::path(path + ".oeworld"));
    }

    std::shared_ptr<World> World::Load(const std::string& path)
    {
        // TODO: Add load new scene and clear old

        auto data = YAML::LoadFile(path + ".oeworld");

        if (!data["World"])
            return {};

        const auto& worldName = data["World"].as<std::string>();

        if (worldName.empty())
            return {};

        const auto& entities = data["Entities"];

        auto world = std::make_shared<World>(worldName, path);

        for (auto entity : entities)
        {
            // Entities always have Tag and Transform components
            const auto& name = entity["TagComponent"]["Tag"].as<std::string>();
            auto transformComponent = entity["TransformComponent"];

            auto mainCameraComponent = entity["MainCameraComponent"];
            auto modelComponent = entity["ModelComponent"];

            Entity loadedEntity = world->CreateEntity(name);

            auto& tc = loadedEntity.GetComponent<TransformComponent>();

            tc.Translation = transformComponent["Translation"].as<glm::vec3>();
            tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
            tc.Scale = transformComponent["Scale"].as<glm::vec3>();

            if (mainCameraComponent)
            {
                auto& mainCamera = loadedEntity.AddComponent<MainCameraComponent>();
                mainCamera.Position = mainCameraComponent["Position"].as<glm::vec3>();
                mainCamera.Front = mainCameraComponent["Front"].as<glm::vec3>();
                mainCamera.Up = mainCameraComponent["Up"].as<glm::vec3>();
                mainCamera.WorldUp = mainCameraComponent["WorldUp"].as<glm::vec3>();
                mainCamera.Right = mainCameraComponent["Right"].as<glm::vec3>();
                mainCamera.Yaw = mainCameraComponent["Yaw"].as<float>();
                mainCamera.Pitch = mainCameraComponent["Pitch"].as<float>();
                mainCamera.MovementSpeed = mainCameraComponent["MovementSpeed"].as<float>();
                mainCamera.MouseSensitivity = mainCameraComponent["MouseSensitivity"].as<float>();
                mainCamera.OrthoNear = mainCameraComponent["OrthoNear"].as<float>();
                mainCamera.OrthoFar = mainCameraComponent["OrthoFar"].as<float>();
                mainCamera.PerspectiveNear = mainCameraComponent["PerspectiveNear"].as<float>();
                mainCamera.PerspectiveFar = mainCameraComponent["PerspectiveFar"].as<float>();
                mainCamera.Fov = mainCameraComponent["Fov"].as<float>();
            }

            if (modelComponent)
            {
                auto& model = loadedEntity.AddComponent<ModelComponent>().Model;
                if (modelComponent["Path"].IsDefined())
                    model->Load(modelComponent["Path"].as<std::string>());
                if (modelComponent["Vertices"].IsDefined())
                {
                    std::vector<Renderer::GL::Vertex> vertices;
                    size_t i{};
                    while (true)
                    {
                        const YAML::Node& nd = modelComponent["Vertices"][std::to_string(i)];
                        if (nd.IsDefined())
                        {
                            vertices.push_back({nd["Color"].as<glm::vec4>(), nd["Position"].as<glm::vec3>(), nd["Normal"].as<glm::vec3>(),
                                                nd["TexCoords"].as<glm::vec2>()});
                            i++;
                        }
                        else
                            break;
                    }
                    model->Load(vertices);
                }
            }
        }

        return world;
    }

    bool World::Save(bool reWrite)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;

        out << YAML::Key << "World" << YAML::Value << mData.Name;
        out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

        mRegistry.each([&](const auto entityID) {
            const Entity entity = {entityID, this};
            if (!entity)
                return;

            SaveEntity(out, entity);
        });

        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::ofstream fileOut;

        if (reWrite)
            fileOut.open(mData.Path + ".oeworld", std::ios::ate);
        else
            fileOut.open(mData.Path + ".oeworld");

        if (!fileOut.is_open())
            return false;

        fileOut << out.c_str();
        fileOut.close();

        return true;
    }

    Entity World::GetEntity(const std::string& name)
    {
        for (auto [fst, snd] : mRegistry.storage())
        {
            for (const auto f : snd)
            {
                auto& comp = mRegistry.get<TagComponent>(f);
                if (comp.Tag == name)
                    return {to_entity(mRegistry, comp), this};
            }
        }
        return CreateEntity(name);
    }

    entt::registry& World::GetEntities()
    {
        return mRegistry;
    }

    void World::UpdateEntities()
    {
        auto view = mRegistry.view<const TagComponent, const TransformComponent>();

        MainCameraComponent* mainCamera{};

        for (auto entity : view)
        {
            mainCamera = &*mRegistry.try_get<MainCameraComponent>(entity);
            if (mainCamera)
                break;
        }

        for (auto entity : view)
        {
            // const auto& tagComponent = view.get<const TagComponent>(entity);
            const auto& transformComponent = view.get<const TransformComponent>(entity);
            const auto& modelComponent = mRegistry.try_get<const ModelComponent>(entity);
            const auto& spriteComponent = mRegistry.try_get<const Sprite2DComponent>(entity);

            //            if (modelComponent)
            //            {
            //                mMainShader.Use();
            //                mMainShader.SetUniform("uModel", transformComponent.GetTransform());
            //                mMainShader.SetUniform("uView", mainCamera->GetViewMatrix());
            //                mMainShader.SetUniform("uProjection", mainCamera->GetPerspectiveProjection());
            //                modelComponent->Model->Draw();
            //            }

            if (spriteComponent)
            {
                const auto sprite = spriteComponent->Sprite2D;
                mSprite2DShader.Use();
                mSprite2DShader.SetUniform("uModel", transformComponent.GetTransform());
                mSprite2DShader.SetUniform("uView", glm::mat4(1.0f));
                mSprite2DShader.SetUniform("uProjection", mainCamera->GetOrthoProjection());
                if (sprite->IsKeepAR())
                    mSprite2DShader.SetUniform(
                        "uAR", Core::Root::GetWindow()->GetAr() /
                                   (static_cast<float>(sprite->GetTexture()->GetData()->Width) / sprite->GetTexture()->GetData()->Height));

                mSprite2DShader.SetUniform("uTextureAlpha", sprite->GetAlpha());
                mSprite2DShader.SetUniform("uUseTextureAlpha", sprite->IsUseTextureAlpha());
                mSprite2DShader.SetUniform("uKeepAspectRatio", sprite->IsKeepAR());
                sprite->Draw();
            }
        }
    }
} // namespace oe::World
