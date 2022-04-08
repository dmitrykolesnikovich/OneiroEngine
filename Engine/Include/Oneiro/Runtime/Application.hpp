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
    class OE_API Application
    {
    public:
        virtual ~Application();
        virtual bool Init();
        virtual bool Update();
        virtual void Shutdown();
        virtual void HandleKey(Input::Key key, Input::Action action);
        virtual void HandleButton(Input::Button button, Input::Action action);
        bool IsStopped() const { return mIsStopped; }
    protected:
        void Stop() { mIsStopped = true; }
        static void SetKeyInput(bool isEnabled = true);
        static void SetButtonInput(bool isEnabled = true);
    private:
        bool mIsStopped{};        
    };

    OE_API std::shared_ptr<Application> CreateApplication(int argc, char* argv[]);
}
