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

#include "devtools_base/convert/data_converter.h"
#include "devtools_base/convert/nlohmann_converter.h"

namespace hippy::devtools {
DataConverter* DataConverter::DefaultConverter() {
  static DataConverter* converter = nullptr;
  static std::once_flag flag;
  std::call_once(flag, [] { converter = new hippy::devtools::NlohmannConverter(); });
  return converter;
}
}  // namespace hippy::devtools
