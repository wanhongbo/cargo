/*
 *  Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Contact: Pawel Broda <p.broda@partner.samsung.com>
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
 * @author  Pawel Broda (p.broda@partner.samsung.com)
 * @brief   Null backend for logger
 */

#ifndef LOGGER_BACKEND_NULL_HPP
#define LOGGER_BACKEND_NULL_HPP

#include "logger/backend.hpp"

namespace logger {

/**
 * Null logging backend
 */
class NullLogger : public LogBackend {
public:
    void log(LogLevel            /*logLevel*/,
             const std::string&  /*file*/,
             const unsigned int& /*line*/,
             const std::string&  /*func*/,
             const std::string&  /*message*/) override {}
};

} // namespace logger

#endif // LOGGER_BACKEND_NULL_HPP
