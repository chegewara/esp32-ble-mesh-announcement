#include "esp_ble_mesh_defs.h"

#include "ble_mesh_node.h"
#include "ble_mesh_provisioner.h"
#include "generic_model.h"

#include "callbacks.h"

#define TAG "mesh-cb"

void _default_ble_mesh_provisioning_cb(esp_ble_mesh_prov_cb_event_t event, esp_ble_mesh_prov_cb_param_t *params)
{
    printf("provisioning callback event: 0x%04x => %d\n", event, event);
    auto provisioner = BLEmeshProvisioner::GetInstance();

    switch (event)
    {
        case ESP_BLE_MESH_PROV_REGISTER_COMP_EVT:
            printf("ERROR: %d\n", params->prov_register_comp.err_code);
            break;
        case ESP_BLE_MESH_NODE_PROV_OUTPUT_NUMBER_EVT:{
            auto number = params->node_prov_output_num.number;
            auto action = params->node_prov_output_num.action;
            printf("\tnumber: %ld\n", number);
            break;
        }
        case ESP_BLE_MESH_PROVISIONER_RECV_UNPROV_ADV_PKT_EVT:
        {
            provisioner->onNewDevice(params);
            break;
        }
        case ESP_BLE_MESH_PROVISIONER_PROV_COMPLETE_EVT:
        {
            provisioner->onDeviceProvisionedCmpl(params);
            break;
        }
        case ESP_BLE_MESH_NODE_PROV_RESET_EVT:{
            auto node = IBLEmeshDevice::GetInstance();
            node->enableProvisioning();
            ESP_LOGI(TAG, "ESP_BLE_MESH_NODE_PROV_RESET_EVT");
            break;
        }
        default:
            break;
    }
}

void _default_ble_mesh_config_client_cb(esp_ble_mesh_cfg_client_cb_event_t event, esp_ble_mesh_cfg_client_cb_param_t *params)
{
    printf("callback event: 0x%04x\n", event);
    auto device = IBLEmeshDevice::GetInstance();
    uint32_t opcode;
    uint16_t addr;
    int err;

    opcode = params->params->opcode;
    addr = params->params->ctx.addr;

    ESP_LOGI(TAG, "%s, error_code = 0x%02x, event = 0x%02x, addr: 0x%04x, opcode: 0x%04" PRIx32,
             __func__, params->error_code, event, params->params->ctx.addr, opcode);

    if (params->error_code)
    {
        ESP_LOGE(TAG, "Send config client message failed, opcode 0x%04" PRIx32, opcode);
        return;
    }
    if (device)
    {
        auto model = device->findModel(params->params->model);
        if (model)
            model->onEvent(event, params->params->ctx.recv_op, params);
        else
            ESP_LOGE(TAG, "missing config client model");
    }
}

void _default_ble_mesh_config_server_cb(esp_ble_mesh_cfg_server_cb_event_t event, esp_ble_mesh_cfg_server_cb_param_t *params)
{
    printf("cfg srv callback event: %d\n", event);
    auto device = IBLEmeshDevice::GetInstance();
    if (device)
    {
        auto model = device->findModel(params->model);
        if (model)
            model->onEvent(event, params->ctx.recv_op, params);
        else
            ESP_LOGE(TAG, "missing config server model");
    }
}

void _default_ble_mesh_generic_client_cb(esp_ble_mesh_generic_client_cb_event_t event, esp_ble_mesh_generic_client_cb_param_t *params)
{
    printf("generic client callback event: 0x%04x\n", event);

    auto device = IBLEmeshDevice::GetInstance();
    IBLEMeshModel* model = NULL;
    if(device)
        model = device->findModel(params->params->model);
    if(model)
        model->onEvent(event, params->params->ctx.recv_op, params);
}

void _default_ble_mesh_generic_server_cb(esp_ble_mesh_generic_server_cb_event_t event, esp_ble_mesh_generic_server_cb_param_t *params)
{
    printf("generic server callback event: %d\n", event);
    auto device = IBLEmeshDevice::GetInstance();

    IBLEMeshModel *model = nullptr;
    if (device)
    {
        model = device->findModel(params->model);
        if (model)
            model->onEvent(event, params->ctx.recv_op, params);
        else
            ESP_LOGE(TAG, "missing generic server model");
    }
}

void _default_ble_mesh_lighting_client_cb(esp_ble_mesh_light_client_cb_event_t event, esp_ble_mesh_light_client_cb_param_t *params)
{
    printf("lightness client callback event: %d\n", event);
    auto device = IBLEmeshDevice::GetInstance();
    IBLEMeshModel* model = NULL;
    if(device)
        model = device->findModel(params->params->model);
    if(model)
        model->onEvent(event, params->params->ctx.recv_op, params);
}

void _default_ble_mesh_lighting_server_cb(esp_ble_mesh_lighting_server_cb_event_t event, esp_ble_mesh_lighting_server_cb_param_t *params)
{
    printf("lightness server callback event: %d\n", event);
    auto device = IBLEmeshDevice::GetInstance();
    if (device)
    {
        auto model = device->findModel(params->model);
        model->onEvent(event, params->ctx.recv_op, params);
    }
}

void _default_ble_mesh_time_scene_client_cb(esp_ble_mesh_time_scene_client_cb_event_t event, esp_ble_mesh_time_scene_client_cb_param_t *params)
{

}

void _default_ble_mesh_time_scene_server_cb(esp_ble_mesh_time_scene_server_cb_event_t event, esp_ble_mesh_time_scene_server_cb_param_t *params)
{
    printf("time, scene server callback event: %d\n", event);
    auto device = IBLEmeshDevice::GetInstance();
    if (device)
    {
        auto model = device->findModel(params->model);
        model->onEvent(event, params->ctx.recv_op, params);
    }
}

void _default_ble_mesh_sensor_client_cb(esp_ble_mesh_sensor_client_cb_event_t event, esp_ble_mesh_sensor_client_cb_param_t *params)
{

}

void _default_ble_mesh_sensor_server_cb(esp_ble_mesh_sensor_server_cb_event_t event, esp_ble_mesh_sensor_server_cb_param_t *params)
{

}

void _default_ble_mesh_custom_model_cb(esp_ble_mesh_model_cb_event_t event, esp_ble_mesh_model_cb_param_t *params)
{
    // printf("vendor callback event: %d\n", event);
    auto device = IBLEmeshDevice::GetInstance();
    IBLEMeshModel *model = nullptr;

    switch (event)
    {
        case ESP_BLE_MESH_MODEL_OPERATION_EVT:
        {
            if (device)
            {
                model = device->findModel(params->model_operation.model);
                if (model)
                    model->onEvent(event, params->model_operation.ctx->recv_op, params);
                else
                    ESP_LOGE(TAG, "missing vendor server model");
            }
            break;
        }
        case ESP_BLE_MESH_MODEL_PUBLISH_COMP_EVT: {
            if (device)
            {
                model = device->findModel(params->model_publish_comp.model);
                if (model)
                    model->onEvent(event, -1, params);
            }
            break;
        }

        default:
            ESP_LOGE(TAG, "not handled custom model event: %d", event);
            break;
    }
}



