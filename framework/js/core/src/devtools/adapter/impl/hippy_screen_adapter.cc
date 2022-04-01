//
// Copyright (c) 2021 Tencent Corporation. All rights reserved.
// Created by thomasyqguo on 2021/10/15.
//

#include "devtools/adapter/hippy_screen_adapter.h"
#include "devtools/devtool_utils.h"

namespace hippy {
namespace devtools {
constexpr const char* kScreenShot = "screenShot";
constexpr const char* kScreenWidth = "width";
constexpr const char* kScreenHeight = "height";
constexpr const char* kMaxWidth = "maxWidth";
constexpr const char* kMaxHeight = "maxHeight";
constexpr const char* kQuality = "quality";
constexpr const char* kGetScreenShot = "getScreenShot";

void HippyScreenAdapter::GetScreenShot(const tdf::devtools::ScreenRequest& request, CoreScreenshotCallback callback) {
  if (callback) {
    std::function func = [this, callback, request] {
      std::shared_ptr<DomManager> dom_manager = DomManager::Find(static_cast<int32_t>(dom_id_));
      if (dom_manager) {
        auto root_node = dom_manager->GetNode(dom_manager->GetRootId());
        auto children = root_node->GetChildren();
        if (!children.empty()) {
          tdf::base::DomValue::DomValueObjectType domValueObject;
          domValueObject[kMaxWidth] = tdf::base::DomValue(request.req_width);
          domValueObject[kMaxHeight] = tdf::base::DomValue(request.req_height);
          domValueObject[kQuality] = tdf::base::DomValue(request.quality);
          tdf::base::DomValue::DomValueArrayType domValueArray;
          domValueArray.push_back(tdf::base::DomValue(domValueObject));
          tdf::base::DomValue argumentValue(domValueArray);
          hippy::dom::DomArgument argument(argumentValue);
          std::function screen_shot_callback = [callback](std::shared_ptr<DomArgument> arg) {
            tdf::base::DomValue result_dom_value;
            arg->ToObject(result_dom_value);
            tdf::base::DomValue::DomValueObjectType base64_dom_value;
            if (result_dom_value.IsArray()) {
              base64_dom_value = result_dom_value.ToArrayChecked()[0].ToObjectChecked();
            } else {
              base64_dom_value = result_dom_value.ToObjectChecked();
            }
            std::string base64_str = base64_dom_value.find(kScreenShot)->second.ToStringChecked();
            int32_t width = base64_dom_value.find(kScreenWidth)->second.ToInt32Checked();
            int32_t height = base64_dom_value.find(kScreenHeight)->second.ToInt32Checked();
            TDF_BASE_DLOG(INFO) << "GetScreenShot callback " << base64_str.size();
            callback(base64_str, width, height);
          };
          children[0]->CallFunction(kGetScreenShot, argument, screen_shot_callback);
        }
      }
    };
    DevToolUtils::PostDomTask(dom_id_, func);
  }
}
}  // namespace devtools
}  // namespace hippy
