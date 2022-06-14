//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include <Oneiro/Core/ResourceManager.hpp>
#include <iostream>
#include "Oneiro/Renderer/OpenGL/Sprite2D.hpp"
#include "Oneiro/Core/Window.hpp"
#include "Oneiro/Renderer/Renderer.hpp"
#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Runtime/Engine.hpp"
#include "Oneiro/Core/Logger.hpp"

namespace oe::Renderer::GL
{
	void Sprite2D::Init(const std::string& path, bool keepAspectRatio)
	{
		mKeepAR = keepAspectRatio;
		constexpr auto vertexShaderSrc = R"(
                #version 330 core
                layout (location = 0) in vec3 aPos;
                uniform mat4 uView;
                uniform mat4 uProjection;
                uniform mat4 uModel;
                out vec2 TexCoords;
                uniform float uAR;
                uniform bool uKeepAspectRatio;
                void main()
                {
                    vec2 scale = uKeepAspectRatio ? vec2(uAR > 1 ? 1 / uAR : 1, uAR < 1 ? uAR : 1) : vec2(1.0);
                    TexCoords = aPos.xy;
                    gl_Position = uProjection * uView * uModel * vec4(aPos.xy * scale, 0.0, 1.0);
                }
            )";

		constexpr auto fragmentShaderSrc = R"(
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

		mShader.LoadShaderSrc<gl::VERTEX_SHADER>(vertexShaderSrc);
		mShader.LoadShaderSrc<gl::FRAGMENT_SHADER>(fragmentShaderSrc);
		mShader.CreateProgram();

		constexpr float vertices[] = {1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 0.0f};

		mVAO.Generate();
		mVBO.Generate();
		mVAO.Bind();
		mVBO.Bind();
		mVBO.BufferData(sizeof(vertices), vertices);
		VertexAttribPointer<float>(0, 3, 3);
		mVAO.UnBind();
		mVBO.UnBind();
		mTexture = Core::GetTextureManager().Add(std::make_shared<Texture<gl::TEXTURE_2D>>(path));
		mTextureData = mTexture->GetData();
		//        if (!Load2DTexture(path, &mTexture, &mTextureData))
		//            log::get("log")->warn("Failed to load texture from " + path + " path!");
	}

	bool Sprite2D::Load()
	{
		//        if (!mTexture->IsLoaded())
		//        {
		//            mTexture->Load();
		//            return true;
		//        }
		return false;
	}

	bool Sprite2D::UnLoad()
	{
		//        if (mTexture->IsLoaded())
		//        {
		//            mTexture->UnLoad();
		//            return true;
		//        }
		return false;
	}

	void Sprite2D::Draw()
	{
		mShader.Use();

		if (mKeepAR)
			mShader.SetUniform("uAR", Core::Root::GetWindow()->GetAr() /
			                   (static_cast<float>(mTextureData->Width) / mTextureData->Height));

		mShader.SetUniform("uTextureAlpha", mAlpha);
		mShader.SetUniform("uModel", mModel);
		mShader.SetUniform("uUseTextureAlpha", mUseTextureAlpha);
		mShader.SetUniform("uKeepAspectRatio", mKeepAR);
		mShader.SetUniform("uProjection", glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f));
		mShader.SetUniform("uView", glm::mat4(1.0f));

		mVAO.Bind();
		mTexture->Bind();
		DrawArrays(TRIANGLES, 6);
	}

	void Sprite2D::Move(const glm::vec3& pos)
	{
		mModel = translate(mModel, pos);
	}

	void Sprite2D::Scale(const glm::vec3& scale)
	{
		mModel = glm::scale(mModel, scale);
	}

	void Sprite2D::SetAlpha(float alpha) { mAlpha = alpha; }

	void Sprite2D::SetUsingTextureAlpha(bool useTextureAlpha)
	{
		mUseTextureAlpha = useTextureAlpha;
	}

	const Texture<gl::TEXTURE_2D>* Sprite2D::GetTexture() const { return mTexture; }

	float Sprite2D::GetAlpha() const { return mAlpha; }
}
