#pragma once


#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_ble_mesh_defs.h"
#include "esp_ble_mesh_common_api.h"
#include "esp_ble_mesh_networking_api.h"
#include "esp_ble_mesh_provisioning_api.h"
#include "esp_ble_mesh_config_model_api.h"
#include "esp_ble_mesh_generic_model_api.h"
#include "esp_ble_mesh_local_data_operation_api.h"

#include "ble_mesh_prov.h"
#include "ble_mesh_config_srv.h"
#include "ble_mesh_model.h"
#include "ble_mesh_element.h"
#include "ble_mesh_config_cli.h"


#include "vector"


class IBLEmeshDevice : public BLEmeshProvisioning
{
protected:
    IBLEmeshDevice() {
        primary_element = new BLEmeshPrimaryElement();
        addElement(primary_element);
        _configSrv = new BLEmeshConfigSrv("conf srv");
        primary_element->addCfgSrv(_configSrv);
    }

    static IBLEmeshDevice* singleton_;

    static BLEmeshPrimaryElement* primary_element;

    std::vector<BLEmeshElement*> elements;

    uint8_t dev_uuid[16] = { 0xdd, 0xdd };

    esp_ble_mesh_comp_t composition = {};

    esp_ble_mesh_elem_t* sig_mesh_elements = nullptr;

    uint16_t primary_address;

    size_t el_cnt = 0;

    BLEmeshConfigSrv* _configSrv = nullptr;
public:

    virtual void registerCallbacks();

    /**
     * Singletons should not be cloneable.
     */
    IBLEmeshDevice(IBLEmeshDevice &other) = delete;
    /**
     * Singletons should not be assignable.
     */
    void operator=(const IBLEmeshDevice &) = delete;
    /**
     * This is the static method that controls the access to the singleton
     * instance. On the first run, it creates a singleton object and places it
     * into the static field. On subsequent runs, it returns the client existing
     * object stored in the static field.
     */

    static IBLEmeshDevice *GetInstance();
    ~IBLEmeshDevice() = default;

    virtual esp_err_t init_ble();

    virtual esp_err_t init_mesh(uint16_t cid = CID_ESP);

    void addElement(BLEmeshElement* el) {
        elements.push_back(el);
    }

    BLEmeshPrimaryElement* rootElement() {
        return primary_element;
    }

    void addPrimaryModel(IBLEMeshModel* model) {
        elements.at(0)->addModel(model);
    }

    virtual BLEmeshElement* findElement(esp_ble_mesh_model_t* mdl);

    virtual IBLEMeshModel* findModel(uint16_t model_id);

    virtual IBLEMeshModel* findModel(uint16_t address, uint16_t model_id, uint16_t company_id = 0xffff);

    virtual IBLEMeshModel* findModel(esp_ble_mesh_model_t* mdl);

    virtual BLEmeshElement* getElement(uint16_t address) {
        uint8_t cnt = address - esp_ble_mesh_get_primary_element_address() - 1;
        return elements.at(cnt);
    }

    virtual BLEmeshConfigSrv* configSrv() {
        return _configSrv;
    }

private:
    virtual void restoreContext()
    {
        for (size_t i = 0; i < elements.size(); i++)
        {
            auto el = elements.at(i);
            for (size_t j = 0; j < el->count(); j++)
            {
                auto model = el->getModel(j);
                auto mdl = model->get();
                model->appKey(mdl->keys[0]);

                for (size_t n = 0; n < CONFIG_BLE_MESH_SUBNET_COUNT; n++)
                {
                    if(esp_ble_mesh_node_get_local_net_key(n) != NULL)
                    {
                        model->netKey(n);
                    }
                }
            }
        }
    }


    void addModels();
};
