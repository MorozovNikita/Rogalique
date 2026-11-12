#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include <unordered_map>

enum class LogLevel { Info, Warning, Error };

class LogSink {
public:
    virtual void log(LogLevel level, const std::string& message) = 0;
    virtual ~LogSink() = default;
};

inline std::string LogLevelToString(LogLevel level) {
    switch (level) {
    case LogLevel::Info: return "[Info]";
    case LogLevel::Warning: return "[Warning]";
    case LogLevel::Error: return "[Error]";
    default: return "[UNKNOWN]";
    }
}

class ConsoleSink : public LogSink {
public:
    void log(LogLevel level, const std::string& message) override {
        std::cout << LogLevelToString(level) << " " << message << std::endl;
    }
};

class FileSink : public LogSink {
private:
    std::ofstream logFile;

public:
    FileSink(const std::string& filename) {
        logFile.open(filename, std::ios::app);
    }

    void log(LogLevel level, const std::string& message) override {
        if (logFile) {
            logFile << LogLevelToString(level) << " " << message << std::endl;
        }
    }

    bool isOpen() const { return logFile.is_open(); }

    ~FileSink() {
        if (logFile.is_open()) logFile.close();
    }
};

class Logger {
private:
    std::vector<std::shared_ptr<LogSink>> sinks;
    std::mutex logMutex;

public:
    void addSink(std::shared_ptr<LogSink> sink) {
        sinks.push_back(sink);
    }

    void log(LogLevel level, const std::string& message) {
        std::lock_guard<std::mutex> lock(logMutex);
        for (auto& sink : sinks) {
            sink->log(level, message);
        }
    }

    void info(const std::string& message) { log(LogLevel::Info, message); }
    void warn(const std::string& message) { log(LogLevel::Warning, message); }
    void error(const std::string& message) { log(LogLevel::Error, message); }
};

class LoggerRegistry {
private:
    std::unordered_map<std::string, std::shared_ptr<Logger>> loggers;
    std::shared_ptr<Logger> defaultLogger;
    std::mutex registryMutex;

public:
    static LoggerRegistry& getInstance() {
        static LoggerRegistry instance;
        return instance;
    }

    std::shared_ptr<Logger> getLogger(const std::string& name) {
        std::lock_guard<std::mutex> lock(registryMutex);
        if (loggers.find(name) != loggers.end()) {
            return loggers[name];
        }
        return defaultLogger;
    }

    void setDefaultLogger(std::shared_ptr<Logger> logger) {
        std::lock_guard<std::mutex> lock(registryMutex);
        defaultLogger = logger;
    }

    void registerLogger(const std::string& name, std::shared_ptr<Logger> logger) {
        std::lock_guard<std::mutex> lock(registryMutex);
        loggers[name] = logger;
    }
};

inline void InitializeLogging(const std::string& fileName) {
    auto logger = std::make_shared<Logger>();
    logger->addSink(std::make_shared<ConsoleSink>());
    auto fileSink = std::make_shared<FileSink>(fileName);
    logger->addSink(fileSink);

    auto& registry = LoggerRegistry::getInstance();
    registry.registerLogger("global", logger);
    registry.setDefaultLogger(logger);

    if (!fileSink->isOpen())
        logger->warn("Failed to open log file: " + fileName);
}

#define LOG_INFO(message) \
    do { \
        if (const auto logger_ = LoggerRegistry::getInstance().getLogger("global")) \
            logger_->info(message); \
    } while (0)

#define LOG_WARN(message) \
    do { \
        if (const auto logger_ = LoggerRegistry::getInstance().getLogger("global")) \
            logger_->warn(message); \
    } while (0)

#define LOG_ERROR(message) \
    do { \
        if (const auto logger_ = LoggerRegistry::getInstance().getLogger("global")) \
            logger_->error(message); \
    } while (0)