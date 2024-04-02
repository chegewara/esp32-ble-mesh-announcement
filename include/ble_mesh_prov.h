#pragma once

#include "stdio.h"
#include <cstring>

#include "esp_err.h"
#include "esp_ble_mesh_defs.h"
#include "esp_ble_mesh_common_api.h"
#include "esp_ble_mesh_networking_api.h"
#include "esp_ble_mesh_provisioning_api.h"

#include "esp_ble_mesh_config_model_api.h"
#include "esp_ble_mesh_generic_model_api.h"

#include <vector>

#define CID_ESP 0x02E5
#define TAG "ble-mesh"

class BLEmeshProvisioning
{
protected:
    // provisioner only
    esp_ble_mesh_prov_t provision = {
        .prov_unicast_addr = 0x0001,
        .prov_start_address = 0x0005,
    };
    /** ???? */
    static uint16_t _net_idx;
    uint16_t addr;
    uint8_t flags;
    uint32_t iv_index;
    /** ???? */

public:
    BLEmeshProvisioning()
    {
#if 0
            provision.output_size = 1;
            provision.output_actions = ESP_BLE_MESH_BLINK;
            provision.input_actions = ESP_BLE_MESH_PUSH;
            provision.input_size = 1;
#else
        provision.output_size = 0;
        provision.output_actions = 0;
#endif
        // provision.oob_type = 1;
    }
    ~BLEmeshProvisioning() {}

    virtual esp_err_t enableProvisioning(uint8_t bearer = (ESP_BLE_MESH_PROV_ADV | ESP_BLE_MESH_PROV_GATT)) = 0;
    virtual void setupOOB() {}

    static void netIdx(uint16_t idx)
    {
        _net_idx = idx;
    }
    static uint16_t netIdx()
    {
        return _net_idx;
    }

};
