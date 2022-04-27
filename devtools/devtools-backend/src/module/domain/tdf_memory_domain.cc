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

#include "module/domain/tdf_memory_domain.h"
#include "api/devtools_backend_service.h"
#include "module/domain_register.h"

namespace hippy::devtools {

std::string_view TDFMemoryDomain::GetDomainName() { return kFrontendKeyDomainNameTDFMemory; }

void TDFMemoryDomain::RegisterMethods() { REGISTER_DOMAIN(TDFMemoryDomain, GetHeapMeta, Deserializer); }

void TDFMemoryDomain::GetHeapMeta(const Deserializer& request) {
  auto memory_adapter = GetDataProvider()->memory_adapter;
  if (!memory_adapter) {
    ResponseErrorToFrontend(request.GetId(), kErrorNotSupport, "get heap meta failed, no data.");
    return;
  }
  memory_adapter->CollectMemoryUsage([this, request](const MemoryMetas& memoryMetas) {
    ResponseResultToFrontend(request.GetId(), memoryMetas.Serialize());
  });
}
}  // namespace devtools::devtools
