//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#define OE_DLL_EXPORT
#include "Oneiro/Core/Oneiro.hpp"
#include "Oneiro/Core/Input.hpp"

#include <memory>

namespace oe::Runtime
{
    class Application
    {
    public:
        virtual ~Application() = default;
        virtual bool Init() = 0;
        virtual bool Update() = 0;
        virtual void Close() = 0;
        virtual void HandleKey(Input::Key key, Input::Action action) = 0;
        virtual void HandleButton(Input::Button button, Input::Action action) = 0;
        [[nodiscard]] bool IsStopped() const { return mIsStopped; }
    protected:
        void Stop() { mIsStopped = true; }
    private:
        bool mIsStopped{};
    };

    OE_API std::shared_ptr<Application> CreateApplication(int argc, char* argv[]);
}