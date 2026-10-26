
#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shape.h"

// 圆形类
class Circle : public Shape {
public:
    // 构造函数
    Circle(double radius = 0, const Color& color = Color(0, 0, 0), 
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
    static std::unique_ptr<Circle> deserialize(const std::string& data);

    // 获取/设置半径
    double getRadius() const { return radius_; }
    void setRadius(double radius);

    // 获取圆形详细信息
    std::string getDetails() const override;

private:
    double radius_;
};

#endif // CIRCLE_H
