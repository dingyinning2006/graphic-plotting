
#ifndef ADD_SHAPE_DIALOG_H
#define ADD_SHAPE_DIALOG_H

#include <QDialog>
#include <QMap>
#include <QVariant>
#include "Shape.h"

class QComboBox;
class QDoubleSpinBox;
class QPushButton;
class QColorButton;
class QFormLayout;
class QDialogButtonBox;

// 添加图形对话框类
class AddShapeDialog : public QDialog {
    Q_OBJECT

public:
    // 构造函数
    AddShapeDialog(QWidget* parent = nullptr);

    // 析构函数
    ~AddShapeDialog() override;

    // 获取创建的图形
    std::shared_ptr<Shape> getShape() const { return createdShape_; }

    // 设置图形类型
    void setShapeType(ShapeType type);

    // 设置初始值
    void setInitialValues(const QMap<QString, QVariant>& values);

private slots:
    // 图形类型改变
    void shapeTypeChanged(int index);

    // 颜色按钮点击
    void colorButtonClicked();

    // 确认按钮点击
    void accept() override;

private:
    // 创建UI
    void createUI();

    // 更新UI控件
    void updateUI();

    // 根据类型创建图形
    void createShape();

    // UI控件
    QComboBox* shapeTypeCombo_;
    QMap<QString, QWidget*> paramWidgets_;
    QPushButton* colorButton_;
    QColor selectedColor_;

    // 创建的图形
    std::shared_ptr<Shape> createdShape_;

    // 图形类型列表
    QList<ShapeType> shapeTypes_;
};

// 自定义颜色按钮
class QColorButton : public QPushButton {
    Q_OBJECT

public:
    explicit QColorButton(const QColor& color = Qt::white, QWidget* parent = nullptr);

    // 获取颜色
    QColor color() const { return currentColor_; }

    // 设置颜色
    void setColor(const QColor& color);

signals:
    // 颜色改变
    void colorChanged(const QColor& color);

private slots:
    // 选择颜色
    void chooseColor();

private:
    QColor currentColor_;
};

#endif // ADD_SHAPE_DIALOG_H
