/*
 * SoundConnect.h
 *
 * The sound connect's Advanced Front-End.
 *
 * Copyright: 2017-12-3 nay0648_7
 */

#ifndef SRC_INCLUDE_SOUNDCONNECT_H_
#define SRC_INCLUDE_SOUNDCONNECT_H_

#include <stdint.h>
#include <time.h>

#include "Params.h"
#include "RET_CODE.h"

class SoundConnect {
 public:
  /**
   * initialize the front-end
   * params:            parameters
   */
  explicit SoundConnect(Params *params);

  /**
   * initialize the front-end
   * confpath:          configure file path
   */
  explicit SoundConnect(const char *confpath);

  /**
   * initialize the front-end
   * argc:              no. of arguments
   * argv:              arguments list
   */
  SoundConnect(int argc, char **argv);

  /**
   * release the resource
   */
  ~SoundConnect();

  /**
   * get the version information
   */
  static const char *version();

  /**
   * get licence information
   */
  static const char *licenceInfo();

  /**
   * get the no. of mic channels
   */
  int numMics();

  /**
   * get the no. of reference channels
   */
  int numRefs();

  /**
   * get the no. of pilot channels
   */
  int numPilots();

  /**
   * get the no. of input channels
   */
  int numIns();

  /**
   * get the no. of output channels
   */
  int numOuts();

  /**
   * get the input sample rate (Hz)
   */
  int sampleRateIn();

  /**
   * get the output sample rate (Hz)
   */
  int sampleRateOut();

  /**
   * get no. of bits per sample for input signals
   */
  int bitsPerSampleIn();

  /**
   * get no. of bits per sample for output signals
   */
  int bitsPerSampleOut();

  /**
   * get input block size (no. of frames)
   */
  int blockSizeIn();

  /**
   * get output block size (no. of frames)
   */
  int blockSizeOut();

  /**
   * get the no. of samples in an input block:
   * numins*blocksizein
   */
  int samplesPerBlockIn();

  /**
   * get the no. of samples in an output block:
   * numouts*blocksizeout
   */
  int samplesPerBlockOut();

  /**
   * get the no. of bytes in an input block:
   * numins*blocksizein*nbitsin/8
   */
  int bytesPerBlockIn();

  /**
   * get the no. of bytes in an output block:
   * numouts*blocksizeout*nbitsout/8
   */
  int bytesPerBlockOut();

  /**
   * get time delay (second) introduced by the algorithm
   */
  float delay();

  /**
   * get reference delay (second), mic delay is 0
   * negative: reference advances mic,
   * positive: reference behinds mic,
   * FLT_MAX: mic and reference are weakly correlated
   */
  float refDelay();

  /**
   * lock output channel
   * n:                 output channel index
   * info:              information used for the lock, different array size for
   *                    different configuration
   */
  void lock(int n, float *info);

  /**
   * unlock output channel
   * n:                 output channel index
   */
  void unlock(int n);

  /**
   * get beamforming look direction (degree)
   * n:                 output channel index
   * az:                returned azimuth
   * el:                returned elevation
   */
  void getLook(int n, float *az, float *el);

  /**
   * set beamforming look direction (degree)
   * n:                 output channel index
   * az:                look azimuth [-180, 180]
   * el:                look elevation [-90, 90]
   */
  void setLook(int n, float az, float el);

  /**
   * set pilot information, this must be set before process()
   * pilot:             pilot information, [numpilots], range [0, 1]
   */
  void setPilots(float *pilot);

  /**
   * get ns level
   * n:                 channel index
   */
  float nsGetLevel(int n);

  /**
   * set ns level
   * n:                 channel index
   * level:             ns level
   */
  void nsSetLevel(int n, float level);

  /**
   * process one data block
   * datain:            input multi-channel (mic+ref) data
   * dataout:           output data, NULL supported
   */
  void process(float *datain, float *dataout);

  /**
   * process one data block
   * datain:            input multi-channel (mic+ref) data
   * dataout:           output data, NULL supported
   */
  void process(char *datain, char *dataout);

  /**
   * process an audio file
   * audioin:           input audio path, wav and pcm are supported
   * afeout:            output enhancement result
   * numframesin:       no. of frames processed
   * cprocess:          clock spent for process, NULL supported
   */
  RET_CODE processBatch(const char *audioin, const char *afeout,
                        int64_t *numframesin, clock_t *cprocess);

  /**
   * get the volume of the last output block
   * n:                 output channel index
   * vol:               the returned volume
   */
  float volume(int n);

  /**
   * get human voice gain in dB
   * return:            gain (dB)
   */
  float mixGetMicGain();

  /**
   * set human voice gain in dB
   * gain:              gain (dB)
   */
  void mixSetMicGain(float gain);

  /**
   * get background music gain in dB
   * return:            gain (dB)
   */
  float mixGetRefGain();

  /**
   * set background music gain in dB
   * gain:              gain (dB)
   */
  void mixSetRefGain(float gain);

  /**
   * set reverberation to the mixer
   * path:              reverb wav file path, NULL to clear reverberation
   */
  void mixSetReverb(const char *path);

  /**
   * estimate the direction-of-arrival (degree)
   * n:                 output channel index
   * az:                returned azimuth
   * el:                returned elevation
   */
  void sslEstimateDOA(int n, float *az, float *el);

  /**
   * clear the former sound source localization result
   * n:                 output channel index
   */
  void sslClear(int n);

  /**
   * get vad decision threshold
   */
  float vadGetLevel();

  /**
   * set vad decision threshold
   * thres:             threshold
   */
  void vadSetLevel(float thres);

  /**
   * get realtime target presence probability
   * n:                 output channel index
   */
  float vadProb(int n);

  /**
   * get vad hangbefore (second), real_start_time = (time_of_is_target = 1) -
   * vadHangbefore()
   */
  float vadHangbefore();

  /**
   * get target / nontarget indicator
   * n:                 output channel index
   */
  char vadIsActive(int n);

  /**
   * clear vad internal state
   * n:                 output channel index
   */
  void vadClear(int n);

  /**
   * get the no. of keywords
   */
  int kwsNumKeywords();

  /**
   * get keyword string
   * kwidx:             keyword index
   */
  const char *kwsKeyword(int kwidx);

  /**
   * find keyword index
   * keyword:           the keyword string
   * return:            -1 if not found
   */
  int kwsKeywordIndex(const char *keyword);

  /**
   * to see if a keyword is enabled or not
   * kwidx:               keyword index
   */
  char kwsGetKeywordEnable(int kwidx);

  /**
   * enable / disable a keyword
   * kwidx:               keyword index
   * enable:              0: disable, 1: enable
   */
  void kwsSetKeywordEnable(int kwidx, char enable);

  /**
   * get keyword detection threshold
   * kwidx:             keyword index
   */
  float kwsGetLevel(int kwidx);

  /**
   * set keyword detection threshold
   * kwidx:             keyword index
   * thres:             the threshold
   */
  void kwsSetLevel(int kwidx, float thres);

  /**
   * get keyword threshold in daytime with normal false alarm rate
   * kwidx:               keyword index
   */
  float kwsDayLevel(int kwidx);

  /**
   * get keyword threshold at night with lower false alarm rate
   * kwidx:               keyword index
   */
  float kwsNightLevel(int kwidx);

  /**
   * to see if a keyword is found
   * return:            0: not spotted
   *                    1: spotted
   *                    2: best end
   *                    3: both spotted and best end
   *                    4: suspect keyword (both spot and bestend)
   */
  char kwsIsSpotted();

  /**
   * get the keyword index of the last spot
   */
  int kwsSpottedKeywordIndex();

  /**
   * get the confidence of last spot
   */
  float kwsConfidence();

  /**
   * get relative keyword start time (second) of last spot
   * return:            negative value, means before current time
   */
  float kwsKeywordOffset();

  /**
   * get keyword length (second) of last spot
   */
  float kwsKeywordLength();

  /**
   * get best channel index
   */
  int kwsBestChannel();

  //  /**
  //   * save runtime context to file
  //   * path:              file path
  //   */
  //  void saveContext(const char *path);

  //  /**
  //   * load runtime context from file
  //   * path:              file path
  //   */
  //  void loadContext(const char *path);

 private:
  class Private;
  Private *pp;
};

#endif  // SRC_INCLUDE_SOUNDCONNECT_H_
