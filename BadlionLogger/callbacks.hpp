#pragma once

#include "mm.hpp"

using namespace utils;

PVOID __stdcall MmGetSystemRoutineAddress_hk( PUNICODE_STRING routine )
{
	const auto ret = MmGetSystemRoutineAddress( routine );
	DbgPrint( "[*] MmGetSystemRoutineAddress( ROUTINE: %wZ ) -> %llx\n\n",
		routine, reinterpret_cast< UINT64 >( _ReturnAddress( ) ) );

	return ret;
}

NTSTATUS __stdcall ZwTerminateProcess_hk( HANDLE h_proc, NTSTATUS exit_status )
{
	DbgPrint( "[*] ZwTerminateProcess( HANDLE: %llx, EXIT_STATUS: %llx ) -> %llx\n\n",
		reinterpret_cast< UINT64 >( h_proc ), exit_status, reinterpret_cast< UINT64 >( _ReturnAddress( ) ) );
	return ZwTerminateProcess( h_proc, exit_status );
}

void __stdcall RtlInitUnicodeString_hk( PUNICODE_STRING dest, PCWSTR src )
{
	RtlInitUnicodeString( dest, src );
	DbgPrint( "[*] RtlInitUnicodeString( DEST: %wZ ) -> %llx\n\n",
		dest, reinterpret_cast< UINT64 >( _ReturnAddress( ) ) );
}

NTSTATUS __stdcall PsLookupProcessByProcessId_hk( HANDLE pid, PEPROCESS* process )
{
	DbgPrint( "[*] PsLookupProcessByProcessId( PID: %u, PEPROCESS: %llx ) -> %llx\n\n",
		reinterpret_cast< UINT64 >( pid ), reinterpret_cast< UINT64 >( process ), reinterpret_cast< UINT64 >( _ReturnAddress( ) ) );
	return PsLookupProcessByProcessId( pid, process );
}

NTSTATUS PsSetCreateProcessNotifyRoutineEx_hk( PCREATE_PROCESS_NOTIFY_ROUTINE_EX callback, BOOLEAN remove )
{
	DbgPrint( "[*] PsSetCreateProcessNotifyRoutineEx( CALLBACK: %llx, REMOVE: %i ) -> %llx\n\n",
		reinterpret_cast< UINT64 >( callback ), remove, reinterpret_cast< UINT64 >( _ReturnAddress( ) ) );
	return PsSetCreateProcessNotifyRoutineEx( callback, remove );
}

NTSTATUS PsSetLoadImageNotifyRoutine_hk( PLOAD_IMAGE_NOTIFY_ROUTINE callback )
{
	DbgPrint( "[*] PsSetLoadImageNotifyRoutine( CALLBACK: %llx ) -> %llx\n\n",
		reinterpret_cast< UINT64 >( callback ), reinterpret_cast< UINT64 >( _ReturnAddress( ) ) );

	return PsSetLoadImageNotifyRoutine( callback );
}

NTSTATUS IoCreateDevice_hk(
	PDRIVER_OBJECT drv_object,
	ULONG des,
	PUNICODE_STRING dev_name,
	ULONG dev_type,
	ULONG dev_chars,
	BOOLEAN exclus,
	PDEVICE_OBJECT* dev_object )
{

	DbgPrint( "[*] IoCreateDevice( DRVOBJ: %llx, EXTENSIONS: %u, NAME: %wZ, TYPE: %u, CHARACTERISTICS: %u, EXCLUSIVE: %u, DEVOBJ: %llx ) -> %llx\n\n",
		reinterpret_cast< UINT64 >( drv_object ),
		des,
		dev_name,
		dev_type,
		dev_chars,
		exclus,
		reinterpret_cast< UINT64 >( dev_object ),
		reinterpret_cast< UINT64 >( _ReturnAddress( ) )
	);

	return IoCreateDevice( drv_object, des, dev_name, dev_type, dev_chars, exclus, dev_object );
}

NTSTATUS ZwOpenProcess_hk( PHANDLE handle, ACCESS_MASK access, POBJECT_ATTRIBUTES attr, PCLIENT_ID pci )
{
	DbgPrint( "[*] ZwOpenProcess( HANDLE: %llx, MASK: %u -- ) -> %llx\n\n",
		reinterpret_cast< UINT64 >( handle ),
		access,
		reinterpret_cast< UINT64 >( _ReturnAddress( ) )
	);
	return ZwOpenProcess( handle, access, attr, pci );
}
