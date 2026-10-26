
#ifndef SHAPE_CANVAS_H
#define SHAPE_CANVAS_H

#include <QWidget>
#include <QVector>
#include <memory>
#include "Shape.h"

// 图形画布类 - 用于绘制和管理图形
class ShapeCanvas : public QWidget {
    Q_OBJECT

public:
    // 构造函数
    ShapeCanvas(QWidget* parent = nullptr);

    // 析构函数
    ~ShapeCanvas() override;

    // 添加图形
    void addShape(std::shared_ptr<Shape> shape);

    // 删除图形
    bool removeShape(int id);

    // 清空所有图形
    void clearShapes();

    // 获取图形数量
    int getShapeCount() const { return shapes_.size(); }

    // 获取图形列表
    const QVector<std::shared_ptr<Shape>>& getShapes() const { return shapes_; }

    // 设置当前缩放比例
    void setZoomFactor(qreal factor);

    // 获取当前缩放比例
    qreal getZoomFactor() const { return zoomFactor_; }

    // 重置缩放
    void resetZoom();

    // 居中显示所有图形
    void centerView();

    // 设置是否显示网格
    void setShowGrid(bool show);

    // 设置是否显示坐标轴
    void setShowAxes(bool show);

    // 设置是否显示图形ID
    void setShowIds(bool show);

    // 设置背景颜色
    void setBackgroundColor(const QColor& color);

    // 设置网格颜色
    void setGridColor(const QColor& color);

    // 设置坐标轴颜色
    void setAxesColor(const QColor& color);

    // 设置选中图形的高亮颜色
    void setHighlightColor(const QColor& color);

    // 获取画布中心点
    QPointF getCanvasCenter() const;

    // 将场景坐标转换为画布坐标
    QPointF mapToCanvas(const QPointF& point) const;

    // 将画布坐标转换为场景坐标
    QPointF mapFromCanvas(const QPointF& point) const;

    // 获取场景边界矩形
    QRectF getSceneBounds() const;

    // 查找指定点位置的图形
    std::shared_ptr<Shape> findShapeAt(const QPointF& point) const;

    // 获取选中的图形ID列表
    QVector<int> getSelectedShapeIds() const;

    // 设置选中的图形
    void setSelectedShapes(const QVector<int>& ids);

    // 清除图形选择
    void clearSelection();

    // 获取当前鼠标位置的场景坐标
    QPointF getCurrentMousePosition() const { return currentMousePos_; }

signals:
    // 图形被选中
    void shapeSelected(const std::shared_ptr<Shape>& shape);

    // 鼠标位置变化
    void mousePositionChanged(const QPointF& position);

    // 缩放比例变化
    void zoomChanged(qreal factor);

    // 画布被点击
    void canvasClicked(const QPointF& position);

protected:
    // 绘制事件
    void paintEvent(QPaintEvent* event) override;

    // 鼠标按下事件
    void mousePressEvent(QMouseEvent* event) override;

    // 鼠标释放事件
    void mouseReleaseEvent(QMouseEvent* event) override;

    // 鼠标移动事件
    void mouseMoveEvent(QMouseEvent* event) override;

    // 鼠标滚轮事件
    void wheelEvent(QWheelEvent* event) override;

    // 调整大小事件
    void resizeEvent(QResizeEvent* event) override;

    // 绘制网格
    void drawGrid(QPainter& painter) const;

    // 绘制坐标轴
    void drawAxes(QPainter& painter) const;

    // 绘制所有图形
    void drawShapes(QPainter& painter) const;

    // 绘制单个图形
    void drawShape(QPainter& painter, const std::shared_ptr<Shape>& shape) const;

    // 高亮选中的图形
    void drawHighlights(QPainter& painter) const;

    // 应用缩放变换
    void applyTransform(QPainter& painter) const;

private:
    // 图形列表
    QVector<std::shared_ptr<Shape>> shapes_;

    // 缩放比例
    qreal zoomFactor_;

    // 偏移量
    QPointF offset_;

    // 是否正在拖动
    bool isDragging_;
    QPointF dragStartPos_;

    // 鼠标位置
    QPointF currentMousePos_;

    // 选中的图形ID列表
    QVector<int> selectedShapeIds_;

    // 显示选项
    bool showGrid_;
    bool showAxes_;
    bool showIds_;

    // 颜色设置
    QColor backgroundColor_;
    QColor gridColor_;
    QColor axesColor_;
    QColor highlightColor_;
};

#endif // SHAPE_CANVAS_H
