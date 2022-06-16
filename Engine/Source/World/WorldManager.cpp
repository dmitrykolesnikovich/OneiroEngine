//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/World/WorldManager.hpp"

#include <fstream>

#include "yaml-cpp/yaml.h"

#include "Oneiro/World/Entity.hpp"
#include "Oneiro/Core/Random.hpp"

namespace YAML
{
    template<>
    struct convert<glm::vec3>
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

    template<>
    struct convert<glm::vec4>
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
}

namespace
{
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
        { // Begin TagComponent
            out << YAML::Key << "TagComponent";
            out << YAML::BeginMap;

            auto& tag = entity.GetComponent<oe::TagComponent>().Tag;
            out << YAML::Key << "Tag" << YAML::Value << tag;

            out << YAML::EndMap;
        } // End TagComponent

        { // Begin TransformComponent
            out << YAML::Key << "TransformComponent";
            out << YAML::BeginMap;

            const auto& tc = entity.GetComponent<oe::TransformComponent>();
            out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
            out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
            out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

            out << YAML::EndMap;
        } // End TransformComponent

        if (entity.HasComponent<oe::MainCameraComponent>())
        { // Begin MainCameraComponent

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

            out << YAML::EndMap;
        } // End MainCameraComponent

        if (entity.HasComponent<oe::CameraComponent>())
        { // Begin CameraComponent
            out << YAML::Key << "CameraComponent";
            out << YAML::BeginMap;

            const auto& camera = entity.GetComponent<oe::CameraComponent>();
            out << YAML::Key << "Translation" << YAML::Value << camera.Translation;
            out << YAML::Key << "Up" << YAML::Value << camera.Up;
            out << YAML::Key << "Center" << YAML::Value << camera.Center;

            out << YAML::Key << "Near" << YAML::Value << camera.Near;
            out << YAML::Key << "Far" << YAML::Value << camera.Far;
            out << YAML::Key << "Fov" << YAML::Value << camera.Fov;

            out << YAML::EndMap;
        } // End CameraComponent

        out << YAML::EndMap; // End Entity
    }
}

namespace oe::World
{
    WorldManager::WorldManager()
    {
        if (mInstance || mWorld)
            return;
        mInstance = this;
        mWorld = std::make_unique<World>();
    }

    void WorldManager::SaveWorld(const std::string& filePath, const std::string& worldName, bool reWrite)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;

        out << YAML::Key << "World" << YAML::Value << worldName;
        out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

        mWorld->mRegistry.each([&](const auto entityID)
                               {
                                   const Entity entity = {entityID, mWorld.get()};
                                   if (!entity)
                                       return;

                                   SaveEntity(out, entity);
                               });

        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::ofstream fout;

    	if (reWrite)
            fout.open(filePath, std::ios::ate);
        else
            fout.open(filePath);

        fout << out.c_str();
    }

    bool WorldManager::LoadWorld(const std::string& filePath) const
    {
        // TODO: Add load new scene and clear old

        auto data = YAML::LoadFile(filePath);

        if (!data["World"])
            return false;

        mWorld->mName = data["World"].as<std::string>();
        const auto& entities = data["Entities"];

        for (auto entity : entities)
        {
            // Entities always have Tag and Transform components
            const auto& name = entity["TagComponent"]["Tag"].as<std::string>();
            auto transformComponent = entity["TransformComponent"];
            auto mainCameraComponent = entity["MainCameraComponent"];
            auto cameraComponent = entity["CameraComponent"];

            Entity loadedEntity = mWorld->CreateEntity(name);

            auto& tc = loadedEntity.GetComponent<TransformComponent>();

            tc.Translation = transformComponent["Translation"].as<glm::vec3>();
            tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
            tc.Scale = transformComponent["Scale"].as<glm::vec3>();

            if (cameraComponent)
            {
                auto& camera = loadedEntity.AddComponent<CameraComponent>();

                camera.Translation = cameraComponent["Translation"].as<glm::vec3>();
                camera.Up = cameraComponent["Up"].as<glm::vec3>();
                camera.Center = cameraComponent["Center"].as<glm::vec3>();

                camera.Near = cameraComponent["Near"].as<float>();
                camera.Far = cameraComponent["Far"].as<float>();
                camera.Fov = cameraComponent["Fov"].as<float>();
            }

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
            }
        }

        return true;
    }

    World* WorldManager::GetWorld()
    {
        return mWorld.get();
    }
}
