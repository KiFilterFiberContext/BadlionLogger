#include "mm.hpp"
#include <intrin.h>

namespace utils
{
	// seems to not work on some occassions so the other solution is temporary
	bool mm::safer_write_memory( const PVOID address, const PVOID value, const size_t len )
	{
		if ( !MmIsAddressValid( address ) )
			return false;

		PMDL mdl = IoAllocateMdl( address, len, FALSE, FALSE, nullptr );
		if ( nullptr == mdl )
			return false;

		MmProbeAndLockPages( mdl, KernelMode, IoReadAccess );
		const auto page = MmMapLockedPagesSpecifyCache( mdl, KernelMode, MmNonCached, nullptr, FALSE, NormalPagePriority );

		const auto status = MmProtectMdlSystemAddress( mdl, PAGE_EXECUTE_READWRITE );
		if ( NT_SUCCESS( status ) ) 
			RtlCopyMemory( page, value, len );

		MmUnmapLockedPages( page, mdl );
		MmUnlockPages( mdl );

		IoFreeMdl( mdl );
		return true;
	}

	void mm::unsafe_disable_protection( )
	{
		auto cr0 = __readcr0( );

		cr0 &= 0xfffffffffffeffff;

		__writecr0( cr0 );
		_disable( );
	}

	void mm::unsafe_enable_protection( )
	{
		auto cr0 = __readcr0( );

		cr0 |= 0x10000;

		_enable( );
		__writecr0( cr0 );
	}
}