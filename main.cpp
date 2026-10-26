
#include <QApplication>
#include <QMainWindow>
#include <QSettings>
#include <QMessageBox>
#include "MainWindow.h"
#include "ShapeExceptions.h"

// 全局异常处理函数
void handleException(const std::exception& e) {
    QMessageBox::critical(nullptr, "错误", QString("发生错误: %1").arg(e.what()));
}

int main(int argc, char* argv[]) {
    // 创建QApplication对象
    QApplication app(argc, argv);

    // 设置应用程序信息
    QApplication::setApplicationName("图形绘制与面积周长计算系统");
    QApplication::setApplicationVersion("1.0");
    QApplication::setOrganizationName("图形计算");

    // 设置默认样式
    QApplication::setStyle("Fusion");

    try {
        // 创建主窗口
        MainWindow mainWindow;

        // 恢复窗口状态
        QSettings settings;
        if (!settings.value("geometry").isNull()) {
            mainWindow.restoreGeometry(settings.value("geometry").toByteArray());
        }
        if (!settings.value("windowState").isNull()) {
            mainWindow.restoreState(settings.value("windowState").toByteArray());
        }

        // 显示主窗口
        mainWindow.show();

        // 设置全局异常处理
        std::set_terminate([]() {
            try {
                std::rethrow_exception(std::current_exception());
            } catch (const ShapeException& e) {
                handleException(e);
            } catch (const std::exception& e) {
                handleException(e);
            } catch (...) {
                handleException(std::runtime_error("未知异常"));
            }
            std::abort();
        });

        // 执行应用程序
        return app.exec();
    } catch (const std::exception& e) {
        QMessageBox::critical(nullptr, "启动错误", 
            QString("应用程序启动失败: %1").arg(e.what()));
        return 1;
    }
}
