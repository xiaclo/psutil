/*
 * Copyright (c) 2009, Giampaolo Rodola'. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include <Python.h>
#include <windows.h>
#include <pdh.h>

#include "../../_psutil_common.h"


// We use an exponentially weighted moving average, just like Unix systems do
// https://en.wikipedia.org/wiki/Load_(computing)#Unix-style_load_calculation
// This constant serves as the damping factor.
#define LOADAVG_FACTOR_1F 0.9200444146293232478931553241
// The time interval in seconds between taking load counts
#define SAMPLING_INTERVAL 5


double load_1m = 0;


VOID CALLBACK LoadCallback(PVOID hCounter) {
    PDH_FMT_COUNTERVALUE displayValue;
    double currentLoad = displayValue.doubleValue;
    double newLoad;

    PdhGetFormattedCounterValue(
        (PDH_HCOUNTER)hCounter, PDH_FMT_DOUBLE, 0, &displayValue);

    newLoad = load_1m * LOADAVG_FACTOR_1F + currentLoad * \
        (1.0 - LOADAVG_FACTOR_1F);
    load_1m = newLoad;
}


PyObject *
psutil_init_loadcounter(PyObject *self, PyObject *args) {
    WCHAR *szCounterPath = L"\\System\\Processor Queue Length";
    PDH_STATUS s;
    BOOL ret;
    HQUERY hQuery;
    HCOUNTER hCounter;
    HANDLE event;
    HANDLE waitHandle;

    if ((PdhOpenQueryW(NULL, 0, &hQuery)) != ERROR_SUCCESS)
        goto error;

    s = PdhAddEnglishCounterW(hQuery, szCounterPath, 0, &hCounter);
    if (s != ERROR_SUCCESS)
        goto error;

    event = CreateEventW(NULL, FALSE, FALSE, L"LoadUpdateEvent");
    if (event == NULL) {
        PyErr_SetFromWindowsErr(GetLastError());
        return NULL;
    }

    s = PdhCollectQueryDataEx(hQuery, SAMPLING_INTERVAL, event);
    if (s != ERROR_SUCCESS)
        goto error;

    ret = RegisterWaitForSingleObject(
        &waitHandle,
        event,
        (WAITORTIMERCALLBACK)LoadCallback,
        (PVOID)
        hCounter,
        INFINITE,
        WT_EXECUTEDEFAULT);

    if (ret == 0) {
        PyErr_SetFromWindowsErr(GetLastError());
        return NULL;
    }

    Py_RETURN_NONE;

error:
    PyErr_SetExcFromWindowsErr(PyExc_OSError, 0);
    return NULL;
}


/*
 * Gets the 1 minute load average (processor queue length) for the system.
 * InitializeLoadCounter must be called before this function to engage the
 * mechanism that records load values.
 */
PyObject *
psutil_get_loadcounter(PyObject *self, PyObject *args) {
    PyObject* load = PyFloat_FromDouble(load_1m);
    return load;
}
