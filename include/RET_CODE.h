/*
 * RET_CODE.h
 *
 * The return code of the front-end lib.
 *
 * Copyright: 2017-11-15 nay0648_7
 */

#ifndef ALGO_SOUNDCONNECT_INCLUDE_RET_CODE_H_
#define ALGO_SOUNDCONNECT_INCLUDE_RET_CODE_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * close the debug options
 */
// #define NDEBUG

/*
 * use neon
 */
// #define ENABLE_NEON

/**
 * supports CEI's NEON option
 */
#ifdef USE_NEON
#ifndef ENABLE_NEON
#define ENABLE_NEON
#endif
#endif

/*
 * The return code.
 */
typedef enum RET_CODE_ {
  RET_OK = 0,
  RET_NOT_SUPPORTED = -100,
  RET_NOT_INITIALIZED,        // instance not initialized
  RET_LICNENCE_CHECK_FAILED,  // licence check failed
  RET_ILLEGAL_PARAMETER,      // illegal parameter
  RET_ILLEGAL_DATA,           // illegal data
  RET_INSUFFICIENT_MEMORY,    // insufficient memory space
  RET_DIVIDE_BY_ZERO,         // arithmetic exception
} RET_CODE;

#ifdef __cplusplus
}
#endif

#endif  // ALGO_SOUNDCONNECT_INCLUDE_RET_CODE_H_
