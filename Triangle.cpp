
#include "Triangle.h"
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <QPainter>
#include <QBrush>
#include <QPen>

// 构造函数
Triangle::Triangle(double sideA, double sideB, double sideC,
                  const Color& color, const Point& position)
    : Shape(ShapeType::TRIANGLE, color, position), 
      sideA_(sideA), sideB_(sideB), sideC_(sideC) {
    if (!isTriangleValid(sideA_, sideB_, sideC_)) {
        throw InvalidTriangleException(sideA_, sideB_, sideC_);
    }

    if (sideA_ < 0 || sideB_ < 0 || sideC_ < 0) {
        throw NegativeParameterException("边长", 
            sideA_ < 0 ? sideA_ : (sideB_ < 0 ? sideB_ : sideC_));
    }
}

// 计算面积（使用海伦公式）
double Triangle::getArea() const {
    return calculateAreaWithHeron();
}

// 计算周长
double Triangle::getPerimeter() const {
    return sideA_ + sideB_ + sideC_;
}

// 绘制三角形
void Triangle::draw(void* painter) const {
    QPainter* qp = static_cast<QPainter*>(painter);
    if (!qp) return;

    qp->setBrush(QBrush(QColor(color_.r, color_.g, color_.b)));

    // 计算三角形顶点坐标（等边三角形简化）
    double height = (sqrt(3)/2) * sideA_; // 假设sideA为底边
    QPointF p1(position_.x, position_.y - height/2);  // 顶点
    QPointF p2(position_.x - sideA_/2, position_.y + height/2); // 左下
    QPointF p3(position_.x + sideA_/2, position_.y + height/2); // 右下

    qp->drawPolygon(QPolygonF({p1, p2, p3}));
}

// 验证三角形是否合法
bool Triangle::isValid() const {
    return isTriangleValid(sideA_, sideB_, sideC_) && 
           sideA_ > 0 && sideB_ > 0 && sideC_ > 0;
}

// 克隆函数
std::unique_ptr<Shape> Triangle::clone() const {
    return std::make_unique<Triangle>(*this);
}

// 序列化
std::string Triangle::serialize() const {
    std::ostringstream oss;
    oss << "Triangle," << id_ << "," << sideA_ << "," << sideB_ << "," << sideC_ << "," 
        << color_.r << "," << color_.g << "," << color_.b << ","
        << position_.x << "," << position_.y;
    return oss.str();
}

// 反序列化
std::unique_ptr<Triangle> Triangle::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string type;
    int id;
    double sideA, sideB, sideC, x, y;
    int r, g, b;

    std::getline(iss, type, ',');
    std::getline(iss, type, ','); // 跳过类型，直接读取ID
    id = std::stoi(type);

    std::getline(iss, type, ',');
    sideA = std::stod(type);

    std::getline(iss, type, ',');
    sideB = std::stod(type);

    std::getline(iss, type, ',');
    sideC = std::stod(type);

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

    auto triangle = std::make_unique<Triangle>(sideA, sideB, sideC, Color(r, g, b), Point(x, y));
    triangle->id_ = id; // 设置ID以保持一致性
    return triangle;
}

// 设置边长A
void Triangle::setSideA(double sideA) {
    if (!isTriangleValid(sideA, sideB_, sideC_)) {
        throw InvalidTriangleException(sideA, sideB_, sideC_);
    }
    if (sideA < 0) {
        throw NegativeParameterException("边长A", sideA);
    }
    sideA_ = sideA;
}

// 设置边长B
void Triangle::setSideB(double sideB) {
    if (!isTriangleValid(sideA_, sideB, sideC_)) {
        throw InvalidTriangleException(sideA_, sideB, sideC_);
    }
    if (sideB < 0) {
        throw NegativeParameterException("边长B", sideB);
    }
    sideB_ = sideB;
}

// 设置边长C
void Triangle::setSideC(double sideC) {
    if (!isTriangleValid(sideA_, sideB_, sideC)) {
        throw InvalidTriangleException(sideA_, sideB_, sideC);
    }
    if (sideC < 0) {
        throw NegativeParameterException("边长C", sideC);
    }
    sideC_ = sideC;
}

// 验证三边能否构成三角形
bool Triangle::isTriangleValid(double a, double b, double c) const {
    return (a + b > c) && (a + c > b) && (b + c > a);
}

// 使用海伦公式计算面积
double Triangle::calculateAreaWithHeron() const {
    double s = getPerimeter() / 2;
    return sqrt(s * (s - sideA_) * (s - sideB_) * (s - sideC_));
}

// 获取三角形详细信息
std::string Triangle::getDetails() const {
    std::ostringstream oss;
    oss << Shape::typeToString(getType()) << " (ID: " << getId() << ")\n";
    oss << "边长A: " << sideA_ << "\n";
    oss << "边长B: " << sideB_ << "\n";
    oss << "边长C: " << sideC_ << "\n";
    oss << "面积: " << getArea() << "\n";
    oss << "周长: " << getPerimeter() << "\n";
    oss << "颜色: RGB(" << color_.r << "," << color_.g << "," << color_.b << ")\n";
    oss << "位置: (" << position_.x << "," << position_.y << ")";
    return oss.str();
}
