#include <libtock-sync/services/alarm.h>

#include "sensor.h"

int main(void) {
  uint8_t buffer[256] = {};
  size_t buffer_len   = 0;

  // float vwc_adj = (3.879e-4 * sens_data.vwc) - 0.6956;

  // metadata
  // 200 for logger_id and cell_id  are known to exist
  // should change to newly created cells on dirtviz
  Metadata meta = Metadata_init_zero;
  meta.ts        = 1769113673;
  meta.logger_id = 200;
  meta.cell_id   = 200;

  SensorMeasurement meas[4] = {};

  // vwc measurements
  SensorMeasurement* vwc_raw = &meas[0];
  vwc_raw->type          = SensorType_TEROS12_VWC;
  vwc_raw->which_value   = SensorMeasurement_decimal_tag;
  vwc_raw->value.decimal = 2000.;

  SensorMeasurement* vwc_adj = &meas[1];
  vwc_adj->type          = SensorType_TEROS12_VWC_ADJ;
  vwc_adj->which_value   = SensorMeasurement_decimal_tag;
  vwc_adj->value.decimal = 20.;

  SensorMeasurement* temp = &meas[2];
  temp->type          = SensorType_TEROS12_TEMP;
  temp->which_value   = SensorMeasurement_decimal_tag;
  temp->value.decimal = 22.;

  SensorMeasurement* ec = &meas[3];
  ec->type               = SensorType_TEROS12_EC;
  ec->which_value        = SensorMeasurement_unsigned_int_tag;
  ec->value.unsigned_int = 20.;

  SensorStatus status = SENSOR_OK;
  status = EncodeRepeatedSensorMeasurements(meta, meas, sizeof(meas), buffer, sizeof(buffer), &buffer_len);
  if (status != SENSOR_OK) {
    // ERROR in formatting or buffer
    return -1;
  }

  return 0;
}
