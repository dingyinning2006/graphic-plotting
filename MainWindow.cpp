
#include "MainWindow.h"
#include <QApplication>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QActionGroup>
#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QColorDialog>
#include <QSplitter>
#include <QGroupBox>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QSettings>
#include <QFont>
#include <QBrush>
#include <QPen>

// 构造函数
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      hasUnsavedChanges_(false) {
    // 设置窗口属性
    setWindowTitle("图形绘制与面积周长计算系统");
    setMinimumSize(800, 600);

    // 初始化UI
    initUI();

    // 连接信号槽
    connectSignalsSlots();

    // 恢复窗口状态
    restoreDefaults();
}

// 析构函数
MainWindow::~MainWindow() {
}

// 初始化UI
void MainWindow::initUI() {
    // 创建菜单栏
    createMenuBar();

    // 创建工具栏
    createToolBar();

    // 创建状态栏
    createStatusBar();

    // 创建中央部件
    createCentralWidget();

    // 更新UI状态
    updateToolBarState();
    updateMenuState();
}

// 创建菜单栏
void MainWindow::createMenuBar() {
    menuBar_ = menuBar();

    // 文件菜单
    QMenu* fileMenu = menuBar_->addMenu("文件(&F)");

    QAction* newAction = fileMenu->addAction("新建(&N)", this, &MainWindow::newProject);
    newAction->setShortcut(QKeySequence::New);

    QAction* openAction = fileMenu->addAction("打开(&O)", this, &MainWindow::openProject);
    openAction->setShortcut(QKeySequence::Open);

    fileMenu->addSeparator();

    QAction* saveAction = fileMenu->addAction("保存(&S)", this, &MainWindow::saveProject);
    saveAction->setShortcut(QKeySequence::Save);

    QAction* saveAsAction = fileMenu->addAction("另存为(&A)", this, &MainWindow::saveProjectAs);
    saveAsAction->setShortcut(QKeySequence::SaveAs);

    fileMenu->addSeparator();

    QAction* exportCsvAction = fileMenu->addAction("导出为CSV(&C)", this, &MainWindow::exportToCSV);

    QAction* importCsvAction = fileMenu->addAction("从CSV导入(&I)", this, &MainWindow::importFromCSV);

    fileMenu->addSeparator();

    QAction* exitAction = fileMenu->addAction("退出(&X)", this, &MainWindow::exitApplication);
    exitAction->setShortcut(QKeySequence::Quit);

    // 编辑菜单
    QMenu* editMenu = menuBar_->addMenu("编辑(&E)");

    QAction* addShapeAction = editMenu->addAction("添加图形(&A)", this, &MainWindow::addShape);
    addShapeAction->setShortcut(QKeySequence("Ctrl+A"));

    QAction* editShapeAction = editMenu->addAction("编辑图形(&D)", this, &MainWindow::editSelectedShape);
    editShapeAction->setShortcut(QKeySequence("Ctrl+D"));

    QAction* removeShapeAction = editMenu->addAction("删除图形(&R)", this, &MainWindow::removeSelectedShape);
    removeShapeAction->setShortcut(QKeySequence::Delete);

    editMenu->addSeparator();

    QAction* clearAllAction = editMenu->addAction("清空所有图形(&C)", this, &MainWindow::clearAllShapes);

    // 排序菜单
    QMenu* sortMenu = menuBar_->addMenu("排序(&S)");

    QAction* sortByAreaAction = sortMenu->addAction("按面积排序(&A)", this, &MainWindow::sortByArea);

    QAction* sortByPerimeterAction = sortMenu->addAction("按周长排序(&P)", this, &MainWindow::sortByPerimeter);

    QAction* sortByIdAction = sortMenu->addAction("按ID排序(&I)", this, &MainWindow::sortById);

    QAction* sortByTypeAction = sortMenu->addAction("按类型排序(&T)", this, &MainWindow::sortByType);

    // 视图菜单
    QMenu* viewMenu = menuBar_->addMenu("视图(&V)");

    QAction* showAllAction = viewMenu->addAction("显示所有图形(&A)", this, &MainWindow::showAllShapes);

    viewMenu->addSeparator();

    QAction* zoomInAction = viewMenu->addAction("放大(&I)", this, &MainWindow::zoomIn);
    zoomInAction->setShortcut(QKeySequence::ZoomIn);

    QAction* zoomOutAction = viewMenu->addAction("缩小(&O)", this, &MainWindow::zoomOut);
    zoomOutAction->setShortcut(QKeySequence::ZoomOut);

    QAction* resetZoomAction = viewMenu->addAction("重置缩放(&R)", this, &MainWindow::resetZoom);

    // 帮助菜单
    QMenu* helpMenu = menuBar_->addMenu("帮助(&H)");

    QAction* helpAction = helpMenu->addAction("使用帮助(&H)", this, &MainWindow::showHelp);

    QAction* aboutAction = helpMenu->addAction("关于(&A)", this, &MainWindow::showAbout);
}

// 创建工具栏
void MainWindow::createToolBar() {
    toolBar_ = addToolBar("主工具栏");
    toolBar_->setMovable(false);
    toolBar_->setIconSize(QSize(24, 24));

    // 文件操作
    QAction* newAction = new QAction(QIcon::fromTheme("document-new"), "新建", this);
    connect(newAction, &QAction::triggered, this, &MainWindow::newProject);
    toolBar_->addAction(newAction);

    QAction* openAction = new QAction(QIcon::fromTheme("document-open"), "打开", this);
    connect(openAction, &QAction::triggered, this, &MainWindow::openProject);
    toolBar_->addAction(openAction);

    QAction* saveAction = new QAction(QIcon::fromTheme("document-save"), "保存", this);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveProject);
    toolBar_->addAction(saveAction);

    toolBar_->addSeparator();

    // 图形操作
    QAction* addShapeAction = new QAction(QIcon::fromTheme("list-add"), "添加图形", this);
    connect(addShapeAction, &QAction::triggered, this, &MainWindow::addShape);
    toolBar_->addAction(addShapeAction);

    QAction* editShapeAction = new QAction(QIcon::fromTheme("document-properties"), "编辑图形", this);
    connect(editShapeAction, &QAction::triggered, this, &MainWindow::editSelectedShape);
    toolBar_->addAction(editShapeAction);

    QAction* removeShapeAction = new QAction(QIcon::fromTheme("list-remove"), "删除图形", this);
    connect(removeShapeAction, &QAction::triggered, this, &MainWindow::removeSelectedShape);
    toolBar_->addAction(removeShapeAction);

    toolBar_->addSeparator();

    // 视图操作
    QAction* zoomInAction = new QAction(QIcon::fromTheme("zoom-in"), "放大", this);
    connect(zoomInAction, &QAction::triggered, this, &MainWindow::zoomIn);
    toolBar_->addAction(zoomInAction);

    QAction* zoomOutAction = new QAction(QIcon::fromTheme("zoom-out"), "缩小", this);
    connect(zoomOutAction, &QAction::triggered, this, &MainWindow::zoomOut);
    toolBar_->addAction(zoomOutAction);

    QAction* resetZoomAction = new QAction(QIcon::fromTheme("zoom-original"), "重置缩放", this);
    connect(resetZoomAction, &QAction::triggered, this, &MainWindow::resetZoom);
    toolBar_->addAction(resetZoomAction);
}

// 创建状态栏
void MainWindow::createStatusBar() {
    statusBar_ = statusBar();

    // 添加永久信息
    QLabel* statusLabel = new QLabel("就绪");
    statusBar_->addPermanentWidget(statusLabel);

    // 添加坐标信息
    QLabel* coordLabel = new QLabel("坐标: ");
    statusBar_->addPermanentWidget(coordLabel);

    // 添加图形数量信息
    updateStatusBar();
}

// 创建中央部件
void MainWindow::createCentralWidget() {
    centralWidget_ = new QWidget(this);
    setCentralWidget(centralWidget_);

    mainLayout_ = new QHBoxLayout(centralWidget_);

    // 创建分割器
    QSplitter* splitter = new QSplitter(Qt::Horizontal);
    mainLayout_->addWidget(splitter);

    // 创建图形画布
    createShapeCanvas();
    splitter->addWidget(shapeCanvas_);

    // 创建右侧面板
    QWidget* rightPanel = new QWidget();
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    splitter->addWidget(rightPanel);

    // 创建表格视图
    createTableWidget();
    rightLayout->addWidget(tableWidget_);

    // 创建属性编辑器
    createPropertyEditor();
    rightLayout->addWidget(propertyEditor_);

    // 设置分割器比例
    splitter->setStretchFactor(0, 7); // 画布占70%
    splitter->setStretchFactor(1, 3); // 右侧面板占30%
}

// 创建图形画布
void MainWindow::createShapeCanvas() {
    shapeCanvas_ = new ShapeCanvas(this);
    shapeCanvas_->setMinimumWidth(400);
    shapeCanvas_->setMinimumHeight(400);
}

// 创建表格视图
void MainWindow::createTableWidget() {
    tableWidget_ = new QTableWidget(this);
    tableWidget_->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget_->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidget_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget_->setAlternatingRowColors(true);
    tableWidget_->setSortingEnabled(true);

    // 设置表头
    QStringList headers;
    headers << "ID" << "类型" << "面积" << "周长" << "颜色" << "位置";
    tableWidget_->setColumnCount(headers.size());
    tableWidget_->setHorizontalHeaderLabels(headers);

    // 设置列宽
    tableWidget_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tableWidget_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tableWidget_->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    tableWidget_->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    tableWidget_->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    tableWidget_->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);

    // 设置行高
    tableWidget_->verticalHeader()->setDefaultSectionSize(24);
}

// 创建属性编辑器
void MainWindow::createPropertyEditor() {
    propertyEditor_ = new QGroupBox("属性编辑器", this);
    propertyLayout_ = new QFormLayout(propertyEditor_);

    // 创建属性控件
    propertyWidgets_["id"] = new QLabel();
    propertyWidgets_["type"] = new QLabel();
    propertyWidgets_["area"] = new QLabel();
    propertyWidgets_["perimeter"] = new QLabel();
    propertyWidgets_["color"] = new QPushButton("选择颜色");
    propertyWidgets_["positionX"] = new QDoubleSpinBox();
    propertyWidgets_["positionY"] = new QDoubleSpinBox();

    // 设置属性控件
    static_cast<QLabel*>(propertyWidgets_["id"])->setText("ID: ");
    static_cast<QLabel*>(propertyWidgets_["type"])->setText("类型: ");
    static_cast<QLabel*>(propertyWidgets_["area"])->setText("面积: ");
    static_cast<QLabel*>(propertyWidgets_["perimeter"])->setText("周长: ");

    static_cast<QDoubleSpinBox*>(propertyWidgets_["positionX"])->setRange(-9999, 9999);
    static_cast<QDoubleSpinBox*>(propertyWidgets_["positionX"])->setDecimals(2);
    static_cast<QDoubleSpinBox*>(propertyWidgets_["positionX"])->setSingleStep(0.1);

    static_cast<QDoubleSpinBox*>(propertyWidgets_["positionY"])->setRange(-9999, 9999);
    static_cast<QDoubleSpinBox*>(propertyWidgets_["positionY"])->setDecimals(2);
    static_cast<QDoubleSpinBox*>(propertyWidgets_["positionY"])->setSingleStep(0.1);

    // 添加属性控件到布局
    propertyLayout_->addRow("ID:", propertyWidgets_["id"]);
    propertyLayout_->addRow("类型:", propertyWidgets_["type"]);
    propertyLayout_->addRow("面积:", propertyWidgets_["area"]);
    propertyLayout_->addRow("周长:", propertyWidgets_["perimeter"]);
    propertyLayout_->addRow("颜色:", propertyWidgets_["color"]);
    propertyLayout_->addRow("位置X:", propertyWidgets_["positionX"]);
    propertyLayout_->addRow("位置Y:", propertyWidgets_["positionY"]);

    // 添加保存按钮
    QPushButton* saveButton = new QPushButton("保存更改");
    propertyLayout_->addRow(saveButton);

    // 连接信号槽
    connect(static_cast<QPushButton*>(propertyWidgets_["color"]), &QPushButton::clicked,
            [this]() {
                QColor currentColor;
                // 获取当前图形的颜色
                auto selectedShape = getSelectedShape();
                if (selectedShape) {
                    currentColor = QColor(selectedShape->getColor().r, 
                                        selectedShape->getColor().g, 
                                        selectedShape->getColor().b);
                } else {
                    currentColor = Qt::blue;
                }

                QColor color = QColorDialog::getColor(currentColor, this, "选择颜色");
                if (color.isValid()) {
                    // 更新颜色显示
                    static_cast<QPushButton*>(propertyWidgets_["color"])->setStyleSheet(
                        QString("background-color: %1; min-width: 50px; min-height: 25px;")
                        .arg(color.name()));

                    // 更新选中图形的颜色
                    if (selectedShape) {
                        selectedShape->setColor(Color(color.red(), color.green(), color.blue()));
                        updateTableWidget();
                        updateStatusBar();
                    }
                }
            });

    connect(saveButton, &QPushButton::clicked,
            [this]() {
                auto selectedShape = getSelectedShape();
                if (selectedShape) {
                    // 更新位置
                    double x = static_cast<QDoubleSpinBox*>(propertyWidgets_["positionX"])->value();
                    double y = static_cast<QDoubleSpinBox*>(propertyWidgets_["positionY"])->value();
                    selectedShape->setPosition(Point(x, y));

                    // 更新表格和画布
                    updateTableWidget();
                    shapeCanvas_->update();

                    // 更新状态栏
                    updateStatusBar();

                    // 标记为未保存
                    hasUnsavedChanges_ = true;
                }
            });
}

// 连接信号槽
void MainWindow::connectSignalsSlots() {
    // 表格行选择变化
    connect(tableWidget_, &QTableWidget::itemSelectionChanged,
            this, &MainWindow::tableRowSelectionChanged);

    // 图形选择变化
    connect(shapeCanvas_, &ShapeCanvas::shapeSelected,
            this, &MainWindow::shapeSelectionChanged);

    // 添加图形完成
    connect(shapeCanvas_, &ShapeCanvas::shapeAdded,
            this, &MainWindow::shapeAdded);

    // 编辑图形完成
    connect(shapeCanvas_, &ShapeCanvas::shapeEdited,
            this, &MainWindow::shapeEdited);

    // 鼠标位置变化
    connect(shapeCanvas_, &ShapeCanvas::mousePositionChanged,
            [this](const QPointF& position) {
                statusBar_->showMessage(QString("坐标: (%1, %2)").arg(position.x()).arg(position.y()));
            });
}

// 更新工具栏按钮状态
void MainWindow::updateToolBarState() {
    bool hasShapes = shapeManager_.getShapeCount() > 0;
    bool hasSelection = tableWidget_->selectedItems().count() > 0;

    // 更新工具栏按钮状态
    for (auto action : toolBar_->actions()) {
        if (action->text() == "删除图形") {
            action->setEnabled(hasSelection);
        } else if (action->text() == "保存") {
            action->setEnabled(hasShapes);
        } else if (action->text() == "导出为CSV") {
            action->setEnabled(hasShapes);
        }
    }
}

// 更新菜单项状态
void MainWindow::updateMenuState() {
    bool hasShapes = shapeManager_.getShapeCount() > 0;
    bool hasSelection = tableWidget_->selectedItems().count() > 0;

    // 更新菜单项状态
    for (auto menu : menuBar_->findChildren<QMenu*>()) {
        for (auto action : menu->actions()) {
            if (action->text() == "删除图形") {
                action->setEnabled(hasSelection);
            } else if (action->text() == "清空所有图形") {
                action->setEnabled(hasShapes);
            } else if (action->text() == "导出为CSV") {
                action->setEnabled(hasShapes);
            } else if (action->text() == "从CSV导入") {
                action->setEnabled(true); // 总是允许导入
            }
        }
    }
}

// 获取当前选中的图形
std::shared_ptr<Shape> MainWindow::getSelectedShape() const {
    QList<QTableWidgetItem*> selectedItems = tableWidget_->selectedItems();
    if (selectedItems.isEmpty()) {
        return nullptr;
    }

    // 获取选中行的ID
    int row = selectedItems.first()->row();
    int id = tableWidget_->item(row, 0)->text().toInt();

    // 在管理器中查找图形
    return shapeManager_.findShapeById(id);
}

// 获取选中的图形ID列表
QVector<int> MainWindow::getSelectedShapeIds() const {
    QVector<int> ids;
    QList<QTableWidgetItem*> selectedItems = tableWidget_->selectedItems();

    if (selectedItems.isEmpty()) {
        return ids;
    }

    // 获取选中行的ID
    int row = selectedItems.first()->row();
    int id = tableWidget_->item(row, 0)->text().toInt();
    ids.append(id);

    return ids;
}

// 高亮选中的图形
void MainWindow::highlightSelectedShapes() {
    shapeCanvas_->setSelectedShapes(getSelectedShapeIds());
}

// 保存项目文件
bool MainWindow::saveProjectFile(const QString& filename) {
    try {
        shapeManager_.saveToFile(filename.toStdString());
        setCurrentProjectFile(filename);
        hasUnsavedChanges_ = false;
        return true;
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "保存错误", 
            QString("保存项目失败: %1").arg(e.what()));
        return false;
    }
}

// 加载项目文件
bool MainWindow::loadProjectFile(const QString& filename) {
    try {
        shapeManager_.loadFromFile(filename.toStdString());
        setCurrentProjectFile(filename);
        hasUnsavedChanges_ = false;
        updateTableWidget();
        updateStatusBar();
        return true;
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "加载错误", 
            QString("加载项目失败: %1").arg(e.what()));
        return false;
    }
}

// 设置当前项目文件路径
void MainWindow::setCurrentProjectFile(const QString& path) {
    currentProjectFile_ = path;
    setWindowTitle(windowTitle().section(" - ", 0, 0) + " - " + path);
}

// 获取当前项目文件路径
QString MainWindow::getCurrentProjectFile() const {
    return currentProjectFile_;
}

// 检查是否有未保存的更改
bool MainWindow::hasUnsavedChanges() const {
    return hasUnsavedChanges_;
}

// 提示保存未保存的更改
bool MainWindow::promptToSaveUnsavedChanges() {
    if (!hasUnsavedChanges_) {
        return true;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "未保存的更改",
        "您有未保存的更改，是否保存？",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    if (reply == QMessageBox::Save) {
        if (currentProjectFile_.isEmpty()) {
            return saveProjectAs();
        } else {
            return saveProjectFile(currentProjectFile_);
        }
    } else if (reply == QMessageBox::Discard) {
        return true;
    } else {
        return false;
    }
}

// 恢复默认设置
void MainWindow::restoreDefaults() {
    // 加载设置
    QSettings settings;

    // 恢复窗口几何
    if (!settings.value("geometry").isNull()) {
        restoreGeometry(settings.value("geometry").toByteArray());
    }

    // 恢复窗口状态
    if (!settings.value("windowState").isNull()) {
        restoreState(settings.value("windowState").toByteArray());
    }

    // 恢复其他设置
    shapeCanvas_->setShowGrid(settings.value("showGrid", true).toBool());
    shapeCanvas_->setShowAxes(settings.value("showAxes", true).toBool());
    shapeCanvas_->setShowIds(settings.value("showIds", true).toBool());

    // 居中显示所有图形
    shapeCanvas_->centerView();
}

// 更新状态栏信息
void MainWindow::updateStatusBar() {
    statusBar_->showMessage(QString("图形总数: %1, 总面积: %2, 总周长: %3")
        .arg(shapeManager_.getShapeCount())
        .arg(shapeManager_.getTotalArea(), 0, 'f', 2)
        .arg(shapeManager_.getTotalPerimeter(), 0, 'f', 2));
}

// 更新表格视图
void MainWindow::updateTableWidget() {
    // 清空表格
    tableWidget_->setRowCount(0);

    // 获取所有图形
    const auto& shapes = shapeManager_.getShapes();

    // 设置表格行数
    tableWidget_->setRowCount(shapes.size());

    // 填充表格
    for (size_t i = 0; i < shapes.size(); ++i) {
        const auto& shape = shapes[i];

        // ID
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(shape->getId()));
        tableWidget_->setItem(i, 0, idItem);

        // 类型
        QTableWidgetItem* typeItem = new QTableWidgetItem(Shape::typeToString(shape->getType()));
        tableWidget_->setItem(i, 1, typeItem);

        // 面积
        QTableWidgetItem* areaItem = new QTableWidgetItem(QString::number(shape->getArea(), 'f', 2));
        tableWidget_->setItem(i, 2, areaItem);

        // 周长
        QTableWidgetItem* perimeterItem = new QTableWidgetItem(QString::number(shape->getPerimeter(), 'f', 2));
        tableWidget_->setItem(i, 3, perimeterItem);

        // 颜色
        const auto& color = shape->getColor();
        QString colorStr = QString("RGB(%1,%2,%3)").arg(color.r).arg(color.g).arg(color.b);
        QTableWidgetItem* colorItem = new QTableWidgetItem(colorStr);
        colorItem->setBackground(QColor(color.r, color.g, color.b));
        tableWidget_->setItem(i, 4, colorItem);

        // 位置
        const auto& pos = shape->getPosition();
        QString posStr = QString("(%1,%2)").arg(pos.x).arg(pos.y, 0, 'f', 2);
        QTableWidgetItem* posItem = new QTableWidgetItem(posStr);
        tableWidget_->setItem(i, 5, posItem);
    }
}

// 表格行选择变化
void MainWindow::tableRowSelectionChanged() {
    // 更新属性编辑器
    auto selectedShape = getSelectedShape();
    if (selectedShape) {
        // 更新属性显示
        static_cast<QLabel*>(propertyWidgets_["id"])->setText("ID: " + QString::number(selectedShape->getId()));
        static_cast<QLabel*>(propertyWidgets_["type"])->setText("类型: " + Shape::typeToString(selectedShape->getType()));
        static_cast<QLabel*>(propertyWidgets_["area"])->setText("面积: " + QString::number(selectedShape->getArea(), 'f', 2));
        static_cast<QLabel*>(propertyWidgets_["perimeter"])->setText("周长: " + QString::number(selectedShape->getPerimeter(), 'f', 2));

        // 更新颜色显示
        const auto& color = selectedShape->getColor();
        static_cast<QPushButton*>(propertyWidgets_["color"])->setStyleSheet(
            QString("background-color: rgb(%1,%2,%3); min-width: 50px; min-height: 25px;")
            .arg(color.r).arg(color.g).arg(color.b));

        // 更新位置显示
        const auto& pos = selectedShape->getPosition();
        static_cast<QDoubleSpinBox*>(propertyWidgets_["positionX"])->setValue(pos.x);
        static_cast<QDoubleSpinBox*>(propertyWidgets_["positionY"])->setValue(pos.y);
    } else {
        // 清空属性显示
        static_cast<QLabel*>(propertyWidgets_["id"])->setText("ID: ");
        static_cast<QLabel*>(propertyWidgets_["type"])->setText("类型: ");
        static_cast<QLabel*>(propertyWidgets_["area"])->setText("面积: ");
        static_cast<QLabel*>(propertyWidgets_["perimeter"])->setText("周长: ");
        static_cast<QPushButton*>(propertyWidgets_["color"])->setStyleSheet("");
        static_cast<QDoubleSpinBox*>(propertyWidgets_["positionX"])->setValue(0);
        static_cast<QDoubleSpinBox*>(propertyWidgets_["positionY"])->setValue(0);
    }

    // 高亮选中的图形
    highlightSelectedShapes();

    // 更新工具栏和菜单状态
    updateToolBarState();
    updateMenuState();
}

// 图形选择变化
void MainWindow::shapeSelectionChanged() {
    // 更新表格选择
    auto selectedIds = shapeCanvas_->getSelectedShapeIds();
    if (!selectedIds.isEmpty()) {
        // 找到对应的行
        for (int row = 0; row < tableWidget_->rowCount(); ++row) {
            int id = tableWidget_->item(row, 0)->text().toInt();
            if (selectedIds.contains(id)) {
                // 选中行
                tableWidget_->selectRow(row);
                break;
            }
        }
    } else {
        // 清空选择
        tableWidget_->clearSelection();
    }
}

// 添加图形完成
void MainWindow::shapeAdded(const std::shared_ptr<Shape>& shape) {
    if (shape) {
        // 添加到管理器
        shapeManager_.addShape(shape);

        // 更新UI
        updateTableWidget();
        updateStatusBar();

        // 标记为未保存
        hasUnsavedChanges_ = true;

        // 更新工具栏和菜单状态
        updateToolBarState();
        updateMenuState();
    }
}

// 编辑图形完成
void MainWindow::shapeEdited(const std::shared_ptr<Shape>& shape) {
    if (shape) {
        // 更新UI
        updateTableWidget();
        updateStatusBar();

        // 标记为未保存
        hasUnsavedChanges_ = true;
    }
}

// 新建项目
void MainWindow::newProject() {
    if (promptToSaveUnsavedChanges()) {
        // 清空图形
        shapeManager_.clearShapes();

        // 清空当前项目文件
        setCurrentProjectFile("");

        // 更新UI
        updateTableWidget();
        updateStatusBar();

        // 标记为未保存
        hasUnsavedChanges_ = false;

        // 更新工具栏和菜单状态
        updateToolBarState();
        updateMenuState();
    }
}

// 打开项目
void MainWindow::openProject() {
    if (promptToSaveUnsavedChanges()) {
        QString filename = QFileDialog::getOpenFileName(this, "打开项目", "", "项目文件 (*.proj)");
        if (!filename.isEmpty()) {
            if (loadProjectFile(filename)) {
                // 成功加载
            }
        }
    }
}

// 保存项目
void MainWindow::saveProject() {
    if (currentProjectFile_.isEmpty()) {
        saveProjectAs();
    } else {
        saveProjectFile(currentProjectFile_);
    }
}

// 另存为项目
bool MainWindow::saveProjectAs() {
    QString filename = QFileDialog::getSaveFileName(this, "保存项目", "", "项目文件 (*.proj)");
    if (!filename.isEmpty()) {
        if (!filename.endsWith(".proj")) {
            filename += ".proj";
        }
        return saveProjectFile(filename);
    }
    return false;
}

// 导出为CSV
void MainWindow::exportToCSV() {
    QString filename = QFileDialog::getSaveFileName(this, "导出为CSV", "", "CSV文件 (*.csv)");
    if (!filename.isEmpty()) {
        if (!filename.endsWith(".csv")) {
            filename += ".csv";
        }

        try {
            std::ofstream outFile(filename.toStdString());
            if (!outFile) {
                throw std::runtime_error("无法创建文件");
            }

            // 写入CSV表头
            outFile << "ID,类型,面积,周长,颜色,位置\n";

            // 写入每个图形的数据
            const auto& shapes = shapeManager_.getShapes();
            for (const auto& shape : shapes) {
                outFile << shape->serialize() << "\n";
            }

            outFile.close();
            QMessageBox::information(this, "成功", "项目已成功导出为CSV文件");
        } catch (const std::exception& e) {
            QMessageBox::warning(this, "错误", QString("导出失败: %1").arg(e.what()));
        }
    }
}

// 从CSV导入
void MainWindow::importFromCSV() {
    QString filename = QFileDialog::getOpenFileName(this, "从CSV导入", "", "CSV文件 (*.csv)");
    if (!filename.isEmpty()) {
        try {
            std::ifstream inFile(filename.toStdString());
            if (!inFile) {
                throw std::runtime_error("无法打开文件");
            }

            // 读取CSV文件内容
            std::string line;
            std::vector<std::shared_ptr<Shape>> shapes;

            // 跳过表头
            std::getline(inFile, line);

            // 读取每个图形
            while (std::getline(inFile, line)) {
                if (!line.empty()) {
                    size_t commaPos = line.find(',');
                    if (commaPos != std::string::npos) {
                        std::string type = line.substr(0, commaPos);
                        std::string data = line.substr(commaPos + 1);

                        try {
                            auto shape = Shape::deserialize(type + "," + data);
                            if (shape) {
                                shapes.push_back(shape);
                            }
                        } catch (const std::exception& e) {
                            std::cerr << "导入图形时出错: " << e.what() << std::endl;
                        }
                    }
                }
            }

            inFile.close();

            // 添加到管理器
            for (const auto& shape : shapes) {
                shapeManager_.addShape(shape);
            }

            // 更新UI
            updateTableWidget();
            updateStatusBar();

            // 标记为未保存
            hasUnsavedChanges_ = true;

            // 更新工具栏和菜单状态
            updateToolBarState();
            updateMenuState();

            QMessageBox::information(this, "成功", QString("成功导入 %1 个图形").arg(shapes.size()));
        } catch (const std::exception& e) {
            QMessageBox::warning(this, "错误", QString("导入失败: %1").arg(e.what()));
        }
    }
}

// 退出应用程序
void MainWindow::exitApplication() {
    if (promptToSaveUnsavedChanges()) {
        // 保存窗口设置
        QSettings settings;
        settings.setValue("geometry", saveGeometry());
        settings.setValue("windowState", saveState());

        // 退出应用程序
        QApplication::quit();
    }
}

// 添加图形
void MainWindow::addShape() {
    AddShapeDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        auto shape = dialog.getShape();
        if (shape) {
            // 添加到画布
            shapeCanvas_->addShape(shape);

            // 添加到管理器
            shapeManager_.addShape(shape);

            // 更新UI
            updateTableWidget();
            updateStatusBar();

            // 标记为未保存
            hasUnsavedChanges_ = true;

            // 更新工具栏和菜单状态
            updateToolBarState();
            updateMenuState();
        }
    }
}

// 删除选中的图形
void MainWindow::removeSelectedShape() {
    auto selectedShape = getSelectedShape();
    if (selectedShape) {
        // 从管理器中删除
        shapeManager_.removeShapeById(selectedShape->getId());

        // 从画布中删除
        shapeCanvas_->removeShape(selectedShape->getId());

        // 更新UI
        updateTableWidget();
        updateStatusBar();

        // 标记为未保存
        hasUnsavedChanges_ = true;

        // 更新工具栏和菜单状态
        updateToolBarState();
        updateMenuState();
    }
}

// 清空所有图形
void MainWindow::clearAllShapes() {
    if (QMessageBox::question(this, "确认", "确定要清空所有图形吗？", 
                            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        // 清空管理器
        shapeManager_.clearShapes();

        // 清空画布
        shapeCanvas_->clearShapes();

        // 更新UI
        updateTableWidget();
        updateStatusBar();

        // 标记为未保存
        hasUnsavedChanges_ = true;

        // 更新工具栏和菜单状态
        updateToolBarState();
        updateMenuState();
    }
}

// 编辑选中的图形
void MainWindow::editSelectedShape() {
    auto selectedShape = getSelectedShape();
    if (selectedShape) {
        // 创建编辑对话框
        AddShapeDialog dialog(this);
        dialog.setShapeType(selectedShape->getType());

        // 设置初始值
        QMap<QString, QVariant> initialValues;
        initialValues["type"] = Shape::typeToString(selectedShape->getType());
        initialValues["color"] = QString("RGB(%1,%2,%3)")
            .arg(selectedShape->getColor().r)
            .arg(selectedShape->getColor().g)
            .arg(selectedShape->getColor().b);

        switch (selectedShape->getType()) {
            case ShapeType::CIRCLE: {
                auto circle = std::dynamic_pointer_cast<Circle>(selectedShape);
                if (circle) {
                    initialValues["radius"] = circle->getRadius();
                }
                break;
            }
            case ShapeType::RECTANGLE: {
                auto rect = std::dynamic_pointer_cast<Rectangle>(selectedShape);
                if (rect) {
                    initialValues["width"] = rect->getWidth();
                    initialValues["height"] = rect->getHeight();
                }
                break;
            }
            case ShapeType::TRIANGLE: {
                auto triangle = std::dynamic_pointer_cast<Triangle>(selectedShape);
                if (triangle) {
                    initialValues["sideA"] = triangle->getSideA();
                    initialValues["sideB"] = triangle->getSideB();
                    initialValues["sideC"] = triangle->getSideC();
                }
                break;
            }
            default:
                break;
        }

        dialog.setInitialValues(initialValues);

        if (dialog.exec() == QDialog::Accepted) {
            auto newShape = dialog.getShape();
            if (newShape) {
                // 更新图形属性
                newShape->setId(selectedShape->getId());

                // 从管理器中替换
                for (auto& shape : shapeManager_.getShapes()) {
                    if (shape->getId() == selectedShape->getId()) {
                        shape = newShape;
                        break;
                    }
                }

                // 从画布中替换
                shapeCanvas_->removeShape(selectedShape->getId());
                shapeCanvas_->addShape(newShape);

                // 更新UI
                updateTableWidget();
                updateStatusBar();

                // 标记为未保存
                hasUnsavedChanges_ = true;

                // 更新工具栏和菜单状态
                updateToolBarState();
                updateMenuState();
            }
        }
    }
}

// 按面积排序
void MainWindow::sortByArea() {
    shapeManager_.sortByArea();
    updateTableWidget();
}

// 按周长排序
void MainWindow::sortByPerimeter() {
    shapeManager_.sortByPerimeter();
    updateTableWidget();
}

// 按ID排序
void MainWindow::sortById() {
    shapeManager_.sortById();
    updateTableWidget();
}

// 按类型排序
void MainWindow::sortByType() {
    // 获取所有图形
    auto shapes = shapeManager_.getShapes();

    // 定义类型顺序
    QMap<ShapeType, int> typeOrder;
    typeOrder[ShapeType::CIRCLE] = 0;
    typeOrder[ShapeType::RECTANGLE] = 1;
    typeOrder[ShapeType::TRIANGLE] = 2;

    // 排序
    std::sort(shapes.begin(), shapes.end(),
        [&typeOrder](const std::shared_ptr<Shape>& a, const std::shared_ptr<Shape>& b) {
            return typeOrder[a->getType()] < typeOrder[b->getType()];
        });

    // 更新管理器
    shapeManager_.clearShapes();
    for (const auto& shape : shapes) {
        shapeManager_.addShape(shape);
    }

    // 更新表格
    updateTableWidget();
}

// 显示所有图形
void MainWindow::showAllShapes() {
    shapeCanvas_->resetZoom();
    shapeCanvas_->centerView();
}

// 放大
void MainWindow::zoomIn() {
    shapeCanvas_->setZoomFactor(shapeCanvas_->getZoomFactor() * 1.2);
}

// 缩小
void MainWindow::zoomOut() {
    shapeCanvas_->setZoomFactor(shapeCanvas_->getZoomFactor() / 1.2);
}

// 重置缩放
void MainWindow::resetZoom() {
    shapeCanvas_->resetZoom();
}

// 显示帮助
void MainWindow::showHelp() {
    QString helpText = R"(
<h2>图形绘制与面积周长计算系统使用帮助</h2>

<h3>基本操作</h3>
<ul>
<li>添加图形：点击"添加图形"按钮，选择图形类型并设置参数</li>
<li>选择图形：点击画布上的图形或表格中的行来选择图形</li>
<li>编辑图形：选中图形后，在属性编辑器中修改属性</li>
<li>删除图形：选中图形后，点击"删除图形"按钮</li>
</ul>

<h3>视图操作</h3>
<ul>
<li>缩放：使用鼠标滚轮或工具栏按钮进行缩放</li>
<li>平移：按住鼠标左键拖动画布</li>
<li>居中：点击"显示所有图形"按钮居中显示所有图形</li>
</ul>

<h3>文件操作</h3>
<ul>
<li>新建项目：创建新的空白项目</li>
<li>打开项目：从文件加载已保存的项目</li>
<li>保存项目：将当前项目保存到文件</li>
<li>导出为CSV：将图形数据导出为CSV文件</li>
<li>从CSV导入：从CSV文件导入图形数据</li>
</ul>

<h3>排序功能</h3>
<ul>
<li>按面积排序：根据图形面积从小到大排序</li>
<li>按周长排序：根据图形周长从小到大排序</li>
<li>按ID排序：根据图形ID从小到大排序</li>
<li>按类型排序：根据图形类型排序</li>
</ul>
)";

    QMessageBox::information(this, "使用帮助", helpText);
}

// 显示关于
void MainWindow::showAbout() {
    QMessageBox::about(this, "关于", 
        R"(
<h2>图形绘制与面积周长计算系统</h2>
<p>版本: 1.0</p>
<p>一个使用C++和Qt实现的图形绘制与面积周长计算系统。</p>
<p>支持创建圆形、矩形和三角形，计算它们的面积和周长。</p>
<p>© 2023 图形计算系统</p>
)");
}
