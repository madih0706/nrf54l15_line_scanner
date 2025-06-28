/*
        Code for testing adc on the nrf54l15 dev board
        Copied from this link: https://academy.nordicsemi.com/courses/nrf-connect-sdk-intermediate/lessons/lesson-6-analog-to-digital-converter-adc/topic/exercise-1-interfacing-with-adc-using-zephyr-api/
*/

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/devicetree.h>

static const struct adc_dt_spec adc_channel = ADC_DT_SPEC_GET(DT_PATH(zephyr_user));

int main(void)
{
        //validate that the ADC peripheral is ready before setting it up
        if (!adc_is_ready_dt(&adc_channel)) {
                printk("ADC controller devivce %s not ready\n", adc_channel.dev->name);
                return 0;
        } else {
                printk("ADC controller devivce %s is ready\n", adc_channel.dev->name);
        }

        //set up ADC channel
        int err = adc_channel_setup_dt(&adc_channel);
        if (err < 0) {
                printk("Could not setup channel #%d (%d)\n", 0, err);
                return 0;
        } else {
                printk("Set up channel\n");
        }

        // define a variable of type struct adc_sequence and a buffer of type int16_t to 
        // specify where the samples are to be written
        int16_t buf;
        struct adc_sequence sequence = {
                .buffer = &buf,
                /* buffer size in bytes, not number of samples */
                .buffer_size = sizeof(buf),
                //Optional
                //.calibrate = true,
        };

        // intialize adc sequence
        err = adc_sequence_init_dt(&adc_channel, &sequence);
        if (err < 0) {
                printk("Could not initalize sequnce");
                return 0;
        } else {
                printk("Intitialized sequence");
        }

        int16_t val_mv;
        while (1) {
                //read a sample from the adc channel
                err = adc_read(adc_channel.dev, &sequence);
                if (err < 0) {
                        printk("Could not read (%d)", err);
                        continue;
                }

                err = adc_raw_to_millivolts_dt(&adc_channel, &val_mv);
                /* conversion to mV may not be supported, skip if not */
                if (err < 0) {
                        printk(" (value in mV not available)\n");
                } else {
                        printk(" = %d mV", val_mv);
                }
        }

}
