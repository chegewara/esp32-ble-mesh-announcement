#pragma once

#include <map>
#include <string>

#include "ble_mesh_dev.h"
#include "ble_mesh_config_srv.h"
#include "ble_mesh_config_cli.h"
#include "esp_ble_mesh_defs.h"

#include "callbacks.h"

class BLEmeshNode : public IBLEmeshDevice
{
protected:
    using IBLEmeshDevice::IBLEmeshDevice;

private:
public:
    static BLEmeshNode *GetInstance();
    ~BLEmeshNode() = default;

    BLEmeshNode(BLEmeshNode &other) = delete;
    void operator=(const BLEmeshNode &) = delete;

    esp_err_t enableProvisioning(uint8_t bearer = (ESP_BLE_MESH_PROV_ADV | ESP_BLE_MESH_PROV_GATT)) override;
};
