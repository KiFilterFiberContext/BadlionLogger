#include "pe.hpp"

namespace utils
{
	pe::pe( const UINT64 base_address ) : base( base_address ), nt_hdrs( nullptr )
	{
		const auto dos_hdr = reinterpret_cast< PIMAGE_DOS_HEADER >( base_address );
		if ( dos_hdr->e_magic != IMAGE_DOS_SIGNATURE )
			return;

		auto nt_hdr = reinterpret_cast< PIMAGE_NT_HEADERS >( base_address + dos_hdr->e_lfanew );
		if ( nt_hdr->Signature != IMAGE_NT_SIGNATURE )
			return;

		nt_hdrs = nt_hdr; // lol
	}

	const PIMAGE_IMPORT_DESCRIPTOR pe::get_image_desc( ) const
	{
		if ( nullptr == nt_hdrs )
			return nullptr;

		const auto import_dir = nt_hdrs->OptionalHeader.DataDirectory[ 1 ];
		if ( import_dir.Size == 0 )
			return nullptr;
		
		return reinterpret_cast< PIMAGE_IMPORT_DESCRIPTOR >( base + static_cast< UINT64 >( import_dir.VirtualAddress ) );
	}

	UINT64 pe::get_kernel_image( const STRING& name )
	{
		if ( nullptr == ZwQuerySystemInformation )
			return NULL;

		UINT64 m_base = 0;
		ULONG rlen = 0;

		ZwQuerySystemInformation( SystemModuleInformation, nullptr, rlen, &rlen );

		auto m_info =
			reinterpret_cast< PSYSTEM_MODULE_INFORMATION >( ExAllocatePoolWithTag( NonPagedPool, rlen, 'oaml' ) );
		if ( nullptr == m_info )
			return NULL;

		const auto status = ZwQuerySystemInformation( SystemModuleInformation, m_info, rlen, &rlen );
		if ( NT_SUCCESS( status ) )
		{
			for ( size_t i = 0; i < m_info->ModulesCount; ++i ) 
			{
				const auto mod = m_info->Modules[ i ];
				
				STRING m_name;
				RtlInitAnsiString( &m_name, reinterpret_cast< PCSZ >( mod.FullPathName + mod.OffsetToFileName ) );

				if ( !RtlCompareString( &name, &m_name, TRUE ) )
				{
					m_base = reinterpret_cast< UINT64 >( mod.ImageBase );
					break;
				}
			}
		}

		ExFreePoolWithTag( m_info, 'oaml' );
		return m_base;
	}
}