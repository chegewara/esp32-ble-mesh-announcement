#pragma once

#include "stdio.h"
#include <map>
#include "string.h"
#include "esp_err.h"
#include "esp_log.h"

#include "esp_ble_mesh_defs.h"
#include "esp_ble_mesh_common_api.h"
#include "esp_ble_mesh_networking_api.h"
#include "esp_ble_mesh_provisioning_api.h"
#include "esp_ble_mesh_generic_model_api.h"
#include "esp_ble_mesh_local_data_operation_api.h"

#include "ble_mesh_prov.h"
#include "ble_context.h"

extern "C" bool bt_mesh_is_provisioned(void);
extern "C" bool bt_mesh_is_provisioner(void);

#undef BLE_MESH_MODEL_PUB_DEFINE

#define BLE_MESH_MODEL_PUB_DEFINE(_name, _msg_len, _role, __n__)     \
    NET_BUF_SIMPLE_DEFINE_STATIC(bt_mesh_pub_msg_##__n__, _msg_len); \
    esp_ble_mesh_model_pub_t *_tmp = _name;                          \
    _tmp->msg = &bt_mesh_pub_msg_##__n__;                            \
    _tmp->update = (uint32_t)NULL;                                   \
    _tmp->dev_role = _role;

extern "C" void copy_memcpy(esp_ble_mesh_model_t *models, esp_ble_mesh_model_t *model);
// extern "C" void copy_memcpy(esp_ble_mesh_model_t models[], esp_ble_mesh_model_t* model, uint8_t start, uint8_t cnt);

class BLEmeshModelCb;

class IBLEMeshModel : public IBLEcontext
{
protected:
    uint8_t get_auto_rsp = ESP_BLE_MESH_SERVER_AUTO_RSP;
    uint8_t set_auto_rsp = ESP_BLE_MESH_SERVER_AUTO_RSP;
    uint8_t status_auto_rsp = ESP_BLE_MESH_SERVER_AUTO_RSP;

protected:
    BLEmeshModelCb *cb = nullptr;                /// @todo do we need it here, or maybe per model class???
    const char *_name = nullptr;                 /// @brief object instance name, for debug purpose
    esp_ble_mesh_model_pub_t pub = {};           /// @brief net buf for data send/publish
    esp_ble_mesh_model_t *_model = nullptr;      /// @brief mesh model pointer, used to build composition, detect which model is event callback for
    uint16_t model_id = ESP_BLE_MESH_CID_NVAL;   /// @brief helper for underlying operations
    uint16_t company_id = ESP_BLE_MESH_CID_NVAL; /// @brief helper for underlying operations
    esp_ble_mesh_elem_t *element;                /// @brief parent element, which this model belongs to
    uint8_t tid = 1;
    esp_ble_mesh_server_state_type_t type = (esp_ble_mesh_server_state_type_t)0; // esp_ble_mesh_server_model_update_state

public:
    friend class BLEmeshElement;

    IBLEMeshModel() = delete;
    explicit IBLEMeshModel(const char *name) : _name(name) {}
    ~IBLEMeshModel() {}

    const char *name()
    {
        return _name;
    }

    virtual void init(void *assoc) = 0;

    virtual esp_err_t registerCallback() = 0;

    virtual void onEvent(uint32_t event, uint32_t op_code, void *params) = 0;

    bool is_vendor()
    {
        return company_id != ESP_BLE_MESH_CID_NVAL;
    }

    /// @brief use normally in code
    /// @return
    virtual esp_ble_mesh_model_t *get() final
    {
        esp_ble_mesh_model_t *mdl = NULL;
        if (is_vendor())
        {
            if (element)
                mdl = esp_ble_mesh_find_vendor_model(element, company_id, model_id);
        }
        else
        {
            if (element)
                mdl = esp_ble_mesh_find_sig_model(element, model_id);
        }

        if (element)
        {
            assert(mdl == _model); /// debug purpose only, to make sure there is no bug in code
        } else {
            ESP_LOGE(TAG, "element is NULL withing mesh model");
        }
        return _model;
    }

    /// @brief optional
    void base(esp_ble_mesh_elem_t *el)
    {
        element = el;
    }

    /**
     * @brief model is prepared in the \ref init(), then during composition preparation we have to copy values;
     *          from copied values we have new model, which is registered in low level code, and we want to use that new pointer in all operations
     * 
     * @param mdl 
     */
    virtual void replace(esp_ble_mesh_model_t *mdl) final
    {
        free(_model);
        _model = mdl;
    }

    /**
     * @brief check is this is the model we are looking for in \ref findModel functions
     * 
     * @param mdl 
     * @return true 
     * @return false 
     */
    virtual bool model(esp_ble_mesh_model_t *mdl) final
    {
        return mdl == get();
    }

    virtual esp_err_t publish(uint32_t opcode, uint8_t* data, size_t len)
    {
        esp_err_t err = esp_ble_mesh_model_publish(_model, opcode, len, data, ROLE_NODE);
        if(err)
            ESP_LOGW(__func__, "opcode: 0x%06lx, len: %d, status: %s", opcode, len, esp_err_to_name(err));
        
        return err;
    }

    virtual esp_err_t sendMsg(uint16_t address, uint32_t opcode, uint8_t* data, size_t len) {
        auto ctx = getCtx();
        ctx->addr = address;
        esp_err_t err = esp_ble_mesh_server_model_send_msg(_model, ctx, opcode, len, data);
        if(err)
            ESP_LOGW(__func__, "opcode: 0x%06lx, len: %d, status: %s", opcode, len, esp_err_to_name(err));

        return err;
    }

    virtual esp_err_t update(esp_ble_mesh_server_state_type_t type, esp_ble_mesh_server_state_value_t *value) {
        return esp_ble_mesh_server_model_update_state(_model, type, value);
    }

    virtual void autoResponse(bool get, bool set, bool status) {
        get_auto_rsp = get;
        set_auto_rsp = set;
        status_auto_rsp = status;
    }

    /// @note only few generic models can update state
    /// @see esp_ble_mesh_server_state_type_t
#if CONFIG_BLE_MESH_SERVER_MODEL
    virtual esp_err_t updateState(esp_ble_mesh_server_state_type_t type, esp_ble_mesh_server_state_value_t *state) {
        return esp_ble_mesh_server_model_update_state(_model, type, state);
    }
#endif

    virtual void setCb(BLEmeshModelCb *_cb) final
    {
        cb = _cb;
    }

    virtual esp_err_t bindLocalAppKey(uint8_t appIdx = 0);

private:
    /// @brief use internally, before elements are prepared
    /// @todo make it protected, friend class Element
    /// @return
    virtual esp_ble_mesh_model_t *get_() final
    {
        return _model;
    }
};

class BLEmeshModelCb
{
public:
    virtual void onSet(IBLEMeshModel *model, void *params) {}
    virtual void onGet(IBLEMeshModel *model, void *params) {}
    virtual void onChange(IBLEMeshModel *model, void *params) {}
    virtual void onEvent(IBLEMeshModel *model, uint32_t event, void *params) {}
};

class ModelScenesCb
{
public:
    virtual void storeScene(uint16_t scn) {}

    virtual void recallScene(uint16_t scn) {}

    virtual void deleteScene(uint16_t scn) {}
};
