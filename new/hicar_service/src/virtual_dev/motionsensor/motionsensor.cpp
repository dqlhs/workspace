#include "dmsdp_gps_handler.h"

#include <string.h>

#include <mutex>
#include <thread>
#include <algorithm>
#include <functional>

#include <asio/serial_port.hpp>

#include "unique_config.h"
#include "log/oxygen_log.h"
#include "utility/format.h"

#define SERIAL_READ_BUFFER_SIZE 1024

const static uint8_t ublox_baud_rate_115200 [] = {
    0xb5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00,
    0x00, 0x00, 0xd0, 0x08, 0x00, 0x00, 0x00, 0xc2,
    0x01, 0x00, 0x07, 0x00, 0x07, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xc4, 0x96, 0xb5, 0x62, 0x06, 0x00,
    0x01, 0x00, 0x01, 0x08, 0x22
};

const static uint8_t ublox_update_freq_10hz [] = {
    0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0x64, 0x00,
    0x01, 0x00, 0x01, 0x00, 0x7A, 0x12, 0xB5, 0x62,
    0x06, 0x08, 0x00, 0x00, 0x0E, 0x30
};

// open
const static uint8_t ublox_open_gpdtm [] = {
    0x24, 0x45, 0x49, 0x47, 0x50, 0x51, 0x2c, 0x44,
    0x54, 0x4d, 0x2a, 0x33, 0x42, 0x0d, 0x0a, 0xb5,
    0x62, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x0a, 0x01,
    0x05, 0x24
};

const static uint8_t ublox_open_gpgbs [] = {
    0x24, 0x45, 0x49, 0x47, 0x50, 0x51, 0x2c, 0x47,
    0x42, 0x53, 0x2a, 0x33, 0x30, 0x0d, 0x0a, 0xb5,
    0x62, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x09, 0x01,
    0x04, 0x22
};

const static uint8_t ublox_open_gpgga [] = {
    0x24, 0x45, 0x49, 0x47, 0x50, 0x51, 0x2c, 0x47,
    0x47, 0x41, 0x2a, 0x32, 0x37, 0x0d, 0x0a, 0xb5,
    0x62, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x00, 0x01,
    0xfb, 0x10
};

const static uint8_t ublox_open_gpgll [] = {
    0x24, 0x45, 0x49, 0x47, 0x50, 0x51, 0x2c, 0x47,
    0x4c, 0x4c, 0x2a, 0x32, 0x31, 0x0d, 0x0a, 0xb5,
    0x62, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x01, 0x01,
    0xfc, 0x12
};

const static uint8_t ublox_open_gpgrs [] = {
    0x24, 0x45, 0x49, 0x47, 0x50, 0x51, 0x2c, 0x47,
    0x52, 0x53, 0x2a, 0x32, 0x30, 0x0d, 0x0a, 0xb5,
    0x62, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x06, 0x01,
    0x01, 0x1c
};

const static uint8_t ublox_open_gpgsa [] = {
    0x24, 0x45, 0x49, 0x47, 0x50, 0x51, 0x2c, 0x47,
    0x53, 0x41, 0x2a, 0x33, 0x33, 0x0d, 0x0a, 0xb5,
    0x62, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x02, 0x01,
    0xfd, 0x14
};

const static uint8_t ublox_open_gpgst [] = {
    0x24, 0x45, 0x49, 0x47, 0x50, 0x51, 0x2c, 0x47,
    0x53, 0x54, 0x2a, 0x32, 0x36, 0x0d, 0x0a, 0xb5,
    0x62, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x07, 0x01,
    0x02, 0x1e
};

const static uint8_t ublox_open_gpgsv [] = {
    0x24, 0x45, 0x49, 0x47, 0x50, 0x51, 0x2c, 0x47,
    0x53, 0x56, 0x2a, 0x32, 0x34, 0x0d, 0x0a, 0xb5,
    0x62, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x03, 0x01,
    0xfe, 0x16
};

const static uint8_t ublox_open_gprmc [] = {
    0x24, 0x45, 0x49, 0x47, 0x50, 0x51, 0x2c, 0x52,
    0x4d, 0x43, 0x2a, 0x33, 0x41, 0x0d, 0x0a, 0xb5,
    0x62, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x04, 0x01,
    0xff, 0x18
};

const static uint8_t ublox_open_gpvtg [] = {
    0x24, 0x45, 0x49, 0x47, 0x50, 0x51, 0x2c, 0x56,
    0x54, 0x47, 0x2a, 0x32, 0x33, 0x0d, 0x0a, 0xb5,
    0x62, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x05, 0x01,
    0x00, 0x1a
};

const static uint8_t ublox_open_gpzda [] = {
    0x24, 0x45, 0x49, 0x47, 0x50, 0x51, 0x2c, 0x5a,
    0x44, 0x41, 0x2a, 0x33, 0x39, 0x0d, 0x0a, 0xb5,
    0x62, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x08, 0x01,
    0x03, 0x20
};

// close
const static uint8_t ublox_close_gpdtm [] = {
    0x24, 0x45, 0x49, 0x47, 0x50, 0x51, 0x2c, 0x44,
    0x54, 0x4d, 0x2a, 0x33, 0x42, 0x0d, 0x0a, 0xb5,
    0x62, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x0a, 0x00,
    0x04, 0x23
};

const static uint8_t ublox_close_gpgbs [] = {
    0x24, 0x45, 0x49, 0x47, 0x50, 0x51, 0x2c, 0x47,
    0x42, 0x53, 0x2a, 0x33, 0x30, 0x0d, 0x0a, 0xb5,
    0x62, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x09, 0x00,
    0x03, 0x21
};

const static uint8_t ublox_close_gpgga [] = {
    0x24, 0x45, 0x49, 0x47, 0x50, 0x51, 0x2c, 0x47,
    0x47, 0x41, 0x2a, 0x32, 0x37, 0x0d, 0x0a, 0xb5,
    0x62, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x00, 0x00,
    0xfa, 0x0f
};

const static uint8_t ublox_close_gpgll [] = {
    0x24, 0x45, 0x49, 0x47, 0x50, 0x51, 0x2c, 0x47,
    0x4c, 0x4c, 0x2a, 0x32, 0x31, 0x0d, 0x0a, 0xb5,
    0x62, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x01, 0x00,
    0xfb, 0x11
};

const static uint8_t ublox_close_gpgrs [] = {
    0x24, 0x45, 0x49, 0x47, 0x50, 0x51, 0x2c, 0x47,
    0x52, 0x53, 0x2a, 0x32, 0x30, 0x0d, 0x0a, 0xb5,
    0x62, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x06, 0x00,
    0x00, 0x1b
};

const static uint8_t ublox_close_gpgsa [] = {
    0x24, 0x45, 0x49, 0x47, 0x50, 0x51, 0x2c, 0x47,
    0x53, 0x41, 0x2a, 0x33, 0x33, 0x0d, 0x0a, 0xb5,
    0x62, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x02, 0x00,
    0xfc, 0x13
};

const static uint8_t ublox_close_gpgst [] = {
    0x24, 0x45, 0x49, 0x47, 0x50, 0x51, 0x2c, 0x47,
    0x53, 0x54, 0x2a, 0x32, 0x36, 0x0d, 0x0a, 0xb5,
    0x62, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x07, 0x00,
    0x01, 0x1d
};

const static uint8_t ublox_close_gpgsv [] = {
    0x24, 0x45, 0x49, 0x47, 0x50, 0x51, 0x2c, 0x47,
    0x53, 0x56, 0x2a, 0x32, 0x34, 0x0d, 0x0a, 0xb5,
    0x62, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x03, 0x00,
    0xfd, 0x15
};

const static uint8_t ublox_close_gprmc [] = {
    0x24, 0x45, 0x49, 0x47, 0x50, 0x51, 0x2c, 0x52,
    0x4d, 0x43, 0x2a, 0x33, 0x41, 0x0d, 0x0a, 0xb5,
    0x62, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x04, 0x00,
    0xfe, 0x17
};

const static uint8_t ublox_close_gpvtg [] = {
    0x24, 0x45, 0x49, 0x47, 0x50, 0x51, 0x2c, 0x56,
    0x54, 0x47, 0x2a, 0x32, 0x33, 0x0d, 0x0a, 0xb5,
    0x62, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x05, 0x00,
    0xff, 0x19
};

const static uint8_t ublox_close_gpzda [] = {
    0x24, 0x45, 0x49, 0x47, 0x50, 0x51, 0x2c, 0x5a,
    0x44, 0x41, 0x2a, 0x33, 0x39, 0x0d, 0x0a, 0xb5,
    0x62, 0x06, 0x01, 0x03, 0x00, 0xf0, 0x08, 0x00,
    0x02, 0x1f
};


namespace hsae {

class motion_sensor {
public:
    static motion_sensor & instance();

    static int32_t register_gps_ability(DMSDPGpsHandler *gpsHandler);

    static int32_t register_gps_data_callback (DMSDPGPSCallback* callback);
    static int32_t unregister_gps_data_callback();

    static int32_t get_gps_data_report_freq(int32_t* locationFreq, int32_t* drivingFreq, int32_t* sensorFreq);
    static int32_t set_gps_data_report_freq(int32_t locationFreq, int32_t drivingFreq, int32_t sensorFreq);
    static int32_t register_listener(const DMSDPListener *listener);
    static int32_t business_control(uint32_t cmd, char *inputPara, uint32_t inLen, char *outputPara, uint32_t outLen);

private:
    motion_sensor();
    void serial_async_read_handler(asio::error_code _ec, std::size_t _size);

    void start_service();

    void stop_service();

    bool service_status_running();

private:
    asio::io_context io_;
    asio::serial_port serial_;
    std::thread thread_;
    std::vector<char> buffer_;

    std::mutex motion_sensor_lock_;
    DMSDPGPSCallback motion_sensor_cb_;

    int32_t gps_freq_;
    int32_t body_freq_;
    int32_t sensor_freq_;
};

motion_sensor::motion_sensor()
    : serial_(io_),
      buffer_(SERIAL_READ_BUFFER_SIZE)
{
    // init private value
    memset(&motion_sensor_cb_, 0, sizeof (DMSDPGPSCallback));
}

void motion_sensor::serial_async_read_handler(asio::error_code _ec, const std::size_t _size)
{
    if (_ec) {
        ERROR("read error : %s", _ec.message().c_str());
        return;
    }

    // handler
    if (_size > 0) {
        std::string gps;
        std::copy(buffer_.begin(), buffer_.begin() + _size, std::back_inserter(gps));
        gps.erase(std::remove_if(gps.begin(), gps.end(), [] (char _c) {
            if (_c == '\0' || _c == '\r' || _c == '\n') return true;
            return false;
        }), gps.end());
        TRACE("recv ublox gps data [%d] : %s", (int)gps.size(), gps.c_str());
        std::unique_lock<std::mutex> lock(instance().motion_sensor_lock_);
        if (motion_sensor_cb_.GPSLocation) {
            motion_sensor_cb_.GPSLocation(&gps.front(), static_cast<int32_t>(gps.size()));
        } else {
            WARN("motion_sensor_cb_.GPSLocation == nullptr");
        }
    }

    // register
    serial_.async_read_some(asio::buffer(&buffer_.front(), buffer_.size()),
                            std::bind(&motion_sensor::serial_async_read_handler, this,
                                      std::placeholders::_1, std::placeholders::_2));
}

void motion_sensor::start_service()
{
    if (service_status_running()) {
        WARN("service is running");
        return;
    }

    auto & gps_interface = unique_config::instance().gps_interface();
    INFO("GPS interface : %s", gps_interface.c_str());

    // config
    asio::serial_port config(io_);
    config.open(gps_interface);
    config.set_option(asio::serial_port::baud_rate(9600));
    config.set_option(asio::serial_port::character_size(8));
    config.set_option(asio::serial_port::stop_bits(asio::serial_port::stop_bits::one));
    config.set_option(asio::serial_port::parity(asio::serial_port::parity::none));
    config.set_option(asio::serial_port::flow_control(asio::serial_port::flow_control::none));
    // change baud rate
    config.write_some(asio::buffer(ublox_baud_rate_115200, sizeof (ublox_baud_rate_115200)));
    config.close();

    serial_.open(gps_interface);

    serial_.set_option(asio::serial_port::baud_rate(115200));
    serial_.set_option(asio::serial_port::character_size(8));
    serial_.set_option(asio::serial_port::stop_bits(asio::serial_port::stop_bits::one));
    serial_.set_option(asio::serial_port::parity(asio::serial_port::parity::none));
    serial_.set_option(asio::serial_port::flow_control(asio::serial_port::flow_control::none));

    // set update freq
    serial_.write_some(asio::buffer(ublox_update_freq_10hz, sizeof (ublox_update_freq_10hz)));
    // close GPVTG
    serial_.write_some(asio::buffer(ublox_close_gpvtg, sizeof (ublox_close_gpvtg)));
    // close GPGSA
    serial_.write_some(asio::buffer(ublox_close_gpgsa, sizeof (ublox_close_gpgsa)));
    // close GPGSV
    serial_.write_some(asio::buffer(ublox_close_gpgsv, sizeof (ublox_close_gpgsv)));
    // close GPGLL
    serial_.write_some(asio::buffer(ublox_close_gpgll, sizeof (ublox_close_gpgll)));

    serial_.async_read_some(asio::buffer(&buffer_.front(), buffer_.size()),
                            std::bind(&motion_sensor::serial_async_read_handler, this,
                                      std::placeholders::_1, std::placeholders::_2));

    thread_ = std::thread([&] () {
        try {
            io_.run();
        } catch (std::exception e) {
            serial_.close();
            ERROR("serial running error : %s", e.what());
        }
    });
}

void motion_sensor::stop_service()
{
    if (thread_.joinable()) {
        // end asio io_context main loop
        io_.stop();

        // recycling resources
        thread_.join();
        serial_.close();
    }
}

bool motion_sensor::service_status_running()
{
    return thread_.joinable();
}

motion_sensor & motion_sensor::instance()
{
    static motion_sensor inst;
    return inst;
}

int32_t motion_sensor::register_gps_ability(DMSDPGpsHandler * gpsHandler)
{
    if (gpsHandler == nullptr) {
        WARN("register_gps_ability para is nullptr");
        return DMSDP_ERR_INVALID_PARAMETER;
    }

    INFO("register_gps_ability->%p", (void *)gpsHandler);
    memset(gpsHandler, 0, sizeof (DMSDPGpsHandler));

    gpsHandler->RegisterGPSDataCallback = motion_sensor::register_gps_data_callback;
    gpsHandler->UnRegisterGPSDataCallback = motion_sensor::unregister_gps_data_callback;
    gpsHandler->GetGPSDataReportFreq = motion_sensor::get_gps_data_report_freq;
    gpsHandler->SetGPSDataReportFreq = motion_sensor::set_gps_data_report_freq;
    gpsHandler->RegisterListener = motion_sensor::register_listener;
    gpsHandler->BusinessControl = motion_sensor::business_control;

    return DMSDP_OK;
}

int32_t motion_sensor::register_gps_data_callback(DMSDPGPSCallback * callback)
{
    if (callback == nullptr) {
        WARN("register_gps_data_callback para is nullptr");
        return DMSDP_ERR_INVALID_PARAMETER;
    }

    INFO("register_gps_data_callback->%p", (void *)callback);

    std::unique_lock<std::mutex> lock(instance().motion_sensor_lock_);
    instance().motion_sensor_cb_ = *callback;
    instance().start_service();
    return DMSDP_OK;
}

int32_t motion_sensor::unregister_gps_data_callback()
{
    INFO("unregister_gps_data_callback");
    std::unique_lock<std::mutex> lock(instance().motion_sensor_lock_);
    memset(&(instance().motion_sensor_cb_), 0, sizeof (DMSDPGPSCallback));
    instance().stop_service();
    return DMSDP_OK;
}

int32_t motion_sensor::get_gps_data_report_freq(int32_t * locationFreq, int32_t * drivingFreq, int32_t * sensorFreq)
{
    if ((!locationFreq) || (!drivingFreq) || (!sensorFreq)) {
        WARN("get_gps_data_report_freq para have nullptr value");
        return DMSDP_ERR_INVALID_PARAMETER;
    }

    std::unique_lock<std::mutex> lock(instance().motion_sensor_lock_);
    *locationFreq = instance().gps_freq_;
    *drivingFreq  = instance().body_freq_;
    *sensorFreq   = instance().sensor_freq_;
    return DMSDP_OK;
}

int32_t motion_sensor::set_gps_data_report_freq(int32_t locationFreq, int32_t drivingFreq, int32_t sensorFreq)
{
    std::unique_lock<std::mutex> lock(instance().motion_sensor_lock_);
    instance().gps_freq_    = locationFreq;
    instance().body_freq_   = drivingFreq;
    instance().sensor_freq_ = sensorFreq;
    return DMSDP_OK;
}

int32_t motion_sensor::register_listener(const DMSDPListener * listener)
{
    if (listener == nullptr) {
        WARN("register_listener para is nullptr");
        return DMSDP_ERR_INVALID_PARAMETER;
    }

    INFO("register_listener->%p", (void *)listener);
    std::unique_lock<std::mutex> lock(instance().motion_sensor_lock_);
    return 0;
}

int32_t motion_sensor::business_control(uint32_t cmd, char * inputPara, uint32_t inLen, char * outputPara, uint32_t outLen)
{
    (void)cmd;
    (void)inputPara;
    (void)inLen;
    (void)outputPara;
    (void)outLen;
    std::unique_lock<std::mutex> lock(instance().motion_sensor_lock_);
    return 0;
}

};

int32_t DMSDPGetGpsHandler(DMSDPGpsHandler *gpsHandler)
{
    INFO("init");
    return hsae::motion_sensor::register_gps_ability(gpsHandler);
}

