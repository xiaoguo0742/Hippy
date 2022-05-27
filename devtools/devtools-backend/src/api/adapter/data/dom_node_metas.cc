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

#include "api/adapter/data/dom_node_metas.h"
#include <devtools_base/convert/data_converter.h>
#include <devtools_base/logging.h>
#include "devtools_base/dom_value.h"
#include "devtools_base/transform_string_util.h"
#include "module/inspect_props.h"

namespace hippy::devtools {
constexpr char kNodeType[] = "nodeType";
constexpr char kFlexNodeStyle[] = "flexNodeStyle";
constexpr char kAttributes[] = "attributes";
constexpr char kBgColor[] = "bgColor";
constexpr char kText[] = "text";
constexpr char kBase64[] = "base64";
constexpr char kId[] = "id";
constexpr char kDomRelativeRenderId[] = "domRelativeRenderId";
constexpr char kBounds[] = "bounds";
constexpr char kDomNodeLeft[] = "left";
constexpr char kDomNodeRight[] = "right";
constexpr char kDomNodeTop[] = "top";
constexpr char kDomNodeBottom[] = "bottom";
constexpr char kDomNodeChild[] = "child";

hippy::devtools::DomValue::DomValueObjectType DomNodeMetas::ToDomValue() const {
  DomValue::DomValueObjectType root_object;
  root_object[kId] = DomValue(std::to_string(node_id_));
  root_object[kNodeType] = DomValue(node_type_);
  root_object[kAttributes] = DataConverter::DefaultConverter()->ConvertFromString(total_props_);
  root_object[kFlexNodeStyle] =
      DataConverter::DefaultConverter()->ConvertFromString(TransformStringUtil::CombineNodeDefaultValue(style_props_));
  root_object[kWidth] = DomValue(std::to_string(static_cast<int>(width_)));
  root_object[kHeight] = DomValue(std::to_string(static_cast<int>(height_)));
  root_object[kDomRelativeRenderId] = DomValue(0);
  root_object[kBgColor] = DomValue(0);
  root_object[kBorderColor] = DomValue(0);
  root_object[kText] = DomValue("");
  root_object[kBase64] = DomValue("");

  DomValue::DomValueObjectType bounds_object;
  bounds_object[kDomNodeLeft] = DomValue(std::to_string(static_cast<int>(bound_.left)));
  bounds_object[kDomNodeRight] = DomValue(std::to_string(static_cast<int>(bound_.right)));
  bounds_object[kDomNodeTop] = DomValue(std::to_string(static_cast<int>(bound_.top)));
  bounds_object[kDomNodeBottom] = DomValue(std::to_string(static_cast<int>(bound_.bottom)));
  root_object[kBounds] = bounds_object;

  DomValue::DomValueArrayType child_array;
  for (auto& child : children_) {
    child_array.push_back(DomValue(child.ToDomValue()));
  }
  root_object[kDomNodeChild] = DomValue(child_array);
  return root_object;
}

std::string DomNodeMetas::Serialize() const {
  std::string node_str = DataConverter::DefaultConverter()->ConvertToString(DomValue(ToDomValue()));
  return node_str;
}
}  // namespace hippy::devtools
