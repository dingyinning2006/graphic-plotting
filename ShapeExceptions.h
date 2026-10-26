
#ifndef SHAPE_EXCEPTIONS_H
#define SHAPE_EXCEPTIONS_H

#include <stdexcept>
#include <string>

// 图形异常基类
class ShapeException : public std::runtime_error {
public:
    explicit ShapeException(const std::string& message)
        : std::runtime_error(message) {}
};

// 图形未找到异常
class ShapeNotFoundException : public ShapeException {
public:
    explicit ShapeNotFoundException(int id)
        : ShapeException("未找到ID为 " + std::to_string(id) + " 的图形") {}
};

// 无效参数异常
class InvalidParameterException : public ShapeException {
public:
    InvalidParameterException(const std::string& paramName, const std::string& reason)
        : ShapeException("参数 '" + paramName + "' 无效: " + reason) {}
};

// 文件操作异常
class FileOperationException : public ShapeException {
public:
    explicit FileOperationException(const std::string& operation, const std::string& filename)
        : ShapeException("文件操作失败: " + operation + " 文件 '" + filename + "'") {}
};

// 序列化异常
class SerializationException : public ShapeException {
public:
    explicit SerializationException(const std::string& reason)
        : ShapeException("序列化失败: " + reason) {}
};

// 反序列化异常
class DeserializationException : public ShapeException {
public:
    explicit DeserializationException(const std::string& reason)
        : ShapeException("反序列化失败: " + reason) {}
};

#endif // SHAPE_EXCEPTIONS_H
