//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

namespace oe
{
	struct TagComponent
	{
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(std::string tag) : Tag(std::move(tag)) {}

		std::string Tag{};
	};

	struct TransformComponent
	{
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3 & translation) : Translation(translation) {}

		glm::vec3 Translation{};
		glm::vec3 Rotation{};
		glm::vec3 Scale{1.0f};

		[[nodiscard]] glm::mat4 GetTransform() const
		{
			const glm::mat4 rotation = toMat4(glm::quat(Rotation));
			return translate(glm::mat4(1.0f), Translation)
				* rotation
				* scale(glm::mat4(1.0f), Scale);
		}
	};
}