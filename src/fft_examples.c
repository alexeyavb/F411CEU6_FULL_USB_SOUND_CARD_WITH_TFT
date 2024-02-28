#include "fft_examples.h"
#include "usbd_audio.h"


/* Global variables */
arm_rfft_fast_instance_f32 fftHandler;
uint16_t pInFFTBufferFH[AUDIO_TOTAL_BUF_SIZE/4];
uint16_t pInFFTBufferFHFull[AUDIO_TOTAL_BUF_SIZE/4];

float peakVal = 0.00f, peakValL = 0.00f, peakValR = 0.00f;

bool flgBufferCopyComplete;
bool flgBufferCopyCompleteFull;
bool firstHalf;
extern void DrawPeaksIndicator(float lValue, float RValue);

struct PeakRecord{
    uint32_t idx;
    uint32_t Frq;
    struct pPeakRecord *prev;    
    struct pPeakRecord *next;
    float peakL, peakR, peakTotal;
    uint32_t currenttick;
} *PPEAK_RECORD;

float fftBufferInLft[FFT_BUFFER_SIZE];
float fftBufferInRght[FFT_BUFFER_SIZE];
float fftBufferOutLft[FFT_BUFFER_SIZE];
float fftBufferOutRght[FFT_BUFFER_SIZE];
uint8_t fftFlag;


void FFT_Init_FFTHandlers(void){
    flgBufferCopyComplete = false;
    flgBufferCopyCompleteFull = false;
    firstHalf = true;
    arm_rfft_fast_init_f32(&fftHandler,FFT_BUFFER_SIZE);
}

void FFT_TEST_Array(uint32_t audioFs){
    uint16_t fftInIdx = 0U;
    // peakValL = 0.00f; peakValR =0.00f;
    for(uint16_t i = 0U; i < AUDIO_TOTAL_BUF_SIZE/4; i+=8){   
        if (firstHalf){
            fftBufferInLft[fftInIdx]     = INT16_TO_FLOAT(pInFFTBufferFH[i]  );
            fftBufferInRght[fftInIdx]    = INT16_TO_FLOAT(pInFFTBufferFH[i+1]);
            fftBufferInLft[fftInIdx+1]   = INT16_TO_FLOAT(pInFFTBufferFH[i+2]);
            fftBufferInRght[fftInIdx+1]  = INT16_TO_FLOAT(pInFFTBufferFH[i+3]);
            fftBufferInLft[fftInIdx+2]   = INT16_TO_FLOAT(pInFFTBufferFH[i+4]);
            fftBufferInRght[fftInIdx+2]  = INT16_TO_FLOAT(pInFFTBufferFH[i+5]);
            fftBufferInLft[fftInIdx+3]   = INT16_TO_FLOAT(pInFFTBufferFH[i+6]);
            fftBufferInRght[fftInIdx+3]  = INT16_TO_FLOAT(pInFFTBufferFH[i+7]);
        }
        else{
            fftBufferInLft[fftInIdx]     = INT16_TO_FLOAT(pInFFTBufferFHFull[i]  );
            fftBufferInRght[fftInIdx]    = INT16_TO_FLOAT(pInFFTBufferFHFull[i+1]);
            fftBufferInLft[fftInIdx+1]   = INT16_TO_FLOAT(pInFFTBufferFHFull[i+2]);
            fftBufferInRght[fftInIdx+1]  = INT16_TO_FLOAT(pInFFTBufferFHFull[i+3]);
            fftBufferInLft[fftInIdx+2]   = INT16_TO_FLOAT(pInFFTBufferFHFull[i+4]);
            fftBufferInRght[fftInIdx+2]  = INT16_TO_FLOAT(pInFFTBufferFHFull[i+5]);
            fftBufferInLft[fftInIdx+3]   = INT16_TO_FLOAT(pInFFTBufferFHFull[i+6]);
            fftBufferInRght[fftInIdx+3]  = INT16_TO_FLOAT(pInFFTBufferFHFull[i+7]);

        }

        fftInIdx+=4;
        if(fftInIdx >= FFT_BUFFER_SIZE){
            arm_rfft_fast_f32(&fftHandler, fftBufferInLft, fftBufferOutLft, 0);
            arm_rfft_fast_f32(&fftHandler, fftBufferInRght, fftBufferOutRght, 0);
            fftFlag = 1U;
            fftInIdx = 0U;
            FFT_CalculatePeaksAndFrq(audioFs);
        }
    }    
}

void FFT_CalculatePeaksAndFrq(uint32_t audioFs){
//     extern void printMsg(char* format, ...);
//     float peakVal = 0.00f, peakValL = 0.00f, peakValR = 0.00f;
//     (int)(20*log10f(complexABS(fft_out_buf[i], fft_out_buf[i+1])))-offset;

    uint16_t frqIdx = 0U; uint32_t rz = 0U;
    
    for(uint16_t i = 0U; i < FFT_BUFFER_SIZE; i++){
        float peakValLeft = 0.00f;
        float peakValRight = 0.00f;
        float CurPeakVal1 = 0.00f;
//         float CurPeakVal2 = 0.00f;
        float peakHz = 0.00f;

        // Общий уровень
        CurPeakVal1 = SMOOTH_ABS_LR(fftBufferOutLft[i], fftBufferOutRght[i]);
//        CurPeakVal2 = SMOOTH_ABS_LR_F(fftBufferOutLft[i], fftBufferOutRght[i]);
        
        // По каналам
        peakValLeft = SMOOTH_ABS(fftBufferOutLft[i]);
        peakValRight = SMOOTH_ABS(fftBufferOutRght[i]);
        
        // Результирующие переменные    
        
        peakVal = MAX(peakVal,CurPeakVal1);
        peakValL = MAX(peakValLeft,peakValL);
        peakValR = MAX(peakValRight,peakValR);
        
        peakHz = CALCULATE_FREQUENCY(frqIdx, audioFs);
        rz = MAX(peakHz,rz);        
        UNUSED(rz);
        frqIdx++;
    }
}

void PrepareArrays(uint32_t audiofreq){
         FFT_TEST_Array(audiofreq);                
}