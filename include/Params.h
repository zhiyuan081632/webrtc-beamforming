/*
 * Params.h
 *
 * All front-end parameters.
 *
 * Copyright: 2020-06-30 yueyue.nyy
 */

#ifndef SRC_INCLUDE_PARAMS_H_
#define SRC_INCLUDE_PARAMS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "RET_CODE.h"

// max no. of input and output channels
#ifndef MAX_NUM_CHS
#define MAX_NUM_CHS 50
#endif

// max no. of reference channels
#ifndef MAX_NUM_REFS
#define MAX_NUM_REFS 10
#endif

/**
 * data block type
 */
typedef enum BLOCK_TYPE_ {
  /**
   * default block type
   */
  BLOCK_DEFAULT = 0,

  /**
   * 1 ms data block for real-time processing
   */
  BLOCK_1MS = 1,

  /**
   * 5 ms data block for real-time processing
   */
  BLOCK_5MS = 5,

  /**
   * 10 ms data block
   */
  BLOCK_10MS = 100,

  /**
   * 10 ms data block, better performance but longer delay
   */
  BLOCK_10MS_LD = 101,

  /**
   * data block with lower computational complexity but lower performance
   */
  BLOCK_10MS_LOW = 102,

  /**
   * 16 ms data block
   */
  BLOCK_16MS = 160,

  /**
   * 16 ms data block, better performance but longer delay
   */
  BLOCK_16MS_LD = 161,

  /**
   * data block with lower computational complexity but lower performance
   */
  BLOCK_16MS_LOW = 162,

  /**
   * 20 ms data block
   */
  BLOCK_20MS = 200,

  /**
   * 20 ms data block, better performance but longer delay
   */
  BLOCK_20MS_LD = 201,

  /**
   * data block with lower computational complexity but lower performance
   */
  BLOCK_20MS_LOW = 202,

  /**
   * 32 ms data block
   */
  BLOCK_32MS = 320,

  /**
   * 32 ms data block, better performance but longer delay
   */
  BLOCK_32MS_LD = 321,

  /**
   * data block with lower computational complexity but lower performance
   */
  BLOCK_32MS_LOW = 322,

  /**
   * 40 ms data block
   */
  BLOCK_40MS = 400,

  /**
   * data block with lower computational complexity but lower performance
   */
  BLOCK_40MS_LOW = 402,
} BLOCK_TYPE;

/**
 * working mode.
 */
typedef enum MODE_ {
  /**
   * communication mode
   */
  MODE_COMM = 0,

  /**
   * mixer mode, used to mix sounds and add reverberation
   */
  MODE_MIXER = 1,

  /**
   * Used to perform algorithm evaluation.
   */
  MODE_EVAL = 2,

  /**
   * Used for signal quality testing.
   */
  MODE_TEST = 3,

  /**
   * General mode, suitable for most applications.
   */
  MODE_GENERAL = 4,

  /**
   * Exhibition mode, for high reverberation scenes.
   */
  MODE_EXHIBITION = 5,

  /**
   * multistream mode
   */
  MODE_MULTISTREAM = 9,

  /**
   * general train mode for data simulation
   */
  MODE_TRAIN = 600,

  /**
   * extract ideal ratio mask feature
   */
  MODE_TRAIN_IRM = 601,

  /**
   * extract nbf feature
   */
  MODE_TRAIN_NBF = 602,

  /**
   * train vocal, accompaniment separation for songs
   */
  MODE_TRAIN_VOCAL = 603,

  /**
   * feature extraction for kws training
   */
  MODE_FEAT_EXT = 700,

  /**
   * perform kws by extracted features
   */
  MODE_FEAT_KWS = 701,

  /**
   * extract simulated kws feature
   */
  MODE_FEAT_SIMU_KWS = 702,

  /**
   * perform decode only
   */
  MODE_DECODE_KWS = 703,

  /**
   * front-end for low power consumption applications
   */
  MODE_EMBEDDED_LOWPOW = 56540,

  /**
   * dominant mode for embedded applications.
   */
  MODE_EMBEDDED_DOMINANT = 56541,

  /**
   * embedded bss mode
   */
  MODE_EMBEDDED_BSS = 56542,

  /**
   * general embedded multistream mode
   */
  MODE_EMBEDDED_MULTISTREAM = 56543,

  /**
   * command mode
   */
  MODE_EMBEDDED_CMD = 56545,

  /**
   * mono kws mode
   */
  MODE_EMBEDDED_MONO_KWS = 56546,

  /**
   * mono vad mode
   */
  MODE_EMBEDDED_MONO_VAD = 56547,

  /**
   * application for diffuse environments
   */
  MODE_EMBEDDED_DIFFUSE = 56548,

  /**
   * application for diffuse environments
   */
  MODE_EMBEDDED_DIFFUSE_V3 = 56549,
} MODE;

/**
 * Acoustic echo cancellation algorithms.
 */
typedef enum AEC_ALGORITHM_ {
  /**
   * without any operation
   */
  AEC_ALG_BYPASS = 0,

  /**
   * the basic AEC algorithm
   * 1xx
   */
  AEC_ALG_BASIC = 100,

  /**
   * instantaneous AEC
   */
  AEC_ALG_INST = 101,

  /**
   * the recursive least-square structure, version 3
   */
  AEC_ALG_RLS_V3 = 103,

  /**
   * the Liu Xin's AEC algorithm
   * 2xx
   */
  AEC_ALG_LX_GENERAL = 201,

  /**
   * low complexity version
   */
  AEC_ALG_LX_LOW_COMPLEXITY = 202,

  /**
   * unified algorithms
   * 3xx
   *
   * perform dr and aec together
   */
  AEC_ALG_DRAEC = 301,

  /*
   * perform aec and bss together, instantaneous model
   */
  AEC_ALG_STEREO_AEC_BSS_INST = 303,

  /**
   * Kalman approaches
   * 4xx
   *
   * STFT domain Kalman AEC
   */
  AEC_ALG_KALMAN = 400,

  /*
   * STFT domain Kalman DR and AEC
   */
  AEC_ALG_KALMAN_DRAEC = 401,
} AEC_ALGORITHM;

/**
 * Deverberation algorithms.
 */
typedef enum DR_ALGORITHM_ {
  /**
   * without any operation
   */
  DR_ALG_BYPASS = 0,

  /**
   * the basic DR algorithm
   */
  DR_ALG_BASIC = 1,

  /**
   * the BSS based DR algorithm
   * 1xx
   */
  DR_ALG_BSS = 100,

  /**
   * the draec algorithm based on bss framework
   */
  DR_ALG_DRAEC = 101,
} DR_ALGORITHM;

/**
 * Beamforming and source separation algorithms (multi-channel).
 */
typedef enum BF_ALGORITHM_ {
  /**
   * without any operation
   */
  BF_ALG_BYPASS = 0,

  /**
   * fixed beamforming
   */
  BF_ALG_FIXED = 1,

  /**
   * delay-sum beamforming
   */
  BF_ALG_DELAY_SUM = 2,

  /**
   * used for low power embedded applications
   */
  BF_ALG_LOWPOW = 200,

  /**
   * enhance dominant target speech using external pilot information
   * 3xx
   */
  BF_ALG_DOMINANT_PILOT = 301,

  /**
   * perform enhancement using KWS information
   */
  BF_ALG_DOMINANT_KWS = 303,

  /**
   * perform enhancement in diffuse scene
   */
  BF_ALG_DOMINANT_DIFFUSE = 304,

  /**
   * instantaneous stereo BSS
   * 4xx
   */
  BF_ALG_STEREO_BSS_INST = 401,

  /**
   * general stereo BSS
   */
  BF_ALG_STEREO_BSS_V4 = 403,

  /**
   * instantaneous multi-channel blind source separation algorithm
   * 5xx
   */
  BF_ALG_BSS_INST = 501,

  /**
   * general multi-channel bss
   */
  BF_ALG_BSS = 502,

  /**
   * bss for smart cockpit
   */
  BF_ALG_BSS_COCKPIT = 503,

  /**
   * bss for 4 seats cockpit with T structure
   */
  BF_ALG_BSS_T4 = 504,

  /**
   * nn based separation
   * separate vocal and accompaniment
   */
  BF_ALG_VOCAL = 601,
} BF_ALGORITHM;

/**
 * Noise suppression and aec post-filtering algorithms.
 */
typedef enum NS_ALGORITHM_ {
  /**
   * bypass the algorithm
   */
  NS_ALG_BYPASS = 0,

  /**
   * signal based aec postfiltering
   */
  NS_ALG_RES_BASIC = 1,

  /**
   * aec postfiltering with pilot information
   */
  NS_ALG_RES_PILOT = 2,

  /**
   * nn mask based aec postfiltering
   */
  NS_ALG_RES_NNMASK = 100,

  /**
   * the signal + nn mask aec postfiltering
   */
  NS_ALG_RES_NNRES = 101,

  /**
   * rnnoise based ns
   */
  NS_ALG_RNNOISE = 200,

  /**
   * vocal suppression for songs
   */
  NS_ALG_VOCAL = 601,
} NS_ALGORITHM;

/**
 * Postfiltering algorithms.
 */
typedef enum PF_ALGORITHM_ {
  /**
   * bypass the algorithm
   */
  PF_ALG_BYPASS = 0,

  /**
   * perform postfiltering using vad information
   */
  PF_ALG_VAD = 1,
} PF_ALGORITHM;

/**
 * Automatic gain control algorithms.
 */
typedef enum AGC_ALGORITHM_ {
  /**
   * bypass the operation
   */
  AGC_ALG_BYPASS = 0,

  /**
   * the basic version
   */
  AGC_ALG_BASIC = 1,

  /**
   * used to adapt agc gain for old kws
   */
  AGC_ALG_KWS = 2,
} AGC_ALGORITHM;

/**
 * volume algorithm
 */
typedef enum VOLUME_ALGORITHM_ {
  /**
   * bypass the volume calculation
   */
  VOLUME_ALG_BYPASS = 0,

  /**
   * average RMS (Root Mean Square)
   */
  VOLUME_ALG_AVERAGE_RMS = 1,

  /**
   * peak RMS
   */
  VOLUME_ALG_PEAK_RMS = 2,

  /**
   * normalized average RMS, from 0 to 100
   */
  VOLUME_ALG_NORMALIZED_AVERAGE_RMS = 3,

  /**
   * normalized peak RMS, from 0 to 100
   */
  VOLUME_ALG_NORMALIZED_PEAK_RMS = 4,
} VOLUME_ALGORITHM;

/**
 * volume positions
 */
typedef enum VOLUME_POSITION_ {
  /**
   * original volume
   */
  VOLUME_POSITION_ORIGINAL = 0,

  /**
   * volume before agc
   */
  VOLUME_POSITION_BEFORE_AGC = 1,

  /**
   * volume after agc
   */
  VOLUME_POSITION_AFTER_AGC = 2,
} VOLUME_POSITION;

/**
 * Feature extraction algorithms.
 */
typedef enum FEAT_ALGORITHM_ {
  /**
   * default feature
   */
  FEAT_ALG_DEFAULT = 0,

  /**
   * default feature
   */
  FEAT_ALG_DEFAULT_2 = 1,
} FEAT_ALGORITHM;

/**
 * Sound source localization algorithms.
 */
typedef enum SSL_ALGORITHM_ {
  /**
   * bypass the operation
   */
  SSL_ALG_BYPASS = 0,

  /**
   * basic ssl algorithm
   */
  SSL_ALG_BASIC = 1,

  /**
   * multi-ssl by bf
   */
  SSL_ALG_POST = 100,
} SSL_ALGORITHM;

/**
 * Voice activity detection algorithms.
 */
typedef enum VAD_ALGORITHM_ {
  /**
   * bypass the operation
   */
  VAD_ALG_BYPASS = 0,

  /**
   * phase based vad
   */
  VAD_ALG_PHASE = 1,

  /**
   * dominant power based vad
   */
  VAD_ALG_DOMINANT = 2,

  /**
   * use pilot information as vad
   */
  VAD_ALG_PILOT = 3,

  /**
   * nn based vad
   */
  VAD_ALG_NNVAD = 100,

  /**
   * nn based double-talk detector
   */
  VAD_ALG_NNDTD = 101,
} VAD_ALGORITHM;

/**
 * Keyword spotting algorithms.
 */
typedef enum KWS_ALGORITHM_ {
  /**
   * bypass the operation
   */
  KWS_ALG_BYPASS = 0,

  /**
   * basic kws algorithm
   */
  KWS_ALG_BASIC = 1,

  /**
   * attention based kws algorithm
   */
  KWS_ALG_ATTENTION = 2,

  /**
   * kws with shadow model
   */
  KWS_ALG_SHADOW = 3,

  /**
   * kws with channel selection
   */
  KWS_ALG_SELE = 4,

  /**
   * kws with channel selection
   */
  KWS_ALG_SELE_V2 = 5,
} KWS_ALGORITHM;

/**
 * Keyword detection policy.
 */
typedef enum KWS_DETECTION_POLICY_ {
  /**
   * detect the sequence less than 10 modeling units
   */
  KWS_DETECT_GENERAL10 = 0,

  /**
   * detect the sequence format: A B C D, relaxed constraint
   */
  KWS_DETECT_ABCD3 = 1,

  /**
   * detect the sequence format: A B C D, relaxed constraint
   */
  KWS_DETECT_ABCD2 = 2,

  /**
   * detect the sequence format: A B A B
   */
  KWS_DETECT_ABAB = 3,

  /**
   * detect the sequence format: A B A B, relaxed constraint
   */
  KWS_DETECT_ABAB3 = 4,

  /**
   * detect the sequence less than 10 modeling units, sequence order is
   * considered
   */
  KWS_DETECT_STRICT10 = 10,
} KWS_DETECTION_POLICY;

/**
 * Test item
 */
typedef enum TEST_ITEM_ {
  /**
   * bypass all operations except decimation
   */
  TEST_BYPASS = 0,

  /**
   * bypass all operations except decimation, output mic signals
   */
  TEST_BYPASS_MIC = 1,

  /**
   * bypass all operations except decimation, output reference signals
   */
  TEST_BYPASS_REF = 2,

  /**
   * signal saturation test
   */
  TEST_SATURATION = 100,

  /**
   * signal discontinuity test
   */
  TEST_DISCONTINUITY = 101,

  /**
   * signal similarity test
   */
  TEST_SIMILARITY = 102,

  /**
   * test mic amplitude difference
   */
  TEST_AMP_DIFF = 103,

  /**
   * test mic phase difference
   */
  TEST_PHASE_DIFF = 104,

  /**
   * test mic frequency responses
   */
  TEST_FREQ = 105,

  /*
   * calculate ERLE
   */
  TEST_ERLE = 200,

  /**
   * test mic-reference offset
   */
  TEST_REF_OFFSET = 201,
} TEST_ITEM;

typedef struct Params_ {
  /*
   * common parameters
   */
  int fsin;                         // input sample rate (Hz)
  int fsout;                        // output sample rate (Hz)
  int nbitsin;                      // no. of bits per sample for input signals
  int nbitsout;                     // no. of bits per sample for output signals
  BLOCK_TYPE block_type;            // data block type
  int nummics;                      // no. of mic channels
  int numrefs;                      // no. of reference channels
  int numpilots;                    // no. of pilot channels
  char pilots_as_audio;             // parse pilot information from audio
  int numins;                       // no. of total input channels
  char *chorder;                    // channel order
  int numouts;                      // no. of output channels
  char *fe_in_path;                 // input audio path for debug
  char *fe_out_path;                // output audio path for debug
  int log_level;                    // log level
  float mic_coor[MAX_NUM_CHS * 3];  // mic coordinates (x, y, z) (meter)
  MODE mode;                        // the working mode
  MODE submode;                     // submode
  float init_time;  // audio length (second) used to initialize the frontend
  int hpf_power;    // high-pass filtering power
  float saturation_level;  // saturation level

  /*
   * aec parameters
   */
  AEC_ALGORITHM aec_algorithm;  // the aec algorithm
  int aec_power;                // aec power
  int aec_power2;               // second level filter length
  float aec_level;              // aec level
  float aec_forget;             // forgetting factor
  float aec_gain;               // aec internal gain (dB)
  float aec_tde_refoffset;      // fixed reference offset
  float aec_tde_maxoffset;      // max offset supported

  /*
   * deververation params
   */
  DR_ALGORITHM dr_algorithm;  // deverberation algorithm
  int dr_power;               // filter length
  float dr_level;             // algorithm configuration

  /*
   * beamforming params
   */
  BF_ALGORITHM bf_algorithm;     // the beamforming algorithm
  int bf_power;                  // controls bf filter length
  float bf_level;                // detailed algorithm configuration
  float bf_lookaz[MAX_NUM_CHS];  // beamforming look azimuth [-180, 180], or
                                 // [-90, 90] for linear array
  float bf_lookel[MAX_NUM_CHS];  // beamforming look elevation [-90, 90]
  char bf_islocked;     // if the beam is locked to bf_look when initialized
  char *bf_model_base;  // the configuration file path

  /*
   * parameters about ns
   */
  NS_ALGORITHM ns_algorithm;  // ns algorithm
  int ns_power;               // ns filter length
  float ns_level;             // distortion parameter
  char *ns_model_base;        // aec model base

  /*
   * postfiltering params
   */
  PF_ALGORITHM pf_algorithm;  // postfiltering algorithm
  float pf_level;             // the attenuation gain

  /*
   * parameters about agc
   */
  AGC_ALGORITHM agc_algorithm;  // agc algorithm
  int agc_power;                // AGC_Power
  int agc_level;                // AGC_Level

  /*
   * params about volume
   */
  VOLUME_ALGORITHM volume_algorithm;  // volume algorithm
  VOLUME_POSITION volume_position;    // where to calculate volume

  /*
   * mix and sound effect params
   */
  float mix_mic_gain;     // human voice gain (dB)
  float mix_ref_gain;     // background music gain (dB)
  char *mix_reverb_path;  // reverb wav file path

  /*
   * feature extraction
   */
  FEAT_ALGORITHM feat_algorithm;  // feature extraction algorithm
  int feat_size;                  // feature vector size
  float feat_freq_range[2];  // normalized frequency ratio range for feature

  /**
   * paramsters about sound source localization
   */
  SSL_ALGORITHM ssl_algorithm;  // ssl algorithm
  int ssl_log_level;            // ssl log level
  int ssl_numscans;             // no. of scanning directions
  char ssl_halfscan;        // true to scan [-90, 90], false to scan [-180, 180]
  float ssl_el;             // elevation of the scans [-90, 90]
  float ssl_forget;         // the forgetting factor for the ssl algorithm
  float ssl_freq_range[2];  // normalized frequency ratio range for ssl

  /*
   * vad parameters
   */
  VAD_ALGORITHM vad_algorithm;  // vad algorithm
  int vad_log_level;            // vad log level
  float vad_level;              // vad decision threshold
  float vad_forget;             // forgetting factor for vad
  float vad_target_radius;      // target signal radius (degree)
  float vad_min_snr;            // minimal snr threshold
  float vad_min_tvar;           // minimal target variance
  float vad_min_nvar;           // minimal noise variance
  float vad_min_tsize;          // minimal target length (s)
  float vad_hangbefore;         // vad hangbefore time (s)
  float vad_hangover;           // vad hangover time (s)
  char *vad_model_base;         // vad model base

  /*
   * keyword spotting params
   */
  KWS_ALGORITHM kws_algorithm;  // kws algorithm
  int kws_log_level;            // kws log level
  char kws_block_selfwake;      // check self wake
  char *kws_out_path;           // keyword utterance output directory
  int kws_decode_sparsity;      // decoder sparsity policy
  char **kws_decode_desc;       // keyword decoding description
  char *kws_detection_policy;   // keyword detection policy
  char *kws_fgratio;            // confidence fusion ratio
  char *kws_level;              // spotting threshold
  char *kws_level2;             // secondary spotting threshold
  float kws_level2_duration;    // secondary threshold duration (s)
  char *kws_night_level;        // night mode threshold
  char **kws_model_base;        // kws model base

  /*
   * command params
   */
  KWS_ALGORITHM cmd_algorithm;  // kws algorithm
  int cmd_decode_sparsity;      // decoder sparsity policy
  char **cmd_decode_desc;       // keyword decoding description
  char *cmd_detection_policy;   // keyword detection policy
  char *cmd_level;              // spotting threshold
  char **cmd_model_base;        // kws model base

  /**
   * parameters about the signal quality testing
   */
  TEST_ITEM test_items[MAX_NUM_CHS];  // test items
  float test_saturation_level;     // max signal amplitude to regard as saturate
  float test_max_saturation_rate;  // max saturation rate
  float test_discontinuity_level;  // max discontinuity level
  float test_max_discontinuity_rate;  // max discontinuity rate
  float test_max_similarity;          // max signal similarity
  float test_max_amp_diff_rate;       // max amplitude difference rate
  float test_freqdb_min_similarity;   // min frequency response similarity
  char *test_freqdb_lower;            // frequency response (dB) lower bound
  char *test_freqdb_upper;            // frequency response (dB) upper bound

  /**
   * parameters for model training
   */
  int train_repeat;          // repeat times for offline front-end processing
  float train_input_offset;  // input audio offset (s)
  float train_input_len;     // input audio length (s)
  float train_batch_size;    // data batch size (s)
  int train_rand_seed;       // the rand seed
  int train_worker_id;       // worker id

  // rir simulation
  float train_sound_speed_range[2];  // sound speed range (m/s)
  float train_roomx_range[2];        // simulated room length range (m)
  float train_roomy_range[2];        // simulated room width range (m)
  float train_roomz_range[2];        // simulated room height range (m)
  float train_sourcez_range[2];      // source height range (m)
  float train_receiverz_range[2];    // receiver height range (m)
  float train_distance_range[2];     // distance between source and receiver (m)
  float train_min_spacing;           // minimal space to the wall (m)
  float train_min_azdiff;            // minimal azimuth difference (degree)
  float train_rt60_range[2];         // rt60 range (s)
  float train_coor_stddev;           // coordinate standard deviation (m)
  char *train_rir_out_path;          // rir output path

  // target simulation
  char **train_pos_list;        // positive utterance list
  float train_speed_range[2];   // speed variation range
  float train_tfmask_ratio[3];  // tf mask ratio
  float train_tmask_size[2];    // normalized mask size in time dimension
  float train_fmask_size[2];    // normalized mask size in frequency dimension
  char **train_neg_list;        // negative utterance list
  float train_pos_ratio_range[2];     // positive utterance probability range
  float train_target_blank_range[2];  // space (s) between target utterances
  float train_label_offset_range[2];  // target label offset range (s)
  float train_2gram_ratio;            // 2gram mutation ratio
  float train_confuse_ratio;          // confusing word ratio
  char **train_confuse_desc;          // confusing words description
  char **train_target_rir_list;       // target rir list

  // interf simulation
  char **train_interf_list;           // interference speech list
  int train_numinterfs_range[2];      // no. of interferences
  float train_interf_blank_range[2];  // space (s) between interf utterances
  float train_sir_range[2];           // signal-to-interference ratio range (dB)
  char **train_interf_rir_list;       // interference rir list

  // echo simulation
  char **train_ref_list;                           // reference signal list
  int train_echo_type;                             // echo type
  float train_loudspeaker_coor[MAX_NUM_REFS * 3];  // loudspeaker coordinate
  float train_ref_blank_range[2];  // space (s) between farend utterances
  float train_ser_range[2];        // signal-to-echo ratio range(dB)
  char **train_echo_path_list;     // echo path list

  // noise simulation
  char **train_noise1_list;  // pure noise list (type 1)
  char **train_noise2_list;  // pure noise list (type 2)
  int train_noise1_type;     // noise type (type 1)
  int train_noise2_type;     // noise type (type 2)
  float train_noise1_ratio;  // type 1 noise ratio
  float train_snr_range[2];  // signal-to-noise ratio (dB)
  float train_noise_floor;   // noise floor (dB)

  // volume and gain
  float train_volume_range[2];    // volume range (dB) in data simulation
  float train_bgvolume_range[2];  // background noise volume range (dB)
  float train_micgain_range[2];   // mic gain range (dB)
  float train_agc_bypass_ratio;   // probability to bypass agc

  // scenes
  char **train_scene_desc;  // scene description
} Params;

/**
 * get params for embedded applications
 * params:              the struct
 */
void Params_getParams(Params *params);

/**
 * get bf model params reference
 */
float *Params_bfModel();

/**
 * get ns model params reference
 */
float *Params_nsModel();

/**
 * get vad model params reference
 */
float *Params_vadModel();

/**
 * get kws model params reference
 */
float *Params_kwsModel();

/**
 * get cmd model params reference
 */
float *Params_cmdModel();

#ifdef __cplusplus
}
#endif

#endif  // SRC_INCLUDE_PARAMS_H_
