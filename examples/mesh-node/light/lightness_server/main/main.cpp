#include <stdio.h>

#include "esp_err.h"
#include "ble_mesh_light_lightness_models.h"

#include "ble_mesh_node.h"

class BLEmeshModelCallbacks : public BLEmeshModelCb
{
    void onSet(IBLEMeshModel *model, void *param) override;
    void onChange(IBLEMeshModel *model, void *param) override;
};

static auto lightnessSrv = new LightLightnessSrvModel("lightness 1s");
static auto lightnessSetup = new LightLightnessSetupModel("lightness 1ss");
static auto callback = new BLEmeshModelCallbacks();

extern "C" void app_main(void)
{
    auto device = BLEmeshNode::GetInstance();
    device->addPrimaryModel(lightnessSrv);
    device->addPrimaryModel(lightnessSetup);
    device->init_ble();
    device->init_mesh();
    device->registerCallbacks();
    device->enableProvisioning(2);
    lightnessSrv->setCb(callback);

    lightnessSrv->keys(0, 0); // hardcoded keys index for demo purpose; works with nrf mesh
    lightnessSetup->keys(0, 0); // hardcoded keys index for demo purpose; works with nrf mesh

    BLEmeshConfigSrv::proxy(1);
    BLEmeshConfigSrv::beacon(1);
    BLEmeshConfigSrv::relay(1);
}

void BLEmeshModelCallbacks::onSet(IBLEMeshModel *model, void *param)
{
    auto p = (esp_ble_mesh_lighting_server_cb_param_t *)param;
    printf("%s => callback set: %d\n", model->name(), p->value.set.lightness.lightness);
}

void BLEmeshModelCallbacks::onChange(IBLEMeshModel *model, void *param)
{
    auto p = (esp_ble_mesh_lighting_server_cb_param_t *)param;
    printf("%s => callback change: %d\n", model->name(), p->value.state_change.lightness_set.lightness);
}
