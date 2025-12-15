#ifndef BMI270_I2C_H_
#define BMI270_I2C_H_


#include "hal/i2c/i2c.h"


// More Informations from sensor: https://content.arduino.cc/assets/bmi270-ds000.pdf
// --------------- DEFINITION ---------------
#define BMI270_I2C_ADR          0x68

#define BMI270_ODR_0P78    0x01
#define BMI270_ODR_1P5     0x02
#define BMI270_ODR_3P1     0x03
#define BMI270_ODR_6P25    0x04
#define BMI270_ODR_12P5    0x05
#define BMI270_ODR_25      0x06
#define BMI270_ODR_50      0x07
#define BMI270_ODR_100     0x08
#define BMI270_ODR_200     0x09
#define BMI270_ODR_400     0x0A
#define BMI270_ODR_800     0x0B
#define BMI270_ODR_1K6     0x0C
#define BMI270_ODR_3K2     0x0D
#define BMI270_ODR_6K4     0x0E

#define BMI270_ACC_RANGE_2G    0x00
#define BMI270_ACC_RANGE_4G    0x01
#define BMI270_ACC_RANGE_8G    0x02
#define BMI270_ACC_RANGE_16G   0x03
#define BMI270_GYRO_RANGE_2000   0x00
#define BMI270_GYRO_RANGE_1000   0x01
#define BMI270_GYRO_RANGE_500    0x02
#define BMI270_GYRO_RANGE_250    0x03
#define BMI270_GYRO_RANGE_125    0x04

// Handler for configuring and controlling the device configuration
typedef struct {
    i2c_t *i2c_mod;
    bool    en_adv_pwr_mode;
    bool    en_temp_sensor;
    bool    en_gyro_sensor;
    uint8_t gyro_odr;
    uint8_t gyro_range;
    bool    en_acc_sensor;
    uint8_t acc_odr;
    uint8_t acc_range;
    bool    do_noise_performance_opt;
    bool    init_done;
} bmi270_i2c_t;


static bmi270_i2c_t BMI270_I2C_DEFAULT_CONFIG = {
    .i2c_mod = &DEVICE_I2C_DEFAULT,
    .en_adv_pwr_mode = false,
    .en_temp_sensor = false,
    .en_gyro_sensor = false,
    .gyro_odr = BMI270_ODR_100,
    .gyro_range = BMI270_GYRO_RANGE_1000,
    .en_acc_sensor = false,
    .acc_odr = BMI270_ODR_100,
    .acc_range = BMI270_ACC_RANGE_4G,    
    .do_noise_performance_opt = false,
    .init_done = false
};


// Handler for configuring and controlling the data
typedef struct {
    double temp;
    double time;
    double gyro_x;
    double gyro_y;
    double gyro_z;
    double acc_x;
    double acc_y;
    double acc_z;
} bmi270_data_t;

static bmi270_data_t BMI270_DATA_DEFAULT = {
    .temp = 0.0,
    .time = 0.0,
    .gyro_x = 0,
    .gyro_y = 0,
    .gyro_z = 0,
    .acc_x = 0,
    .acc_y = 0,
    .acc_z = 0
};

// --------------- CALLABLE FUNCTIONS ---------------
/*! \brief Function for reading the Chip ID of accelerator sensor BMI270 from Bosch
*   \param  handler     Handler for setting the device
*   \param  print_id    Boolean to print the sensor ID
*   \return             Boolean if right Chip ID of sensor is available (0x24)
*/
bool BMI270_i2c_read_id(bmi270_i2c_t *handler, bool print_id);


/*! \brief Function for software-based reset of accelerator sensor BMI270 from Bosch
*   \param  handler     Handler for setting the device
*   \return             Boolean if right Chip ID of sensor is available (0x24)
*/
bool BMI270_i2c_soft_reset(bmi270_i2c_t *handler);


/*! \brief Function for initialisation of accelerator sensor BMI270 from Bosch
*   \param  handler     Handler for setting the device
*   \return             Boolean if init is done
*/
bool BMI270_i2c_init(bmi270_i2c_t *handler);


/*! \brief Function for setting the gyroscope configuration of BMI270 from Bosch
*   \param  handler     Handler for setting the device
*   \return             Boolean if setting is done
*/
bool BMI270_i2c_set_gyroscope_settings(bmi270_i2c_t *handler);


/*! \brief Function for setting the accelerator configuration of BMI270 from Bosch
*   \param  handler     Handler for setting the device
*   \return             Boolean if setting is done
*/
bool BMI270_i2c_set_accelerator_settings(bmi270_i2c_t *handler);


/*! \brief Function for getting the content of the error registration of BMI270 from Bosch
*   \param  handler     Handler for setting the device
*   \return             Data from register (bit 0: Fatal error, 1..4: Internal error, contact Bosch, 6: fifo error, 7: aux error) 
*/
uint8_t BMI270_i2c_get_error_register(bmi270_i2c_t *handler);


/*! \brief Function for getting the content of status register of BMI270 from Bosch
*   \param  handler     Handler for setting the device
*   \return             Data from register 
*/
uint8_t BMI270_i2c_get_status_register(bmi270_i2c_t *handler);


/*! \brief Function for getting the content of internal status register of BMI270 from Bosch
*   \param  handler     Handler for setting the device
*   \param  print_status Boolean for printing the status into terminal
*   \return             Data from register
*/
uint8_t BMI270_i2c_get_status_internal_register(bmi270_i2c_t *handler, bool print_status);


/*! \brief Function for getting the content of power register of BMI270 from Bosch
*   \param  handler     Handler for setting the device
*   \return             Data from register (bit 0: aux_en, 1: gyr_en, 2: acc_en, 3: temp_en)
*/
uint8_t BMI270_i2c_get_power_register(bmi270_i2c_t *handler);


/*! \brief Function for getting the temperatur value from BMI270 from Bosch
*   \param  handler     Handler for setting the device
*   \return             Temperature data sensor in °C
*/
double BMI270_i2c_get_temperature(bmi270_i2c_t *handler);


/*! \brief Function for getting the active sensor time from BMI270 from Bosch
*   \param  handler     Handler for setting the device
*   \return             Actual runtime in s (with overflow)
*/
double BMI270_i2c_get_sensor_time(bmi270_i2c_t *handler);


/*! \brief Function for getting the accelerator sensor data from BMI270 from Bosch
*   \param  handler     Handler for setting the device
*   \return             x-, y-, z-axis data in struct
*/
bmi270_data_t BMI270_i2c_get_accelerator_data(bmi270_i2c_t *handler);


/*! \brief Function for getting the gyroscope sensor data from BMI270 from Bosch
*   \param  handler     Handler for setting the device
*   \return             x-, y-, z-axis data in struct
*/
bmi270_data_t BMI270_i2c_get_gyroscope_data(bmi270_i2c_t *handler);


/*! \brief Function for getting all sensor data from BMI270 from Bosch
*   \param  handler     Handler for setting the device
*   \return             x-, y-, z-axis data in struct
*/
bmi270_data_t BMI270_i2c_get_all_data(bmi270_i2c_t *handler);


#endif
