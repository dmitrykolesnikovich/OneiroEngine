//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Base.hpp"

namespace oe::Renderer::GL
{
	template <int BufferType, int UsageType>
	class Buffer
	{
	public:
		~Buffer()
		{
			gl::DeleteBuffers(1, &mID);
		}

		constexpr void Generate()
		{
			gl::GenBuffers(1, &mID);
		}

		constexpr static void BufferData(int64_t size, const void* data)
		{
			gl::BufferData(BufferType, size, data, UsageType);
		}

		template <class T>
		constexpr static void BufferData(int64_t size, const void* data)
		{
			gl::BufferData(BufferType, size * sizeof(T), data, UsageType);
		}

		constexpr static void BufferSubData(int64_t size, ptrdiff_t offset, const void* data)
		{
			gl::BufferSubData(BufferType, offset, size, data);
		}

		constexpr void Bind() const
		{
			gl::BindBuffer(BufferType, mID);
		}

		constexpr void UnBind() const
		{
			gl::BindBuffer(BufferType, 0);
		}

		uint32_t Get() { return mID; }
	private:
		uint32_t mID{};
	};

	IS_SAME_TEMPLATE(float)
	constexpr void VertexAttribPointer(int index, int size, int stride, int offset = 0)
	{
		gl::EnableVertexAttribArray(index);
		gl::VertexAttribPointer(index, size, gl::FLOAT, gl::FALSE_, static_cast<int>(stride *
			                        sizeof(float)),
		                        offset == 0 ? nullptr : (void*)(offset * sizeof(float)));
	}

	template <class T, class U>
		requires (std::is_same_v<T, float>)
	constexpr void VertexAttribPointer(int index, int size, int stride, int offset = 0)
	{
		gl::EnableVertexAttribArray(index);
		gl::VertexAttribPointer(index, size, gl::FLOAT, gl::FALSE_, static_cast<int>(stride * sizeof(U)),
		                        offset == 0 ? nullptr : (void*)(offset));
	}

	IS_SAME_TEMPLATE(int)
	constexpr void VertexAttribPointer(int index, int size, int stride, int offset = 0)
	{
		gl::EnableVertexAttribArray(index);
		gl::VertexAttribPointer(index, size, gl::INT, gl::FALSE_, static_cast<int>(stride * sizeof(int)),
		                        offset == 0 ? nullptr : (void*)(offset * sizeof(int)));
	}

	IS_SAME_TEMPLATE(char)
	constexpr void VertexAttribPointer(int index, int size, int stride, int offset = 0)
	{
		gl::EnableVertexAttribArray(index);
		gl::VertexAttribPointer(index, size, gl::UNSIGNED_BYTE, gl::TRUE_, static_cast<int>(stride *
			                        sizeof(char)), offset == 0 ? nullptr : (void*)(offset * sizeof(char)));
	}
}
