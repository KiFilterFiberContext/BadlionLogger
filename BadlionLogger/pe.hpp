#pragma once

#include "defs.hpp"

namespace utils
{
	class pe
	{
	private:
		PIMAGE_NT_HEADERS nt_hdrs;
		UINT64 base;

	public:
		pe( const UINT64 base_address );

		const PIMAGE_IMPORT_DESCRIPTOR get_image_desc( ) const;

		static UINT64 get_kernel_image( const STRING& name );
	};
}