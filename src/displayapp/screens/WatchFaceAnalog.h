#pragma once

#include <lvgl/src/lv_core/lv_obj.h>
#include <chrono>
#include <cstdint>
#include <memory>
#include "displayapp/screens/Screen.h"
#include "components/datetime/DateTimeController.h"
#include "components/battery/BatteryController.h"
#include "components/ble/BleController.h"
#include "components/ble/NotificationManager.h"
#include "displayapp/screens/BatteryIcon.h"
#include "utility/DirtyValue.h"

namespace Pinetime {
  namespace Controllers {
    class Settings;
    class Battery;
    class Ble;
    class NotificationManager;
  }

  namespace Applications {
    namespace Screens {

      class WatchFaceAnalog : public Screen {
      public:
        WatchFaceAnalog(Controllers::DateTime& dateTimeController,
                        const Controllers::Battery& batteryController,
                        const Controllers::Ble& bleController,
                        Controllers::NotificationManager& notificationManager,
                        Controllers::Settings& settingsController);

        ~WatchFaceAnalog() override;

        bool OnTouchEvent(TouchEvents event) override;
        bool OnButtonPushed() override;

        void Refresh() override;

        void UpdateSelected(lv_obj_t* object, lv_event_t event);

      private:
        uint8_t sHour, sMinute, sSecond;
        uint32_t savedTick = 0;

        Utility::DirtyValue<uint8_t> batteryPercentRemaining {0};
        Utility::DirtyValue<bool> isCharging {};
        Utility::DirtyValue<bool> bleState {};
        Utility::DirtyValue<std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>> currentDateTime;
        Utility::DirtyValue<bool> notificationState {false};
        Utility::DirtyValue<std::chrono::time_point<std::chrono::system_clock, std::chrono::days>> currentDate;

        bool tfHourEnable, sTfHourEnable;

        lv_obj_t* btnClose;
        lv_obj_t* btnClockStyle;
        lv_obj_t* btnSecondHand;
        lv_obj_t* minor_scales;
        lv_obj_t* major_scales;
        lv_obj_t* large_scales_12;
        lv_obj_t* large_scales_24_a;
        lv_obj_t* large_scales_24_b;
        lv_obj_t* circle;
        lv_obj_t* zero;
        lv_obj_t* twelve;

        lv_obj_t* hour_body;
        lv_obj_t* minute_body;
        lv_obj_t* second_body;

        lv_point_t hour_point[2];
        lv_point_t minute_point[2];
        lv_point_t second_point[2];

        lv_style_t hour_line_style;
        lv_style_t minute_line_style;
        lv_style_t second_line_style;

        lv_obj_t* label_date_day;
        lv_obj_t* plugIcon;
        lv_obj_t* notificationIcon;
        lv_obj_t* bleIcon;

        BatteryIcon batteryIcon;

        Controllers::DateTime& dateTimeController;
        const Controllers::Battery& batteryController;
        const Controllers::Ble& bleController;
        Controllers::NotificationManager& notificationManager;
        Controllers::Settings& settingsController;

        void UpdateClock();
        void SetBatteryIcon();
        void CloseMenu();

        lv_task_t* taskRefresh;
      };
    }

    template <>
    struct WatchFaceTraits<WatchFace::Analog> {
      static constexpr WatchFace watchFace = WatchFace::Analog;
      static constexpr const char* name = "Analog face";

      static Screens::Screen* Create(AppControllers& controllers) {
        return new Screens::WatchFaceAnalog(controllers.dateTimeController,
                                            controllers.batteryController,
                                            controllers.bleController,
                                            controllers.notificationManager,
                                            controllers.settingsController);
      };

      static bool IsAvailable(Pinetime::Controllers::FS& /*filesystem*/) {
        return true;
      }
    };
  }
}
