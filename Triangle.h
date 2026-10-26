
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Shape.h"

// 三角形类
class Triangle : public Shape {
public:
    // 构造函数
    Triangle(double sideA = 0, double sideB = 0, double sideC = 0,
             const Color& color = Color(0, 0, 0), 
             const Point& position = Point(0, 0));

    // 实现纯虚函数
    double getArea() const override;
    double getPerimeter() const override;
    void draw(void* painter) const override;
    bool isValid() const override;

    // 克隆函数
    std::unique_ptr<Shape> clone() const override;

    // 序列化/反序列化
    std::string serialize() const override;
    static std::unique_ptr<Triangle> deserialize(const std::string& data);

    // 获取/设置边长
    double getSideA() const { return sideA_; }
    double getSideB() const { return sideB_; }
    double getSideC() const { return sideC_; }
    void setSideA(double sideA);
    void setSideB(double sideB);
    void setSideC(double sideC);

    // 获取三角形详细信息
    std::string getDetails() const override;

private:
    double sideA_;
    double sideB_;
    double sideC_;

    // 验证三边能否构成三角形
    bool isTriangleValid(double a, double b, double c) const;

    // 使用海伦公式计算面积
    double calculateAreaWithHeron() const;
};

#endif // TRIANGLE_H
