#include "mm.hpp"
#include <intrin.h>

namespace utils
{
	void mm::safer_write_memory( const PVOID address, const PVOID value, const size_t len )
	{
		if ( !MmIsAddressValid( address ) )
			return;

		PMDL mdl = IoAllocateMdl( address, len, FALSE, FALSE, nullptr );
		if ( nullptr == mdl )
			return;

		MmProbeAndLockPages( mdl, KernelMode, IoReadAccess );
		const auto page = MmMapLockedPagesSpecifyCache( mdl, KernelMode, MmNonCached, nullptr, FALSE, NormalPagePriority );

		const auto status = MmProtectMdlSystemAddress( mdl, PAGE_EXECUTE_READWRITE );
		if ( NT_SUCCESS( status ) ) 
			RtlCopyMemory( page, value, len );

		MmUnmapLockedPages( page, mdl );
		MmUnlockPages( mdl );

		IoFreeMdl( mdl );
	}

	// it's recommended not to flip CR0's WP bit as it can cause instability because of scheduling but safe_write_memory is for that purpose
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