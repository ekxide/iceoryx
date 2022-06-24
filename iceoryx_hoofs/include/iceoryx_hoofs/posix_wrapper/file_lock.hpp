// Copyright (c) 2021 - 2022 by Apex.AI Inc. All rights reserved.
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
//
// SPDX-License-Identifier: Apache-2.0
#ifndef IOX_HOOFS_POSIX_WRAPPER_FILE_LOCK_HPP
#define IOX_HOOFS_POSIX_WRAPPER_FILE_LOCK_HPP

#include "iceoryx_hoofs/cxx/expected.hpp"
#include "iceoryx_hoofs/cxx/filesystem.hpp"
#include "iceoryx_hoofs/cxx/string.hpp"
#include "iceoryx_hoofs/design_pattern/builder.hpp"
#include "iceoryx_hoofs/platform/file.hpp"

namespace iox
{
namespace posix
{
enum class FileLockError
{
    INVALID_FILE_NAME,
    INVALID_FILE_PATH,
    FILE_PATH_TOO_LONG,
    LOCKED_BY_OTHER_PROCESS,
    ACCESS_DENIED,
    QUOTA_EXHAUSTED,
    SYSTEM_LIMIT,
    PROCESS_LIMIT,
    NO_SUCH_DIRECTORY,
    SPECIAL_FILE,
    FILE_TOO_LARGE,
    FILE_IN_USE,
    OUT_OF_MEMORY,
    I_O_ERROR,
    SYS_CALL_NOT_IMPLEMENTED,
    INTERNAL_LOGIC_ERROR,
};

/// @brief Posix file lock C++ wrapping class
///        Following RAII, the lock is acquired on creation and released on destruction. Releasing the locks works even
///        if the process crashes with a segfault or using SIGKILL. 'lslocks' can be used to display all system-wide
///        locks (see man page)
/// @code
///    iox::posix::FileLock::create(nameOfmyLock)
///        .and_then([] { std::cout << "We aquired the lock!" << std::endl; })
///        .or_else([](auto& error) {
///            if (error == FileLockError::LOCKED_BY_OTHER_PROCESS)
///            {
///                std::cout << "Some other process is running and holds the lock!" << std::endl;
///            }
///        });
/// @endcode
class FileLock
{
  public:
    static constexpr int32_t INVALID_FD = -1;
    static constexpr const char LOCK_FILE_SUFFIX[] = ".lock";
    static constexpr uint64_t FILENAME_LENGTH = platform::IOX_MAX_FILENAME_LENGTH
                                                - sizeof(platform::IOX_LOCK_FILE_PATH_PREFIX) / sizeof(char)
                                                - sizeof(LOCK_FILE_SUFFIX) / sizeof(char);

    using FileName_t = cxx::string<FILENAME_LENGTH>;
    using FilePath_t = cxx::string<platform::IOX_MAX_PATH_LENGTH>;
    using PathName_t = cxx::string<platform::IOX_MAX_PATH_LENGTH - FILENAME_LENGTH - 1>;

    FileLock(const FileLock&) = delete;
    FileLock& operator=(const FileLock&) = delete;
    FileLock(FileLock&& rhs) noexcept;
    FileLock& operator=(FileLock&& rhs) noexcept;

    ~FileLock() noexcept;

  private:
    enum class LockOperation : int32_t
    {
        LOCK = LOCK_EX | LOCK_NB,
        UNLOCK = LOCK_UN
    };

    int32_t m_fd{INVALID_FD};
    FilePath_t m_fileLockPath;

  private:
    friend class FileLockBuilder;
    FileLock(const int32_t fileDescriptor, const FilePath_t& path) noexcept;

    void invalidate() noexcept;

    static FileLockError convertErrnoToFileLockError(const int32_t errnum, const FilePath_t& fileLockPath) noexcept;
    static cxx::expected<FileLockError> closeFileDescriptor(const int32_t fileDescriptor,
                                                            const FilePath_t& fileLockPath) noexcept;
};

class FileLockBuilder
{
    IOX_BUILDER_PARAMETER(FileLock::FileName_t, name, "")

    IOX_BUILDER_PARAMETER(FileLock::PathName_t, path, platform::IOX_LOCK_FILE_PATH_PREFIX)

    IOX_BUILDER_PARAMETER(cxx::perms, permission, cxx::perms::none)

  public:
    cxx::expected<FileLock, FileLockError> create() noexcept;
};
} // namespace posix
} // namespace iox

#endif // IOX_HOOFS_POSIX_WRAPPER_FILE_LOCK_HPP
