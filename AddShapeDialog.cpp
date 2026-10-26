
#include "AddShapeDialog.h"
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QColorDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMap>
#include <QVariant>
#include <QColor>
#include <QDebug>
#include <Circle.h>
#include <Rectangle.h>
#include <Triangle.h>

// 构造函数
AddShapeDialog::AddShapeDialog(QWidget* parent)
    : QDialog(parent),
      selectedColor_(Qt::blue),
      createdShape_(nullptr)
{
    // 设置窗口属性
    setWindowTitle("添加图形");
    setModal(true);

    // 创建UI
    createUI();

    // 添加图形类型
    shapeTypes_ = {ShapeType::CIRCLE, ShapeType::RECTANGLE, ShapeType::TRIANGLE};

    // 设置默认图形类型
    shapeTypeCombo_->setCurrentIndex(0);
    shapeTypeChanged(0);
}

// 析构函数
AddShapeDialog::~AddShapeDialog() {
}

// 创建UI
void AddShapeDialog::createUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 图形类型选择
    QGroupBox* typeGroup = new QGroupBox("图形类型", this);
    QVBoxLayout* typeLayout = new QVBoxLayout(typeGroup);

    shapeTypeCombo_ = new QComboBox(this);
    typeLayout->addWidget(shapeTypeCombo_);

    mainLayout->addWidget(typeGroup);

    // 参数设置
    QGroupBox* paramGroup = new QGroupBox("参数设置", this);
    QFormLayout* paramLayout = new QFormLayout(paramGroup);

    mainLayout->addWidget(paramGroup);

    // 颜色选择
    QGroupBox* colorGroup = new QGroupBox("颜色设置", this);
    QHBoxLayout* colorLayout = new QHBoxLayout(colorGroup);

    colorButton_ = new QColorButton(selectedColor_, this);
    colorLayout->addWidget(new QLabel("颜色:"));
    colorLayout->addWidget(colorButton_);
    colorLayout->addStretch();

    mainLayout->addWidget(colorGroup);

    // 按钮组
    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    mainLayout->addWidget(buttonBox);

    // 连接信号槽
    connect(shapeTypeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AddShapeDialog::shapeTypeChanged);
    connect(colorButton_, &QColorButton::colorChanged,
            this, &AddShapeDialog::colorButtonClicked);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &AddShapeDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

// 更新UI控件
void AddShapeDialog::updateUI() {
    // 清除现有参数控件
    for (auto widget : paramWidgets_) {
        delete widget;
    }
    paramWidgets_.clear();

    // 根据图形类型添加相应的参数控件
    switch (shapeTypes_[shapeTypeCombo_->currentIndex()]) {
        case ShapeType::CIRCLE: {
            QDoubleSpinBox* radiusSpin = new QDoubleSpinBox(this);
            radiusSpin->setRange(0.1, 1000.0);
            radiusSpin->setValue(50.0);
            radiusSpin->setSuffix(" 像素");
            radiusSpin->setDecimals(1);
            paramWidgets_["radius"] = radiusSpin;
            static_cast<QFormLayout*>(paramWidgets_.parent())->addRow("半径:", radiusSpin);
            break;
        }
        case ShapeType::RECTANGLE: {
            QDoubleSpinBox* widthSpin = new QDoubleSpinBox(this);
            widthSpin->setRange(0.1, 1000.0);
            widthSpin->setValue(100.0);
            widthSpin->setSuffix(" 像素");
            widthSpin->setDecimals(1);
            paramWidgets_["width"] = widthSpin;
            static_cast<QFormLayout*>(paramWidgets_.parent())->addRow("宽度:", widthSpin);

            QDoubleSpinBox* heightSpin = new QDoubleSpinBox(this);
            heightSpin->setRange(0.1, 1000.0);
            heightSpin->setValue(60.0);
            heightSpin->setSuffix(" 像素");
            heightSpin->setDecimals(1);
            paramWidgets_["height"] = heightSpin;
            static_cast<QFormLayout*>(paramWidgets_.parent())->addRow("高度:", heightSpin);
            break;
        }
        case ShapeType::TRIANGLE: {
            QDoubleSpinBox* sideASpin = new QDoubleSpinBox(this);
            sideASpin->setRange(0.1, 1000.0);
            sideASpin->setValue(100.0);
            sideASpin->setSuffix(" 像素");
            sideASpin->setDecimals(1);
            paramWidgets_["sideA"] = sideASpin;
            static_cast<QFormLayout*>(paramWidgets_.parent())->addRow("边长A:", sideASpin);

            QDoubleSpinBox* sideBSpin = new QDoubleSpinBox(this);
            sideBSpin->setRange(0.1, 1000.0);
            sideBSpin->setValue(100.0);
            sideBSpin->setSuffix(" 像素");
            sideBSpin->setDecimals(1);
            paramWidgets_["sideB"] = sideBSpin;
            static_cast<QFormLayout*>(paramWidgets_.parent())->addRow("边长B:", sideBSpin);

            QDoubleSpinBox* sideCSpin = new QDoubleSpinBox(this);
            sideCSpin->setRange(0.1, 1000.0);
            sideCSpin->setValue(100.0);
            sideCSpin->setSuffix(" 像素");
            sideCSpin->setDecimals(1);
            paramWidgets_["sideC"] = sideCSpin;
            static_cast<QFormLayout*>(paramWidgets_.parent())->addRow("边长C:", sideCSpin);
            break;
        }
        default:
            break;
    }
}

// 图形类型改变
void AddShapeDialog::shapeTypeChanged(int index) {
    Q_UNUSED(index);
    updateUI();
}

// 颜色按钮点击
void AddShapeDialog::colorButtonClicked() {
    selectedColor_ = colorButton_->color();
}

// 确认按钮点击
void AddShapeDialog::accept() {
    try {
        // 创建图形
        createShape();

        if (createdShape_) {
            QDialog::accept();
        } else {
            QMessageBox::warning(this, "错误", "无法创建图形，请检查参数是否合法");
        }
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "错误", QString("创建图形失败: %1").arg(e.what()));
    }
}

// 设置图形类型
void AddShapeDialog::setShapeType(ShapeType type) {
    int index = shapeTypes_.indexOf(type);
    if (index >= 0) {
        shapeTypeCombo_->setCurrentIndex(index);
        shapeTypeChanged(index);
    }
}

// 设置初始值
void AddShapeDialog::setInitialValues(const QMap<QString, QVariant>& values) {
    // 设置图形类型
    if (values.contains("type")) {
        QString typeName = values["type"].toString();
        if (typeName == "Circle") {
            setShapeType(ShapeType::CIRCLE);
        } else if (typeName == "Rectangle") {
            setShapeType(ShapeType::RECTANGLE);
        } else if (typeName == "Triangle") {
            setShapeType(ShapeType::TRIANGLE);
        }
    }

    // 设置参数值
    for (auto it = values.begin(); it != values.end(); ++it) {
        QString key = it.key();
        if (key != "type" && paramWidgets_.contains(key)) {
            QWidget* widget = paramWidgets_[key];
            if (widget) {
                if (auto spinBox = qobject_cast<QDoubleSpinBox*>(widget)) {
                    spinBox->setValue(it.value().toDouble());
                }
            }
        }
    }

    // 设置颜色
    if (values.contains("color")) {
        QString colorStr = values["color"].toString();
        QColor color(colorStr);
        if (color.isValid()) {
            selectedColor_ = color;
            colorButton_->setColor(selectedColor_);
        }
    }
}

// 根据类型创建图形
void AddShapeDialog::createShape() {
    ShapeType type = shapeTypes_[shapeTypeCombo_->currentIndex()];

    try {
        switch (type) {
            case ShapeType::CIRCLE: {
                double radius = static_cast<QDoubleSpinBox*>(paramWidgets_["radius"])->value();
                createdShape_ = std::make_shared<Circle>(radius, 
                    Color(selectedColor_.red(), selectedColor_.green(), selectedColor_.blue()));
                break;
            }
            case ShapeType::RECTANGLE: {
                double width = static_cast<QDoubleSpinBox*>(paramWidgets_["width"])->value();
                double height = static_cast<QDoubleSpinBox*>(paramWidgets_["height"])->value();
                createdShape_ = std::make_shared<Rectangle>(width, height,
                    Color(selectedColor_.red(), selectedColor_.green(), selectedColor_.blue()));
                break;
            }
            case ShapeType::TRIANGLE: {
                double sideA = static_cast<QDoubleSpinBox*>(paramWidgets_["sideA"])->value();
                double sideB = static_cast<QDoubleSpinBox*>(paramWidgets_["sideB"])->value();
                double sideC = static_cast<QDoubleSpinBox*>(paramWidgets_["sideC"])->value();
                createdShape_ = std::make_shared<Triangle>(sideA, sideB, sideC,
                    Color(selectedColor_.red(), selectedColor_.green(), selectedColor_.blue()));
                break;
            }
            default:
                break;
        }
    } catch (const std::exception& e) {
        createdShape_ = nullptr;
        throw;
    }
}

// QColorButton 构造函数
QColorButton::QColorButton(const QColor& color, QWidget* parent)
    : QPushButton(parent),
      currentColor_(color)
{
    updateButton();
}

// 设置颜色
void QColorButton::setColor(const QColor& color) {
    if (currentColor_ != color) {
        currentColor_ = color;
        updateButton();
        emit colorChanged(color);
    }
}

// 选择颜色
void QColorButton::chooseColor() {
    QColor color = QColorDialog::getColor(currentColor_, this, "选择颜色");
    if (color.isValid()) {
        setColor(color);
    }
}

// 更新按钮
void QColorButton::updateButton() {
    setStyleSheet(QString("background-color: %1; min-width: 50px; min-height: 25px;")
                 .arg(currentColor_.name()));

    // 如果颜色较暗，使用白色文本，否则使用黑色文本
    int brightness = (currentColor_.red() * 299 + currentColor_.green() * 587 + currentColor_.blue() * 114) / 1000;
    setTextColor(brightness < 128 ? Qt::white : Qt::black);
}
