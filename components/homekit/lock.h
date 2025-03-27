#pragma once

#include "esphome/core/component.h"
#include "esphome/components/lock/lock.h"
#include "esp_hap_main.h"
#include "esp_hap_core.h"
#include "esp_hap_acc.h"
#include "esp_hap_serv.h"
#include "esp_hap_char.h"

namespace esphome {
namespace homekit {

enum class AInfo {
    NAME,
    MODEL,
    SN,
    MANUFACTURER,
    FW_REV
};

enum HKFinish {
    TAN = 0,
    GOLD,
    SILVER,
    BLACK
};

class LockEntity {
private:
    std::map<AInfo, const char*> accessory_info = {
        {AInfo::NAME, NULL}, 
        {AInfo::MODEL, "HAP-LOCK"}, 
        {AInfo::SN, NULL}, 
        {AInfo::MANUFACTURER, "rednblkx"}, 
        {AInfo::FW_REV, "0.1"}
    };
    
    lock::Lock* ptrToLock;
    std::unique_ptr<hap_tlv8_val_t> hkFinishTlvData;

#ifdef USE_HOMEKEY
    static nvs_handle savedHKdata;
    static readerData_t readerData;
    static pn532::PN532* nfc_ctx;
    std::vector<uint8_t> ecpData;
    std::vector<HKAuthTrigger*> triggers_onhk_;
    std::vector<HKFailTrigger*> triggers_onhk_fail_;
#endif

public:
    LockEntity(lock::Lock* lockPtr);
    void setup();
    void setInfo(std::map<AInfo, const char*> info);
    void set_hk_hw_finish(HKFinish color);

protected:
    static int acc_identify(hap_acc_t* ha);
    static int lock_write(hap_write_data_t write_data[], int count, void* serv_priv, void* write_priv);
    void on_lock_update(lock::Lock* obj);

#ifdef USE_HOMEKEY
    static int nfcAccess_write(hap_write_data_t write_data[], int count, void* serv_priv, void* write_priv);
    static void hap_event_handler(hap_event_t event, void* data);
    void register_onhk_trigger(HKAuthTrigger* trig);
    void register_onhkfail_trigger(HKFailTrigger* trig);
    void set_nfc_ctx(pn532::PN532* ctx);
#endif
};

} // namespace homekit
} // namespace esphome
