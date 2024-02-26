#ifndef __FFT_EXAMPLES_H__
#define __FFT_EXAMPLES_H__

#ifdef __cplusplus
extern "C" {
#endif
#ifndef ARM_MATH_CM4
#define ARM_MATH_CM4 1U
#endif

#ifndef __FPU_PRESENT
#define __FPU_PRESENT 1U
#endif //__FPU_PRESENT

/* Includes ------------------------------------------------------------------*/
#include <arm_math.h>
#include <stdbool.h>
/* Defines  ------------------------------------------------------------------*/

/* FFT settings */
// #define FFT_SAMPLES				           /* 128 real party and 128 imaginary parts */
// #define FFT_SIZE				   (SAMPLES / 2U)   /* FFT size is always the same size as we have samples, so 128 in our case */
// #define FFT_BAR_MAX_HEIGHT		80U              /* 80 px on the LCD */
#define FFT_BUFFER_SIZE       (256U)

/* GLOBAL VARIABLES  ------------------------------------------------------------------ */


/* fft processing variables ===*/
extern arm_rfft_fast_instance_f32 fftHandler;

extern uint16_t pInFFTBufferFH[];
extern float fftBufferInLft[];
extern float fftBufferInRght[];
extern float fftBufferOutLft[];
extern float fftBufferOutRght[];
extern uint8_t fftFlag;

extern bool flgBufferCopyComplete; // copy buffer from txCpltCallback complete

/* MATH MACROS  ------------------------------------------------------------------ */
#define INT16_TO_FLOAT_SCALAR (0.00003051850947599719f)

/* 1/ln(2) */
#define INV_LN2 (1.442695040888963f)

/* 2*pi */
#define TWO_PI (6.283185307179586f)

/* pi/2 */
#define PI_OVER_TWO (1.5707963267948966f)

/* 1/(TWO_PI) */
#define INVERSE_TWO_PI (0.159154943091895f)

/* ln(10.0)/20.0 */
#define LOG_TEN_OVER_TWENTY (0.11512925464970228420089957273422)

/* 20.0/ln(10.0) */
#define TWENTY_OVER_LOG_TEN (8.6858896380650365530225783783321)

#define SQRT_TWO_OVER_TWO (0.70710678118654757273731092936941422522068023681641)

/* Utility Function MATH MACROS  ------------------------------------------------------------------ */

/* Limit value value to the range (l, u) */
#define LIMIT(value,lower,upper) ((value) < (lower) ? (lower) : \
                                 ((value) > (upper) ? (upper) : (value)))


/* Linearly interpolate between y0 and y1

  y(x) = (1 - x) * y(0) + x * y(1) | x in (0, 1)

 Function-style signature:
    float LIN_INTERP(float x, float y0, float y1)
    double LIN_INTERP(double x, double y0, double y1)

 Parameters:
    x:  x-value at which to calculate y.
    y0: y-value at x = 0.
    y1: y-value at x = 1.

 Returns:
    Interpolated y-value at specified x.
 */
#define LIN_INTERP(x, y0, y1) ((y0) + (x) * ((y1) - (y0)))


/* Convert frequency from Hz to Radians per second
 Function-style signature:
    float HZ_TO_RAD(float frequency)
    double HZ_TO_RAD(double frequency)

 Parameters:
    frequency:  Frequency in Hz.

 Returns:
    Frequency in Radians per second.
 */
#define HZ_TO_RAD(f) (TWO_PI * (f))


/* Convert frequency from Radians per second to Hz

 Function-style signature:
    float RAD_TO_HZ(float frequency)
    double RAD_TO_HZ(double frequency)

 Parameters:
 frequency:  Frequency in Radians per second.

 Returns:
 Frequency in Hz.
 */
#define RAD_TO_HZ(omega) (INVERSE_TWO_PI * (omega))


/* Fast exponentiation function
 y = e^x
 Function-style signature:
    float F_EXP(float x)
    double F_EXP(double x)
 Parameters:
    x:  Value to exponentiate.
 Returns:
    e^x.
 */
#define F_EXP(x) ((362880 + (x) * (362880 + (x) * (181440 + (x) * \
                  (60480 + (x) * (15120 + (x) * (3024 + (x) * (504 + (x) * \
                  (72 + (x) * (9 + (x) ))))))))) * 2.75573192e-6)


/* Decibel to Amplitude Conversion */
#define DB_TO_AMP(x) ((x) > -150.0 ? expf((x) * LOG_TEN_OVER_TWENTY) : 0.0f)
#define DB_TO_AMPD(x) ((x) > -150.0 ? exp((x) * LOG_TEN_OVER_TWENTY) : 0.0)

/* Amplitude to Decibel Conversion */
#define AMP_TO_DB(x) (((x) < 0.0000000298023223876953125) ? -150.0 : \
                      (logf(x) * TWENTY_OVER_LOG_TEN))
#define AMP_TO_DBD(x) (((x) < 0.0000000298023223876953125) ? -150.0 : \
                       (log(x) * TWENTY_OVER_LOG_TEN))

/* Smoothed Absolute Value */
#define SMOOTH_ABS(x) (sqrtf(((x) * (x)) + 0.025) - sqrtf(0.025))
#define SMOOTH_ABS_LR(x,y) ((SMOOTH_ABS((x)) + SMOOTH_ABS((y)))/2)
#define SMOOTH_ABS_LR_F(x,y) (\
   sqrtf(\
      ((x)*(x)) + ((y)*(y))\
   )/2\
)

#define CALCULATE_FREQUENCY(x,y) ((x)*(y)/((float)(FFT_BUFFER_SIZE)))
/* Exported FUNCTIONS ------------------------------------------------------------------ */

// This maps to range -32767 to 32767:
#define INT16_TO_FLOAT(x) ((float)((x) * 3.0518509476e-5)/3.0518509475e-5)
// #define INT16_TO_FLOAT(x) (float) ((int) ((x)<<16)|(x));
#define INT16_TO_FLOAT_SCLR(x) ((float)(((x)*(INT16_TO_FLOAT_SCALAR)/3.0518509476e-5)+3.0518509476e-5))
#define FLOAT_TO_UINT16(x) ((uint16_t) ((32767 * (x) + 32768.5)) - 32768)
#define FLOAT_TO_INT16(x) ((int16_t) ((32767 * (x) + 32768.5)) - 32768)

void FFT_Init_FFTHandlers(void);
void FFT_TEST_Array(uint32_t audioFs);
void FFT_CalculatePeaksAndFrq(uint32_t audioFs);

#ifdef __cplusplus
}
#endif

#endif // __FFT_EXAMPLES_H__