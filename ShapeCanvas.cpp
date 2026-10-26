
#include "ShapeCanvas.h"
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QColorDialog>
#include <QBrush>
#include <QPen>
#include <QPolygonF>
#include <QRectF>
#include <QTransform>
#include <algorithm>
#include <cmath>
#include <iostream>

// 构造函数
ShapeCanvas::ShapeCanvas(QWidget* parent)
    : QWidget(parent),
      zoomFactor_(1.0),
      isDragging_(false),
      showGrid_(true),
      showAxes_(true),
      showIds_(true),
      backgroundColor_(Qt::white),
      gridColor_(200, 200, 200),
      axesColor_(100, 100, 100),
      highlightColor_(0, 120, 215, 100) // 半透明的蓝色
{
    // 设置背景
    setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, backgroundColor_);
    setPalette(palette);

    // 设置鼠标追踪
    setMouseTracking(true);
}

// 析构函数
ShapeCanvas::~ShapeCanvas() {
}

// 添加图形
void ShapeCanvas::addShape(std::shared_ptr<Shape> shape) {
    if (!shape) {
        return;
    }

    shapes_.append(shape);
    update();
}

// 删除图形
bool ShapeCanvas::removeShape(int id) {
    bool removed = false;

    shapes_.erase(
        std::remove_if(shapes_.begin(), shapes_.end(),
            [id, &removed](const std::shared_ptr<Shape>& shape) {
                if (shape->getId() == id) {
                    removed = true;
                    return true;
                }
                return false;
            }),
        shapes_.end()
    );

    // 从选中列表中移除
    selectedShapeIds_.removeAll(id);

    if (removed) {
        update();
    }

    return removed;
}

// 清空所有图形
void ShapeCanvas::clearShapes() {
    shapes_.clear();
    selectedShapeIds_.clear();
    update();
}

// 设置缩放比例
void ShapeCanvas::setZoomFactor(qreal factor) {
    if (factor <= 0) {
        return;
    }

    zoomFactor_ = factor;
    emit zoomChanged(zoomFactor_);
    update();
}

// 重置缩放
void ShapeCanvas::resetZoom() {
    zoomFactor_ = 1.0;
    offset_ = QPointF(width() / 2.0, height() / 2.0);
    update();
}

// 居中显示所有图形
void ShapeCanvas::centerView() {
    if (shapes_.isEmpty()) {
        return;
    }

    // 计算所有图形的边界
    QRectF bounds = getSceneBounds();

    // 计算中心点
    QPointF center = bounds.center();

    // 设置偏移量，使中心点位于画布中心
    offset_ = QPointF(width() / 2.0 - center.x() * zoomFactor_,
                     height() / 2.0 - center.y() * zoomFactor_);

    update();
}

// 设置是否显示网格
void ShapeCanvas::setShowGrid(bool show) {
    showGrid_ = show;
    update();
}

// 设置是否显示坐标轴
void ShapeCanvas::setShowAxes(bool show) {
    showAxes_ = show;
    update();
}

// 设置是否显示图形ID
void ShapeCanvas::setShowIds(bool show) {
    showIds_ = show;
    update();
}

// 设置背景颜色
void ShapeCanvas::setBackgroundColor(const QColor& color) {
    backgroundColor_ = color;
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, backgroundColor_);
    setPalette(palette);
    update();
}

// 设置网格颜色
void ShapeCanvas::setGridColor(const QColor& color) {
    gridColor_ = color;
    update();
}

// 设置坐标轴颜色
void ShapeCanvas::setAxesColor(const QColor& color) {
    axesColor_ = color;
    update();
}

// 设置选中图形的高亮颜色
void ShapeCanvas::setHighlightColor(const QColor& color) {
    highlightColor_ = color;
    update();
}

// 获取画布中心点
QPointF ShapeCanvas::getCanvasCenter() const {
    return QPointF(width() / 2.0, height() / 2.0);
}

// 将场景坐标转换为画布坐标
QPointF ShapeCanvas::mapToCanvas(const QPointF& point) const {
    return QPointF(point.x() * zoomFactor_ + offset_.x(),
                  point.y() * zoomFactor_ + offset_.y());
}

// 将画布坐标转换为场景坐标
QPointF ShapeCanvas::mapFromCanvas(const QPointF& point) const {
    return QPointF((point.x() - offset_.x()) / zoomFactor_,
                  (point.y() - offset_.y()) / zoomFactor_);
}

// 获取场景边界矩形
QRectF ShapeCanvas::getSceneBounds() const {
    if (shapes_.isEmpty()) {
        return QRectF(-100, -100, 200, 200); // 默认边界
    }

    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double maxY = std::numeric_limits<double>::lowest();

    for (const auto& shape : shapes_) {
        // 简化处理：使用图形位置和估算大小
        QPointF pos = shape->getPosition();
        double size = 50; // 默认大小

        switch (shape->getType()) {
            case ShapeType::CIRCLE: {
                auto circle = std::dynamic_pointer_cast<Circle>(shape);
                if (circle) {
                    size = circle->getRadius();
                }
                break;
            }
            case ShapeType::RECTANGLE: {
                auto rect = std::dynamic_pointer_cast<Rectangle>(shape);
                if (rect) {
                    size = std::max(rect->getWidth(), rect->getHeight()) / 2;
                }
                break;
            }
            case ShapeType::TRIANGLE: {
                auto triangle = std::dynamic_pointer_cast<Triangle>(shape);
                if (triangle) {
                    size = std::max({triangle->getSideA(), triangle->getSideB(), triangle->getSideC()}) / 2;
                }
                break;
            }
            default:
                break;
        }

        minX = std::min(minX, pos.x() - size);
        minY = std::min(minY, pos.y() - size);
        maxX = std::max(maxX, pos.x() + size);
        maxY = std::max(maxY, pos.y() + size);
    }

    return QRectF(minX, minY, maxX - minX, maxY - minY);
}

// 查找指定点位置的图形
std::shared_ptr<Shape> ShapeCanvas::findShapeAt(const QPointF& point) const {
    // 从后向前查找（最后绘制的在最上层）
    for (int i = shapes_.size() - 1; i >= 0; --i) {
        const auto& shape = shapes_[i];

        // 简化处理：检查点是否在图形的边界框内
        QPointF pos = shape->getPosition();
        double size = 50; // 默认大小

        switch (shape->getType()) {
            case ShapeType::CIRCLE: {
                auto circle = std::dynamic_pointer_cast<Circle>(shape);
                if (circle) {
                    size = circle->getRadius();
                    // 检查点是否在圆内
                    double dx = point.x() - pos.x();
                    double dy = point.y() - pos.y();
                    if (dx * dx + dy * dy <= size * size) {
                        return shape;
                    }
                }
                break;
            }
            case ShapeType::RECTANGLE: {
                auto rect = std::dynamic_pointer_cast<Rectangle>(shape);
                if (rect) {
                    // 检查点是否在矩形内
                    if (fabs(point.x() - pos.x()) <= rect->getWidth() / 2 &&
                        fabs(point.y() - pos.y()) <= rect->getHeight() / 2) {
                        return shape;
                    }
                }
                break;
            }
            case ShapeType::TRIANGLE: {
                auto triangle = std::dynamic_pointer_cast<Triangle>(shape);
                if (triangle) {
                    // 简化处理：使用外接圆
                    size = std::max({triangle->getSideA(), triangle->getSideB(), triangle->getSideC()}) / 2;
                    double dx = point.x() - pos.x();
                    double dy = point.y() - pos.y();
                    if (dx * dx + dy * dy <= size * size) {
                        return shape;
                    }
                }
                break;
            }
            default:
                break;
        }
    }

    return nullptr;
}

// 获取选中的图形ID列表
QVector<int> ShapeCanvas::getSelectedShapeIds() const {
    return selectedShapeIds_;
}

// 设置选中的图形
void ShapeCanvas::setSelectedShapes(const QVector<int>& ids) {
    selectedShapeIds_ = ids;
    update();
}

// 清除图形选择
void ShapeCanvas::clearSelection() {
    selectedShapeIds_.clear();
    update();
}

// 绘制事件
void ShapeCanvas::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 填充背景
    painter.fillRect(rect(), backgroundColor_);

    // 应用变换
    applyTransform(painter);

    // 绘制网格
    if (showGrid_) {
        drawGrid(painter);
    }

    // 绘制坐标轴
    if (showAxes_) {
        drawAxes(painter);
    }

    // 绘制所有图形
    drawShapes(painter);

    // 绘制高亮
    drawHighlights(painter);
}

// 鼠标按下事件
void ShapeCanvas::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        // 检查是否点击了图形
        QPointF scenePos = mapFromCanvas(event->pos());
        auto clickedShape = findShapeAt(scenePos);

        if (clickedShape) {
            // 选中图形
            if (!selectedShapeIds_.contains(clickedShape->getId())) {
                selectedShapeIds_.append(clickedShape->getId());
                update();
            }

            // 发送信号
            emit shapeSelected(clickedShape);
        } else {
            // 开始拖动画布
            isDragging_ = true;
            dragStartPos_ = event->pos();

            // 清除选择
            clearSelection();
        }

        // 发送画布点击信号
        emit canvasClicked(scenePos);
    }
}

// 鼠标释放事件
void ShapeCanvas::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        isDragging_ = false;
    }
}

// 鼠标移动事件
void ShapeCanvas::mouseMoveEvent(QMouseEvent* event) {
    // 更新当前鼠标位置
    currentMousePos_ = mapFromCanvas(event->pos());
    emit mousePositionChanged(currentMousePos_);

    if (isDragging_) {
        // 拖动画布
        QPointF delta = event->pos() - dragStartPos_;
        offset_ += delta;
        dragStartPos_ = event->pos();

        update();
    }
}

// 鼠标滚轮事件
void ShapeCanvas::wheelEvent(QWheelEvent* event) {
    // 获取鼠标位置
    QPointF mousePos = event->pos();

    // 计算缩放因子
    qreal scaleFactor = 1.0;
    if (event->angleDelta().y() > 0) {
        scaleFactor = 1.1; // 放大
    } else {
        scaleFactor = 0.9; // 缩小
    }

    // 计算鼠标在场景中的位置
    QPointF scenePos = mapFromCanvas(mousePos);

    // 更新缩放比例
    qreal newZoom = zoomFactor_ * scaleFactor;
    if (newZoom < 0.1) newZoom = 0.1; // 最小缩放
    if (newZoom > 10.0) newZoom = 10.0; // 最大缩放

    // 计算新的偏移量，使鼠标指向的场景点保持不变
    offset_ = mousePos - scenePos * newZoom;

    // 更新缩放比例
    zoomFactor_ = newZoom;
    emit zoomChanged(zoomFactor_);

    update();
}

// 调整大小事件
void ShapeCanvas::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);

    // 更新偏移量，保持视图中心不变
    QPointF center = mapFromCanvas(getCanvasCenter());
    offset_ = getCanvasCenter() - center * zoomFactor_;

    update();
}

// 绘制网格
void ShapeCanvas::drawGrid(QPainter& painter) const {
    painter.setPen(QPen(gridColor_, 1));

    // 计算合适的网格间距
    double gridSize = 50;
    while (gridSize * zoomFactor_ < 20) {
        gridSize *= 2;
    }
    while (gridSize * zoomFactor_ > 100) {
        gridSize /= 2;
    }

    // 计算网格起始点
    double startX = fmod(offset_.x(), gridSize * zoomFactor_);
    if (startX > 0) startX -= gridSize * zoomFactor_;
    double startY = fmod(offset_.y(), gridSize * zoomFactor_);
    if (startY > 0) startY -= gridSize * zoomFactor_;

    // 绘制垂直线
    for (double x = startX; x < width(); x += gridSize * zoomFactor_) {
        painter.drawLine(QPointF(x, 0), QPointF(x, height()));
    }

    // 绘制水平线
    for (double y = startY; y < height(); y += gridSize * zoomFactor_) {
        painter.drawLine(QPointF(0, y), QPointF(width(), y));
    }
}

// 绘制坐标轴
void ShapeCanvas::drawAxes(QPainter& painter) const {
    painter.setPen(QPen(axesColor_, 2));

    // 绘制X轴
    double xAxisY = fmod(offset_.y(), height());
    if (xAxisY > 0) xAxisY -= height();
    painter.drawLine(QPointF(0, xAxisY), QPointF(width(), xAxisY));

    // 绘制Y轴
    double yAxisX = fmod(offset_.x(), width());
    if (yAxisX > 0) yAxisX -= width();
    painter.drawLine(QPointF(yAxisX, 0), QPointF(yAxisX, height()));
}

// 绘制所有图形
void ShapeCanvas::drawShapes(QPainter& painter) const {
    for (const auto& shape : shapes_) {
        drawShape(painter, shape);

        // 显示ID
        if (showIds_) {
            QPointF pos = mapToCanvas(shape->getPosition());
            painter.setPen(QPen(Qt::black, 1));
            painter.drawText(QPointF(pos.x() + 5, pos.y() - 5), 
                           QString("ID:%1").arg(shape->getId()));
        }
    }
}

// 绘制单个图形
void ShapeCanvas::drawShape(QPainter& painter, const std::shared_ptr<Shape>& shape) const {
    if (!shape) return;

    // 设置画笔
    QPen pen(QColor(shape->getColor().r, shape->getColor().g, shape->getColor().b));
    pen.setWidth(2);
    painter.setPen(pen);

    // 绘制图形
    shape->draw(&painter);
}

// 高亮选中的图形
void ShapeCanvas::drawHighlights(QPainter& painter) const {
    if (selectedShapeIds_.isEmpty()) return;

    painter.setPen(QPen(highlightColor_, 2, Qt::DashLine));

    for (int id : selectedShapeIds_) {
        // 查找选中的图形
        for (const auto& shape : shapes_) {
            if (shape->getId() == id) {
                // 绘制高亮边框
                QPointF pos = shape->getPosition();

                switch (shape->getType()) {
                    case ShapeType::CIRCLE: {
                        auto circle = std::dynamic_pointer_cast<Circle>(shape);
                        if (circle) {
                            painter.drawEllipse(pos, circle->getRadius() * zoomFactor_, 
                                              circle->getRadius() * zoomFactor_);
                        }
                        break;
                    }
                    case ShapeType::RECTANGLE: {
                        auto rect = std::dynamic_pointer_cast<Rectangle>(shape);
                        if (rect) {
                            painter.drawRect(QRectF(pos.x() - rect->getWidth()/2 * zoomFactor_, 
                                                  pos.y() - rect->getHeight()/2 * zoomFactor_,
                                                  rect->getWidth() * zoomFactor_, 
                                                  rect->getHeight() * zoomFactor_));
                        }
                        break;
                    }
                    case ShapeType::TRIANGLE: {
                        auto triangle = std::dynamic_pointer_cast<Triangle>(shape);
                        if (triangle) {
                            // 简化处理：使用外接圆
                            double size = std::max({triangle->getSideA(), triangle->getSideB(), triangle->getSideC()}) / 2;
                            painter.drawEllipse(pos, size * zoomFactor_, size * zoomFactor_);
                        }
                        break;
                    }
                    default:
                        break;
                }

                break;
            }
        }
    }
}

// 应用缩放变换
void ShapeCanvas::applyTransform(QPainter& painter) const {
    painter.translate(offset_);
    painter.scale(zoomFactor_, zoomFactor_);
}
