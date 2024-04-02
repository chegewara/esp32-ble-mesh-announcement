#pragma once

#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gatt_common_api.h"
#include "esp_ble_mesh_defs.h"
#include "esp_ble_mesh_common_api.h"
#include "esp_ble_mesh_networking_api.h"
#include "esp_ble_mesh_provisioning_api.h"

#include "ble_mesh_model.h"

#include "esp_ble_mesh_config_model_api.h"
#include "esp_ble_mesh_generic_model_api.h"

#include "callbacks.h"

class BLEmeshConfigCli : public IBLEMeshModel
{
protected:
    esp_ble_mesh_client_t _client = {};

public:
    using IBLEMeshModel::IBLEMeshModel;
    ~BLEmeshConfigCli() {}

    void init(void* p) override
    {
        (void)p;
        model_id = ESP_BLE_MESH_MODEL_ID_CONFIG_CLI;
        _model = new esp_ble_mesh_model_t(ESP_BLE_MESH_MODEL_CFG_CLI(&_client));
    }
    
    esp_err_t registerCallback() override {
        return registerCallback(NULL);
    }

    virtual esp_err_t registerCallback(esp_ble_mesh_cfg_client_cb_t callback) {
        if (callback == NULL)
        {
            return esp_ble_mesh_register_config_client_callback(_default_ble_mesh_config_client_cb);
        }

        return esp_ble_mesh_register_config_client_callback(callback);
    }
private:
    void getState(esp_ble_mesh_opcode_t opcode, uint16_t addr, esp_ble_mesh_cfg_client_get_state_t *get_state);

    void setState(esp_ble_mesh_opcode_t opcode, uint16_t addr, esp_ble_mesh_cfg_client_set_state_t *set_state);

public:
    /// esp_ble_mesh_config_model_api.h#83
    // COMPOSITION_DATA_GET
    void nodeCompositionDataGet(uint16_t address, uint8_t page = 255);
    // MODEL_PUB_GET
    void modelPubGet(uint16_t address, uint16_t elem, uint16_t model_id, uint16_t company_id = 0xffff);
    // NET_KEY_GET
    void nodeNetKeyGet(uint16_t address);
    // APP_KEY_GET
    void nodeAppKeyGet(uint16_t address, uint16_t net_idx = 0);
    // SIG_MODEL_SUB_GET
    // VENDOR_MODEL_SUB_GET
    void modelSubGet(uint16_t address, uint16_t elem, uint16_t model_id, uint16_t company_id = 0xffff);
    // SIG_MODEL_APP_GET
    // VENDOR_MODEL_APP_GET
    void modelAppGet(uint16_t address, uint16_t elem, uint16_t model_id, uint16_t company_id = 0xffff);

    /// esp_ble_mesh_config_model_api.h#141
    // BEACON_SET
    void nodeBeaconSet(uint16_t address, uint8_t beacon);
    // DEFAULT_TTL_SET
    void nodeDefaultTTLSet(uint16_t address, uint8_t ttl = 7);
    // GATT_PROXY_SET
    void nodeGattProxySet(uint16_t address, uint8_t state);
    // RELAY_SET
    void nodeRelaySet(uint16_t address, uint8_t relay, uint8_t retransmit);
    // MODEL_PUB_SET
    void modelPubSet(uint16_t address, uint16_t element_addr, uint16_t publish_addr, uint16_t publish_app_idx,
                     bool cred_flag, uint8_t publish_ttl, uint8_t publish_period, uint8_t publish_retransmit,
                     uint16_t model_id, uint16_t company_id = 0xffff);
    // MODEL_SUB_ADD
    void modelSubAdd(uint16_t address, uint16_t element_addr, uint16_t sub_addr, uint16_t model_id, uint16_t company_id = 0xffff);
    // MODEL_SUB_DELETE
    void modelSubDelete(uint16_t address, uint16_t element_addr, uint16_t sub_addr, uint16_t model_id, uint16_t company_id = 0xffff);
    // MODEL_SUB_OVERWRITE
    void modelSubOverwrite(uint16_t address, uint16_t element_addr, uint16_t sub_addr, uint16_t model_id, uint16_t company_id = 0xffff);
    // NET_KEY_ADD
    void nodeNetKeyAdd(uint16_t address, uint16_t idx);
    // APP_KEY_ADD
    void nodeAppKeyAdd(uint16_t address, uint16_t netIdx, uint16_t appIdx);
    // MODEL_APP_BIND
    void modelAppKeyBind(uint16_t address, uint16_t element_addr, uint16_t appIdx, uint16_t model_id, uint16_t company_id = 0xffff);
    // NODE_RESET
    void nodeReset(uint16_t address);
    // NET_KEY_UPDATE
    void nodeNetKeyUpdate(uint16_t address, uint16_t idx = 0);
    // NET_KEY_DELETE
    void nodeNetKeyDelete(uint16_t address, uint16_t idx = 0);
    // APP_KEY_UPDATE
    void nodeAppKeyUpdate(uint16_t address, uint16_t netIdx = 0, uint16_t appIdx = 0);
    // APP_KEY_DELETE
    void nodeAppKeyDelete(uint16_t address, uint16_t netIdx = 0, uint8_t appIdx = 0);
    // MODEL_SUB_DELETE_ALL
    void modelSubDeleteAll(uint16_t address, uint16_t element_addr, uint16_t model_id, uint16_t company_id = 0xffff);
    // MODEL_APP_UNBIND
    void modelAppKeyUnbind(uint16_t address, uint16_t elem, uint16_t appIdx, uint16_t model_id, uint16_t company_id = 0xffff);


/// @todo missing 10 set functions
/// @todo missing 11 get functions

    // BEACON_GET
    // DEFAULT_TTL_GET
    // GATT_PROXY_GET
    // RELAY_GET
    // FRIEND_GET
    // HEARTBEAT_PUB_GET
    // HEARTBEAT_SUB_GET
    // NODE_IDENTITY_GET
    // KEY_REFRESH_PHASE_GET
    // LPN_POLLTIMEOUT_GET
    // NETWORK_TRANSMIT_GET

    // FRIEND_SET
    // HEARTBEAT_PUB_SET
    // HEARTBEAT_SUB_SET
    // NODE_IDENTITY_SET
    // KEY_REFRESH_PHASE_SET
    // MODEL_PUB_VIRTUAL_ADDR_SET
    // NETWORK_TRANSMIT_SET

    // MODEL_SUB_VIRTUAL_ADDR_DELETE
    // MODEL_SUB_VIRTUAL_ADDR_OVERWRITE
    // MODEL_SUB_VIRTUAL_ADDR_ADD

    void onEvent(uint32_t event, uint32_t op_code, void *params) override
    {
        ESP_LOGD(__func__, "%s event: %ld, OP code: 0x%04lx\n", name(), event, op_code);
        if(cb)
            cb->onEvent(this, event, params);
    }
};
