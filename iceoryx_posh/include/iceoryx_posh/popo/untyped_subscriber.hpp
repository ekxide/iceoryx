// Copyright (c) 2020 by Robert Bosch GmbH. All rights reserved.
// Copyright (c) 2020-2021 by Apex.AI Inc. All rights reserved.
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

#ifndef IOX_POSH_POPO_UNTYPED_SUBSCRIBER_HPP
#define IOX_POSH_POPO_UNTYPED_SUBSCRIBER_HPP

#include "iceoryx_posh/capro/service_description.hpp"
#include "iceoryx_posh/iceoryx_posh_types.hpp"
#include "iceoryx_posh/popo/base_subscriber.hpp"
#include "iceoryx_utils/cxx/expected.hpp"
#include "iceoryx_utils/cxx/unique_ptr.hpp"

namespace iox
{
namespace popo
{
class Void
{
};

template <template <typename, typename, typename> class base_subscriber_t = BaseSubscriber,
          typename port_t = iox::SubscriberPortUserType>
class UntypedSubscriberImpl : public base_subscriber_t<void, UntypedSubscriberImpl<base_subscriber_t>, port_t>
{
  public:
    using BaseSubscriber = base_subscriber_t<void, UntypedSubscriberImpl<base_subscriber_t>, port_t>;

    UntypedSubscriberImpl(const capro::ServiceDescription& service,
                          const SubscriberOptions& subscriberOptions = SubscriberOptions());
    UntypedSubscriberImpl(const UntypedSubscriberImpl& other) = delete;
    UntypedSubscriberImpl& operator=(const UntypedSubscriberImpl&) = delete;
    UntypedSubscriberImpl(UntypedSubscriberImpl&& rhs) = delete;
    UntypedSubscriberImpl& operator=(UntypedSubscriberImpl&& rhs) = delete;
    virtual ~UntypedSubscriberImpl() = default;

    ///
    /// @brief Take the chunk from the top of the receive queue.
    /// @return The payload pointer of the chunk taken.
    /// @details No automatic cleanup of the associated chunk is performed.
    ///
    cxx::expected<const void*, ChunkReceiveResult> take() noexcept;

    ///
    /// @brief Releases the chunk provided by the payload pointer.
    /// @param payload pointer to the payload of the chunk to be released
    /// @details The chunk must have been previously provided by take and
    ///          not have been already released.
    ///
    void releaseChunk(const void* payload) noexcept;

  protected:
    using BaseSubscriber::port;
};

using UntypedSubscriber = UntypedSubscriberImpl<>;

} // namespace popo
} // namespace iox

#include "iceoryx_posh/internal/popo/untyped_subscriber.inl"

#endif // IOX_POSH_POPO_UNTYPED_SUBSCRIBER_HPP
