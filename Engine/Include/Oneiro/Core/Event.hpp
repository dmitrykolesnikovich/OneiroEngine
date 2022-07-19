//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <functional>
#include <map>

namespace oe::Core::Event
{
    struct Base
    {
        virtual ~Base();
    };

    using EventMap = std::multimap<const std::type_info*, const std::function<void(const Base&)>>;

    class Dispatcher
    {
      public:
        template <typename EventWanted> static void Subscribe(const std::function<void(const Base&)>& fn)
        {
            mEventMap.emplace(&typeid(EventWanted), fn);
        }

        static void Post(const Base& event);

      private:
        static EventMap mEventMap;
    };

    struct FrameBufferSizeEvent : Base
    {
        FrameBufferSizeEvent(int width, int height);

        int Width{}, Height{};
    };

    struct KeyInputEvent : Base
    {
        KeyInputEvent(int key, int action);

        int Key{}, Action{};
    };

    struct MouseButtonEvent : Base
    {
        MouseButtonEvent(int button, int action);

        int Button{}, Action{};
    };

    struct ErrorEvent : Base
    {
        ErrorEvent(int error, const char* description);

        const char* Description{};
        int Error{};
    };

    struct FocusEvent : Base
    {
        FocusEvent(bool isFocused);

        bool IsFocused{};
    };

    struct CursorPosEvent : Base
    {
        CursorPosEvent(double xPos, double yPos);

        double XPos{}, YPos{};
    };
} // namespace oe::Core::Event
