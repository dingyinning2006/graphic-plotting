
#include "Rectangle.h"
#include <sstream>
#include <stdexcept>
#include <QPainter>
#include <QBrush>
#include <QPen>

// 构造函数
Rectangle::Rectangle(double width, double height, 
                    const Color& color, const Point& position)
    : Shape(ShapeType::RECTANGLE, color, position), 
      width_(width), height_(height) {
    if (width < 0 || height < 0) {
        throw NegativeParameterException("宽度或高度", width < 0 ? width : height);
    }
}

// 计算面积
double Rectangle::getArea() const {
    return width_ * height_;
}

// 计算周长
double Rectangle::getPerimeter() const {
    return 2 * (width_ + height_);
}

// 绘制矩形
void Rectangle::draw(void* painter) const {
    QPainter* qp = static_cast<QPainter*>(painter);
    if (!qp) return;

    qp->setBrush(QBrush(QColor(color_.r, color_.g, color_.b)));
    qp->drawRect(QRectF(position_.x - width_/2, position_.y - height_/2, 
                         width_, height_));
}

// 验证矩形是否合法
bool Rectangle::isValid() const {
    return width_ > 0 && height_ > 0;
}

// 克隆函数
std::unique_ptr<Shape> Rectangle::clone() const {
    return std::make_unique<Rectangle>(*this);
}

// 序列化
std::string Rectangle::serialize() const {
    std::ostringstream oss;
    oss << "Rectangle," << id_ << "," << width_ << "," << height_ << "," 
        << color_.r << "," << color_.g << "," << color_.b << ","
        << position_.x << "," << position_.y;
    return oss.str();
}

// 反序列化
std::unique_ptr<Rectangle> Rectangle::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string type;
    int id;
    double width, height, x, y;
    int r, g, b;

    std::getline(iss, type, ',');
    std::getline(iss, type, ','); // 跳过类型，直接读取ID
    id = std::stoi(type);

    std::getline(iss, type, ',');
    width = std::stod(type);

    std::getline(iss, type, ',');
    height = std::stod(type);

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

    auto rect = std::make_unique<Rectangle>(width, height, Color(r, g, b), Point(x, y));
    rect->id_ = id; // 设置ID以保持一致性
    return rect;
}

// 设置宽度
void Rectangle::setWidth(double width) {
    if (width < 0) {
        throw NegativeParameterException("宽度", width);
    }
    width_ = width;
}

// 设置高度
void Rectangle::setHeight(double height) {
    if (height < 0) {
        throw NegativeParameterException("高度", height);
    }
    height_ = height;
}

// 获取矩形详细信息
std::string Rectangle::getDetails() const {
    std::ostringstream oss;
    oss << Shape::typeToString(getType()) << " (ID: " << getId() << ")\n";
    oss << "宽度: " << width_ << "\n";
    oss << "高度: " << height_ << "\n";
    oss << "面积: " << getArea() << "\n";
    oss << "周长: " << getPerimeter() << "\n";
    oss << "颜色: RGB(" << color_.r << "," << color_.g << "," << color_.b << ")\n";
    oss << "位置: (" << position_.x << "," << position_.y << ")";
    return oss.str();
}
