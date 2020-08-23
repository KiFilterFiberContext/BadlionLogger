#pragma once

#include "defs.hpp"

namespace utils
{
	class mm
	{
	public:
		static bool safer_write_memory( const PVOID address, const PVOID value, const size_t len );

		static void unsafe_disable_protection( );

		static void unsafe_enable_protection( );
	};
}