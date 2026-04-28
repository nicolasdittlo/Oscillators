/**
MIT License

Copyright (c) 2022-2025 Alexandre R. J. Francois

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

#ifndef RESONATOR_BANK_H
#define RESONATOR_BANK_H


typedef struct _resonator_bank *resonator_bank;


extern resonator_bank resonator_bank_new(int num_resonators, const float *frequencies,
                                         const float *alphas, const float *betas, const float *gammas,
                                         float sample_rate);
extern void resonator_bank_free(resonator_bank r);

extern float resonator_bank_get_samplerate(resonator_bank r);
extern int resonator_bank_get_num_resonators(resonator_bank r);

extern float resonator_bank_get_frequency_value(resonator_bank r, int index);

extern float resonator_bank_get_alpha_value(resonator_bank r, int index);
extern void resonator_bank_set_all_alphas(resonator_bank r, float alpha);

extern void resonator_bank_get_powers(resonator_bank r, float *dest, int size);
extern void resonator_bank_get_amplitudes(resonator_bank r, float *dest, int size);
extern void resonator_bank_get_phases(resonator_bank r, float *dest, int size);
extern void resonator_bank_get_delta_phases(resonator_bank r, float *dest, int size);

extern void resonator_bank_get_frequencies(resonator_bank r, float *dest, int size);
extern void resonator_bank_get_instantaneous_frequencies(resonator_bank r, float *dest, int size);

extern void resonator_bank_update(resonator_bank r, float sample);
extern void resonator_bank_update_with_samples(resonator_bank r, const float *samples, int size);
extern void resonator_bank_update_with_samples_stride(resonator_bank r,
                                                      const float *frame_data, int frame_size, int sample_stride);

#endif
