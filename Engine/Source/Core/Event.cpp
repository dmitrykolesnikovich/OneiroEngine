//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Event.hpp"

namespace oe::Core::Event
{
    EventMap Dispatcher::mEventMap;

    Base::~Base() = default;

    void Dispatcher::Post(const Base& event)
    {
        const auto& range = mEventMap.equal_range(&typeid(event));
        for (auto it = range.first; it != range.second; ++it)
            it->second(event);
    }

    FrameBufferSizeEvent::FrameBufferSizeEvent(int width, int height) : Width(width), Height(height) {}

    KeyInputEvent::KeyInputEvent(int key, int action) : Key(key), Action(action) {}

    MouseButtonEvent::MouseButtonEvent(int button, int action) : Button(button), Action(action) {}

    ErrorEvent::ErrorEvent(int error, const char* description) : Description(description), Error(error) {}

    FocusEvent::FocusEvent(bool isFocused) : IsFocused(isFocused) {}

    CursorPosEvent::CursorPosEvent(double xPos, double yPos) : XPos(xPos), YPos(yPos) {}
} // namespace oe::Core::Event
