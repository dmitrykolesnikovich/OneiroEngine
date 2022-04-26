//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <fstream>

#include "yaml-cpp/yaml.h"

#include "Entity.hpp"
#include "Oneiro/Core/Random.hpp"

namespace YAML {

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

namespace oe::Scene
{
	inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	static void SaveEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap; // Begin Entity

		out << YAML::Key << "Entity" << YAML::Value << Core::Random::DiceUUID();

		// Entities always have Tag and Transform components

		{ // Begin TagComponent
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap;
		} // End TagComponent

		{ // Begin TransformComponent
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			out << YAML::EndMap; // TransformComponent
		} // End TransformComponent

		out << YAML::EndMap; // End Entity
	}

	class SceneManager
	{
	public:
		SceneManager(Scene* scene) : mScene(scene) {}

		void Save(const std::string& filepath, const std::string& sceneName)
		{
			YAML::Emitter out;
			out << YAML::BeginMap;

			out << YAML::Key << "Scene" << YAML::Value << sceneName;
			out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

			mScene->mRegistry.each([&](const auto entityID)
				{
					const Entity entity = { entityID, mScene };
					if (!entity)
						return;

					SaveEntity(out, entity);
				});

			out << YAML::EndSeq;
			out << YAML::EndMap;

			std::ofstream fout(filepath);
			fout << out.c_str();
		}

		bool Load(const std::string& filePath) const
		{
			// TODO: Add load new scene and clear old
			
			auto data = YAML::LoadFile(filePath);

			if (!data["Scene"])
				return false;

			const std::string& sceneName = data["Scene"].as<std::string>();
			const auto& entities = data["Entities"];

			mScene->mName = sceneName;

			for (auto entity : entities)
			{
				// Entities always have Tag and Transform components
				const auto& name = entity["TagComponent"]["Tag"].as<std::string>();
				auto transformComponent = entity["TransformComponent"];

				Entity loaddedEntity = mScene->CreateEntity(name);

				auto& tc = loaddedEntity.GetComponent<TransformComponent>();

				tc.Translation = transformComponent["Translation"].as<glm::vec3>();
				tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
				tc.Scale = transformComponent["Scale"].as<glm::vec3>();
			}

			return true;
		}
	private:
		Scene* mScene{};
	};
}