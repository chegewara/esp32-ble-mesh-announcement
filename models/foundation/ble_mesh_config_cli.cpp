#include "esp_ble_mesh_config_model_api.h"

#include "ble_mesh_config_cli.h"

void BLEmeshConfigCli::getState(esp_ble_mesh_opcode_t opcode, uint16_t addr, esp_ble_mesh_cfg_client_get_state_t *get_state)
{
    static uint8_t tid = 0;
    esp_ble_mesh_client_common_param_t common = {};
    auto ctx = getCtx();

    common.opcode = opcode;
    common.model = _model;
    common.ctx.net_idx = ctx->net_idx;
    common.ctx.app_idx = ctx->app_idx;
    common.ctx.addr = addr;
    common.ctx.send_ttl = ctx->send_ttl;
    common.msg_timeout = 0; /* 0 indicates that timeout value from menuconfig will be used */

    esp_err_t err = esp_ble_mesh_config_client_get_state(&common, get_state);
    if (err)
    {
        ESP_LOGE("TAG1", "%s:%d failed, opcode: 0x%04lx: %d", __FILE__, __LINE__, opcode, err);
    }
}

void BLEmeshConfigCli::setState(esp_ble_mesh_opcode_t opcode, uint16_t addr, esp_ble_mesh_cfg_client_set_state_t* set_state)
{
    static uint8_t tid = 0;
    esp_ble_mesh_client_common_param_t common = {};
    auto ctx = getCtx();

    common.opcode = opcode;
    common.model = _model;
    common.ctx.net_idx = ctx->net_idx;
    common.ctx.app_idx = ctx->app_idx;
    common.ctx.addr = addr;
    common.ctx.send_ttl = ctx->send_ttl;
    common.msg_timeout = 0; /* 0 indicates that timeout value from menuconfig will be used */


    esp_err_t err = esp_ble_mesh_config_client_set_state(&common, set_state);
    if (err)
    {
        ESP_LOGE("TAG2", "%s failed: 0x%04lx => %d", __func__, opcode, err);
    }
}


void BLEmeshConfigCli::nodeCompositionDataGet(uint16_t address, uint8_t page)
{
    auto opcode = ESP_BLE_MESH_MODEL_OP_COMPOSITION_DATA_GET;
    esp_ble_mesh_cfg_client_get_state_t get_state = {};
    get_state.comp_data_get.page = page;
    getState(opcode, address, &get_state);
}

void BLEmeshConfigCli::modelPubGet(uint16_t address, uint16_t element_addr, uint16_t model_id, uint16_t company_id)
{
    auto opcode = ESP_BLE_MESH_MODEL_OP_MODEL_PUB_GET;
    esp_ble_mesh_cfg_client_get_state_t get_state = {};
    get_state.model_pub_get.element_addr = element_addr;
    get_state.model_pub_get.model_id = model_id;
    get_state.model_pub_get.company_id = company_id;
    getState(opcode, address, &get_state);
}

void BLEmeshConfigCli::nodeNetKeyGet(uint16_t address)
{
    esp_ble_mesh_cfg_client_get_state_t get_state = {};
    auto opcode = ESP_BLE_MESH_MODEL_OP_NET_KEY_GET;
    getState(opcode, address, &get_state);
}

void BLEmeshConfigCli::nodeAppKeyGet(uint16_t address, uint16_t net_idx)
{
    esp_ble_mesh_cfg_client_get_state_t get_state = {};
    get_state.app_key_get.net_idx = net_idx;
    auto opcode = ESP_BLE_MESH_MODEL_OP_APP_KEY_GET;
    getState(opcode, address, &get_state);
}

void BLEmeshConfigCli::modelSubGet(uint16_t address, uint16_t element_addr, uint16_t model_id, uint16_t company_id)
{
    if(company_id == 0xffff){
        esp_ble_mesh_cfg_client_get_state_t get_state = {};
        get_state.sig_model_sub_get.element_addr = element_addr;
        get_state.sig_model_sub_get.model_id = model_id;
        auto opcode = ESP_BLE_MESH_MODEL_OP_SIG_MODEL_SUB_GET;
        getState(opcode, address, &get_state);
    } else {
        esp_ble_mesh_cfg_client_get_state_t get_state = {};
        get_state.vnd_model_sub_get.element_addr = element_addr;
        get_state.vnd_model_sub_get.model_id = model_id;
        get_state.vnd_model_sub_get.company_id = company_id;
        auto opcode = ESP_BLE_MESH_MODEL_OP_VENDOR_MODEL_SUB_GET;
        getState(opcode, address, &get_state);
    }
}

void BLEmeshConfigCli::modelAppGet(uint16_t address, uint16_t element_addr, uint16_t model_id, uint16_t company_id)
{
    if(company_id == 0xffff){
        esp_ble_mesh_cfg_client_get_state_t get_state = {};
        get_state.sig_model_app_get.element_addr = element_addr;
        get_state.sig_model_app_get.model_id = model_id;
        auto opcode = ESP_BLE_MESH_MODEL_OP_SIG_MODEL_APP_GET;
        getState(opcode, address, &get_state);
    } else {
        esp_ble_mesh_cfg_client_get_state_t get_state = {};
        get_state.vnd_model_app_get.element_addr = element_addr;
        get_state.vnd_model_app_get.model_id = model_id;
        get_state.vnd_model_app_get.company_id = company_id;
        auto opcode = ESP_BLE_MESH_MODEL_OP_VENDOR_MODEL_APP_GET;
        getState(opcode, address, &get_state);
    }
}


void BLEmeshConfigCli::nodeBeaconSet(uint16_t address, uint8_t beacon)
{
    esp_ble_mesh_cfg_client_set_state_t set_state = {};
    set_state.beacon_set.beacon = beacon;
    auto opcode = ESP_BLE_MESH_MODEL_OP_BEACON_SET;
    setState(opcode, address, &set_state);
}

void BLEmeshConfigCli::nodeDefaultTTLSet(uint16_t address, uint8_t ttl)
{
    esp_ble_mesh_cfg_client_set_state_t set_state = {};
    set_state.default_ttl_set.ttl = ttl;
    auto opcode = ESP_BLE_MESH_MODEL_OP_DEFAULT_TTL_SET;
    setState(opcode, address, &set_state);
}

void BLEmeshConfigCli::nodeGattProxySet(uint16_t address, uint8_t state)
{
    esp_ble_mesh_cfg_client_set_state_t set_state = {};
    set_state.gatt_proxy_set.gatt_proxy = state;
    auto opcode = ESP_BLE_MESH_MODEL_OP_GATT_PROXY_SET;
    setState(opcode, address, &set_state);
}

void BLEmeshConfigCli::nodeRelaySet(uint16_t address, uint8_t relay, uint8_t retransmit)
{
    esp_ble_mesh_cfg_client_set_state_t set_state = {};
    set_state.relay_set.relay = relay;
    set_state.relay_set.relay_retransmit = retransmit ;
    auto opcode = ESP_BLE_MESH_MODEL_OP_RELAY_SET;
    setState(opcode, address, &set_state);
}

void BLEmeshConfigCli::modelPubSet(uint16_t address, uint16_t element_addr, uint16_t publish_addr, uint16_t publish_app_idx, bool cred_flag, uint8_t publish_ttl, uint8_t publish_period, uint8_t publish_retransmit, uint16_t model_id, uint16_t company_id)
{
    esp_ble_mesh_cfg_client_set_state_t set_state = {};
    set_state.model_pub_set.element_addr = element_addr;
    set_state.model_pub_set.publish_addr = publish_addr;
    set_state.model_pub_set.publish_app_idx = publish_app_idx;
    set_state.model_pub_set.cred_flag = cred_flag;
    set_state.model_pub_set.publish_ttl = publish_ttl;
    set_state.model_pub_set.publish_period = publish_period;
    set_state.model_pub_set.publish_retransmit = publish_retransmit;
    set_state.model_pub_set.model_id = model_id;
    set_state.model_pub_set.company_id = company_id;
    auto opcode = ESP_BLE_MESH_MODEL_OP_MODEL_PUB_SET;
    setState(opcode, address, &set_state);
}

void BLEmeshConfigCli::modelSubAdd(uint16_t address, uint16_t element_addr, uint16_t sub_addr, uint16_t model_id, uint16_t company_id)
{
    esp_ble_mesh_cfg_client_set_state_t set_state = {};
    set_state.model_sub_add.element_addr = element_addr;
    set_state.model_sub_add.sub_addr = sub_addr;
    set_state.model_sub_add.model_id  = model_id;
    set_state.model_sub_add.company_id = company_id;
    auto opcode = ESP_BLE_MESH_MODEL_OP_MODEL_SUB_ADD;
    setState(opcode, address, &set_state);
}

void BLEmeshConfigCli::modelSubDelete(uint16_t address, uint16_t element_addr, uint16_t sub_addr, uint16_t model_id, uint16_t company_id)
{
    esp_ble_mesh_cfg_client_set_state_t set_state = {};
    set_state.model_sub_delete.element_addr = element_addr;
    set_state.model_sub_delete.sub_addr = sub_addr;
    set_state.model_sub_delete.model_id = model_id;
    set_state.model_sub_delete.company_id = company_id;
    auto opcode = ESP_BLE_MESH_MODEL_OP_MODEL_SUB_DELETE;
    setState(opcode, address, &set_state);
}

void BLEmeshConfigCli::modelSubOverwrite(uint16_t address, uint16_t element_addr, uint16_t sub_addr, uint16_t model_id, uint16_t company_id)
{
    esp_ble_mesh_cfg_client_set_state_t set_state = {};
    set_state.model_sub_overwrite.element_addr = element_addr;
    set_state.model_sub_overwrite.sub_addr = sub_addr;
    set_state.model_sub_overwrite.model_id = model_id;
    set_state.model_sub_overwrite.company_id = company_id;
    auto opcode = ESP_BLE_MESH_MODEL_OP_MODEL_SUB_OVERWRITE;
    setState(opcode, address, &set_state);
}

void BLEmeshConfigCli::nodeNetKeyAdd(uint16_t address, uint16_t idx)
{
    esp_ble_mesh_cfg_client_set_state_t set_state = {};
    set_state.net_key_add.net_idx = idx;
    memcpy(set_state.net_key_add.net_key, esp_ble_mesh_provisioner_get_local_net_key(idx), 16);
    auto opcode = ESP_BLE_MESH_MODEL_OP_NET_KEY_ADD;
    setState(opcode, address, &set_state);
}

void BLEmeshConfigCli::nodeAppKeyAdd(uint16_t address, uint16_t netIdx, uint16_t appIdx)
{
    esp_ble_mesh_cfg_client_set_state_t set_state = {};
    set_state.app_key_add.net_idx = netIdx;
    set_state.app_key_add.app_idx = appIdx;
    memcpy(set_state.app_key_add.app_key, esp_ble_mesh_provisioner_get_local_app_key(netIdx, appIdx), 16);
    auto opcode = ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD;
    setState(opcode, address, &set_state);
}

void BLEmeshConfigCli::modelAppKeyBind(uint16_t address, uint16_t element_addr, uint16_t appIdx, uint16_t model_id, uint16_t company_id)
{
    esp_ble_mesh_cfg_client_set_state_t set_state = {};
    auto opcode = ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND;
    set_state.model_app_bind.element_addr = element_addr;
    set_state.model_app_bind.model_app_idx = appIdx;
    set_state.model_app_bind.model_id = model_id;
    set_state.model_app_bind.company_id = company_id;
    setState(opcode, address, &set_state);
}

void BLEmeshConfigCli::nodeReset(uint16_t address)
{
    esp_ble_mesh_cfg_client_set_state_t set_state = {};
    auto opcode = ESP_BLE_MESH_MODEL_OP_NODE_RESET;
    setState(opcode, address, &set_state);
}

void BLEmeshConfigCli::nodeNetKeyUpdate(uint16_t address, uint16_t idx)
{
    esp_ble_mesh_cfg_client_set_state_t set_state = {};
    set_state.net_key_update.net_idx = idx;
    memcpy(set_state.net_key_update.net_key, esp_ble_mesh_provisioner_get_local_net_key(idx), 16);
    auto opcode = ESP_BLE_MESH_MODEL_OP_NET_KEY_UPDATE;
    setState(opcode, address, &set_state);
}

void BLEmeshConfigCli::nodeNetKeyDelete(uint16_t address, uint16_t idx)
{
    esp_ble_mesh_cfg_client_set_state_t set_state = {};
    set_state.net_key_delete.net_idx = idx;
    auto opcode = ESP_BLE_MESH_MODEL_OP_NET_KEY_DELETE;
    setState(opcode, address, &set_state);
}

void BLEmeshConfigCli::nodeAppKeyUpdate(uint16_t address, uint16_t netIdx, uint16_t appIdx)
{
    esp_ble_mesh_cfg_client_set_state_t set_state = {};
    set_state.app_key_update.net_idx = netIdx;
    set_state.app_key_update.app_idx = appIdx;
    memcpy(set_state.app_key_update.app_key, esp_ble_mesh_provisioner_get_local_app_key(netIdx, appIdx), 16);
    auto opcode = ESP_BLE_MESH_MODEL_OP_APP_KEY_UPDATE;
    setState(opcode, address, &set_state);
}

void BLEmeshConfigCli::nodeAppKeyDelete(uint16_t address, uint16_t netIdx, uint8_t appIdx)
{
    esp_ble_mesh_cfg_client_set_state_t set_state = {};
    set_state.app_key_delete.net_idx = netIdx;
    set_state.app_key_delete.app_idx = appIdx;
    auto opcode = ESP_BLE_MESH_MODEL_OP_APP_KEY_DELETE;
    setState(opcode, address, &set_state);
}

void BLEmeshConfigCli::modelSubDeleteAll(uint16_t address, uint16_t element_addr, uint16_t model_id, uint16_t company_id)
{
    esp_ble_mesh_cfg_client_set_state_t set_state = {};
    set_state.model_sub_delete_all.element_addr = element_addr;
    set_state.model_sub_delete_all.model_id = model_id;
    set_state.model_sub_delete_all.company_id = company_id;
    auto opcode = ESP_BLE_MESH_MODEL_OP_MODEL_SUB_DELETE_ALL;
    setState(opcode, address, &set_state);
}

void BLEmeshConfigCli::modelAppKeyUnbind(uint16_t address, uint16_t element_addr, uint16_t appIdx, uint16_t model_id, uint16_t company_id)
{
    esp_ble_mesh_cfg_client_set_state_t set_state = {};
    set_state.model_app_unbind.element_addr = element_addr;
    set_state.model_app_unbind.model_app_idx = appIdx;
    set_state.model_app_unbind.model_id = model_id;
    set_state.model_app_unbind.company_id = company_id;
    auto opcode = ESP_BLE_MESH_MODEL_OP_MODEL_APP_UNBIND;
    setState(opcode, address, &set_state);
}

