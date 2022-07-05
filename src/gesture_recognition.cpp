// Copyright (c) 2021 Xiaomi Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "cyberdog_vision/gesture_recognition.hpp"

namespace cyberdog_vision
{

GestureRecognition::GestureRecognition(const std::string & model_det, const std::string & model_cls)
: max_person_num_(5)
{
  gesture_ptr_ = std::make_shared<handgesture::Hand_Gesture>(model_det, model_cls);
}

void GestureRecognition::GetGestureInfo(
  const cv::Mat & img,
  const std::vector<InferBbox> & body_boxes,
  std::vector<GestureInfo> & infos)
{
  std::vector<handgesture::bbox> infer_bboxes;
  for (auto & body : body_boxes) {
    handgesture::bbox infer_bbox;
    infer_bbox.xmin = body.body_box.x;
    infer_bbox.ymin = body.body_box.y;
    infer_bbox.xmax = body.body_box.x + body.body_box.width;
    infer_bbox.ymax = body.body_box.y + body.body_box.height;
    infer_bbox.score = body.score;
    infer_bboxes.push_back(infer_bbox);
  }

  XMImage xm_img;
  ImgConvert(img, xm_img);
  gesture_ptr_->Inference(xm_img, infer_bboxes, max_person_num_);
  std::vector<handgesture::XMHandGesture> gesture_infos = gesture_ptr_->getResult();

  for (size_t i = 0; i < gesture_infos.size(); ++i) {
    GestureInfo info;
    info.rect = cv::Rect(
      gesture_infos[i].left, gesture_infos[i].top,
      gesture_infos[i].right - gesture_infos[i].left,
      gesture_infos[i].bottom - gesture_infos[i].top);
    info.label = gesture_infos[i].gestureLabel;
    infos.push_back(info);
  }
}

void GestureRecognition::SetRecognitionNum(int num)
{
  max_person_num_ = num;
}

GestureRecognition::~GestureRecognition()
{}

}  // namespace cyberdog_vision
