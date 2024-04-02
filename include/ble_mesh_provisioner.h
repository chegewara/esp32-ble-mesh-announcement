#pragma once

#include <map>
#include <string>

#include "ble_mesh_dev.h"
#include "ble_mesh_config_srv.h"
#include "ble_mesh_config_cli.h"
#include "esp_ble_mesh_defs.h"

#include "callbacks.h"

class BLEmeshProvisioner : public IBLEmeshDevice
{
protected:
    using IBLEmeshDevice::IBLEmeshDevice;

private:
    static BLEmeshConfigCli* _configCli;
    esp_ble_mesh_unprov_dev_add_t add_dev = {};
    std::map<std::string, esp_ble_mesh_unprov_dev_add_t> devices;

    // virtual void setMsgCommonInfo();
    // virtual void getMsgCommonInfo();

public:

    static BLEmeshProvisioner *GetInstance();
    ~BLEmeshProvisioner() = default;

    BLEmeshProvisioner(BLEmeshProvisioner &other) = delete;
    void operator=(const BLEmeshProvisioner &) = delete;

    esp_err_t enableProvisioning(uint8_t bearer = (ESP_BLE_MESH_PROV_ADV | ESP_BLE_MESH_PROV_GATT)) override;

    // virtual void addNodeInfo();
    // virtual void getNodeInfo();
    virtual esp_err_t selfProvisioning();

    esp_err_t filter();

    /// start provisioning node added with onNewDevice
    virtual void startNodeProvisioning()
    {
        esp_ble_mesh_provisioner_add_unprov_dev(&add_dev, (esp_ble_mesh_dev_add_flag_t)(ADD_DEV_RM_AFTER_PROV_FLAG | ADD_DEV_START_PROV_NOW_FLAG | ADD_DEV_FLUSHABLE_DEV_FLAG));
    }

    std::map<std::string, esp_ble_mesh_unprov_dev_add_t> getDevices() {
        return devices;
    }

    const esp_ble_mesh_node_t** getNodes() {
        return esp_ble_mesh_provisioner_get_node_table_entry();
    }

    uint16_t nodesCount() {
        return esp_ble_mesh_provisioner_get_prov_node_count();
    }

    BLEmeshConfigCli* configCli() {
        return _configCli;
    }

    // ESP_BLE_MESH_PROVISIONER_PROV_COMPLETE_EVT
    void onDeviceProvisionedCmpl(esp_ble_mesh_prov_cb_param_t *params) 
    {
        auto node = params->provisioner_prov_complete;
        devices.erase(bt_hex(node.device_uuid, 16));
        _configCli->nodeCompositionDataGet(node.unicast_addr, 0);
    }

    virtual void onNewDevice(esp_ble_mesh_prov_cb_param_t *params);

    virtual void onDeviceCompCpl(uint16_t addr, uint8_t* data, size_t len) {
        esp_ble_mesh_provisioner_store_node_comp_data(addr, data, len);
    }

    void resetNodeCb(uint16_t addr) {
        esp_ble_mesh_provisioner_delete_node_with_addr(addr);
        // nodes.erase(addr);
    }

    void recv_unprov_adv_pkt(uint8_t dev_uuid[16], uint8_t addr[BD_ADDR_LEN], esp_ble_mesh_addr_type_t addr_type, 
                                uint16_t oob_info, uint8_t adv_type, esp_ble_mesh_prov_bearer_t bearer);
    
    void addUnprovisioned(std::string uuid);

    // virtual void nodeAppKeyAdd();
    // virtual void nodeAppKeyUpdate();
    // virtual void nodeAppKeyDelete();

    // virtual void registerCallbacks()
    // {
    //     esp_ble_mesh_register_prov_callback(_default_ble_mesh_provisioning_cb);
    //     esp_ble_mesh_register_config_client_callback(_default_ble_mesh_config_client_cb);
    //     esp_ble_mesh_register_generic_client_callback(_default_ble_mesh_generic_client_cb);
    //     esp_ble_mesh_register_custom_model_callback(_default_ble_mesh_custom_model_cb);
    // }

    /// provisioning functions



    virtual void onEvent(esp_ble_mesh_prov_cb_event_t event, esp_ble_mesh_prov_cb_param_t *params)
    {
        printf("provisioner event: %d\n", event);
    }



    /// config client interface
    // void sendConfigurationSet(esp_ble_mesh_opcode_t opcode, uint16_t address, esp_ble_mesh_cfg_client_set_state_t* set_state) {
    //     _configCli->sendSet(opcode, address, *set_state);
    // }

    // void sendConfigurationGet(esp_ble_mesh_opcode_t opcode, uint16_t address, esp_ble_mesh_cfg_client_get_state_t* get_state) {
    //     _configCli->getState(opcode, address, get_state);
    // }

};


