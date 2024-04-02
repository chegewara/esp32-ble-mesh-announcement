#include "ble_mesh_provisioner.h"

#define TAG ""

static struct esp_ble_mesh_key
{
    uint16_t net_idx = 0;
    uint16_t app_idx = 0;
    uint8_t net_key[16] = {0x54, 0x63, 0x14, 0x4a, 0x3c, 0x4a, 0xf3, 0xc6, 0x98, 0xac, 0x3d, 0x5d, 0x07, 0x79, 0x75, 0x27};
    uint8_t app_key[16] = {0xa5, 0xcc, 0x2a, 0x1a, 0xd2, 0x99, 0x06, 0x7b, 0x8f, 0x39, 0x8a, 0xf3, 0xe0, 0x11, 0x22, 0x9c};
} prov_key;

BLEmeshConfigCli *BLEmeshProvisioner::_configCli = nullptr;

BLEmeshProvisioner *BLEmeshProvisioner::GetInstance()
{
    if (singleton_ == nullptr)
    {
        singleton_ = new BLEmeshProvisioner();
        _configCli = new BLEmeshConfigCli("conf cli");
        primary_element->addModel(_configCli);
    }
    return static_cast<BLEmeshProvisioner *>(singleton_);
}

esp_err_t BLEmeshProvisioner::enableProvisioning(uint8_t bearer)
{
    esp_ble_mesh_provisioner_prov_disable((esp_ble_mesh_prov_bearer_t)bearer);
    return esp_ble_mesh_provisioner_prov_enable((esp_ble_mesh_prov_bearer_t)bearer);
}

esp_err_t BLEmeshProvisioner::filter()
{
    esp_err_t err = ESP_OK;
    uint8_t match[2] = {0xdd, 0xdd};

    err = esp_ble_mesh_provisioner_set_dev_uuid_match(match, sizeof(match), 0x0, false);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to set matching device uuid (err %d)", err);
    }
    return err;
}

esp_err_t BLEmeshProvisioner::selfProvisioning()
{
    esp_err_t err = ESP_OK;
    provision.uuid = dev_uuid;
    provision.prov_uuid = dev_uuid;

    ESP_LOG_BUFFER_HEXDUMP("NET KEY", esp_ble_mesh_provisioner_get_local_net_key(0), 16, ESP_LOG_INFO);
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_ble_mesh_provisioner_update_local_net_key(prov_key.net_key, 0));


    err = esp_ble_mesh_provisioner_add_local_app_key(prov_key.app_key, 0, 0);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to add local AppKey (err %d)", err);
        return err;
    }
    esp_ble_mesh_provisioner_add_local_app_key(NULL, 0, 0xffff);

    ESP_LOG_BUFFER_HEXDUMP("NET KEY", esp_ble_mesh_provisioner_get_local_net_key(0), 16, ESP_LOG_INFO);
    ESP_LOG_BUFFER_HEXDUMP("APP KEY", esp_ble_mesh_provisioner_get_local_app_key(0, 0), 16, ESP_LOG_INFO);

// esp_ble_mesh_provisioner_bind_app_key_to_local_model(0x0001, 0, 0x1001, 0xffff);

// esp_ble_mesh_model_subscribe_group_addr(0x0001, 0xffff, 0x1001, 0xc000);
// // esp_ble_mesh_model_subscribe_group_addr(0x0001, 0xffff, 0x1000, 0xc000);
    return err;
}

void BLEmeshProvisioner::recv_unprov_adv_pkt(uint8_t dev_uuid[16], uint8_t addr[BD_ADDR_LEN], esp_ble_mesh_addr_type_t addr_type,
                                             uint16_t oob_info, uint8_t adv_type, esp_ble_mesh_prov_bearer_t bearer)
{
    if (devices.find(bt_hex(dev_uuid, 16)) != devices.end()) return;

    printf("%s:%d\n", __FILE__, __LINE__);

    esp_ble_mesh_unprov_dev_add_t add_dev = {};
    int err;

    /* Due to the API esp_ble_mesh_provisioner_set_dev_uuid_match, Provisioner will only
     * use this callback to report the devices, whose device UUID starts with 0xdd & 0xdd,
     * to the application layer.
     */

    ESP_LOGW(TAG, "address: %s, address type: %d, bearer type: %d", bt_hex(addr, BD_ADDR_LEN), addr_type, adv_type);
    ESP_LOGW(TAG, "device uuid: %s", bt_hex(dev_uuid, 16));
    ESP_LOGW(TAG, "oob info: %d, bearer: %s", oob_info, (bearer & ESP_BLE_MESH_PROV_ADV) ? "PB-ADV" : "PB-GATT");

    memcpy(add_dev.addr, addr, BD_ADDR_LEN);
    add_dev.addr_type = (esp_ble_mesh_addr_type_t)addr_type;
    memcpy(add_dev.uuid, dev_uuid, 16);
    add_dev.oob_info = oob_info;
    add_dev.bearer = (esp_ble_mesh_prov_bearer_t)bearer;

    devices[bt_hex(dev_uuid, 16)] = add_dev;
}

void BLEmeshProvisioner::addUnprovisioned(std::string uuid)
{
    auto dev = devices[uuid];

    /* Note: If unprovisioned device adv packets have not been received, we should not add
             device with ADD_DEV_START_PROV_NOW_FLAG set. */
    esp_err_t err = esp_ble_mesh_provisioner_add_unprov_dev(&dev,
                            (esp_ble_mesh_dev_add_flag_t)(ADD_DEV_RM_AFTER_PROV_FLAG | ADD_DEV_FLUSHABLE_DEV_FLAG | ADD_DEV_START_PROV_NOW_FLAG));
    if (err)
    {
        ESP_LOGE(TAG, "%s: Add unprovisioned device into queue failed", __func__);
        return;
    }

    devices.erase(uuid);

    // esp_ble_mesh_provisioner_prov_device_with_addr();   // assign unicast address of choice
}

// void provisioner_on_new_device_cb(const char* dev_uuid);
void BLEmeshProvisioner::onNewDevice(esp_ble_mesh_prov_cb_param_t *param)
{
    // printf("%s:%d\n", __FILE__, __LINE__);
    recv_unprov_adv_pkt(param->provisioner_recv_unprov_adv_pkt.dev_uuid, param->provisioner_recv_unprov_adv_pkt.addr,
                        param->provisioner_recv_unprov_adv_pkt.addr_type, param->provisioner_recv_unprov_adv_pkt.oob_info,
                        param->provisioner_recv_unprov_adv_pkt.adv_type, param->provisioner_recv_unprov_adv_pkt.bearer);
}
