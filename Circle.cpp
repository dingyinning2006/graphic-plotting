
#include "Circle.h"
#include <sstream>
#include <cmath>
#include <stdexcept>
#include <QPainter>
#include <QBrush>
#include <QPen>

// 构造函数
Circle::Circle(double radius, const Color& color, const Point& position)
    : Shape(ShapeType::CIRCLE, color, position), radius_(radius) {
    if (radius < 0) {
        throw NegativeParameterException("半径", radius);
    }
}

// 计算面积
double Circle::getArea() const {
    return M_PI * radius_ * radius_;
}

// 计算周长
double Circle::getPerimeter() const {
    return 2 * M_PI * radius_;
}

// 绘制圆形
void Circle::draw(void* painter) const {
    QPainter* qp = static_cast<QPainter*>(painter);
    if (!qp) return;

    qp->setBrush(QBrush(QColor(color_.r, color_.g, color_.b)));
    qp->drawEllipse(QPointF(position_.x, position_.y), radius_, radius_);
}

// 验证圆形是否合法
bool Circle::isValid() const {
    return radius_ > 0;
}

// 克隆函数
std::unique_ptr<Shape> Circle::clone() const {
    return std::make_unique<Circle>(*this);
}

// 序列化
std::string Circle::serialize() const {
    std::ostringstream oss;
    oss << "Circle," << id_ << "," << radius_ << "," 
        << color_.r << "," << color_.g << "," << color_.b << ","
        << position_.x << "," << position_.y;
    return oss.str();
}

// 反序列化
std::unique_ptr<Circle> Circle::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string type;
    int id;
    double radius, x, y;
    int r, g, b;

    std::getline(iss, type, ',');
    std::getline(iss, type, ','); // 跳过类型，直接读取ID
    id = std::stoi(type);

    std::getline(iss, type, ',');
    radius = std::stod(type);

    std::getline(iss, type, ',');
    r = std::stoi(type);
    std::getline(iss, type, ',');
    g = std::stoi(type);
    std::getline(iss, type, ',');
    b = std::stoi(type);

    std::getline(iss, type, ',');
    x = std::stod(type);
    std::getline(iss, type, ',');
    y = std::stod(type);

    auto circle = std::make_unique<Circle>(radius, Color(r, g, b), Point(x, y));
    circle->id_ = id; // 设置ID以保持一致性
    return circle;
}

// 设置半径
void Circle::setRadius(double radius) {
    if (radius < 0) {
        throw NegativeParameterException("半径", radius);
    }
    radius_ = radius;
}

// 获取圆形详细信息
std::string Circle::getDetails() const {
    std::ostringstream oss;
    oss << Shape::typeToString(getType()) << " (ID: " << getId() << ")\n";
    oss << "半径: " << radius_ << "\n";
    oss << "面积: " << getArea() << "\n";
    oss << "周长: " << getPerimeter() << "\n";
    oss << "颜色: RGB(" << color_.r << "," << color_.g << "," << color_.b << ")\n";
    oss << "位置: (" << position_.x << "," << position_.y << ")";
    return oss.str();
}
