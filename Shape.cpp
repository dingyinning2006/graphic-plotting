
#include "Shape.h"
#include <sstream>
#include <memory>
#include <map>
#include <algorithm>
#include <cmath>
#include <QPainter>

// 静态成员变量初始化
int Shape::nextId = 1;

// 构造函数
Shape::Shape(ShapeType type, const Color& color, const Point& position)
    : type_(type), id_(generateId()), color_(color), position_(position) {
}

// 生成唯一ID
int Shape::generateId() {
    return nextId++;
}

// 获取类型字符串
std::string Shape::typeToString(ShapeType type) {
    switch (type) {
        case ShapeType::CIRCLE: return "Circle";
        case ShapeType::RECTANGLE: return "Rectangle";
        case ShapeType::TRIANGLE: return "Triangle";
        default: return "Unknown";
    }
}

// 反序列化 - 静态工厂函数
std::unique_ptr<Shape> Shape::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string typeStr;

    // 读取类型
    std::getline(iss, typeStr, ',');

    // 根据类型创建相应的图形
    if (typeStr == "Circle") {
        // 创建圆形
        double radius, x, y;
        int r, g, b;

        std::getline(iss, typeStr, ','); // 跳过ID
        std::getline(iss, typeStr, ',');
        radius = std::stod(typeStr);

        std::getline(iss, typeStr, ',');
        r = std::stoi(typeStr);
        std::getline(iss, typeStr, ',');
        g = std::stoi(typeStr);
        std::getline(iss, typeStr, ',');
        b = std::stoi(typeStr);

        std::getline(iss, typeStr, ',');
        x = std::stod(typeStr);
        std::getline(iss, typeStr, ',');
        y = std::stod(typeStr);

        auto circle = std::make_unique<Circle>(radius, Color(r, g, b), Point(x, y));
        circle->id_ = std::stoi(typeStr.substr(1)); // 设置ID
        return circle;
    } else if (typeStr == "Rectangle") {
        // 创建矩形
        double width, height, x, y;
        int r, g, b;

        std::getline(iss, typeStr, ','); // 跳过ID
        std::getline(iss, typeStr, ',');
        width = std::stod(typeStr);

        std::getline(iss, typeStr, ',');
        height = std::stod(typeStr);

        std::getline(iss, typeStr, ',');
        r = std::stoi(typeStr);
        std::getline(iss, typeStr, ',');
        g = std::stoi(typeStr);
        std::getline(iss, typeStr, ',');
        b = std::stoi(typeStr);

        std::getline(iss, typeStr, ',');
        x = std::stod(typeStr);
        std::getline(iss, typeStr, ',');
        y = std::stod(typeStr);

        auto rect = std::make_unique<Rectangle>(width, height, Color(r, g, b), Point(x, y));
        rect->id_ = std::stoi(typeStr.substr(1)); // 设置ID
        return rect;
    } else if (typeStr == "Triangle") {
        // 创建三角形
        double sideA, sideB, sideC, x, y;
        int r, g, b;

        std::getline(iss, typeStr, ','); // 跳过ID
        std::getline(iss, typeStr, ',');
        sideA = std::stod(typeStr);

        std::getline(iss, typeStr, ',');
        sideB = std::stod(typeStr);

        std::getline(iss, typeStr, ',');
        sideC = std::stod(typeStr);

        std::getline(iss, typeStr, ',');
        r = std::stoi(typeStr);
        std::getline(iss, typeStr, ',');
        g = std::stoi(typeStr);
        std::getline(iss, typeStr, ',');
        b = std::stoi(typeStr);

        std::getline(iss, typeStr, ',');
        x = std::stod(typeStr);
        std::getline(iss, typeStr, ',');
        y = std::stod(typeStr);

        auto triangle = std::make_unique<Triangle>(sideA, sideB, sideC, Color(r, g, b), Point(x, y));
        triangle->id_ = std::stoi(typeStr.substr(1)); // 设置ID
        return triangle;
    }

    throw ShapeException("未知的图形类型: " + typeStr);
}
