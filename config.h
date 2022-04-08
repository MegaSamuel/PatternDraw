#ifndef TCONFIG_H
#define TCONFIG_H

#include <QObject>
#include <QString>

//------------------------------------------------------------------------------

class TConfig : QObject
{
    Q_OBJECT

public:
    TConfig();

    void          readSettings();
    void          writeSettings();

    void          cfgSetCurrentPath(const QString&  path) noexcept;
    QString       cfgGetCurrentPath() noexcept;

    void          cfgSetOpenPath(const QString&  path = "") noexcept;
    void          cfgSetSavePath(const QString&  path = "") noexcept;
    void          cfgSetConvPath(const QString&  path = "") noexcept;

    QString       cfgGetOpenPath() noexcept;
    QString       cfgGetSavePath() noexcept;
    QString       cfgGetConvPath() noexcept;

    void          cfgSetSerialNumber(const QString&  num = "") noexcept;
    QString       cfgGetSerialNumber() noexcept;

private:
    QString       m_cfg_current_path;    // папка откуда мы запустились

    QString       m_cfg_open_path;       // последняя папка из диалога чтения
    QString       m_cfg_save_path;       // последняя папка из диалога записи
    QString       m_cfg_conv_path;       // последняя папка из диалога записи конвертированного файла

    QString       m_cfg_serial_number;   // серийный номер
};

//------------------------------------------------------------------------------

#endif // TCONFIG_H
