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

#ifndef CYBERDOG_VISION__VISION_MANAGER_HPP_
#define CYBERDOG_VISION__VISION_MANAGER_HPP_

#include <opencv2/opencv.hpp>

#include "rclcpp/rclcpp.hpp"
#include "protocol/srv/body_region.hpp"

#include "cyberdog_vision/shared_memory_op.hpp"
#include "cyberdog_vision/body_detection.hpp"
#include "cyberdog_vision/face_recognition.hpp"
#include "cyberdog_vision/gesture_recognition.hpp"
#include "cyberdog_vision/keypoints_detection.hpp"
#include "cyberdog_vision/person_reid.hpp"

namespace cyberdog_vision
{

using BodyRegionT = protocol::srv::BodyRegion;

class VisionManager : public rclcpp::Node
{
public:
  VisionManager();
  ~VisionManager();

private:
  int Init();
  void ImageProc();
  void BodyDet();
  void ReIDProc();

  void TrackingService(
    const std::shared_ptr<rmw_request_id_t>,
    const std::shared_ptr<BodyRegionT::Request> req,
    std::shared_ptr<BodyRegionT::Response> res);

  int GetMatchBody(const sensor_msgs::msg::RegionOfInterest & roi);

private:
  std::shared_ptr<std::thread> img_proc_thread_;
  std::shared_ptr<std::thread> body_det_thread_;
  std::shared_ptr<std::thread> reid_thread_;

  std::shared_ptr<BodyDetection> body_ptr_;
  std::shared_ptr<PersonReID> reid_ptr_;

  GlobalImageBuf global_img_buf_;
  BodyResults body_results_;

  int shm_id_;
  int sem_set_id_;
  char * shm_addr_;

  size_t buff_size_;
  bool is_tracking_;

private:
  rclcpp::Service<BodyRegionT>::SharedPtr tracking_service_;

};

}  // namespace cyberdog_vision

#endif  // CYBERDOG_VISION__VISION_MANAGER_HPP_
