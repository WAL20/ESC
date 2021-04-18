

typedef struct _ESCdata_  {
	uint8_t ilocalHour;
	uint8_t ilocalMinute;
	int8_t ilocalLatDegrees;
	int8_t ilocalLatMinutes;
	int16_t ilocalLongDegrees;
	int8_t ilocalLongMinutes;
	int8_t iAltDegrees;
	int8_t iAltMinutes;
	int16_t iAzDegrees;
	int8_t iAzMinutes;
	uint8_t iRAHours;
	uint8_t iRAMinutes;
	int8_t iDecDegrees;
	int8_t iDecMinutes;
	int16_t iEncoderPulses;
} ESCdata, *pESCdata;


