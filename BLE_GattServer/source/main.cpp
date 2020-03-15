/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Platform includes
#include <stdio.h>
#include <mbed.h>

#include "platform/Callback.h"
#include "events/EventQueue.h"
#include "platform/NonCopyable.h"

// Bluetooth
#include "ble/BLE.h"

// Local Includes
#include "BLEProcess.h"
#include "SensorProcess.h"
#include "SensorWrapper.h"

using mbed::callback;

/**
 * Main work happens inside SensorProcess and SensorWrapper
 * 
 * SensorWrapper -> Gets data from sensors and stores it
 * SensorProcess -> Uses that data, converts to uint8_t array, sends it over bluetooth
 */
int main() {

    // BLE Instance
    BLE &ble_interface = BLE::Instance();

    // Object Init
    events::EventQueue event_queue;
    SensorService sensor_service;

    // Start Program
    BLEProcess ble_process(event_queue, ble_interface);
    ble_process.on_init(callback(&sensor_service, &SensorService::start));

    // bind the event queue to the ble interface, initialize the interface
    // and start advertising
    ble_process.start();

    // Process the event queue.
    event_queue.dispatch_forever();

    return 0;
}
