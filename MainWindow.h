
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QVector>
#include <memory>
#include "Shape.h"
#include "ShapeManager.h"
#include "AddShapeDialog.h"
#include "ShapeCanvas.h"

class QToolBar;
class QStatusBar;
class QMenu;
class QMenuBar;
class QTableWidget;
class QLabel;
class QPushButton;

// 主窗口类
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    // 构造函数
    MainWindow(QWidget* parent = nullptr);

    // 析构函数
    ~MainWindow() override;

private slots:
    // 文件操作
    void newProject();
    void openProject();
    void saveProject();
    void saveProjectAs();
    void exportToCSV();
    void importFromCSV();
    void exitApplication();

    // 图形操作
    void addShape();
    void removeSelectedShape();
    void clearAllShapes();
    void editSelectedShape();

    // 排序操作
    void sortByArea();
    void sortByPerimeter();
    void sortById();
    void sortByType();

    // 视图操作
    void showAllShapes();
    void zoomIn();
    void zoomOut();
    void resetZoom();

    // 帮助
    void showAbout();
    void showHelp();

    // 更新状态栏信息
    void updateStatusBar();

    // 更新表格视图
    void updateTableWidget();

    // 表格行选择变化
    void tableRowSelectionChanged();

    // 图形选择变化
    void shapeSelectionChanged();

    // 添加图形完成
    void shapeAdded(const std::shared_ptr<Shape>& shape);

    // 编辑图形完成
    void shapeEdited(const std::shared_ptr<Shape>& shape);

private:
    // 初始化UI
    void initUI();

    // 创建菜单栏
    void createMenuBar();

    // 创建工具栏
    void createToolBar();

    // 创建状态栏
    void createStatusBar();

    // 创建中央部件
    void createCentralWidget();

    // 创建图形画布
    void createShapeCanvas();

    // 创建表格视图
    void createTableWidget();

    // 创建属性编辑器
    void createPropertyEditor();

    // 连接信号槽
    void connectSignalsSlots();

    // 更新工具栏按钮状态
    void updateToolBarState();

    // 更新菜单项状态
    void updateMenuState();

    // 获取当前选中的图形
    std::shared_ptr<Shape> getSelectedShape() const;

    // 获取选中的图形ID列表
    QVector<int> getSelectedShapeIds() const;

    // 高亮选中的图形
    void highlightSelectedShapes();

    // 保存项目文件
    bool saveProjectFile(const QString& filename);

    // 加载项目文件
    bool loadProjectFile(const QString& filename);

    // 设置当前项目文件路径
    void setCurrentProjectFile(const QString& path);

    // 获取当前项目文件路径
    QString getCurrentProjectFile() const;

    // 检查是否有未保存的更改
    bool hasUnsavedChanges() const;

    // 提示保存未保存的更改
    bool promptToSaveUnsavedChanges();

    // 恢复默认设置
    void restoreDefaults();

private:
    // UI组件
    QToolBar* toolBar_;
    QStatusBar* statusBar_;
    QMenuBar* menuBar_;

    // 主工作区
    QWidget* centralWidget_;
    QHBoxLayout* mainLayout_;

    // 图形画布
    ShapeCanvas* shapeCanvas_;

    // 表格视图
    QTableWidget* tableWidget_;

    // 属性编辑器
    QWidget* propertyEditor_;
    QFormLayout* propertyLayout_;
    QMap<QString, QWidget*> propertyWidgets_;

    // 图形管理器
    ShapeManager shapeManager_;

    // 当前项目文件路径
    QString currentProjectFile_;

    // 是否有未保存的更改
    bool hasUnsavedChanges_;
};

#endif // MAIN_WINDOW_H
