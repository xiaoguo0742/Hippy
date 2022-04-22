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

#include "api/notification/default/default_elements_response_notification.h"
#include <string>
#include "api/devtools_backend_service.h"
#include "devtools_base/logging.h"

namespace tdf {
namespace devtools {
DefaultElementsResponseAdapter::DefaultElementsResponseAdapter(DocumentUpdateHandler document_update_Handler)
    : document_update_Handler_(document_update_Handler) {}

void DefaultElementsResponseAdapter::NotifyDocumentUpdate() {
//  auto runner = DevtoolsBackendService::GetInstance().GetTaskRunner();
//  if (!runner) {
//    BACKEND_LOGE(TDF_BACKEND, "NotifyDocumentUpdate error, runner is nullptr");
//    return;
//  }
//  if (!document_update_Handler_ || !need_notify_batch_event_) {
//    BACKEND_LOGE(TDF_BACKEND, "NotifyDocumentUpdate error, handler is nullptr or not need");
//    return;
//  }
//  runner->PostTask([this] {
    document_update_Handler_();
//  });
}
}  // namespace devtools
}  // namespace tdf
