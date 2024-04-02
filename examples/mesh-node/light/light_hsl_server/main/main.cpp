#include <stdio.h>

#include "esp_err.h"
#include "ble_mesh_light_hsl_models.h"
#include "ble_mesh_light_lightness_models.h"


#include "ble_mesh_node.h"

class BLEmeshModelCallbacks : public BLEmeshModelCb
{
    void onEvent(IBLEMeshModel *model, uint32_t event, void *param) override;
};

static auto lightSrv = new LightHSLSrvModel("hsl light 1s");
static auto lightSetup = new LightHSLSetupModel("hsl 1ss");
static auto hueSrv = new LightHueSrvModel("hsl hue 1s");
static auto saturationSrv = new LightSaturationSrvModel("hsl sat 1s");

static auto lightnessSrv = new LightLightnessSrvModel("lightness 1s");
static auto lightnessSetup = new LightLightnessSetupModel("lightness 1ss");

static auto callback = new BLEmeshModelCallbacks();

// https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/libraries/bluetooth_services/mesh/light_hsl_srv.html#model-composition
extern "C" void app_main(void)
{
    auto device = BLEmeshNode::GetInstance();
    device->init_ble();
    device->addPrimaryModel(lightnessSrv);
    device->addPrimaryModel(lightnessSetup);
    device->addPrimaryModel(lightSetup);
    device->addPrimaryModel(lightSrv);

    auto elem1 = new BLEmeshElement();
    elem1->addModel(saturationSrv);
    auto elem2 = new BLEmeshElement();
    elem2->addModel(hueSrv);
    device->addElement(elem2);
    device->addElement(elem1);

    lightSrv->setCb(callback);
    hueSrv->setCb(callback);
    saturationSrv->setCb(callback);
    lightnessSrv->setCb(callback);

    lightnessSrv->keys(0, 0); // hardcoded keys index for demo purpose; works with nrf mesh
    lightnessSetup->keys(0, 0); // hardcoded keys index for demo purpose; works with nrf mesh
    lightSrv->keys(0, 0); // hardcoded keys index for demo purpose; works with nrf mesh
    lightSetup->keys(0, 0); // hardcoded keys index for demo purpose; works with nrf mesh
    saturationSrv->keys(0, 0); // hardcoded keys index for demo purpose; works with nrf mesh
    hueSrv->keys(0, 0); // hardcoded keys index for demo purpose; works with nrf mesh

    device->init_mesh();
    device->registerCallbacks();
    device->enableProvisioning();

    BLEmeshConfigSrv::proxy(1);
    BLEmeshConfigSrv::beacon(1);
    BLEmeshConfigSrv::relay(1);
}

void BLEmeshModelCallbacks::onEvent(IBLEMeshModel *model, uint32_t event, void *param)
{
    auto p = (esp_ble_mesh_lighting_server_cb_param_t *)param;
    printf("%s => callback set: %d\n", model->name(), p->value.set.lightness.lightness);
}
