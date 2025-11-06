#include "material_sound_speed_manager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

// 构造函数：初始化并加载配置
MaterialSoundSpeedManager::MaterialSoundSpeedManager(const QString& configPath)
    : m_configPath(configPath)
    , m_currentSoundSpeed(-1) // 初始化为无效值
{
    // 加载配置文件，若失败则初始化默认数据
    if (!loadConfig()) {
        qWarning() << "配置文件加载失败，使用默认材料数据";
        // 初始化默认材料（根据需求设置）
        m_materialMap.insert("AL2219", 3150.0);
        m_materialMap.insert("AL7050", 3093.0);
        m_materialMap.insert("AL6061", 3199.0);
        // 设置默认当前材料
        m_currentMaterial = "AL7050";
        m_currentSoundSpeed = m_materialMap["AL7050"];
        // 保存默认数据到文件
        saveConfig();
    }
}

// 1. 获取当前材料及声速
QPair<QString, double> MaterialSoundSpeedManager::getCurrentMaterialAndSpeed()
{
    return QPair<QString, double>(m_currentMaterial, m_currentSoundSpeed);
}

// 2. 根据材料名称获取声速
double MaterialSoundSpeedManager::getSpeedByMaterial(const QString& materialName)
{
    if (m_materialMap.contains(materialName)) {
        return m_materialMap[materialName];
    } else {
        qWarning() << "材料" << materialName << "不存在";
        return -1; // 返回-1表示材料不存在
    }
}

// 3. 根据材料名称设置声速（存在则更新，不存在则新增）
bool MaterialSoundSpeedManager::setSpeedByMaterial(const QString& materialName, double speed)
{
    if (speed <= 0) {
        qWarning() << "声速必须为正数（当前值：" << speed << "）";
        return false;
    }
    // 更新或新增材料声速
    m_materialMap[materialName] = speed;
    // 若设置的是当前材料，同步更新当前声速
    if (materialName == m_currentMaterial) {
        m_currentSoundSpeed = speed;
    }
    // 保存到文件
    return saveConfig();
}

// 额外：设置当前材料（需确保材料已存在）
bool MaterialSoundSpeedManager::setCurrentMaterial(const QString& materialName)
{
    if (m_materialMap.contains(materialName)) {
        m_currentMaterial = materialName;
        m_currentSoundSpeed = m_materialMap[materialName];
        return saveConfig(); // 保存当前材料设置
    } else {
        qWarning() << "无法设置当前材料：" << materialName << "不存在";
        return false;
    }
}

// 从JSON文件加载配置
bool MaterialSoundSpeedManager::loadConfig()
{
    QFile file(m_configPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "无法打开配置文件：" << file.errorString();
        return false;
    }

    // 解析JSON
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        qWarning() << "JSON格式错误，无法解析配置文件";
        file.close();
        return false;
    }

    QJsonObject root = doc.object();
    QJsonObject setting = root["material_sound_speed_setting"].toObject();

    // 加载材料列表
    QJsonArray materials = setting["materials_list"].toArray();
    for (const auto& item : materials) {
        QJsonObject mat = item.toObject();
        QString name = mat["material_name"].toString();
        double speed = mat["sound_speed_m_per_s"].toDouble();
        if (!name.isEmpty() && speed > 0) {
            m_materialMap.insert(name, speed);
        }
    }

    // 加载当前材料及声速
    m_currentMaterial = setting["current_material"].toString();
    m_currentSoundSpeed = setting["current_sound_speed_m_per_s"].toDouble();

    // 校验当前材料是否存在于列表中
    if (!m_materialMap.contains(m_currentMaterial) || m_currentSoundSpeed <= 0) {
        qWarning() << "当前材料配置无效，自动重置为默认";
        m_currentMaterial = "AL7050";
        m_currentSoundSpeed = m_materialMap.value("AL7050", 3093.0);
    }

    file.close();
    return true;
}

// 将配置保存到JSON文件
bool MaterialSoundSpeedManager::saveConfig()
{
    QFile file(m_configPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "无法写入配置文件：" << file.errorString();
        return false;
    }

    // 构建JSON结构
    QJsonObject root;
    QJsonObject setting;

    // 保存材料列表
    QJsonArray materials;
    for (auto it = m_materialMap.begin(); it != m_materialMap.end(); ++it) {
        QJsonObject mat;
        mat["material_name"] = it.key();
        mat["sound_speed_m_per_s"] = it.value();
        materials.append(mat);
    }
    setting["materials_list"] = materials;

    // 保存当前材料及声速
    setting["current_material"] = m_currentMaterial;
    setting["current_sound_speed_m_per_s"] = m_currentSoundSpeed;

    root["material_sound_speed_setting"] = setting;

    // 写入文件
    QJsonDocument doc(root);
    file.write(doc.toJson(QJsonDocument::Indented)); // 格式化输出，便于人工编辑
    file.close();
    return true;
}