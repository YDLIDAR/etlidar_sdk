/*****************************************************************************
*  EAI TOF LIDAR DRIVER                                                      *
*  Copyright (C) 2018 EAI TEAM  chushuifurong618@eaibot.com.                 *
*                                                                            *
*  This file is part of EAI TOF LIDAR DRIVER.                                *
*                                                                            *
*  @file     example.cpp                                                     *
*  @brief    ETLidar Driver example                                          *
*  Details.                                                                  *
*                                                                            *
*  @author   Tony.Yang                                                       *
*  @email    chushuifurong618@eaibot.com                                     *
*  @version  1.0.0(版本号)                                                    *
*  @date     chushuifurong618@eaibot.com                                     *
*                                                                            *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         : Description                                              *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2018/08/09 | 1.0.0     | Tony.Yang      | Create file                     *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/


#include "ETLidarDriver.h"
#include <config.h>

#include <iostream>
#include <iterator>
#include <algorithm>
#include <regex>

#if defined(_WIN32)
# pragma warning(disable: 4786)
# pragma  comment(lib, "etlidar_driver.lib")
#endif

using namespace ydlidar;


int main(int argc, char **argv) {

  std::string lidarIP;
  ydlidar::init(argc, argv);

again:
  printf("Please enter the lidar IP[192.168.0.11](yes):");
  std::cin >> lidarIP;
  regex reg("(\\d{1,3}).(\\d{1,3}).(\\d{1,3}).(\\d{1,3})");
  smatch m;

  if (!ydlidar::ok()) {
    return 0;
  }

  if (lidarIP.find("yes") != std::string::npos) {
    lidarIP = "192.168.0.11";
  }

  if (!regex_match(lidarIP, m, reg)) {
    ydlidar::console.warning("ip address input error, please again.");
    goto again;
  }

  ydlidar::console.message("SDK Version: %s", SDK_VERSION);
  ydlidar::console.message("LIDAR Version: %s", EHLIDAR_VERSION);
  ydlidar::ETLidarDriver lidar;

  if (!ydlidar::ok()) {
    return 0;
  }

  result_t ans = lidar.connect(lidarIP);

  if (!IS_OK(ans)) {
    ydlidar::console.error("Failed to connecting lidar...");
    return 0;
  }

  bool rs = lidar.turnOn();

  while (rs && ydlidar::ok()) {
    lidarData scan;
    ans = lidar.grabScanData(scan);

    if (IS_OK(ans)) {
      ydlidar::console.message("scan recevied[%llu]: %d ranges [%f]hz",
                               scan.system_timestamp, scan.data.size(), 1e9 / scan.scan_time);
    } else {
      ydlidar::console.warning("Failed to get scan data");
    }
  }

  lidar.turnOff();
  lidar.disconnect();
  return 0;
}
