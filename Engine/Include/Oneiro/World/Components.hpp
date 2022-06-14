//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Animation/Animation.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL

#include "glm/gtx/quaternion.hpp"

#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Core/Window.hpp"
#include "Oneiro/Renderer/OpenGL/Sprite2D.hpp"

namespace oe
{
	struct TagComponent
	{
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;

		TagComponent(std::string tag) : Tag(std::move(tag))
		{
		}

		std::string Tag{};
	};

	struct TransformComponent
	{
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;

		TransformComponent(const glm::vec3& translation, const glm::vec3& rotation,
		                   const glm::vec3& scale) : Translation(translation), Rotation(rotation),
		                                             Scale(scale)
		{
		}

		glm::vec3 Translation{};
		glm::vec3 Rotation{};
		glm::vec3 Scale{1.0f};

		[[nodiscard]] glm::mat4 GetTransform() const
		{
			const glm::mat4 rotation = toMat4(glm::quat(Rotation));
			return translate(glm::mat4(1.0f), Translation) * rotation *
				scale(glm::mat4(1.0f), Scale);
		}
	};

	struct Sprite2DComponent
	{
		Sprite2DComponent() = default;
		Sprite2DComponent(const Sprite2DComponent&) = default;

		Sprite2DComponent(Renderer::GL::Sprite2D* sprite2D) : Sprite2D(sprite2D)
		{
		}

		Renderer::GL::Sprite2D* Sprite2D{};
	};

	struct AnimationComponent
	{
		AnimationComponent() = default;
		AnimationComponent(const AnimationComponent&) = default;

		AnimationComponent(Animation::Animation* animation) : Animation(animation)
		{
		}

		Animation::Animation* Animation{};
	};

	struct CameraComponent
	{
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		glm::vec3 Translation{2.0f};
		glm::vec3 Up{};
		glm::vec3 Center{};

		float Near{0.1f};
		float Far{100.0f};
		float Fov{45.0f};

		[[nodiscard]] glm::mat4 GetView() const
		{
			return lookAt(Translation, Center, Up);
		}

		[[nodiscard]] glm::mat4 GetPerspectiveProjection() const
		{
			return glm::perspective(glm::radians(Fov),
			                        static_cast<float>(Core::Root::GetWindow()->GetWidth()) /
			                        static_cast<float>(
				                        Core::Root::GetWindow()->GetHeight()), Near,
			                        Far);
		}

		[[nodiscard]] glm::mat4 GetOrthoProjection() const
		{
			return glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, Near, Far);
		}
	};
}
