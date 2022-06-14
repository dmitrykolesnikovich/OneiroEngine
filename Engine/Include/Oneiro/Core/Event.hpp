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
		template <typename EventWanted>
		static void Subscribe(const std::function<void(const Base&)>& fn)
		{
			mEventMap.emplace(&typeid(EventWanted), fn);
		}

		static void Post(const Base& event)
		{
			const auto& range = mEventMap.equal_range(&typeid(event));
			for (auto it = range.first; it != range.second; ++it)
				it->second(event);
		}

	private:
		static EventMap mEventMap;
	};

	struct FrameBufferSizeEvent : Base
	{
		FrameBufferSizeEvent(int width, int height) : Width(width), Height(height)
		{
		}

		int Width{}, Height{};
	};

	struct KeyInputEvent : Base
	{
		KeyInputEvent(int key, int action) : Key(key), Action(action)
		{
		}

		int Key{}, Action{};
	};

	struct MouseButtonEvent : Base
	{
		MouseButtonEvent(int button, int action) : Button(button), Action(action)
		{
		}

		int Button{}, Action{};
	};

	struct ErrorEvent : Base
	{
		ErrorEvent(int error, const char* description) : Description(description), Error(error)
		{
		}

		const char* Description{};
		int Error{};
	};

	struct FocusEvent : Base
	{
		FocusEvent(bool isFocused) : IsFocused(isFocused)
		{
		}

		bool IsFocused{};
	};
}
