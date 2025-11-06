#ifndef MATERIAL_SOUND_SPEED_MANAGER_H
#define MATERIAL_SOUND_SPEED_MANAGER_H

#include <QString>
#include <QMap>
#include <QPair> // 用于返回"材料-声速"对

// 管理材料声速的类（依赖Qt的JSON模块和文件操作）
class MaterialSoundSpeedManager
{
public:
    // 构造函数：传入JSON配置文件路径（默认使用"material_sound_speed.json"）
    explicit MaterialSoundSpeedManager(const QString& configPath = "/home/pi/qt/display/material_sound_speed.json");

    // 1. 获取当前材料及声速（返回pair：<材料名称, 声速>，声速为-1表示获取失败）
    QPair<QString, double> getCurrentMaterialAndSpeed();

    // 2. 根据材料名称获取声速（返回声速，-1表示材料不存在）
    double getSpeedByMaterial(const QString& materialName);

    // 3. 根据材料名称设置声速（若材料不存在则新增，成功返回true，失败返回false）
    bool setSpeedByMaterial(const QString& materialName, double speed);

    // （额外）设置当前材料（需确保材料已存在，成功返回true）
    bool setCurrentMaterial(const QString& materialName);

    QStringList getAllMaterialNames() const {
        return m_materialMap.keys(); // 返回QMap中所有key（材料名称）
    }

private:
    // 从JSON文件加载配置
    bool loadConfig();

    // 将当前配置保存到JSON文件
    bool saveConfig();

private:
    QString m_configPath;               // JSON配置文件路径
    QString m_currentMaterial;          // 当前材料名称
    double m_currentSoundSpeed;         // 当前材料的声速（米/秒）
    QMap<QString, double> m_materialMap;// 存储所有材料的"名称-声速"映射
};

#endif // MATERIAL_SOUND_SPEED_MANAGER_H