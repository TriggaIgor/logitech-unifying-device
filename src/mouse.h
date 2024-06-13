#include <Arduino.h>
#include <elapsedMillis.h>
#include "ludevice.h"
#include <esphome.h>

double x = 0, y = 0, x0 = 0, y00 = 0, x1 = 0, y11 = 0, r = 100;
elapsedMillis move_timer;
int last_timer;
ludevice kespb(D4, D3);

float mouseSpeed = 10.0f;
float degreesToRadians = 2.0f * 3.14f / 360.0f;
bool keydown = false;

class AdvancedBlinkComponent : public Component
{
public:
  bool enable = true;

  AdvancedBlinkComponent(esphome::template_::TemplateSwitch *&_enable)
  {
    _enable->add_on_state_callback([this](bool newState)
                                   { enable = newState; });
  }

    void setup() override
    {
        int retcode;
        kespb.begin();
        while (1)
        {
            if (kespb.reconnect())
            {
                printf("Reconnected!\r\n");
                break;
            }
            else
            {
                retcode = kespb.pair();
                if (retcode == true)
                {
                    if (kespb.register_device())
                    {
                        printf("Paired and connected\r\n");
                        break;
                    }
                }
                else
                {
                    if (retcode == false)
                    {
                        printf("No dongle wants to pair\r\n");
                        delay(5000); 
                    }
                }
            }
            yield();
        }
    }

    void left_rand() {
    float i = 0;
    int delta = 0;
    for( i = 0; i < random(2,2)*PI; i = i + PI/random(2, 20)){
        last_timer=move_timer;
        r = i*random(20, 25) +i;
        x1 = r * sin(i);
        y11 = r * cos(i);
        x = x1 - x0;
        y = y11 - y00;
        x0 = x1;
        y00 = y11;
        kespb.move(x,y);
        delay(r/2);
    } 
    }

    void loop()
    {
    if (enable == false)
    {
      return;
    }
    if ((move_timer > random(5000, 25000))){
        left_rand();
        move_timer=0;
    }
    kespb.loop();
    }

};