/*
 * Tencent is pleased to support the open source community by making
 * Hippy available.
 *
 * Copyright (C) 2017-2019 THL A29 Limited, a Tencent company.
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <functional>
#include <vector>

namespace hippy {
namespace devtools {
constexpr uint32_t kTunnelBufferSize = 32768;
/*
 *  header
 *  ---------------------------------------------------
 *   Flag  | Length  |      Frame Body    |
 *  ---------------------------------------------------
 *   8bit    32bit
 *
 */
struct Header {
  uint8_t flag;
  uint8_t body_length[4];

  int bodySize() {
    uint32_t value = body_length[3] | (body_length[2] << 8) | (body_length[1] << 16) | (body_length[0] << 24);
    return value;
  }
};

class StreamHandler {
 public:
  StreamHandler() { stream_buffer_.reserve(kTunnelBufferSize); }
  ~StreamHandler() { stream_buffer_.clear(); }
  void HandleSendStream(void *data, int32_t len, int32_t flag);
  void HandleReceiveStream(void *data, int32_t len);
  std::function<void(void *, int32_t)> send_stream_callback_;
  std::function<void(void *, int32_t, int32_t)> receive_stream_callback_;

 private:
  std::vector<char> stream_buffer_ = std::vector<char>();
};
}  // namespace devtools
}  // namespace hippy
