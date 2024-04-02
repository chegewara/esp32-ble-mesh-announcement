#include "ble_mesh_element.h"

extern "C" void copy_memcpy(esp_ble_mesh_model_t* models, esp_ble_mesh_model_t* model);


void BLEmeshElement::prepareElement()
{
    uint8_t sig_cnt = 0;
    uint8_t vnd_cnt = 0;
    for (size_t i = 0; i < models.size(); i++)
    {
        auto _mdl = models.at(i);
        printf("model ID: 0x%04x\n", _mdl->model_id);
        auto mdl = _mdl->get_();
        if(_mdl->is_vendor()) {
            copy_memcpy(&vnd_models[vnd_cnt], mdl);
            models.at(i)->replace(&vnd_models[vnd_cnt]);
            vnd_cnt += 1;
        } else {
            copy_memcpy(&sig_models[sig_cnt], mdl);
            models.at(i)->replace(&sig_models[sig_cnt]);
            sig_cnt += 1;
        }
    }

    /// \brief for location value see
    /// \see https://bitbucket.org/bluetooth-SIG/public/src/main/assigned_numbers/core/namespace.yaml
    element = new (esp_ble_mesh_elem_t) {
        .location = 0,
        .sig_model_count = sig_cnt,
        .vnd_model_count = vnd_cnt,
        .sig_models = sig_models,
        .vnd_models = vnd_models
    };

    for (size_t i = 0; i < models.size(); i++)
    {
        models.at(i)->base(element);
    }
    printf("prepare mesh element, sig count: %d, vnd count: %d\n", sig_cnt, vnd_cnt);
}
