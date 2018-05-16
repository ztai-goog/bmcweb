/*
// Copyright (c) 2018 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/
#pragma once

#include "node.hpp"
#include "sensors.hpp"

namespace redfish {

class Thermal : public Node {
 public:
  Thermal(CrowApp& app)
      : Node((app), "/redfish/v1/Chassis/<str>/Thermal/", std::string()) {
    Node::json["@odata.type"] = "#Thermal.v1_2_0.Thermal";
    Node::json["@odata.context"] = "/redfish/v1/$metadata#Thermal.Thermal";
    Node::json["Id"] = "Thermal";
    Node::json["Name"] = "Thermal";

    entityPrivileges = {
        {boost::beast::http::verb::get, {{"Login"}}},
        {boost::beast::http::verb::head, {{"Login"}}},
        {boost::beast::http::verb::patch, {{"ConfigureManager"}}},
        {boost::beast::http::verb::put, {{"ConfigureManager"}}},
        {boost::beast::http::verb::delete_, {{"ConfigureManager"}}},
        {boost::beast::http::verb::post, {{"ConfigureManager"}}}};
  }

 private:
  void doGet(crow::response& res, const crow::request& req,
             const std::vector<std::string>& params) override {
    if (params.size() != 1) {
      res.result(boost::beast::http::status::internal_server_error);
      res.end();
      return;
    }
    const std::string& chassis_name = params[0];

    res.json_value = Node::json;
    auto asyncResp = std::make_shared<AsyncResp>(
        res, chassis_name,
        std::initializer_list<const char*>{
            "/xyz/openbmc_project/Sensors/fan",
            "/xyz/openbmc_project/Sensors/temperature"});
    getChassisData(asyncResp);
  }
};

}  // namespace redfish