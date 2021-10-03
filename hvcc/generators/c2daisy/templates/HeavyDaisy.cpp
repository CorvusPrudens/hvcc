{{copyright}}

#include "{{class_name}}.hpp"

#define SAMPLE_RATE 48000.f

using namespace daisy;

DSY_BOARD* hardware;

int num_params;

Heavy_{{name}} hv(SAMPLE_RATE);

void ProcessControls();

void audiocallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
    hv.process((float**) in, out, size);
    ProcessControls();
}

{% if board =='field' %}
void UpdateLeds(float *knob_vals)
{
    // knob_vals is exactly 8 members
    size_t knob_leds[] = {
        DaisyField::LED_KNOB_1,
        DaisyField::LED_KNOB_2,
        DaisyField::LED_KNOB_3,
        DaisyField::LED_KNOB_4,
        DaisyField::LED_KNOB_5,
        DaisyField::LED_KNOB_6,
        DaisyField::LED_KNOB_7,
        DaisyField::LED_KNOB_8,
    };
    size_t keyboard_leds[] = {
        DaisyField::LED_KEY_A1,
        DaisyField::LED_KEY_A2,
        DaisyField::LED_KEY_A3,
        DaisyField::LED_KEY_A4,
        DaisyField::LED_KEY_A5,
        DaisyField::LED_KEY_A6,
        DaisyField::LED_KEY_A7,
        DaisyField::LED_KEY_A8,
        DaisyField::LED_KEY_B2,
        DaisyField::LED_KEY_B3,
        DaisyField::LED_KEY_B5,
        DaisyField::LED_KEY_B6,
        DaisyField::LED_KEY_B7,
    };
    for(size_t i = 0; i < 8; i++)
    {
        hardware->led_driver.SetLed(knob_leds[i], knob_vals[i]);
    }
    for(size_t i = 0; i < 13; i++)
    {
        hardware->led_driver.SetLed(keyboard_leds[i], 1.f);
    }
    hardware->led_driver.SwapBuffersAndTransmit();
}

void UpdateCV(uint16_t *cv_vals)
{
    hardware->SetCvOut1(cv_vals[0]);
    hardware->SetCvOut2(cv_vals[1]);
}

void UpdateGate(uint8_t gate)
{
    dsy_gpio_write(&hardware->gate_out, gate);
}
{% endif %}

static void sendHook(HeavyContextInterface *c, const char *receiverName, uint32_t receiverHash, const HvMessage * m) {
  // Do something with message sent from Pd patch through
  // [send receiverName @hv_event] object(s)

{% if board =='field' %}

    if (receiverName == "Led1")
    {
        kvals[0] = msg_getFloat(m, 0);
    }
    else if (receiverName == "Led2")
    {
        kvals[1] = msg_getFloat(m, 0);
    }
    else if (receiverName == "Led3")
    {
        kvals[2] = msg_getFloat(m, 0);
    }
    else if (receiverName == "Led4")
    {
        kvals[3] = msg_getFloat(m, 0);
    }
    else if (receiverName == "Led5")
    {
        kvals[4] = msg_getFloat(m, 0);
    }
    else if (receiverName == "Led6")
    {
        kvals[5] = msg_getFloat(m, 0);
    }
    else if (receiverName == "Led7")
    {
        kvals[6] = msg_getFloat(m, 0);
    }
    else if (receiverName == "Led8")
    {
        kvals[7] = msg_getFloat(m, 0);
    }
    else if (receiverName == "Ctrlout1")
    {
        cvvals[0] = (uint16_t) roundf(msg_getFloat(m, 0) * 4096.0f);
    }
    else if (receiverName == "Ctrlout2")
    {
        cvvals[1] = (uint16_t) roundf(msg_getFloat(m, 0) * 4096.0f);
    }
    else if (receiverName == "Gate1out")
    {
        gate1_out = (uint8_t) msg_getFloat(m, 0);
    }

{% endif %}

}

int main(void)
{
    hardware = &boardsHardware;
    {% if board == 'seed' %}
    hardware->Configure();
    {% endif %}
    num_params = hv.getParameterInfo(0,NULL);

    hv.setSendHook(sendHook);

    hardware->Init();

    {% if board != 'seed' %}
    hardware->StartAdc();
    {% endif %}

    hardware->StartAudio(audiocallback);
    // GENERATE POSTINIT
    for(;;)
    {
        {% if board == 'patch' %}
        hardware->DisplayControls(false);
        {% elif board == 'field' %}
        UpdateLeds(kvals);
        UpdateCV(cvvals);
        UpdateGate(gate1_out);
        {% endif %}
    }
}

void ProcessControls()
{
    {% if board != 'seed' %}
    hardware->ProcessAllControls();
    {% endif %}

    for (int i = 0; i < num_params; i++)
    {
        HvParameterInfo info;
        hv.getParameterInfo(i, &info);

        {% if board == 'seed' %}
        hv.sendFloatToReceiver(info.hash, 0.f);
        {% endif %}

        std::string name(info.name);

        for (int j = 0; j < DaisyNumParameters; j++){
            if (DaisyParameters[j].name == name)
            {
            float sig = DaisyParameters[j].Process();

            if (DaisyParameters[j].mode == ENCODER || DaisyParameters[j].mode == KNOB)
                hv.sendFloatToReceiver(info.hash, sig);
            else if(sig)
                hv.sendBangToReceiver(info.hash);
            }
        }
    }
}
