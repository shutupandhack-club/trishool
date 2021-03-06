/**
 * Copyright(C) 2018, Juniper Networks, Inc.
 * All rights reserved
 *
 * This SOFTWARE is licensed under the license provided in the LICENSE.txt
 * file.  By downloading, installing, copying, or otherwise using the
 * SOFTWARE, you agree to be bound by the terms of that license.  This
 * SOFTWARE is not an official Juniper product.
 *
 * Third-Party Code: This SOFTWARE may depend on other components under
 * separate copyright notice and license terms.  Your use of the source
 * code for those components is subject to the term and conditions of
 * the respective license as noted in the Third-Party source code.
 */

/**
 * @author Anoop Saldanha
 */

/* Logging utility */

#ifndef __UTIL_LOG__H__
#define __UTIL_LOG__H__

#include "trishool/trishool-common.h"
#include "trishool/status.h"

#ifdef WIN32
#define __FILENAME__ (strrchr(__FILE__, '\\') ? \
                      strrchr(__FILE__, '\\') + 1 : \
                      __FILE__)
#endif

/**
 * The log levels.  One needs to specify the log level of the alerts needed
 * in their call to EyLogInit().
 */
typedef enum tri_log_level_t {
    TRI_LOG_LEVEL_NOTSET = -1,
    TRI_LOG_LEVEL_NONE = 0,
    TRI_LOG_LEVEL_EMERGENCY,
    TRI_LOG_LEVEL_ALERT,
    TRI_LOG_LEVEL_CRITICAL,
    TRI_LOG_LEVEL_ERROR,
    TRI_LOG_LEVEL_WARNING,
    TRI_LOG_LEVEL_NOTICE,
    TRI_LOG_LEVEL_INFO,
    TRI_LOG_LEVEL_DEBUG,
    TRI_LOG_LEVEL_MAX,
} tri_log_level_t;

extern FILE *g_tri_log_fp;
extern tri_log_level_t g_tri_log_level;

#ifdef BT_PLATFORM_PIN

#define tri_log(log_level, ...)                                         \
    do {                                                                \
        if (log_level <= g_tri_log_level) {                             \
            char buf1[1024];                                            \
            char buf2[1024];                                            \
            snprintf(buf1, sizeof(buf1), __VA_ARGS__);                  \
            snprintf(buf2, sizeof(buf2),                                \
                     "(%s:%d)(%s) <%s> - %s\r\n",                       \
                     __FILENAME__, __LINE__,                            \
                     __FUNCTION__,                                      \
                     tri_log_level_to_string(log_level), buf1);         \
            fprintf(stdout, "%s", buf2);                                \
            fflush(stdout);                                             \
            if (g_tri_log_fp != NULL) {                                 \
                fprintf(g_tri_log_fp, "%s", buf2, strlen(buf2));        \
                fflush(g_tri_log_fp);                                   \
            }                                                           \
        }                                                               \
    } while (0)

#elif BT_PLATFORM_DRIO

#define tri_log(log_level, ...)                                         \
    do {                                                                \
        if (log_level <= g_tri_log_level) {                             \
            char buf1[1024];                                            \
            char buf2[1024];                                            \
            _snprintf_s(buf1, sizeof(buf1), _TRUNCATE, __VA_ARGS__);    \
            _snprintf_s(buf2, sizeof(buf2), _TRUNCATE,                  \
                      "(%s:%d)(%s) <%s> - %s\r\n",                      \
                      __FILENAME__, __LINE__,                           \
                      __FUNCTION__,                                     \
                      tri_log_level_to_string(log_level), buf1);        \
            fprintf(stdout, "%s", buf2);                                \
            fflush(stdout);                                             \
            if (g_tri_log_fp != NULL) {                                 \
                fprintf(g_tri_log_fp, "%s", buf2, strlen(buf2));        \
                fflush(g_tri_log_fp);                                   \
            }                                                           \
        }                                                               \
    } while (0)

#else

#define tri_log(log_level, ...)                                         \
    do {                                                                \
        if (log_level <= g_tri_log_level) {                             \
            char buf1[1024];                                            \
            char buf2[1024];                                            \
            _snprintf_s(buf1, sizeof(buf1), _TRUNCATE, __VA_ARGS__);    \
            _snprintf_s(buf2, sizeof(buf2), _TRUNCATE,                  \
                      "(%s:%d)(%s) <%s> - %s\r\n",                      \
                      __FILENAME__, __LINE__,                           \
                      __FUNCTION__,                                     \
                      tri_log_level_to_string(log_level), buf1);        \
            fprintf(stdout, "%s", buf2);                                \
            fflush(stdout);                                             \
            if (g_tri_log_fp != NULL) {                                 \
                fprintf(g_tri_log_fp, "%s", buf2, strlen(buf2));        \
                fflush(g_tri_log_fp);                                   \
            }                                                           \
        }                                                               \
    } while (0)


#endif /* BT_PLATFORM_PIN - BT_PLATFORM_DRIO */

/**
 * Private API.
 */
const char *tri_log_level_to_string(tri_log_level_t log_level);

/**
 * Various logging APIs.
 */
#define tri_log_emergency(...) tri_log(TRI_LOG_LEVEL_EMERGENCY, __VA_ARGS__)
#define tri_log_alert(...) tri_log(TRI_LOG_LEVEL_ALERT, __VA_ARGS__)
#define tri_log_critical(...) tri_log(TRI_LOG_LEVEL_CRITICAL, __VA_ARGS__)
#define tri_log_error(...) tri_log(TRI_LOG_LEVEL_ERROR, __VA_ARGS__)
#define tri_log_warning(...) tri_log(TRI_LOG_LEVEL_WARNING, __VA_ARGS__)
#define tri_log_notice(...) tri_log(TRI_LOG_LEVEL_NOTICE, __VA_ARGS__)
#define tri_log_info(...) tri_log(TRI_LOG_LEVEL_INFO, __VA_ARGS__)
#define tri_log_debug(...) tri_log(TRI_LOG_LEVEL_DEBUG, __VA_ARGS__)

/**
 * Initialize the logging API.
 *
 * @log_level The log level to be used as offered by EyLogLevel_t.
 * @path Path to a log file to write to.  It can also be NULL in which
 *       case logging to a file is disabled and only console logging is used.
 *
 * @retval TRI_STATUS_OK On successful init.
 * @retval TRI_STATUS_ERROR On an un-successful init.
 */
tri_status_t tri_log_init(tri_log_level_t log_level, const char *path);

/**
 * DeInitialize the logging API.
 *
 * @retval TRI_STATUS_OK On successful de-init.
 * @retval TRI_STATUS_ERROR On an un-successful de-init.
 */
tri_status_t tri_log_deinit();

#endif /* __UTIL_LOG__H__ */
