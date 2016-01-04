/*
 *  Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Contact: Pawel Kubik (p.kubik@samsung.com)
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License
 */

/**
 * @file
 * @author  Pawel Kubik (p.kubik@samsung.com)
 * @brief   Check whether type is accepted by streaming operators
 */

#ifndef CARGO_INTERNALS_IS_STREAMABLE_HPP
#define CARGO_INTERNALS_IS_STREAMABLE_HPP

#include <sstream>

namespace cargo {
namespace internals {

template<typename T>
struct isStreamableIn {
    static std::istream stream;

    template <typename C> static std::true_type
    test(decltype(stream >> (*(static_cast<C*>(nullptr)))));

    template <typename C> static std::false_type
    test(...);

    static constexpr bool value = std::is_same<decltype(test<T>(stream)), std::true_type>::value;
};

template<typename T>
struct isStreamableOut {
    static std::ostream stream;

    template <typename C> static std::true_type
    test(decltype(stream << (*(static_cast<C*>(nullptr)))));

    template <typename C> static std::false_type
    test(...);

    static constexpr bool value = std::is_same<decltype(test<T>(stream)), std::true_type>::value;
};

} // internals
} // namespace cargo

#endif // CARGO_INTERNALS_IS_STREAMABLE_HPP

