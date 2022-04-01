//
// Copyright (c) 2021 Tencent Corporation. All rights reserved.
//

#include "devtools_base/logging.h"
#include <chrono>
#include <sstream>
#include <list>

#ifdef OS_ANDROID
#include <android/log.h>
#else

#endif

namespace tdf {
namespace devtools {
const char *const kBackendLogLevelNames[5] =
    {"INFO", "DEBUG", "WARNING", "ERROR", "FATAL"};
static std::list<LogCallback> log_callbacks_;


const char *GetNameForLogLevel(LogLevel level) {
  if (level >= DEVTOOLS_LOG_INFO && level <= DEVTOOLS_LOG_FATAL)
    return kBackendLogLevelNames[level];
  return "UNKNOWN";
}

std::string Logger::GetTimeStamp() {
  struct timeval tv;
  time_t nowtime;
  struct tm nowtm = { 0 };
  // YYYY-MM-DD hh.mm.ss.sss 共23位，最后一位为\0结束
  static char szTime[24];
  std::chrono::system_clock::duration duration = std::chrono::system_clock::now().time_since_epoch();
  std::chrono::seconds sec = duration_cast<std::chrono::seconds>(duration);
  nowtime = sec.count();
  localtime_r(&nowtime, &nowtm);
  snprintf(szTime,
          sizeof(szTime),
           "%04d-%02d-%02d %02d.%02d.%02d.%03d",
          nowtm.tm_year + 1900,
          nowtm.tm_mon + 1,
          nowtm.tm_mday,
          nowtm.tm_hour,
          nowtm.tm_min,
          nowtm.tm_sec,
          (tv.tv_usec / 1000));
  szTime[23] = 0;
  std::string time(szTime);
  return time;
}

void Logger::Log(LogLevel level, const char *file,
                 int line, const char *module, const char *format, ...) {
  char log[1024];
  va_list args;
  int len = 0;

  va_start(args, format);
  len = vsnprintf(log, sizeof(log), format, args);
  va_end(args);

  if (len <= 0) {
    return;
  }

  std::string message(log, len);
  std::string timestamp = GetTimeStamp();
  std::string level_name = GetNameForLogLevel(level);
  std::string module_name(module);

  const char *slash = strrchr(file, '/');
  const char *filename = slash ? slash + 1 : file;
  std::ostringstream stream;
  stream << "[" << module_name << "][" << level_name << "][" << filename << ":"
         << line << "]" << message << std::endl;
#ifdef OS_ANDROID
  android_LogPriority
      priority = (level < 0) ? ANDROID_LOG_VERBOSE : ANDROID_LOG_UNKNOWN;
  switch (level) {
    case DEVTOOLS_LOG_INFO:
      priority = ANDROID_LOG_INFO;
      break;
    case DEVTOOLS_LOG_DEBUG:
      priority = ANDROID_LOG_DEBUG;
      break;
    case DEVTOOLS_LOG_WARNING:
      priority = ANDROID_LOG_WARN;
      break;
    case DEVTOOLS_LOG_ERROR:
      priority = ANDROID_LOG_ERROR;
      break;
    case DEVTOOLS_LOG_FATAL:
      priority = ANDROID_LOG_FATAL;
      break;
  }
  __android_log_write(priority, "backend", stream.str().c_str());
#else
  std::cout << timestamp << " " << stream.str();
#endif

  std::ostringstream file_stream;
  file_stream << filename;

  auto nano_time_point = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now());
  int64_t nano_time_stamp = nano_time_point.time_since_epoch().count();

  LoggerModel logger_model;
  logger_model.source = tdf::devtools::DEVTOOLS_BACKEND_SOURCE;
  logger_model.module = module_name;
  logger_model.level = level_name;
  logger_model.file_name = file_stream.str();
  logger_model.line_number = line;
  logger_model.time_stamp = nano_time_stamp;

  logger_model.log_data = message;

  DispatchToCallbacks(logger_model);
}

void Logger::RegisterCallback(LogCallback callback) {
  log_callbacks_.push_back(callback);
}

void Logger::DispatchToCallbacks(LoggerModel logger_model) {
  for (auto callback : log_callbacks_) {
    callback(logger_model);
  }
}

}  // namespace devtools
}  // namespace tdf
