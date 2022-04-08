#include <QSettings>
#include <QDir>

#include "config.h"

//------------------------------------------------------------------------------

#define AppVendor   "Cepaleme"
#define AppName     "PatternDraw"

//------------------------------------------------------------------------------

TConfig::TConfig()
{

}

//------------------------------------------------------------------------------

void  TConfig::readSettings()
{
     QSettings  settings(AppVendor, AppName);

     cfgSetSerialNumber(settings.value("number", "" ).toString());

     cfgSetOpenPath(settings.value("open path", "./" ).toString());
     cfgSetSavePath(settings.value("save path", "./" ).toString());
     cfgSetConvPath(settings.value("conv path", "./" ).toString());
}

void  TConfig::writeSettings()
{
     QSettings  settings(AppVendor, AppName);

     settings.setValue("number", cfgGetSerialNumber());

     settings.setValue("open path", cfgGetOpenPath());
     settings.setValue("save path", cfgGetSavePath());
     settings.setValue("conv path", cfgGetConvPath());
}

//------------------------------------------------------------------------------

void  TConfig::cfgSetCurrentPath(const QString&  path) noexcept
{
    m_cfg_current_path = QDir::toNativeSeparators(path);
}

QString  TConfig::cfgGetCurrentPath() noexcept
{
    return m_cfg_current_path;
}

//------------------------------------------------------------------------------

void  TConfig::cfgSetOpenPath(const QString&  path) noexcept
{
    if(0 == path.length())
        m_cfg_open_path = cfgGetCurrentPath();
    else
        m_cfg_open_path = QDir::toNativeSeparators(path) ;
}

void  TConfig::cfgSetSavePath(const QString&  path) noexcept
{
    if(0 == path.length())
        m_cfg_save_path = cfgGetCurrentPath();
    else
        m_cfg_save_path = QDir::toNativeSeparators(path);
}

void  TConfig::cfgSetConvPath(const QString&  path) noexcept
{
    if(0 == path.length())
        m_cfg_conv_path = cfgGetCurrentPath();
    else
        m_cfg_conv_path = QDir::toNativeSeparators(path);
}

QString  TConfig::cfgGetOpenPath() noexcept
{
    return m_cfg_open_path;
}

QString  TConfig::cfgGetSavePath() noexcept
{
    return m_cfg_save_path;
}

QString  TConfig::cfgGetConvPath() noexcept
{
    return m_cfg_conv_path;
}

//------------------------------------------------------------------------------

void  TConfig::cfgSetSerialNumber(const QString&  num) noexcept {
    m_cfg_serial_number = num;
}

QString  TConfig::cfgGetSerialNumber() noexcept {
    return m_cfg_serial_number;
}

//------------------------------------------------------------------------------
