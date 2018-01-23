#define CALIBRATION_SAMPLES 70  // Number of compass readings to take when calibrating
#define CRB_REG_M_2_5GAUSS 0x60 // CRB_REG_M value for magnetometer +/-2.5 gauss full scale
#define CRA_REG_M_220HZ    0x1C // CRA_REG_M value for magnetometer 220 Hz update rate

void setupCompass()
{
  compass.init();
  compass.enableDefault();
  compass.writeReg(LSM303::CRB_REG_M, CRB_REG_M_2_5GAUSS); // +/- 2.5 gauss sensitivity to hopefully avoid overflow problems
  compass.writeReg(LSM303::CRA_REG_M, CRA_REG_M_220HZ);    // 220 Hz compass update rate
  delay(1000); // 良く分からないが必要
}

void  calibrationCompass()
{
  unsigned int index;
  int motorL, motorR;

  LSM303::vector<int16_t> running_min = {
    32767, 32767, 32767
  }
  , running_max = {
    -32767, -32767, -32767
  };

  motorL = 200;
  motorR = -200;
  motors.setLeftSpeed(motorL);
  motors.setRightSpeed(motorR);

  for (index = 0; index < CALIBRATION_SAMPLES; index ++)
  {
    // Take a reading of the magnetic vector and store it in compass.m
    compass.read();

    running_min.x = min(running_min.x, compass.m.x);
    running_min.y = min(running_min.y, compass.m.y);

    running_max.x = max(running_max.x, compass.m.x);
    running_max.y = max(running_max.y, compass.m.y);

    delay(50);
  }

  motorL = 0;
  motorR = 0;
  motors.setLeftSpeed(motorL);
  motors.setRightSpeed(motorR);

  // Set calibrated values to compass.m_max and compass.m_min
  compass.m_max.x = running_max.x;
  compass.m_max.y = running_max.y;
  compass.m_min.x = running_min.x;
  compass.m_min.y = running_min.y;

}

void CalibrationCompassManual()
{
  compass.m_min.x = -3788;
  compass.m_min.y = -525;
  compass.m_max.x = -917;
  compass.m_max.y = 2849;
}

template <typename T> float heading(LSM303::vector<T> v)
{
  float x_scaled =  2.0 * (float)(v.x - compass.m_min.x) / ( compass.m_max.x - compass.m_min.x) - 1.0;
  float y_scaled =  2.0 * (float)(v.y - compass.m_min.y) / (compass.m_max.y - compass.m_min.y) - 1.0;

  float angle = atan2(y_scaled, x_scaled) * 180 / M_PI;
  if (angle < 0)
    angle += 360;
  return angle;
}

// Yields the angle difference in degrees between two headings
float relativeHeading(float heading_from, float heading_to)
{
  float relative_heading = heading_to - heading_from;

  // constrain to -180 to 180 degree range
  if (relative_heading > 180)
    relative_heading -= 360;
  if (relative_heading < -180)
    relative_heading += 360;

  return relative_heading;
}

// Average 10 vectors to get a better measurement and help smooth out
// the motors' magnetic interference.
float averageHeading()
{
  LSM303::vector<int32_t> avg = {
    0, 0, 0
  };

  for (int i = 0; i < 20; i ++)
  {
    compass.read();
    avg.x += compass.m.x;
    avg.y += compass.m.y;
  }
  avg.x /= 20.0;
  avg.y /= 20.0;

  // avg is the average measure of the magnetic vector.
  return heading(avg);
}

float averageHeadingLP()
{
  static LSM303::vector<int32_t> avg = {
    0, 0, 0
  };

  compass.read();
  avg.x = 0.2 * compass.m.x + 0.8 * avg.x;
  avg.y = 0.2 * compass.m.y + 0.8 * avg.y;


  // avg is the average measure of the magnetic vector.
  return heading(avg);
}

float compass_value()
{
  compass.read();
  direction_V.x = 0.2 * compass.m.x + 0.8 * direction_V.x;
  direction_V.y = 0.2 * compass.m.y + 0.8 * direction_V.y;
  direction_V.z = 0.2 * compass.m.z + 0.8 * direction_V.z;

  // avg is the average measure of the magnetic vector.
  return heading(direction_V);
}

