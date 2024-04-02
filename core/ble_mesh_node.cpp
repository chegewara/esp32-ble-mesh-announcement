#include "ble_mesh_node.h"

BLEmeshNode *BLEmeshNode::GetInstance()
{
    if (singleton_ == nullptr)
    {
        singleton_ = new BLEmeshNode();
    }
    return static_cast<BLEmeshNode *>(singleton_);
}

esp_err_t BLEmeshNode::enableProvisioning(uint8_t bearer)
{
    return esp_ble_mesh_node_prov_enable((esp_ble_mesh_prov_bearer_t)bearer);
}
