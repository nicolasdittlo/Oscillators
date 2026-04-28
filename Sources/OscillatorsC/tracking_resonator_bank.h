/**
MIT License

Copyright (c) 2025-2026 Alexandre R. J. Francois

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/**
   Ported to C by Nicolas Dittlo (2026) - peuple.des.limbes@gmail.com
*/

#ifndef TRACKING_RESONATOR_BANK_H
#define TRACKING_RESONATOR_BANK_H


typedef struct _tracking_resonator_bank *tracking_resonator_bank;


extern tracking_resonator_bank tracking_resonator_bank_new(int num_resonators,
                                                           const float *natural_frequencies,
                                                           const float *alphas,
                                                           const float *betas,
                                                           const float *gammas,
                                                           float samplerate);
extern void tracking_resonator_bank_free(tracking_resonator_bank t);

extern float tracking_resonator_bank_get_sample_rate(tracking_resonator_bank t);
extern int tracking_resonator_bank_get_num_resonators(tracking_resonator_bank t);

extern void tracking_resonator_bank_get_frequencies(tracking_resonator_bank r, float *dest, int size);
extern void tracking_resonator_bank_get_natural_frequencies(tracking_resonator_bank t, float *dest, int size);
extern void tracking_resonator_bank_get_resonant_frequencies(tracking_resonator_bank t, float *dest, int size);

extern float tracking_resonator_bank_get_resonant_frequency_value(tracking_resonator_bank t, int index);

extern void tracking_resonator_bank_get_powers(tracking_resonator_bank t, float *dest, int size);
extern void tracking_resonator_bank_get_amplitudes(tracking_resonator_bank t, float *dest, int size);
extern void tracking_resonator_bank_get_phases(tracking_resonator_bank t, float *dest, int size);
extern void tracking_resonator_bank_get_delta_phases(tracking_resonator_bank t, float *dest, int size);

extern float tracking_resonator_bank_get_accPower(tracking_resonator_bank t);

extern void tracking_resonator_bank_update(tracking_resonator_bank t, float sample);
extern void tracking_resonator_bank_update_with_samples(tracking_resonator_bank t,
                                                        const float *samples, int size);
extern void tracking_resonator_bank_update_with_samples_stride(tracking_resonator_bank t,
                                                               const float *frame_data,
                                                               int frame_size,
                                                               int sample_stride);

extern void tracking_resonator_bank_set_time_constant(tracking_resonator_bank t,
                                                      float tau, int frame_length, int sample_stride,
                                                      float samplerate);

#endif
