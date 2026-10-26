
#ifndef SHAPE_H
#define SHAPE_H

#include <string>
#include <stdexcept>

// 颜色结构体
struct Color {
    int r, g, b; // RGB值

    Color(int r = 0, int g = 0, int b = 0) : r(r), g(g), b(b) {}
};

// 点结构体
struct Point {
    double x, y;

    Point(double x = 0, double y = 0) : x(x), y(y) {}
};

// 图形类型枚举
enum class ShapeType {
    CIRCLE,
    RECTANGLE,
    TRIANGLE
};

// 异常类
class ShapeException : public std::runtime_error {
public:
    explicit ShapeException(const std::string& message)
        : std::runtime_error(message) {}
};

// 无效三角形异常
class InvalidTriangleException : public ShapeException {
public:
    InvalidTriangleException(double a, double b, double c)
        : ShapeException("边长 (" + std::to_string(a) + ", " + 
                      std::to_string(b) + ", " + 
                      std::to_string(c) + ") 无法构成合法三角形！") {}
};

// 非正参数异常
class NegativeParameterException : public ShapeException {
public:
    NegativeParameterException(const std::string& paramName, double value)
        : ShapeException("参数 '" + paramName + "' 的值 " + 
                      std::to_string(value) + " 无效！所有尺寸参数必须为正数") {}
};

// 空图形列表异常
class EmptyShapeListException : public ShapeException {
public:
    EmptyShapeListException()
        : ShapeException("图形列表为空，无法执行操作！请先添加图形") {}
};

// 图形抽象基类
class Shape {
public:
    // 构造函数
    Shape(ShapeType type, const Color& color = Color(0, 0, 0), 
          const Point& position = Point(0, 0));

    // 虚析构函数
    virtual ~Shape() = default;

    // 纯虚函数 - 计算面积
    virtual double getArea() const = 0;

    // 纯虚函数 - 计算周长
    virtual double getPerimeter() const = 0;

    // 纯虚函数 - 绘制图形
    virtual void draw(void* painter) const = 0;

    // 纯虚函数 - 验证图形是否合法
    virtual bool isValid() const = 0;

    // 克隆函数
    virtual std::unique_ptr<Shape> clone() const = 0;

    // 序列化
    virtual std::string serialize() const = 0;

    // 反序列化 - 静态工厂函数
    static std::unique_ptr<Shape> deserialize(const std::string& data);

    // 获取类型
    ShapeType getType() const { return type_; }

    // 获取ID
    int getId() const { return id_; }

    // 获取颜色
    const Color& getColor() const { return color_; }

    // 获取位置
    const Point& getPosition() const { return position_; }

    // 设置颜色
    void setColor(const Color& color) { color_ = color; }

    // 设置位置
    void setPosition(const Point& position) { position_ = position; }

    // 获取类型字符串
    static std::string typeToString(ShapeType type);

protected:
    // 生成唯一ID
    static int generateId();

private:
    ShapeType type_;
    int id_;
    Color color_;
    Point position_;
};

#endif // SHAPE_H
