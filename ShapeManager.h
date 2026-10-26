
#ifndef SHAPE_MANAGER_H
#define SHAPE_MANAGER_H

#include <vector>
#include <memory>
#include <algorithm>
#include "Shape.h"
#include "ShapeExceptions.h"

// 比较函数 - 用于按面积排序
struct CompareByArea {
    bool operator()(const std::shared_ptr<Shape>& a, const std::shared_ptr<Shape>& b) const {
        return a->getArea() < b->getArea();
    }
};

// 比较函数 - 用于按周长排序
struct CompareByPerimeter {
    bool operator()(const std::shared_ptr<Shape>& a, const std::shared_ptr<Shape>& b) const {
        return a->getPerimeter() < b->getPerimeter();
    }
};

// 图形管理器 - 管理所有图形对象，提供批量操作功能
class ShapeManager {
public:
    // 构造函数
    ShapeManager() = default;

    // 禁用拷贝构造和赋值
    ShapeManager(const ShapeManager&) = delete;
    ShapeManager& operator=(const ShapeManager&) = delete;

    // 添加图形
    template<typename T, typename... Args>
    std::shared_ptr<T> addShape(Args&&... args) {
        auto shape = std::make_shared<T>(std::forward<Args>(args)...);
        shapes_.push_back(shape);
        return shape;
    }

    // 添加图形（通过智能指针）
    void addShape(std::shared_ptr<Shape> shape) {
        if (!shape) {
            throw ShapeException("尝试添加空图形指针");
        }
        shapes_.push_back(shape);
    }

    // 获取图形数量
    size_t getShapeCount() const { return shapes_.size(); }

    // 获取图形列表
    const std::vector<std::shared_ptr<Shape>>& getShapes() const { return shapes_; }

    // 清空所有图形
    void clearShapes() { shapes_.clear(); }

    // 根据ID查找图形
    std::shared_ptr<Shape> findShapeById(int id) const;

    // 根据类型查找图形
    std::vector<std::shared_ptr<Shape>> findShapesByType(ShapeType type) const;

    // 删除指定ID的图形
    bool removeShapeById(int id);

    // 计算所有图形的总面积
    double getTotalArea() const;

    // 计算所有图形的总周长
    double getTotalPerimeter() const;

    // 按面积排序图形
    void sortByArea();

    // 按周长排序图形
    void sortByPerimeter();

    // 按ID排序图形
    void sortById();

    // 统计各类型图形数量
    std::map<ShapeType, int> countShapesByType() const;

    // 检查是否有图形
    bool isEmpty() const { return shapes_.empty(); }

    // 获取图形的详细信息字符串
    std::string getShapesInfo() const;

    // 保存图形到文件
    void saveToFile(const std::string& filename) const;

    // 从文件加载图形
    void loadFromFile(const std::string& filename);

private:
    std::vector<std::shared_ptr<Shape>> shapes_;

    // 从数据行创建图形
    std::shared_ptr<Shape> createShapeFromData(const std::string& type, 
                                              const std::string& data);
};

#endif // SHAPE_MANAGER_H
