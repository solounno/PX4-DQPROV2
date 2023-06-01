/****************************************************************************
 *
 *   Copyright (c) 2019 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
/**
 * @author Jacob Dahl <dahl.jakejacob@gmail.com>
 * @author Alex Klimaj <alex@arkelectron.com>
 */

#pragma once

#include "sensor_bridge.hpp"
#include <uavcan/equipment/power/BatteryInfo.hpp>
#include <battery/battery.h>
#include <drivers/drv_hrt.h>
#include <px4_platform_common/module_params.h>
#include <uORB/topics/battery_status.h>
#include <uORB/Subscription.hpp>
#include <uORB/topics/actuator_controls.h>

using namespace time_literals;

class UavcanBatteryBridge : public UavcanSensorBridgeBase, public ModuleParams
{
public:
	static const char *const NAME;

	UavcanBatteryBridge(uavcan::INode &node);

	const char *get_name() const override { return NAME; }

	int init() override;

private:

	void battery_sub_cb(const uavcan::ReceivedDataStructure<uavcan::equipment::power::BatteryInfo> &msg);

	typedef uavcan::MethodBinder < UavcanBatteryBridge *,
		void (UavcanBatteryBridge::*)
		(const uavcan::ReceivedDataStructure<uavcan::equipment::power::BatteryInfo> &) >
		BatteryInfoCbBinder;

	static constexpr int BATTERY_INDEX = 1;
	static constexpr int SAMPLE_INTERVAL_US = 20_ms; // assume higher frequency UAVCAN feedback than 50Hz
	Battery _battery{BATTERY_INDEX, this, SAMPLE_INTERVAL_US};

	uavcan::Subscriber<uavcan::equipment::power::BatteryInfo, BatteryInfoCbBinder> _sub_battery;

	actuator_controls_s  _actuator_controls{};
	uORB::Subscription  _actuators_sub{ORB_ID(actuator_controls_0)};
};
