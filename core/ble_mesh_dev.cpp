#include <stdio.h>
#include <string.h>

#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gatt_common_api.h"

#include "esp_gatt_common_api.h"
#include "esp_ble_mesh_defs.h"
#include "esp_ble_mesh_common_api.h"
#include "esp_ble_mesh_networking_api.h"
#include "esp_ble_mesh_provisioning_api.h"
#include "esp_ble_mesh_config_model_api.h"
#include "esp_ble_mesh_generic_model_api.h"
#include "esp_ble_mesh_local_data_operation_api.h"
#include "esp_ble_mesh_lighting_model_api.h"
#include "esp_ble_mesh_time_scene_model_api.h"

#include "ble_mesh_dev.h"

#define TAG "ble-mesh-dev"

/****************************************************************************************************/
extern "C" void add_elements(esp_ble_mesh_elem_t elements[], esp_ble_mesh_elem_t *model, int count);
IBLEmeshDevice *IBLEmeshDevice::singleton_ = nullptr;
BLEmeshPrimaryElement *IBLEmeshDevice::primary_element = nullptr;
/****************************************************************************************************/

void IBLEmeshDevice::registerCallbacks()
{
    ESP_LOGW(TAG, "register callbacks");
    ESP_ERROR_CHECK(esp_ble_mesh_register_prov_callback(_default_ble_mesh_provisioning_cb));

    for (size_t j = 0; j < elements.size(); j++)
    {
        auto el = elements.at(j);
        for (size_t i = 0; i < el->count(); i++)
        {
            auto model = el->getModel(i);
            if(model->is_vendor())
                printf("\telement id: %d, model id: 0x%04x, company id: 0x%04x\n", j, model->get()->vnd.model_id, model->get()->vnd.company_id);
            else
                printf("\telement id: %d, model id: 0x%04x\n", j, model->get()->model_id);
            ESP_ERROR_CHECK(model->registerCallback());        
        }
    }
}

IBLEmeshDevice *IBLEmeshDevice::GetInstance()
{
    // ESP_LOGE("", "never call IBLEmeshDevice::GetInstance(), IBLEmeshDevice is abstract class");;
    // assert(0);
    return singleton_;
}

esp_err_t IBLEmeshDevice::init_ble()
{
    esp_err_t err;

    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES)
    {
        printf("flash init error: %d\n\n\n", err);
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    esp_err_t ret = esp_bt_controller_init(&bt_cfg);
    if (ret)
    {
        ESP_LOGE(TAG, "%s initialize controller failed", __func__);
        return ret;
    }

    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (ret)
    {
        ESP_LOGE(TAG, "%s enable controller failed", __func__);
        return ret;
    }
    ret = esp_bluedroid_init();
    if (ret)
    {
        ESP_LOGE(TAG, "%s init bluetooth failed", __func__);
        return ret;
    }
    ret = esp_bluedroid_enable();
    if (ret)
    {
        ESP_LOGE(TAG, "%s enable bluetooth failed", __func__);
        return ret;
    }
    
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT, ESP_PWR_LVL_P9);

    return ESP_OK;
}

esp_err_t IBLEmeshDevice::init_mesh(uint16_t cid)
{
    addModels();
    memcpy(dev_uuid + 2, esp_bt_dev_get_address(), BD_ADDR_LEN);

    provision.uuid = dev_uuid;

    composition = {
        .cid = cid,
        .element_count = el_cnt,
        .elements = sig_mesh_elements,
    };

    printf("el_count: %d\n\n", el_cnt);

    esp_err_t err = esp_ble_mesh_init(&provision, &composition);

    if (esp_ble_mesh_node_is_provisioned()){
        restoreContext();
    }
    return err;
}

BLEmeshElement* IBLEmeshDevice::findElement(esp_ble_mesh_model_t* mdl)
{
    for (size_t j = 0; j < elements.size(); j++)
    {
        auto cnt = elements.at(j)->count();
        for (size_t i = 0; i < cnt; i++)
        {
            auto model = elements.at(j)->getModel(i);

            if (model->model(mdl))
            {
                ESP_LOGI(TAG, "ble device: model found");
                return elements.at(j);
            }
        }
    }
    return NULL;
}

IBLEMeshModel *IBLEmeshDevice::findModel(uint16_t model_id)
{
    for (size_t j = 0; j < elements.size(); j++)
    {
        auto cnt = elements.at(j)->count();
        for (size_t i = 0; i < cnt; i++)
        {
            auto model = elements.at(j)->getModel(i);

            if (model->get()->model_id == model_id)
            {
                ESP_LOGI(TAG, "ble device: model found");
                return model;
            }
        }
    }
    return NULL;
}

IBLEMeshModel *IBLEmeshDevice::findModel(uint16_t address, uint16_t model_id, uint16_t company_id)
{
    for (size_t j = 0; j < elements.size(); j++)
    {
        auto cnt = elements.at(j)->count();
        for (size_t i = 0; i < cnt; i++)
        {
            auto model = elements.at(j)->getModel(i);
            auto uni = esp_ble_mesh_get_primary_element_address();

            if (company_id == 0xffff && model->get()->model_id == model_id)
            {
                ESP_LOGI(TAG, "ble device: sig model found");
                return model;
            } else if(model->get()->vnd.model_id == model_id && model->get()->vnd.company_id == company_id ) {
                ESP_LOGI(TAG, "ble device: vendor model found");
                return model;
            }
        }
    }
    return NULL;
}

IBLEMeshModel *IBLEmeshDevice::findModel(esp_ble_mesh_model_t *mdl)
{
    for (size_t j = 0; j < elements.size(); j++)
    {
        auto cnt = elements.at(j)->count();
        for (size_t i = 0; i < cnt; i++)
        {
            auto model = elements.at(j)->getModel(i);

            if (model->model(mdl))
            {
                ESP_LOGD(TAG, "ble device: model found");
                return model;
            }
        }
    }
    return NULL;
}

void IBLEmeshDevice::addModels()
{
    assert(primary_element);
    sig_mesh_elements = (esp_ble_mesh_elem_t *)calloc(elements.size(), sizeof(esp_ble_mesh_elem_t));

    for (size_t i = 0; i < elements.size(); i++)
    {
        add_elements(sig_mesh_elements, elements.at(i)->getMeshElement(), el_cnt++);
    }
}
