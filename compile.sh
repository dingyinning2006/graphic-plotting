
#!/bin/bash

# 图形绘制与面积周长计算系统编译脚本

echo "开始编译图形绘制与面积周长计算系统..."

# 检查qmake是否安装
if ! command -v qmake &> /dev/null; then
    echo "错误: qmake未安装，请先安装Qt开发环境"
    exit 1
fi

# 生成Makefile
echo "生成Makefile..."
qmake GraphicPlotting.pro

# 编译项目
echo "编译项目..."
make

# 检查编译是否成功
if [ $? -eq 0 ]; then
    echo "编译成功！"
    echo "运行程序: ./GraphicPlotting"
else
    echo "编译失败！"
    exit 1
fi
