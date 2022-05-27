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

#include "devtools_base/convert/nlohmann_converter.h"
#include <devtools_base/logging.h>

namespace hippy::devtools {
DomValue NlohmannConverter::ConvertFromString(const std::string& content) {
  nlohmann::json json = nlohmann::json::parse(content, nullptr, false);
  return ConvertFromJson(json);
}
std::string NlohmannConverter::ConvertToString(const DomValue& dom_value) {
  nlohmann::json json = ConvertToJson(dom_value);
  return json.dump();
}

DomValue NlohmannConverter::ConvertFromJson(const nlohmann::json& json) {
  if (json.is_object()) {
    DomValue::DomValueObjectType dom_value_object;
    for (auto& item : json.items()) {
      dom_value_object[item.key()] = ConvertFromJson(item.value());
    }
    return DomValue(dom_value_object);
  } else if (json.is_array()) {
    DomValue::DomValueArrayType dom_value_array;
    for (const auto & i : json) {
      dom_value_array.push_back(ConvertFromJson(i));
    }
    return DomValue(dom_value_array);
  } else if (json.is_boolean()) {
    bool bool_value;
    json.get_to(bool_value);
    BACKEND_LOGD(TDF_BACKEND, "DomNodeMetas bool_value:%d", bool_value);
    return DomValue(bool_value);
  } else if (json.is_number_float()) {
    float float_value;
    json.get_to(float_value);
    BACKEND_LOGD(TDF_BACKEND, "DomNodeMetas float_value:%f", float_value);
    return DomValue(float_value);
  } else if (json.is_number_integer()) {
    int32_t int_value;
    json.get_to(int_value);
    BACKEND_LOGD(TDF_BACKEND, "DomNodeMetas int_value:%d", int_value);
    return DomValue(int_value);
  } else if (json.is_string()) {
    std::string string_value;
    json.get_to(string_value);
    BACKEND_LOGD(TDF_BACKEND, "DomNodeMetas string_value:%s", string_value.c_str());
    return DomValue(string_value);
  }
  BACKEND_LOGD(TDF_BACKEND, "DomNodeMetas else:%s", json.dump().c_str());
  return {};
}

nlohmann::json NlohmannConverter::ConvertToJson(const DomValue& dom_value) {
  if (dom_value.IsObject()) {
    nlohmann::json json = nlohmann::json::object();
    for (auto& iterator : dom_value.ToObjectChecked()) {
      std::string key = iterator.first;
      if (iterator.second.IsBoolean()) {
        json[key] = iterator.second.ToBooleanChecked();
      } else if (iterator.second.IsInt32()) {
        json[key] = iterator.second.ToInt32Checked();
      } else if (iterator.second.IsUInt32()) {
        json[key] = iterator.second.IsUInt32();
      } else if (iterator.second.IsDouble()) {
        json[key] = iterator.second.ToDoubleChecked();
      } else if (iterator.second.IsString()) {
        json[key] = iterator.second.ToStringChecked();
      } else if (iterator.second.IsArray()) {
        //        nlohmann::json array = nlohmann::json::array();
        //        for (auto& array_iterator : iterator.second.ToArrayChecked()) {
        //          if (array_iterator.IsNull() || array_iterator.IsUndefined()) {
        //            continue;
        //          }
        //          array.push_back(ConvertToJson(array_iterator));
        //        }
        json[key] = ConvertToJson(iterator.second);
      } else if (iterator.second.IsObject()) {
        json[key] = ConvertToJson(iterator.second);
      }
    }
    return json;
  } else if (dom_value.IsArray()) {
    nlohmann::json array = nlohmann::json::array();
    for (auto& array_item : dom_value.ToArrayChecked()) {
      if (array_item.IsNull() || array_item.IsUndefined()) {
        continue;
      }
      if (array_item.IsObject()) {
        array.push_back(ConvertToJson(array_item));
      } else if (array_item.IsBoolean()) {
        array.push_back(array_item.ToBooleanChecked());
      } else if (array_item.IsInt32()) {
        array.push_back(array_item.ToInt32Checked());
      } else if (array_item.IsUInt32()) {
        array.push_back(array_item.IsUInt32());
      } else if (array_item.IsDouble()) {
        array.push_back(array_item.ToDoubleChecked());
      } else if (array_item.IsString()) {
        array.push_back(array_item.ToStringChecked());
      }
    }
    return array;
  }
  return nlohmann::json::object();
}
}  // namespace hippy::devtools
