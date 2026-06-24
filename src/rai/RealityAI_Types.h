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
#ifndef REALITYAI_TYPES_H_
#define REALITYAI_TYPES_H_
#include <stdlib.h>
#include <stdint.h>
#include "RealityAI_Config.h"

#define RAI_MAX_MONITOR_SCORES 		16
#define RAI_MAX_MONITOR_BYTES 		(8+RAI_MAX_MONITOR_SCORES*4+1)
struct ai_monitor {
  uint8_t data[RAI_MAX_MONITOR_BYTES];
};

struct conversion_struct {
    const uint32_t* data;
    void (*const convert)(const void* usr, rai_data_t* out, int len, struct conversion_struct* conv);
};

struct energy_segmentation_struct {
    int32_t enable;
    int32_t num_channels;
    int32_t samples_per_channel;
    int32_t ihist_len;
    int32_t chan_mask;
    int32_t chan_inv_mask;
    int32_t pproc_kernel_len;
    float   pproc_scale;
    int32_t kernel_len;
    int32_t pre_trigger;
    float   threshold;
    int32_t separation;
    int32_t state;
    int32_t wr_ptr;
    int32_t rd_ptr;
    int32_t sep_cnt;
    int32_t bind;
    rai_data_t* const in_buf;
    rai_data_t* const out_buf;
    rai_data_t* const buffers[8];
    void (*const combine)(const rai_data_t*, rai_data_t*, struct energy_segmentation_struct*);
    void (*const preproc)(const rai_data_t*, rai_data_t*, struct energy_segmentation_struct*);
    rai_data_t* (*const trigger_mode)(int32_t*, struct energy_segmentation_struct*);
};

struct feature_extraction_struct {
    int32_t num_channels;
    int32_t samples_per_channel;
    int32_t window_size;
    int32_t subwindow_size;
    int32_t windows_per_channel;
    int32_t step_size;
    int32_t feature_length;
    void (*const feature_extractor)(rai_data_t*, rai_data_t*, struct feature_extraction_struct*);
};

struct smoothing_struct {
    int32_t counter;
    int32_t pos;
    uint32_t state;
    int32_t len;
    int32_t threshold;
<<<<<<< HEAD
    float  decision_threshold;
    int32_t num_scores;
    const int32_t buffer_size;
    void *const buffer;
    float (*const smoother)(const void*, int32_t, struct smoothing_struct*);
    int (*const decision)(const float* in, int len, struct smoothing_struct*);
};

typedef struct {
    uint8_t  sh, exp;
}std_params_t;

typedef struct {
    int32_t  a,b,c,d,e;
}svm_cal_params_t;

typedef struct {
    float    sf;
    int32_t  sh;
    int32_t  si, z;
}cal_data_t;

typedef struct {
    std_params_t     s;
    svm_cal_params_t p;
    cal_data_t       x, y;
}int8_cal_t;

typedef enum {
    RAI_STATUS_CRITICAL_ERROR = -1,
    RAI_STATUS_VALID = 0,
    RAI_STATUS_NO_RESULTS = 1
}rai_status_t;

struct rai_model_struct {
    struct conversion_struct* Conv;
    struct energy_segmentation_struct* Eseg;
    struct feature_extraction_struct* Fe;
    struct smoothing_struct* Smo;
    const int8_cal_t* Cal;
    const uint32_t* Excl;
    const void* Dev;
    const int32_t class_index;
    const uint32_t flags;
    float* Class_scores;
    int32_t num_classes;
    int32_t num_learners;
    int32_t subset_feature_length;
    float (*const predict)(rai_data_t*, struct rai_model_struct*, int*);
    void* network;
    float results[2];
    struct ai_monitor mon;
    rai_status_t status;
};

struct svm_network_struct {
    const void* Beta;
    const void* Bias;
    const void* Mu;
    const void* Sigma;
    const int8_t* coding_matrix;
    float* learner_scores;
};

struct neural_network_struct {
    const void* Mu;
    const void* Sigma;
    int32_t (*const compute)(void*, void** out, struct rai_model_struct*);
};

struct detector_network_struct {
    const void* Mu;
    const void* Sigma;
    const void* Centroid;
    float threshold;
    int32_t num_clusters;
    float scale;
    int32_t (*const compute)(void*, void** out, struct rai_model_struct*);
};

typedef union {
    int32_t  i32;
    float    f32;
} rai_value_t;

#define RAI_CLZ_NO_RESULTS          0

/* Configuration Commands */
#define RAI_CMD_SMO_CTRL            0x0U
#define RAI_CMD_SMO_LEN             0x1U
#define RAI_CMD_SMO_GRP             0x2U
#define RAI_CMD_DET_THRESH          0x3U

/* Configuration Argument */
#define RAI_VAL_F32(x)              ((rai_value_t){ .f32 = (x) })
#define RAI_VAL_I32(x)              ((rai_value_t){ .i32 = (x) })
#define RAI_VAL_SMO_DISABLE         RAI_VAL_I32(0)
#define RAI_VAL_SMO_ENABLE          RAI_VAL_I32(1)
#define RAI_VAL_SMO_GRP_START       RAI_VAL_I32(1)
#define RAI_VAL_SMO_GRP_END         RAI_VAL_I32(0)

/* Model Options */
=======
    const int32_t buffer_size;
    void *const buffer;
    float (*const smoother)(const void*, int32_t, struct smoothing_struct*);
};

typedef struct {
    uint8_t  sh, exp;
}std_params_t;

typedef struct {
    int32_t  a,b,c,d,e;
}svm_cal_params_t;

typedef struct {
    float    sf;
    int32_t  sh;
    int32_t  si, z;
}cal_data_t;

typedef struct {
    std_params_t     s;
    svm_cal_params_t p;
    cal_data_t       x, y;
}int8_cal_t;

typedef enum {
    RAI_STATUS_CRITICAL_ERROR = -1,
    RAI_STATUS_VALID = 0,
    RAI_STATUS_NO_RESULTS = 1
}rai_status_t;

struct rai_model_struct {
    struct conversion_struct* Conv;
    struct energy_segmentation_struct* Eseg;
    struct feature_extraction_struct* Fe;
    struct smoothing_struct* Smo;
    const int8_cal_t* Cal;
    const uint32_t* Excl;
    const void* Dev;
    const int32_t class_index;
    const uint32_t flags;
    float* Class_scores;
    int32_t num_classes;
    int32_t num_learners;
    int32_t subset_feature_length;
    float (*const predict)(rai_data_t*, struct rai_model_struct*, int*);
    void* network;
    float results[2];
    struct ai_monitor mon;
    rai_status_t status;
};

struct svm_network_struct {
    const void* Beta;
    const void* Bias;
    const void* Mu;
    const void* Sigma;
    const int8_t* coding_matrix;
    float* learner_scores;
};

struct neural_network_struct {
    const void* Mu;
    const void* Sigma;
    int32_t (*const compute)(void*, void** out, struct rai_model_struct*);
};

struct anomaly_network_struct {
    const void* Mu;
    const void* Sigma;
    const void* Centroid;
    float threshold;
    int32_t num_clusters;
    int32_t (*const compute)(void*, void** out, struct rai_model_struct*);
};


#define RAI_CLZ_NO_RESULTS          0

#define RAI_CMD_SMO_CTRL            0x0U
#define RAI_CMD_SMO_LEN             0x1U
#define RAI_CMD_SMO_GRP             0x2U

#define RAI_VAL_SMO_DISABLE         0x0U
#define RAI_VAL_SMO_ENABLE          0x1U
#define RAI_VAL_SMO_GRP_START       0x1U
#define RAI_VAL_SMO_GRP_END         0x0U

>>>>>>> branch 'main' of git@github.com:Wendellee/Embedded-Competition-Renesas-Track.git
#define RAI_FLAGS_TYPE_MASK         0x01U
#define RAI_FLAGS_TYPE_SHIFT        0U
#define RAI_FLAGS_TYPE_CLASSIFY     (0U << RAI_FLAGS_TYPE_SHIFT)
#define RAI_FLAGS_TYPE_REGRESSION   (1U << RAI_FLAGS_TYPE_SHIFT)

#define RAI_FLAGS_QUANT_MASK        0x02U
#define RAI_FLAGS_QUANT_SHIFT       1U
#define RAI_FLAGS_QUANT_DIS         (0U << RAI_FLAGS_QUANT_SHIFT)
#define RAI_FLAGS_QUANT_ENA         (1U << RAI_FLAGS_QUANT_SHIFT)

#define RAI_FLAGS_NET_MASK          0x0CU
#define RAI_FLAGS_NET_SHIFT         2U
#define RAI_FLAGS_NET_SVM           (0U << RAI_FLAGS_NET_SHIFT)
#define RAI_FLAGS_NET_NN            (1U << RAI_FLAGS_NET_SHIFT)
#define RAI_FLAGS_NET_AD            (2U << RAI_FLAGS_NET_SHIFT)

#define RAI_FLAGS_GRP_SMO_MASK      0x10U
#define RAI_FLAGS_GRP_SMO_SHIFT     4U
#define RAI_FLAGS_GRP_SMO_DIS       (0U << RAI_FLAGS_GRP_SMO_SHIFT)
#define RAI_FLAGS_GRP_SMO_ENA       (1U << RAI_FLAGS_GRP_SMO_SHIFT)

#define RAI_FLAGS_SCORE_SMO_MASK    0x20U
#define RAI_FLAGS_SCORE_SMO_SHIFT   5U
#define RAI_FLAGS_SCORE_SMO_DIS     (0U << RAI_FLAGS_SCORE_SMO_SHIFT)
#define RAI_FLAGS_SCORE_SMO_ENA     (1U << RAI_FLAGS_SCORE_SMO_SHIFT)

#ifdef RAI_STACK_TRACE
extern void edsp_stack_init_(void);
extern void edsp_stack_check_(void);
extern int* edsp_start_ptr;
extern int* edsp_end_ptr;
#define edsp_stack_init()  edsp_stack_init_()
#define edsp_stack_check() edsp_stack_check_()
#else
#define edsp_stack_init()
#define edsp_stack_check()
#endif

#endif /* REALITYAI_TYPES_H_ */
