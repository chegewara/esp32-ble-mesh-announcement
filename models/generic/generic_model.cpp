#include "generic_model.h"

esp_err_t IGenericCliModel::sendState(uint16_t address, uint32_t opcode, void *data)
{
    esp_ble_mesh_client_common_param_t params;
    esp_ble_mesh_generic_client_set_state_t set_state;
    auto ctx = getCtx();
    params.model = _model;
    params.opcode = opcode;
    params.msg_timeout = 0; /// menuconfig default timeout
    ctx->addr = address;
    params.ctx = *ctx;

    switch (opcode)
    {
    case ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET:
    case ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK:
        set_state.onoff_set.op_en = false;
        set_state.onoff_set.onoff = *(uint8_t *)data;
        set_state.onoff_set.tid = tid++;
        break;
    case ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_SET:
    case ESP_BLE_MESH_MODEL_OP_GEN_LEVEL_SET_UNACK:
        set_state.level_set.op_en = false;
        set_state.level_set.level = *(int16_t *)data;
        set_state.level_set.tid = tid++;
        break;
    case ESP_BLE_MESH_MODEL_OP_GEN_DELTA_SET:
    case ESP_BLE_MESH_MODEL_OP_GEN_DELTA_SET_UNACK:
        // set_state.delta_set = *(uint8_t *)data;
        break;
    case ESP_BLE_MESH_MODEL_OP_GEN_MOVE_SET:
    case ESP_BLE_MESH_MODEL_OP_GEN_MOVE_SET_UNACK:
        // set_state.move_set = *(uint8_t *)data;
        break;
    case ESP_BLE_MESH_MODEL_OP_GEN_DEF_TRANS_TIME_SET:
    case ESP_BLE_MESH_MODEL_OP_GEN_DEF_TRANS_TIME_SET_UNACK:
        // set_state.def_trans_time_set = *(uint8_t *)data;
        break;
    case ESP_BLE_MESH_MODEL_OP_GEN_ONPOWERUP_SET:
    case ESP_BLE_MESH_MODEL_OP_GEN_ONPOWERUP_SET_UNACK:
        // set_state.power_set = *(uint8_t *)data;
        break;
    case ESP_BLE_MESH_MODEL_OP_GEN_POWER_LEVEL_SET:
    case ESP_BLE_MESH_MODEL_OP_GEN_POWER_LEVEL_SET_UNACK:
        // set_state.power_level_set = *(uint8_t *)data;
        break;
    case ESP_BLE_MESH_MODEL_OP_GEN_POWER_DEFAULT_SET:
    case ESP_BLE_MESH_MODEL_OP_GEN_POWER_DEFAULT_SET_UNACK:
        // set_state.power_default_set = *(uint8_t *)data;
        break;
    case ESP_BLE_MESH_MODEL_OP_GEN_POWER_RANGE_SET:
    case ESP_BLE_MESH_MODEL_OP_GEN_POWER_RANGE_SET_UNACK:
        // set_state.power_range_set = *(uint8_t *)data;
        break;
    case ESP_BLE_MESH_MODEL_OP_GEN_LOC_GLOBAL_SET:
    case ESP_BLE_MESH_MODEL_OP_GEN_LOC_GLOBAL_SET_UNACK:
        memcpy(&set_state.loc_global_set, data, 10);
        break;
    case ESP_BLE_MESH_MODEL_OP_GEN_LOC_LOCAL_SET:
    case ESP_BLE_MESH_MODEL_OP_GEN_LOC_LOCAL_SET_UNACK:
        memcpy(&set_state.loc_local_set, &((uint8_t*)data)[10], 9);
        break;
    case ESP_BLE_MESH_MODEL_OP_GEN_USER_PROPERTY_SET:
    case ESP_BLE_MESH_MODEL_OP_GEN_USER_PROPERTY_SET_UNACK:
        // set_state.user_property_set = *(uint8_t *)data;
        break;
    case ESP_BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTY_SET:
    case ESP_BLE_MESH_MODEL_OP_GEN_ADMIN_PROPERTY_SET_UNACK:
        // set_state.admin_property_set = *(uint8_t *)data;
        break;
    case ESP_BLE_MESH_MODEL_OP_GEN_MANUFACTURER_PROPERTY_SET:
    case ESP_BLE_MESH_MODEL_OP_GEN_MANUFACTURER_PROPERTY_SET_UNACK:
        // set_state.manufacturer_property_set = *(uint8_t *)data;
        break;

    default:
        break;
    }

    return esp_ble_mesh_generic_client_set_state(&params, &set_state);
}

esp_err_t IGenericCliModel::getState(uint16_t address, uint32_t opcode)
{
    esp_ble_mesh_generic_client_get_state_t get_state;
    esp_ble_mesh_client_common_param_t params;
    auto ctx = getCtx();
    params.model = _model;
    params.opcode = opcode;
    params.msg_timeout = 0; /// menuconfig default timeout
    ctx->addr = address;
    params.ctx = *ctx;
    return esp_ble_mesh_generic_client_get_state(&params, &get_state);
}

/**
 * @brief 
 * 
 * @code {.cpp}
    auto status = ((esp_ble_mesh_generic_client_cb_param_t *)params)->status_cb.onoff_status;
    switch (event)
    {
    case ESP_BLE_MESH_GENERIC_CLIENT_PUBLISH_EVT:
    case ESP_BLE_MESH_GENERIC_CLIENT_GET_STATE_EVT:
        printf("\tonoff server status: %d\n", status.present_onoff);
        break;
    }
 * @endcode 
 *
 * @code {.cpp}
    auto status = ((esp_ble_mesh_generic_client_cb_param_t *)params)->status_cb.level_status;
    switch (event)
    {
    case ESP_BLE_MESH_GENERIC_CLIENT_PUBLISH_EVT:
    case ESP_BLE_MESH_GENERIC_CLIENT_GET_STATE_EVT:
        printf("\tonoff server status: %d\n", status.present_level);
        break;
    }
 * @endcode
 * 
 * @code {.cpp}
    auto status = ((esp_ble_mesh_generic_client_cb_param_t *)params)->status_cb;
    if(op_code == ESP_BLE_MESH_MODEL_OP_GEN_LOC_GLOBAL_STATUS) {
        auto loc = status.location_global_status;
        printf("\tlatitude: %ld\n", loc.global_latitude);
        printf("\tlongitude: %ld\n", loc.global_longitude);
        printf("\taltitude: %d\n", loc.global_altitude);
    } else if(op_code == ESP_BLE_MESH_MODEL_OP_GEN_LOC_LOCAL_STATUS) {
        auto loc = status.location_local_status;
        printf("\tnorth: %d\n", loc.local_north);
        printf("\teast: %d\n", loc.local_east);
        printf("\taltitude: %d\n", loc.local_altitude);
        printf("\tfloor: %d\n", loc.floor_number);
    }
 * @endcode
 * 
 *
 * @param event 
 * @param op_code 
 * @param params 
 */
void IGenericCliModel::onEvent(uint32_t event, uint32_t op_code, void *params)
{
    ESP_LOGD(__func__, "%s event: %ld, OP code: 0x%04lx\n", name(), event, op_code);
    if(cb)
        cb->onEvent(this, event, params);
}
