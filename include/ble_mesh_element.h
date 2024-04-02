#pragma once
#include "stdio.h"
#include <vector>

#include "esp_ble_mesh_defs.h"

#include "ble_mesh_model.h"
#include "ble_mesh_config_srv.h"
#include "ble_mesh_health_srv.h"

class BLEmeshElement
{
protected:
    esp_ble_mesh_elem_t* element = nullptr;
    std::vector<IBLEMeshModel*> models;
    esp_ble_mesh_model_t sig_models[20] = {};
    esp_ble_mesh_model_t vnd_models[10] = {};
    
public:
    BLEmeshElement() {}
    ~BLEmeshElement() {}

    void addModel(IBLEMeshModel* mdl, bool init = true)
    {
        models.push_back(mdl);
        if(init) mdl->init(NULL);
    }

    IBLEMeshModel* getModel(uint8_t n) {
        return models.at(n);
    }

    size_t count() {
        size_t count = models.size();
        // assert(count == esp_ble_mesh_get_element_count());
        return count;
    }

    esp_ble_mesh_elem_t* getMeshElement() {
        if(!element) {
            prepareElement();
        }

        return element;
    }

    void addHealthSrv(BLEmeshHealthSrv* mdl) {
        models.push_back(mdl);
    }

    void prepareElement();
};

class BLEmeshPrimaryElement : public BLEmeshElement
{
private:
    
public:
    BLEmeshPrimaryElement() {}
    ~BLEmeshPrimaryElement() {}

    void addCfgSrv(BLEmeshConfigSrv* mdl) {
        addModel(mdl, true);
    }
};
