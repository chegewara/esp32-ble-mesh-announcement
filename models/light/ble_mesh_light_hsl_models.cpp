#include "ble_mesh_light_hsl_models.h"

void LightHSLSetupModel::onEvent(uint32_t event, uint32_t op_code, void *params)
{
    ESP_LOGD(__func__, "%s event: %ld, OP code: 0x%04lx\n", name(), event, op_code);
    if(cb)
        cb->onEvent(this, event, params);

}

void LightHSLSrvModel::onEvent(uint32_t event, uint32_t op_code, void *params)
{
    ESP_LOGD(__func__, "%s event: %ld, OP code: 0x%04lx\n", name(), event, op_code);
    if(cb)
        cb->onEvent(this, event, params);

    auto srv = ((esp_ble_mesh_lighting_server_cb_param_t *)params)->model->user_data;
}

void LightSaturationSrvModel::onEvent(uint32_t event, uint32_t op_code, void *params)
{
    ESP_LOGD(__func__, "%s event: %ld, OP code: 0x%04lx\n", name(), event, op_code);
    if(cb)
        cb->onEvent(this, event, params);

    auto srv = ((esp_ble_mesh_lighting_server_cb_param_t *)params)->model->user_data;
}

void LightHueSrvModel::onEvent(uint32_t event, uint32_t op_code, void *params)
{
    ESP_LOGD(__func__, "%s event: %ld, OP code: 0x%04lx\n", name(), event, op_code);
    if(cb)
        cb->onEvent(this, event, params);

    auto srv = ((esp_ble_mesh_lighting_server_cb_param_t *)params)->model->user_data;
}
