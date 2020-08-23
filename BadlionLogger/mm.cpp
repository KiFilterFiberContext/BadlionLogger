#include "mm.hpp"
#include <intrin.h>

namespace utils
{
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

	// ehh
	void mm::unsafe_write_memory( UINT64* addr, PVOID hook )
	{
		const auto aff = KeSetSystemAffinityThreadEx( 0 );

		auto cr0 = __readcr0( );
		cr0 &= 0xfffffffffffeffff;

		__writecr0( cr0 );
		_disable( );
		
		*addr = reinterpret_cast< UINT64 >( hook );

		cr0 |= 0x10000;

		_enable( );
		__writecr0( cr0 );

		KeSetSystemAffinityThreadEx( aff );
	}
}