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
#ifndef REALITYAI_H_
#define REALITYAI_H_
#include <stdint.h>
#include "RealityAI_Config.h"
#include "RealityAI_Types.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Reality AI data conversion function. Converts user input data to the native data type used by the
 * Reality AI inference engine. User provided data type is automatically detected.
 *
 * \param[in]      usr  Void pointer to input data buffer at the specified user data type e.g. int8,...,float32.
 *                      Expected multi-channel buffer format:
 *                      ch0[0],ch0[1],...,ch0[N-1], ch1[0],ch1[1],...,ch1[N-1], ... where N is SAMPLES_PER_CHANNEL
 * \param[in, out] out  Pointer to the output data buffer at the native data type <rai_data_t>
 * \param[in]      len  Length of the input and output data buffer in elements e.g. <MODELNAME>_SAMPLES_PER_FRAME
 * \param[in]      model Pointer to the model struct
 * \return  Returns 0 on success otherwise -1 on failure
 */
int   RealityAI_convert(void* usr, rai_data_t* out, int32_t len, struct rai_model_struct* model);

/**
 * \brief Reality AI prediction function. Consumes an input data buffer and returns a prediction provided
 * by the RealityAI inference engine.
 * \param[in]    in     Pointer to the input data buffer at the native data type <rai_data_t>
 * \param[in]    model  Pointer to the model struct
 * \return       Returns the floating-point prediction. For classifiers, round and cast to integer type
 */
float  RealityAI_predict(rai_data_t* in, struct rai_model_struct* model);

/** \brief Reality AI energy-trigger segmentation function. Consumes input samples, computes a trigger
 * waveform and outputs a buffer on trigger threshold event.
 *
 * \param[in]    in     Pointer to the input data buffer at the native data type <rai_data_t>
 * \param[out]   out    Pointer to the output data buffer at the native data type <rai_data_t>
 * \param[in]    model  Pointer to the model struct
 * \return       Returns True on trigger event, otherwise False. When True the output buffer is full
 *               amd ready for usage.
 */
int    RealityAI_segmentor(const rai_data_t* in, rai_data_t* out, struct rai_model_struct* model);

/** \brief Reality AI predictive smoothing function. Consumes and smooths the prediction from RealityAI_predict
 * and returns the smoothed prediction. For usage see the model predict API.
 *
 * \param[in]    prediction  Inference result to be added to the internal smoothing buffer
 * \param[in]    model       Pointer to the model struct
 * \return       Returns the smoothed floating-point prediction. For classifiers, round and cast to integer type
 */
float  RealityAI_smooth(float prediction, struct rai_model_struct* model);

/** \brief Reality AI configuration function based on a command/value pair.
 *  As defined in RealityAI_Types.h, commands are prefixed using RAI_CMD and values are prefixed RAI_VAL.
 *  The following command are supported:
 *         RealityAI_config(RAI_CMD_SMO_CTRL, RAI_VAL_SMO_DISABLE, model)    Enable Smoothing
 *         RealityAI_config(RAI_CMD_SMO_LEN,  10, model)                     Modify smoothing buffer length
 *         RealityAI_config(RAI_CMD_SMO_GRP, RAI_VAL_SMO_GRP_START, model)   Start Group Smoothing
 *
 * \param[in]   cmd    Configuration command e.g. RAI_CMD_SMO_CTRL
 * \param[in]   val    Configuration value as supported by the specified command e.g. RAI_VAL_SMO_DISABLE
 *  \param[in]  model  Pointer to the model struct
 * \return  Returns 0 on success otherwise -1 on failure
 */
int    RealityAI_config(uint32_t cmd, uint32_t val, struct rai_model_struct* model);

/** \brief Reality AI helper function to get user data type size in bytes.
 *
 * \param[in]    model   Pointer to the model struct to be used with the RealityAI inference engine.
 * \return       Size of the user data type in bytes
 */
int    RealityAI_get_user_dtype_size(struct rai_model_struct* model);

/** \brief Reality AI helper function to get class scores for classification models only.
 *
 * \param[in, out] scores        Pointer for storing the class scores, model class scores are written
 *                               directly to this buffer.
 * \param[in]      len           Length of the class scores buffer, e.g. <MODELNAME>_NUM_CLASSES
 * \param[in]      softMaxFlag   Flag for enabling softmax or normalized exponential conversion to obtain as
 *                               confidence or probability. Enabling softmax is only required for SVM model types.
 *                               is written to the buffer.
 * \param[in]      model         Pointer to the model struct
 * \return  Returns 0 on success otherwise -1 on failure
 */
int    RealityAI_get_class_scores(float* scores, int32_t len, int softMaxFlag, struct rai_model_struct* model);

/** \brief Reality AI helper function to get learner scores for SVM model types only.
 *
 * \param[in, out] scores  Pointer for storing the learner scores, model learner scores are written
 *                         directly to this buffer.
 * \param[in]      len     Length of the learner scores buffer, e.g. <MODELNAME>_NUM_LEARNERS
 * \param[in]      model   Pointer to the model struct
 * \return  Returns 0 on success otherwise -1 on failure
 */
int    RealityAI_get_learner_scores(float* scores, int32_t len, struct rai_model_struct* model);

/** \brief Reality AI helper function to get the AI Monitor byte array length. This function should
 *  be applied to the Data Shipper diagnostic_data_len field
 *
 * \param[in]  model Pointer to the model struct
 * \return  Returns the array length in bytes
 */
int    RealityAI_get_monitor_data_size(struct rai_model_struct* model);

/** \brief Reality AI helper function to get the byte array pointer for AI Monitor. This function should
 *  be applied to the Data Shipper p_diagnostic_data field. This method calls RealityAI_get_class_scores
 *  with the softmax flag enabled.
 *
 *  For example
 *  rai_data_shipper_write_params_t arg;
 *  arg.diagnostic_data_len = (uint8_t)RealityAI_get_monitor_data_size(p_model);
 *  arg.p_diagnostic_data = (uint8_t*)RealityAI_get_monitor_data(p_model);
 *
 * \param[in]  model Pointer to the model struct
 * \return  Returns a pointer to the AI Monitor data payload
*/
struct ai_monitor* RealityAI_get_monitor_data(struct rai_model_struct* model);

/** \brief Reality AI helper function to determine the status of the inference results. This function should
 *  be called after prediction and smoothing, and is used to determine if the prediction is valid or if a critical
 *  error occurred.
 *
 *  For example:
 *  pred = RealityAI_predict(buffer, p_model);
 *  smo_pred = RealityAI_smooth(pred, p_model);
 *  if( RealityAI_get_status(p_model) == RAI_STATUS_VALID )
 *      // do something
 *
 * \param[in]  model Pointer to the model struct
 * \return  Returns inference status enum type
*/
rai_status_t RealityAI_get_status(struct rai_model_struct* model);

#ifdef __cplusplus
}
#endif
#endif /* REALITYAI_H_ */
