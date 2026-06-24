/************************************************************************************
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  Copyright (c) Renesas of America, Inc.  All rights reserved.
 *  Use and distribution are subject to applicable contract restrictions.
 * **********************************************************************************/
#ifndef REALITYAI_CONFIG_H
#define REALITYAI_CONFIG_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

<<<<<<< HEAD
#define RAI_EDSP_VERSION_CODE   0x10B00
#define RAI_EDSP_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))

#define RAI_PLATFORM_ARM        1
/* #undef RAI_PLATFORM_RX */
/* #undef RAI_PLATFORM_RH850 */
/* #undef RAI_PLATFORM_RL78 */
/* #undef RAI_PLATFORM_X86_64 */
/* #undef RAI_PREC_FIXEDPOINT */
/* #undef RAI_NPOW2_FFT */

#if RAI_PREC_FIXEDPOINT
    typedef int16_t                  rai_data_t;
#else
    typedef float                    rai_data_t;
#endif

/* #undef RAI_STACK_TRACE */
#define RAI_MAX_FFT_LEN         512
#define RAI_MAX_FFT2_LEN        1
#define RAI_MAX_NUM_CLASSES     6
#define RAI_MAX_NUM_CHAN        6
#define RAI_MAX_NUM_LEARNERS    1
#define RAI_MAX_SAMPS_PER_CHAN  512
#define RAI_MAX_FEAT_LEN        1536
=======
#define RAI_EDSP_VERSION_CODE   0x10A00
#define RAI_EDSP_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))

#define RAI_PLATFORM_ARM        1
/* #undef RAI_PLATFORM_RX */
/* #undef RAI_PLATFORM_RH850 */
/* #undef RAI_PLATFORM_RL78 */
/* #undef RAI_PLATFORM_X86_64 */
/* #undef RAI_PREC_FIXEDPOINT */
/* #undef RAI_NPOW2_FFT */

#if RAI_PREC_FIXEDPOINT
    typedef int16_t                  rai_data_t;
#else
    typedef float                    rai_data_t;
#endif

/* #undef RAI_STACK_TRACE */
#define RAI_MAX_FFT_LEN         512
#define RAI_MAX_FFT2_LEN        1
#define RAI_MAX_NUM_CLASSES     4
#define RAI_MAX_NUM_CHAN        3
#define RAI_MAX_NUM_LEARNERS    1
#define RAI_MAX_SAMPS_PER_CHAN  512
#define RAI_MAX_FEAT_LEN        768
>>>>>>> branch 'main' of git@github.com:Wendellee/Embedded-Competition-Renesas-Track.git
#define RAI_MAX_SMO_WIN_LEN     1

#if defined(__clang__) || defined(__GNUC__)
    #define RAI_ATTR_ALIGN(x)           __attribute__((aligned(x)))
    #define RAI_STATIC_FORCEINLINE      __attribute__((always_inline)) static inline
#else
    #define RAI_ATTR_ALIGN(x)
    #define RAI_STATIC_FORCEINLINE
#endif

#define RAI_STRINGIFY(a)   #a
#if defined(__CCRL__)
    #define RAI_ATTR_CONST_SECTION(x)  _Pragma(RAI_STRINGIFY(section const .const ## x))
#else
    #define RAI_ATTR_CONST_SECTION(x)
#endif

#ifdef __cplusplus
}
#endif
#endif /* REALITYAI_CONFIG_H */
