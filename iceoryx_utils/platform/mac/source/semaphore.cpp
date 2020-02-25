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

#include "iceoryx_utils/platform/semaphore.hpp"

int iox_sem_getvalue(iox_sem_t* sem, int* sval)
{
}

int iox_sem_post(iox_sem_t* sem)
{
}

int iox_sem_wait(iox_sem_t* sem)
{
}

int iox_sem_trywait(iox_sem_t* sem)
{
}

int iox_sem_timedwait(iox_sem_t* sem, const struct timespec* abs_timeout)
{
}

int iox_sem_close(iox_sem_t* sem)
{
}

int iox_sem_destroy(iox_sem_t* sem)
{
}

int iox_sem_init(iox_sem_t* sem, int pshared, unsigned int value)
{
}

int iox_sem_unlink(const char* name)
{
}

iox_sem_t* iox_sem_open_impl(const char* name, int oflag, ...)
{
}

