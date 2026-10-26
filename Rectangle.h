
#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Shape.h"

// 矩形类
class Rectangle : public Shape {
public:
    // 构造函数
    Rectangle(double width = 0, double height = 0, 
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
    static std::unique_ptr<Rectangle> deserialize(const std::string& data);

    // 获取/设置宽度和高度
    double getWidth() const { return width_; }
    double getHeight() const { return height_; }
    void setWidth(double width);
    void setHeight(double height);

    // 获取矩形详细信息
    std::string getDetails() const override;

private:
    double width_;
    double height_;
};

#endif // RECTANGLE_H
