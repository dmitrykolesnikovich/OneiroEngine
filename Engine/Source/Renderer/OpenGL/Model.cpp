//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/OpenGL/Model.hpp"

namespace oe::Renderer::GL
{
    void Model::Load(const std::string& path)
    {
        mPath = path;
        mMesh = Core::GetMeshesManager().Add(std::make_shared<Mesh>(mVAO, mVBO, mEBO), path);
    }

    void Model::Draw() const
    {
        const auto& textures = mMesh->GetTextures();
        const auto texturesCount = static_cast<uint8_t>(textures.size());
        for (uint8_t i{}; i < texturesCount; ++i)
            textures[i]->Bind(i);

        mVAO.Bind();

        if (mMesh->IsUseEBO())
        {
            mEBO.Bind();
            DrawElements(TRIANGLES, mMesh->GetIndicesCount(), UNSIGNED_INT);
            mEBO.UnBind();
        }
        else
        {
            DrawArrays(TRIANGLES, mMesh->GetVerticesCount());
        }

        mVAO.UnBind();
        mEBO.UnBind();

        gl::ActiveTexture(gl::TEXTURE0);
        gl::BindTexture(gl::TEXTURE_2D, 0);
    }

    std::string Model::GetPath()
    {
        return mPath;
    }
}
