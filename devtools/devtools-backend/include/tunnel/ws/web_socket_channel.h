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

#include <string>
#include <vector>
#include "tunnel/net_channel.h"

#define ASIO_STANDALONE
#include "websocketpp/client.hpp"
#include "websocketpp/config/asio_client.hpp"

using WSClient = websocketpp::client<websocketpp::config::asio_client>;
using WSMessagePtr = websocketpp::config::asio_tls_client::message_type::ptr;
using WSThread = websocketpp::lib::shared_ptr<websocketpp::lib::thread>;

namespace hippy::devtools {
/**
 * 连接通道的webSocket实现
 */
class WebSocketChannel : public hippy::devtools::NetChannel {
 public:
  explicit WebSocketChannel(const std::string& ws_uri);
  void Connect(ReceiveDataHandler handler) override;
  void Send(const std::string& rsp_data) override;
  void Close(uint32_t code, const std::string& reason) override;

 private:
  void StartConnect(const std::string& ws_uri);
  void HandleSocketInit(const websocketpp::connection_hdl& handle);
  void HandleSocketConnectFail(const websocketpp::connection_hdl& handle);
  void HandleSocketConnectOpen(const websocketpp::connection_hdl& handle);
  void HandleSocketConnectMessage(const websocketpp::connection_hdl& handle, const WSMessagePtr& message_ptr);
  void HandleSocketConnectClose(const websocketpp::connection_hdl& handle);

  WSClient ws_client_;
  websocketpp::connection_hdl connection_hdl_;
  std::string ws_uri_;
  ReceiveDataHandler data_handler_;
  WSThread ws_thread_;
  std::vector<std::string> unset_messages_{};
};
}  // namespace devtools::devtools
