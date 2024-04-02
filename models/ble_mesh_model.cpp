#include "ble_mesh_model.h"

esp_err_t IBLEMeshModel::bindLocalAppKey(uint8_t appIdx)
{
    auto addr = esp_ble_mesh_get_primary_element_address() + _model->element_idx;
    ESP_LOGE("model", "bind local model: 0x%04x, model_id: 0x%04x, appIdx: %d", addr, model_id, appIdx);

#if CONFIG_BLE_MESH_PROVISIONER
    if(bt_mesh_is_provisioner())
        return esp_ble_mesh_provisioner_bind_app_key_to_local_model(addr, appIdx, model_id, company_id);
#endif
#if CONFIG_BLE_MESH_NODE
    if(bt_mesh_is_provisioned())
        return esp_ble_mesh_node_bind_app_key_to_local_model(addr, company_id, model_id, appIdx);
#endif    
    return ESP_FAIL;
}

