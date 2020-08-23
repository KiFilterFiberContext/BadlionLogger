#pragma once

#include "pe.hpp"
#include "mm.hpp"

#include "callbacks.hpp"
	
using namespace utils;

void load_image_callback( PUNICODE_STRING name, HANDLE pid, PIMAGE_INFO image_info )
{
	UNREFERENCED_PARAMETER( pid );

	if ( !wcsstr( name->Buffer, L"BadlionAnticheat.sys" ) )
		return;

	const auto base = reinterpret_cast< UINT64 >( image_info->ImageBase );
	const pe image( base );
	
	DbgPrint( "<< Init >> Image Callback Triggered: %wZ\nBase => %llx\n\n", name, base );

	auto desc = image.get_image_desc( );
	if ( nullptr == desc )
		return;

	DbgPrint( "<< Init >> Retrieving imports...\n" );
	
	while ( desc->Name != NULL ) 
	{
		STRING module_name;
		RtlInitAnsiString( &module_name, reinterpret_cast< PCSZ >( desc->Name + base ) );

		if ( const auto m_addr = pe::get_kernel_image( module_name ) ) 
		{
			DbgPrint( "\n<< Init >> Import Table Entry: %Z\nBase => %llx\n\n", module_name, m_addr );

			auto ori_first_thunk = reinterpret_cast< PIMAGE_THUNK_DATA64 >( base + desc->OriginalFirstThunk );
			auto first_thunk = reinterpret_cast< PIMAGE_THUNK_DATA64 >( base + desc->FirstThunk );

			while ( ori_first_thunk->u1.AddressOfData != NULL )
			{
				const auto p_fname = reinterpret_cast< PIMAGE_IMPORT_BY_NAME >( base + ori_first_thunk->u1.AddressOfData );
				
				STRING func_name;
				RtlInitAnsiString( &func_name, reinterpret_cast< PCSZ >( p_fname->Name ) );

				// kernel moment
				// having std map would be nice

				if ( !strcmp( func_name.Buffer, "ZwTerminateProcess" ) )
					mm::unsafe_write_memory( &first_thunk->u1.Function, ZwTerminateProcess_hk );

				else if ( !strcmp( func_name.Buffer, "MmGetSystemRoutineAddress" ) )
					mm::unsafe_write_memory( &first_thunk->u1.Function, MmGetSystemRoutineAddress_hk );

				else if ( !strcmp( func_name.Buffer, "PsSetCreateProcessNotifyRoutineEx" ) )
					mm::unsafe_write_memory( &first_thunk->u1.Function, PsSetCreateProcessNotifyRoutineEx_hk );

				else if ( !strcmp( func_name.Buffer, "PsSetLoadImageNotifyRoutine" ) )
					mm::unsafe_write_memory( &first_thunk->u1.Function, PsSetLoadImageNotifyRoutine_hk );

				else if ( !strcmp( func_name.Buffer, "RtlInitUnicodeString" ) )
					mm::unsafe_write_memory( &first_thunk->u1.Function, RtlInitUnicodeString_hk );

				else if ( !strcmp( func_name.Buffer, "PsLookupProcessByProcessId" ) )
					mm::unsafe_write_memory( &first_thunk->u1.Function, PsLookupProcessByProcessId_hk );

				else if ( !strcmp( func_name.Buffer, "IoCreateDevice" ) )
					mm::unsafe_write_memory( &first_thunk->u1.Function, IoCreateDevice_hk );

				else if ( !strcmp( func_name.Buffer, "ZwOpenProcess" ) )
					mm::unsafe_write_memory( &first_thunk->u1.Function, ZwOpenProcess_hk );

				
				++first_thunk;
				++ori_first_thunk;
			}
		}

		desc++;
	}
}