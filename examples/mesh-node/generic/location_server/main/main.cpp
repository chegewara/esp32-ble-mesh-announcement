#include <stdio.h>

#include "esp_err.h"
#include "generic_location_models.h"
#include "ble_mesh_node.h"

#include "ble_mesh_time_models.h"

class BLEmeshModelCallbacks : public BLEmeshModelCb
{
    void onSet(IBLEMeshModel *model, void *param) override;
    void onChange(IBLEMeshModel *model, void *param) override;
};

static auto locationSrv = new GenericLocationSrvModel("location 1s");
static auto locationSetup = new GenericLocationSetupModel("location 1ss");
static auto locationSrvCb = new BLEmeshModelCallbacks();

static void example_add_elements_models()
{
    auto device = BLEmeshNode::GetInstance();
    auto primary = device->rootElement();

    {
        primary->addModel(locationSrv);
        primary->addModel(locationSetup, false);
        locationSetup->init(locationSrv);
        locationSrv->setCb(locationSrvCb);
        locationSetup->setCb(locationSrvCb);
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

    locationSrv->keys(0, 0); // hardcoded keys index for demo purpose; works with nrf mesh
    locationSetup->keys(0, 0); // hardcoded keys index for demo purpose; works with nrf mesh

    BLEmeshConfigSrv::proxy(1);
    BLEmeshConfigSrv::beacon(1);
    BLEmeshConfigSrv::relay(1);
}

void BLEmeshModelCallbacks::onSet(IBLEMeshModel *model, void *param)
{
    auto p = (esp_ble_mesh_generic_server_cb_param_t *)param;
    printf("%s => callback set: %d\n", model->name(), p->value.set.location_local.local_north);
}

void BLEmeshModelCallbacks::onChange(IBLEMeshModel *model, void *param)
{
    auto p = (esp_ble_mesh_generic_server_cb_param_t *)param;
    printf("%s => callback change: %d\n", model->name(), p->value.state_change.loc_local_set.north);
}
