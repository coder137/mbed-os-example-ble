#pragma once

#include <stdio.h>
#include <mbed.h>

#include "ble/BLE.h"
#include "ble/GattServer.h"

// #include "ble/Gap.h"
// #include "ble/GattClient.h"
// #include "ble/GapAdvertisingParams.h"
// #include "ble/GapAdvertisingData.h"

// Sensor data
#include "SensorWrapper.h"

// ? debugging
#define printFunction() printf("SensorService: %s\r\n", __FUNCTION__);

// ! Important
#define TEMP_CHARACTERISTIC_ID "12345678-030e-485f-b122-f8f381aa84ed"
#define HUMIDITY_CHARACTERISTIC_ID "23456789-030e-485f-b122-f8f381aa84ed"
#define PRESSURE_CHARACTERISTIC_ID "3456789a-030e-485f-b122-f8f381aa84ed"
#define SENSOR_SERVICE_ID "51311102-030e-485f-b122-f8f381aa84ed"

class SensorService {
    typedef SensorService Self;

public:
    SensorService() :
        _temperature_characteristic(UUID(TEMP_CHARACTERISTIC_ID), _temperature_value, 2, 2, 0x12),
        _humidity_characteristic(UUID(HUMIDITY_CHARACTERISTIC_ID), _humidity_value, 2, 2, 0x12),
        _pressure_characteristic(UUID(PRESSURE_CHARACTERISTIC_ID), _pressure_value, 2, 2, 0x12),
        _sensor_service(UUID(SENSOR_SERVICE_ID), _sensor_characteristics, 3),
        _server(NULL),
        _event_queue(NULL)
    {
        _sensor_characteristics[0] = &_temperature_characteristic;
        _sensor_characteristics[1] = &_humidity_characteristic;
        _sensor_characteristics[2] = &_pressure_characteristic;

        // TODO, Add more here
    }

    void start(BLE &ble_interface, events::EventQueue &event_queue)
    {
        printFunction();

         if (_event_queue) {
            return;
        }

        _server = &ble_interface.gattServer();
        _event_queue = &event_queue;
        _sensor_wrapper.init();

        // register the service
        printf("Adding Sensor Service\r\n");
        ble_error_t err = _server->addService(_sensor_service);

        if (err) {
            printf("Error %u during demo service registration.\r\n", err);
            return;
        }

        // read write handler
        _server->onDataSent(as_cb(&Self::when_data_sent));
        _server->onDataWritten(as_cb(&Self::when_data_written));
        _server->onDataRead(as_cb(&Self::when_data_read));

        // updates subscribtion handlers
        _server->onUpdatesEnabled(as_cb(&Self::when_update_enabled));
        _server->onUpdatesDisabled(as_cb(&Self::when_update_disabled));
        _server->onConfirmationReceived(as_cb(&Self::when_confirmation_received));

        // Update once per second
        _event_queue->call_every(1000 /* ms */, callback(this, &Self::update_sensor));

        // ? debugging, print the handles
        printf("sensor service registered\r\n");
        printf("sensor_service handle: %u\r\n", _sensor_service.getHandle());
        printf("temperature handle: %u\r\n", _temperature_characteristic.getValueHandle());
        printf("humidity handle: %u\r\n", _humidity_characteristic.getValueHandle());
        printf("pressure handle: %u\r\n", _pressure_characteristic.getValueHandle());
    }

private:

    /**
     * Handler called when a notification or an indication has been sent.
     */
    void when_data_sent(unsigned count)
    {
        printFunction();
        printf("sent %u updates\r\n", count);
    }

    /**
     * Handler called after an attribute has been written.
     */
    // ! NOT USED
    void when_data_written(const GattWriteCallbackParams *e)
    {
        printFunction();

        printf("data written:\r\n");
        printf("connection handle: %u\r\n", e->connHandle);
        printf("attribute handle: %u", e->handle);
        printf("write operation: %u\r\n", e->writeOp);
        printf("offset: %u\r\n", e->offset);
        printf("length: %u\r\n", e->len);
        printf("data: ");

        for (size_t i = 0; i < e->len; ++i) {
            printf("%02X", e->data[i]);
        }

        printf("\r\n");
    }

    /**
     * Handler called after an attribute has been read.
     */
    void when_data_read(const GattReadCallbackParams *e)
    {
        printFunction();

        printf("data read:\r\n");
        printf("connection handle: %u\r\n", e->connHandle);
        printf("attribute handle: %u\r\n", e->handle);
    }

    /**
     * Handler called after a client has subscribed to notification or indication.
     *
     * @param handle Handle of the characteristic value affected by the change.
     */
    void when_update_enabled(GattAttribute::Handle_t handle)
    {
        printFunction();
        printf("update enabled on handle %d\r\n", handle);
    }

    /**
     * Handler called after a client has cancelled his subscription from
     * notification or indication.
     *
     * @param handle Handle of the characteristic value affected by the change.
     */
    void when_update_disabled(GattAttribute::Handle_t handle)
    {
        printFunction();

        printf("update disabled on handle %d\r\n", handle);
    }

    /**
     * Handler called when an indication confirmation has been received.
     *
     * @param handle Handle of the characteristic value that has emitted the
     * indication.
     */
    void when_confirmation_received(GattAttribute::Handle_t handle)
    {
        printFunction();

        printf("confirmation received on handle %d\r\n", handle);
    }

    /**
     * Handler called when a write request is received.
     *
     * This handler verify that the value submitted by the client is valid before
     * authorizing the operation.
     */
    void authorize_client_write(GattWriteAuthCallbackParams *e)
    {
        printFunction();

        printf("characteristic %u write authorization\r\n", e->handle);

        if (e->offset != 0) {
            printf("Error invalid offset\r\n");
            e->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_INVALID_OFFSET;
            return;
        }

        if (e->len != 1) {
            printf("Error invalid len\r\n");
            e->authorizationReply = AUTH_CALLBACK_REPLY_ATTERR_INVALID_ATT_VAL_LENGTH;
            return;
        }

        e->authorizationReply = AUTH_CALLBACK_REPLY_SUCCESS;
    }

private:
    /** 
     * 
     * Function declarations
     * 
     */
    void update_sensor(void);

    /**
     * Helper that construct an event handler from a member function of this
     * instance.
     */
    template<typename Arg>
    FunctionPointerWithContext<Arg> as_cb(void (Self::*member)(Arg))
    {
        return makeFunctionPointer(this, member);
    }

    GattCharacteristic _temperature_characteristic;
    GattCharacteristic _humidity_characteristic;
    GattCharacteristic _pressure_characteristic;

    uint8_t _temperature_value[2];
    uint8_t _humidity_value[2];
    uint8_t _pressure_value[2];

    // list of the characteristics of the clock service
    GattCharacteristic* _sensor_characteristics[3]; // TODO, Increase as needed

    // Service
    GattService _sensor_service;

    GattServer* _server;
    events::EventQueue *_event_queue;

    // Sensor
    SensorWrapper _sensor_wrapper;

private:
    // NOTE, Causes problems for some reason
    const uint8_t GATT_CHARACTERISTIC_READ_AND_NOTIFY = GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
    GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY;
};



