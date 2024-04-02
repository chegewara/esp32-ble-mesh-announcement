#include "ble_mesh_light_lightness_models.h"

void LightLightnessSrvModel::onEvent(uint32_t event, uint32_t op_code, void *params)
{
    ESP_LOGD(__func__, "%s event: %ld, OP code: 0x%04lx\n", name(), event, op_code);
    if(cb)
        cb->onEvent(this, event, params);

    auto *param = (esp_ble_mesh_lighting_server_cb_param_t *)params;
    auto srv = ((esp_ble_mesh_light_lightness_srv_t *)params)->model->user_data;

    switch (event)
    {
    case ESP_BLE_MESH_LIGHTING_SERVER_STATE_CHANGE_EVT:
        if (cb)
            cb->onChange(this, params);
        break;
    case ESP_BLE_MESH_LIGHTING_SERVER_RECV_SET_MSG_EVT:
        if (cb)
            cb->onSet(this, params);
        break;

    default:
        break;
    }
}
