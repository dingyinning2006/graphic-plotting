
#include "ShapeManager.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Triangle.h"
#include <sstream>
#include <algorithm>
#include <map>
#include <iomanip>

// 根据ID查找图形
std::shared_ptr<Shape> ShapeManager::findShapeById(int id) const {
    auto it = std::find_if(shapes_.begin(), shapes_.end(), 
        [id](const std::shared_ptr<Shape>& shape) {
            return shape->getId() == id;
        });

    if (it == shapes_.end()) {
        throw ShapeNotFoundException(id);
    }

    return *it;
}

// 根据类型查找图形
std::vector<std::shared_ptr<Shape>> ShapeManager::findShapesByType(ShapeType type) const {
    std::vector<std::shared_ptr<Shape>> result;

    for (const auto& shape : shapes_) {
        if (shape->getType() == type) {
            result.push_back(shape);
        }
    }

    return result;
}

// 删除指定ID的图形
bool ShapeManager::removeShapeById(int id) {
    auto originalSize = shapes_.size();

    shapes_.erase(
        std::remove_if(shapes_.begin(), shapes_.end(),
            [id](const std::shared_ptr<Shape>& shape) {
                return shape->getId() == id;
            }),
        shapes_.end()
    );

    return shapes_.size() < originalSize;
}

// 计算所有图形的总面积
double ShapeManager::getTotalArea() const {
    if (shapes_.empty()) {
        throw EmptyShapeListException();
    }

    double total = 0.0;
    for (const auto& shape : shapes_) {
        total += shape->getArea();
    }

    return total;
}

// 计算所有图形的总周长
double ShapeManager::getTotalPerimeter() const {
    if (shapes_.empty()) {
        throw EmptyShapeListException();
    }

    double total = 0.0;
    for (const auto& shape : shapes_) {
        total += shape->getPerimeter();
    }

    return total;
}

// 按面积排序图形
void ShapeManager::sortByArea() {
    std::sort(shapes_.begin(), shapes_.end(), CompareByArea());
}

// 按周长排序图形
void ShapeManager::sortByPerimeter() {
    std::sort(shapes_.begin(), shapes_.end(), CompareByPerimeter());
}

// 按ID排序图形
void ShapeManager::sortById() {
    std::sort(shapes_.begin(), shapes_.end(),
        [](const std::shared_ptr<Shape>& a, const std::shared_ptr<Shape>& b) {
            return a->getId() < b->getId();
        });
}

// 统计各类型图形数量
std::map<ShapeType, int> ShapeManager::countShapesByType() const {
    std::map<ShapeType, int> counts;

    for (const auto& shape : shapes_) {
        counts[shape->getType()]++;
    }

    return counts;
}

// 获取图形的详细信息字符串
std::string ShapeManager::getShapesInfo() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);

    if (shapes_.empty()) {
        oss << "图形列表为空";
        return oss.str();
    }

    // 统计信息
    auto typeCounts = countShapesByType();
    oss << "=== 图形统计信息 ===\n";
    oss << "图形总数: " << shapes_.size() << "\n";

    for (const auto& pair : typeCounts) {
        oss << Shape::typeToString(pair.first) << ": " << pair.second << "个\n";
    }

    oss << "总面积: " << getTotalArea() << "\n";
    oss << "总周长: " << getTotalPerimeter() << "\n\n";

    // 各图形详细信息
    oss << "=== 各图形详细信息 ===\n";
    for (const auto& shape : shapes_) {
        oss << "--------------------\n";
        oss << shape->getDetails() << "\n";
    }

    return oss.str();
}

// 保存图形到文件
void ShapeManager::saveToFile(const std::string& filename) const {
    if (shapes_.empty()) {
        throw EmptyShapeListException();
    }

    std::ofstream outFile(filename);
    if (!outFile) {
        throw ShapeException("无法打开文件: " + filename);
    }

    // 写入图形数量
    outFile << shapes_.size() << "\n";

    // 写入每个图形
    for (const auto& shape : shapes_) {
        outFile << shape->serialize() << "\n";
    }

    outFile.close();
}

// 从文件加载图形
void ShapeManager::loadFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        throw ShapeException("无法打开文件: " + filename);
    }

    // 清空现有图形
    shapes_.clear();

    // 读取图形数量
    int count;
    inFile >> count;
    inFile.ignore(); // 跳过换行符

    if (count <= 0) {
        inFile.close();
        return;
    }

    // 读取每个图形
    for (int i = 0; i < count; ++i) {
        std::string line;
        std::getline(inFile, line);

        if (!line.empty()) {
            size_t commaPos = line.find(',');
            if (commaPos != std::string::npos) {
                std::string type = line.substr(0, commaPos);
                std::string data = line.substr(commaPos + 1);

                try {
                    auto shape = createShapeFromData(type, data);
                    if (shape) {
                        shapes_.push_back(shape);
                    }
                } catch (const std::exception& e) {
                    std::cerr << "加载图形时出错: " << e.what() << std::endl;
                }
            }
        }
    }

    inFile.close();
}

// 从数据行创建图形
std::shared_ptr<Shape> ShapeManager::createShapeFromData(const std::string& type, 
                                                        const std::string& data) {
    if (type == "Circle") {
        auto circle = Circle::deserialize(data);
        return circle;
    } else if (type == "Rectangle") {
        auto rect = Rectangle::deserialize(data);
        return rect;
    } else if (type == "Triangle") {
        auto triangle = Triangle::deserialize(data);
        return triangle;
    }

    throw ShapeException("未知的图形类型: " + type);
}
