///////////////////////////////////////////////////////////////////////////////
///
/// Copyright (c) 2010 - <company name here>
///
/// Original filename: GateTest.h
/// Project          : GateTest
/// Date of creation : <see GateTest.cpp>
/// Author(s)        : <see GateTest.cpp>
///
/// Purpose          : <see GateTest.cpp>
///
/// Revisions:         <see GateTest.cpp>
///
///////////////////////////////////////////////////////////////////////////////

// $Id$

#ifndef __GATETEST_H_VERSION__
#define __GATETEST_H_VERSION__ 100

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif


#include "drvcommon.h"
#include "drvversion.h"

#define DEVICE_NAME			"\\Device\\GATETEST_DeviceName"
#define SYMLINK_NAME		"\\DosDevices\\GATETEST_DeviceName"
PRESET_UNICODE_STRING(usDeviceName, DEVICE_NAME);
PRESET_UNICODE_STRING(usSymlinkName, SYMLINK_NAME);

#ifndef FILE_DEVICE_GATETEST
#define FILE_DEVICE_GATETEST 0x800
#endif

// Values defined for "Method"
// METHOD_BUFFERED
// METHOD_IN_DIRECT
// METHOD_OUT_DIRECT
// METHOD_NEITHER
// 
// Values defined for "Access"
// FILE_ANY_ACCESS
// FILE_READ_ACCESS
// FILE_WRITE_ACCESS

const ULONG IOCTL_GATETEST_OPERATION = CTL_CODE(FILE_DEVICE_GATETEST, 0x01, METHOD_NEITHER, FILE_READ_DATA | FILE_WRITE_DATA);

#endif // __GATETEST_H_VERSION__
