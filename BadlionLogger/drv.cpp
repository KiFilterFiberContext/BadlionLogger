#include "routine.hpp"

PDEVICE_OBJECT dev_object;

void drv_unload( PDRIVER_OBJECT drv_object )
{
	UNREFERENCED_PARAMETER( drv_object );

	DbgPrint( "<< Logger >> Unloading Driver...\n" );
	PsRemoveLoadImageNotifyRoutine( load_image_callback );

	UNICODE_STRING sym_path = RTL_CONSTANT_STRING( L"\\DosDevices\\BadlionLogger" );

	IoDeleteSymbolicLink( &sym_path );
	IoDeleteDevice( dev_object );
}

NTSTATUS generic_mf_dispatch( PDEVICE_OBJECT device_object, PIRP irp )
{
	UNREFERENCED_PARAMETER( device_object );

	irp->IoStatus.Information = 0;
	irp->IoStatus.Status = STATUS_SUCCESS;

	IofCompleteRequest( irp, IO_NO_INCREMENT );
	return STATUS_SUCCESS;
}

NTSTATUS DriverEntry( PDRIVER_OBJECT driver_object, PUNICODE_STRING reg_path )
{
	UNREFERENCED_PARAMETER( reg_path );

	auto status = STATUS_SUCCESS;

	UNICODE_STRING dev_name = RTL_CONSTANT_STRING( L"\\Device\\BadlionLogger" );
	UNICODE_STRING sym_path = RTL_CONSTANT_STRING( L"\\DosDevices\\BadlionLogger" );

	status = IoCreateDevice(
		driver_object,
		NULL,
		&dev_name,
		FILE_DEVICE_UNKNOWN,
		FILE_DEVICE_SECURE_OPEN,
		FALSE,
		&dev_object );
	if ( !NT_SUCCESS( status ) )
		return status;

	status = IoCreateSymbolicLink( &sym_path, &dev_name );
	if ( !NT_SUCCESS(status) )
	{
		IoDeleteDevice( dev_object );
		return status;
	}

	driver_object->DriverUnload = drv_unload;

	driver_object->MajorFunction[ IRP_MJ_CREATE ] = generic_mf_dispatch;
	driver_object->MajorFunction[ IRP_MJ_CLOSE ] = generic_mf_dispatch;

	DbgPrint( "<< Init >> BadlionLogger initialized successfully!\n" );

	PsSetLoadImageNotifyRoutine( load_image_callback );
	return status;
}

