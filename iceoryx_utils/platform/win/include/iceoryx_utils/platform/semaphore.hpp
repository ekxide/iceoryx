// Copyright (c) 2020 by Robert Bosch GmbH. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "iceoryx_utils/platform/types.hpp"

#include <cstdlib>
#include <stdio.h>
#include <type_traits>

#define _WINSOCKAPI_
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#define SEM_FAILED 0

using sem_t = PVOID;
static constexpr LONG MAX_SEMAPHORE_VALUE = LONG_MAX;
static constexpr int MAX_SEMAPHORE_NAME_LENGTH = 128;

inline int sem_getvalue(sem_t* sem, int* sval)
{
    return -1;
}

inline int sem_post(sem_t* sem)
{
    int retVal = (ReleaseSemaphore(sem, 1, nullptr) == 0) ? -1 : 0;
    return retVal;
}

inline int sem_wait(sem_t* sem)
{
    int retVal = (WaitForSingleObject(sem, INFINITE) == WAIT_FAILED) ? -1 : 0;
    return retVal;
}

inline int sem_trywait(sem_t* sem)
{
    if (WaitForSingleObject(sem, 0) == WAIT_FAILED)
    {
        return -1;
    }
    return 0;
}

inline int sem_timedwait(sem_t* sem, const struct timespec* abs_timeout)
{
    DWORD timeoutInMilliseconds = static_cast<DWORD>(1);
    int retVal = (WaitForSingleObject(sem, timeoutInMilliseconds) == WAIT_FAILED) ? -1 : 0;

    return retVal;
}

inline int sem_close(sem_t* sem)
{
    int retVal = CloseHandle(sem) ? 0 : -1;
    return retVal;
}

inline int sem_destroy(sem_t* sem)
{
    // semaphores are closed in windows when the last process which is
    // holding a semaphore calls CloseHandle
    return 0;
}

inline int sem_init(sem_t* sem, int pshared, unsigned int value)
{
    *sem = CreateSemaphore(nullptr, static_cast<LONG>(value), MAX_SEMAPHORE_VALUE, nullptr);
    if (sem != nullptr)
        return 0;
    return -1;
}

inline sem_t* sem_open(const char* name, int oflag)
{
    return static_cast<sem_t*>(CreateSemaphore(nullptr, 0, MAX_SEMAPHORE_VALUE, name));
}

inline sem_t* sem_open(const char* name, int oflag, mode_t mode, unsigned int value)
{
    // GetLastError returns ERROR_ALREADY_EXISTS ... when specific O_FLAG is set use
    // this
    return static_cast<sem_t*>(CreateSemaphore(nullptr, value, MAX_SEMAPHORE_VALUE, name));
}

inline int sem_unlink(const char* name)
{
    // semaphores are closed in windows when the last process which is
    // holding a semaphore calls CloseHandle
    return 0;
}
