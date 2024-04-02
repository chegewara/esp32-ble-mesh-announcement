#include <stdio.h>

#include "esp_err.h"
#include "ble_mesh_node.h"

#include "vendor_model.h"

#define CID 0x1234
#define VID 0x5678
#define VID1 0x5679
#define ESP_BLE_MESH_VND_MODEL_OP_SEND1 ESP_BLE_MESH_MODEL_OP_3(0x01, CID)
#define ESP_BLE_MESH_VND_MODEL_OP_SEND2 ESP_BLE_MESH_MODEL_OP_3(0x02, CID)
#define ESP_BLE_MESH_VND_MODEL_OP_SEND3 ESP_BLE_MESH_MODEL_OP_3(0x03, CID)
#define ESP_BLE_MESH_VND_MODEL_OP_STATUS ESP_BLE_MESH_MODEL_OP_3(0x04, CID)

static esp_ble_mesh_model_op_t vnd_op[] = {
    ESP_BLE_MESH_MODEL_OP(ESP_BLE_MESH_VND_MODEL_OP_SEND1, 1),
    ESP_BLE_MESH_MODEL_OP(ESP_BLE_MESH_VND_MODEL_OP_SEND2, 2),
    ESP_BLE_MESH_MODEL_OP(ESP_BLE_MESH_VND_MODEL_OP_SEND3, 2),
    ESP_BLE_MESH_MODEL_OP_END,
};

static esp_ble_mesh_client_op_pair_t vnd_op_pair[] = {
    { ESP_BLE_MESH_VND_MODEL_OP_SEND1, ESP_BLE_MESH_VND_MODEL_OP_STATUS },
    { ESP_BLE_MESH_VND_MODEL_OP_SEND2, ESP_BLE_MESH_VND_MODEL_OP_STATUS },
    { ESP_BLE_MESH_VND_MODEL_OP_SEND3, ESP_BLE_MESH_VND_MODEL_OP_STATUS },
};

static esp_ble_mesh_model_op_t vnd_op1[] = {
    ESP_BLE_MESH_MODEL_OP(ESP_BLE_MESH_VND_MODEL_OP_STATUS, 2),
    ESP_BLE_MESH_MODEL_OP_END,
};

class VendorCallback : public BLEmeshModelCb
{
    void onEvent(IBLEMeshModel *model, uint32_t event, void *params) override;
};

auto vendorSrv = new VendorSrvModel("vendor 1s");
auto vendorCli = new VendorSrvModel("vendor 1c");

static void example_add_elements_models()
{
    auto device = BLEmeshNode::GetInstance();
    auto primary = device->rootElement();
    auto callback = new VendorCallback();

    {
        primary->addModel(vendorSrv);
        vendorSrv->init(CID, VID, vnd_op, vnd_op_pair, 3);
        vendorSrv->setCb(callback);
    }

    {
        auto elem1 = new BLEmeshElement();
        elem1->addModel(vendorCli);
        vendorCli->init(CID, VID1, vnd_op, vnd_op_pair, 3);
        device->addElement(elem1);
        vendorCli->setCb(callback);
    }
}

extern "C" void app_main(void)
{
    auto device = BLEmeshNode::GetInstance();

    example_add_elements_models();

    device->init_ble();
    device->init_mesh();
    device->registerCallbacks();
    device->enableProvisioning();

    vendorCli->keys(0, 0); // hardcoded keys index for demo purpose; works with nrf mesh
    vendorSrv->keys(0, 0); // hardcoded keys index for demo purpose; works with nrf mesh

    BLEmeshConfigSrv::proxy(1);
    BLEmeshConfigSrv::beacon(1);
    BLEmeshConfigSrv::relay(1);
}


void VendorCallback::onEvent(IBLEMeshModel *model, uint32_t event, void *params)
{
    printf("\tmodel (%s) event\n", model->name());

    auto param = (esp_ble_mesh_model_cb_param_t*)params;

    switch (event)
    {
        case ESP_BLE_MESH_MODEL_OPERATION_EVT:
        {
            auto val = param->model_operation;
            auto opcode = val.opcode;
            auto len = val.length;
            auto data = val.msg;
            ESP_LOG_BUFFER_HEXDUMP(model->name(), data, len, ESP_LOG_INFO);
            printf("\topcode: 0x%06lx\n", opcode);
            break;
        }

        default:
            break;
    }
}
