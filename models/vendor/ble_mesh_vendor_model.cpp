#include "vendor_model.h"

void VendorSrvModel::init(uint16_t cid, uint16_t vid, esp_ble_mesh_model_op_t* vnd_op, esp_ble_mesh_client_op_pair_t* vnd_op_pair, size_t count)
{
    model_id = vid;
    company_id = cid;

    vendor_client.op_pair = vnd_op_pair;
    vendor_client.op_pair_size = count;

    BLE_MESH_MODEL_PUB_DEFINE(&pub, 3 + 400, ROLE_NODE, __COUNTER__);
    pub.msg->len = 2;
    _model = new esp_ble_mesh_model_t(ESP_BLE_MESH_VENDOR_MODEL(cid, vid, vnd_op, &pub, &vendor_client));
}

void VendorSrvModel::onEvent(uint32_t event, uint32_t op_code, void *params)
{
    printf("%s event: %ld, opcode: 0x%06lx\n", name(), event, op_code);
    auto param = (esp_ble_mesh_model_cb_param_t*)params;
    if(cb)
        cb->onEvent(this, event, params);
}
