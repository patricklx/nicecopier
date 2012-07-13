#ifndef NCSETTINGS_H
#define NCSETTINGS_H

#include <QWidget>
#include <QDialog>
#include <QSettings>

namespace Ui {
    class ncsettings;
}

class NcSettings : public QDialog
{
        Q_OBJECT

    public:
        enum Positions{
            TOP_LEFT,
            TOP_RIGHT,
            BOTTOM_LEFT,
            BOTTOM_RIGHT
        };

        enum WinStyle{
            WIN_TOOL,
            WIN_DIALOG,
            WIN_WINDOW
        };

        enum DefaultHandles{
            HANDLE_REPLACE,
            HANDLE_RENAME,
            HANDLE_IGNORE
        };

        enum SettingsConsts{
            TIME_UNTIL_SHOW,
            DEFAULT_EXIST_HANDLE,
            CONFIRM_DELETE,
            APPEARANCE,
            DELETE_DEST_CANCEL,
            ALWAYS_ON_TOP,
            CONFIRM_CANCEL,
            START_ON_CREATE,
            DELETE_TIME,
            POSITION,
            SHOW_TASKS,
            MAX_RECENT_TASKS,
            DEFAULT_ICONS,
            USE_PLASTIQUE_STYLE,
            PB_COLOR,
            CHECK_UPDATES,
            STYLE,
            TEST_CHECKSUM
        };

        explicit NcSettings(QWidget *parent = 0);
        ~NcSettings();

        static void load();
        static void save();
        void saveSettings();



        bool startWithSystem(bool &run_as_admin);
        void setStartWithSystem(bool start_with_system);


        static void setIsExiting(bool exit);
        static bool isExiting();

        static bool isRunningAsAdmin();

        static void setShutDown(bool shutdown);
        static bool isShutDown();

        static bool getShutDownRights();

        static QPoint screenCenter();

        static void unload();

        template <class T>
        static T getValue(SettingsConsts setting)
        {
            QString str = settingsConsts_str[setting].name;
            QVariant defaultVal = settingsConsts_str[setting].value;
            return NcSettings::settings->value(str,defaultVal).value<T>();
        }

        static void setValue(SettingsConsts setting, QVariant value);

        bool fillStyleComboBox();
        static QString getStylePath();
    signals:
        void settings_changed();

    private:
        Ui::ncsettings *ui;
        static QSettings *settings;
        static QString defaultStyle;

        struct SettingsVals{
                QString name;
                QVariant value;
        };

        static const struct SettingsVals settingsConsts_str[];

    private slots:
        void on_btOk_clicked();
        void on_btApply_clicked();
        void on_btCancel_clicked();
        void on_progressBarColor_clicked();

        void on_usePlastiqueStyle_clicked(bool checked);
};

#endif // NCSETTINGS_H
