
#define Q_WS_WIN
#include "ncsettings.h"
#include "ui_ncsettings.h"

#include <QSettings>
#include <QDir>
#include <QProcess>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QRect>
#include <windows.h>
#include <QWidget>
#include <QDialog>
#include <QColorDialog>
#include <QFileDialog>
#include <QTranslator>
#include <QApplication>


extern "C" BOOL WINAPI CheckTokenMembership(HANDLE, PSID, PBOOL);
extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;

QSettings* NcSettings::settings = NULL;
QString NcSettings::defaultStyle = "";
QTranslator* NcSettings::currentTranslator = NULL;

const struct NcSettings::SettingsVals NcSettings::settingsConsts_str[] = {
    {"TIME_UNTIL_SHOW",2000},
    {"DEFAULT_EXIST_HANDLE",NcSettings::HANDLE_REPLACE},
    {"CONFIRM_DELETE",true},
    {"APPEARANCE",NcSettings::WIN_TOOL},
    {"DELETE_DEST_CANCEL",true},
    {"ALWAYS_ON_TOP",true},
    {"CONFIRM_CANCEL",true},
    {"START_ON_CREATE",true},
    {"DELETE_TIME",5},
    {"POSITION",NcSettings::BOTTOM_RIGHT},
    {"SHOW_TASKS",3},
    {"MAX_RECENT_TASKS",5},
    {"DEFAULT_ICONS",true},
    {"USE_PLASTIQUE_STYLE",false},
    {"PB_COLOR",QColor(Qt::green)},
    {"CHECK_UPDATES",true},
    {"STYLE",""},
    {"TEST_CHECKSUM",false},
    {"LANGUAGE","english"},
    {"IGNORE_PERMISSIONS",false},
    {"CUSTOM",false}
};


NcSettings::NcSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ncsettings)
{
    ui->setupUi(this);
    show();
    this->adjustSize();
    this->resize(ui->tabWidget->size());

    move(screenCenter() - rect().bottomRight()/2);
    initUI();
}


void NcSettings::initUI(){
    if(!isRunningAsAdmin())
    {
        ui->cbadmin->setToolTip(tr("only available as administrator!"));
        ui->cbadmin->setDisabled(true);
    }

    int sys_ver = QSysInfo::windowsVersion();

    if(sys_ver < QSysInfo::WV_VISTA)
    {
        ui->cbadmin->setToolTip(tr("only available on vista and later!"));
        ui->cbadmin->setDisabled(true);
    }

    bool run_as_admin;
    ui->ck_cancel_confirm->setChecked(getValue<bool>(CONFIRM_CANCEL));
    ui->ck_delete_dest->setChecked(getValue<bool>(DELETE_DEST_CANCEL));
    ui->ck_nc_onTop->setChecked(getValue<bool>(ALWAYS_ON_TOP));
    ui->ck_start->setChecked(getValue<bool>(START_ON_CREATE));
    ui->ck_start_wsystem->setChecked(startWithSystem(run_as_admin));
    ui->cbadmin->setChecked(run_as_admin);
    ui->sb_sec_del->setValue(getValue<int>(DELETE_TIME));
    ui->sb_show_tasks->setValue(getValue<int>(SHOW_TASKS));
    ui->cbappearance->setCurrentIndex(getValue<int>(APPEARANCE));
    ui->cb_confirm_delete->setChecked(getValue<bool>(CONFIRM_DELETE));
    ui->timeUntilShow->setValue(getValue<int>(TIME_UNTIL_SHOW));
    ui->sb_max_recentTasks->setValue(getValue<int>(MAX_RECENT_TASKS));
    ui->defaultIcons->setChecked(getValue<bool>(DEFAULT_ICONS));
    ui->usePlastiqueStyle->setChecked(getValue<bool>(USE_PLASTIQUE_STYLE));
    ui->ckUpdates->setChecked(getValue<bool>(CHECK_UPDATES));
    ui->ckCheckSum->setChecked(getValue<bool>(TEST_CHECKSUM));
    ui->cb_default_handle->setCurrentIndex(getValue<int>(DEFAULT_EXIST_HANDLE));
    ui->checkPermissionsCheckBox->setChecked(getValue<bool>(CHECK_PERMISSIONS));
    fillStyleComboBox();
    fillLangugeComboBox();


    QPalette palette = ui->progressBar->palette();
    palette.setColor(QPalette::Highlight,getValue<QColor>(PB_COLOR));
    ui->progressBar->setPalette(palette);

    switch(getValue<int>(POSITION))
    {
    case TOP_RIGHT:
        ui->cbTopRight->setChecked(true);
        break;
    case TOP_LEFT:
        ui->cbTopLeft->setChecked(true);
        break;
    case BOTTOM_RIGHT:
        ui->cbBottomRight->setChecked(true);
        break;
    case BOTTOM_LEFT:
        ui->cbBottomLeft->setChecked(true);
        break;
    }
}


void NcSettings::fillLangugeComboBox()
{
    QDir dir("languages");
    QString currentLanguage = this->getValue<QString>(LANGUAGE);

    QStringList filters;
    filters << "*.qm";
    QStringList list = dir.entryList(filters,QDir::NoDotAndDotDot|QDir::Files,QDir::Name);

    foreach(QString languge,list){

        ui->langCombobox->addItem(languge);
        if( languge==currentLanguage ){

            ui->langCombobox->setCurrentIndex(ui->langCombobox->findText(languge));
        }
    }

}

void NcSettings::fillStyleComboBox()
{
    QDir dir("style");
    QString currentStyle = this->getValue<QString>(STYLE);

    QStringList list = dir.entryList(QDir::NoDotAndDotDot|QDir::AllEntries|QDir::Hidden,QDir::Name);

    foreach(QString style,list){

        if( QFile::exists("style/"+style+"/style.qcss") ){

            ui->styleComboBox->addItem(style);
            if( style==currentStyle ){
                ui->styleComboBox->setCurrentIndex(ui->styleComboBox->findText(style));
            }
        }
    }
}

bool NcSettings::getShutDownRights()
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    // Get a token for this process.

    if (!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return false;

    // Get the LUID for the shutdown privilege.

    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
            &tkp.Privileges[0].Luid);

    tkp.PrivilegeCount = 1;  // one privilege to set
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    // Get the shutdown privilege for this process.


    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,(PTOKEN_PRIVILEGES)NULL, 0);

    // Cannot test the return value of AdjustTokenPrivileges.

    if (GetLastError() != ERROR_SUCCESS)
        return false;
    return true;
}

bool NcSettings::isRunningAsAdmin()
{
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    PSID AdministratorsGroup;
    // Initialize SID.
    if( !AllocateAndInitializeSid( &NtAuthority,
                                   2,
                                   SECURITY_BUILTIN_DOMAIN_RID,
                                   DOMAIN_ALIAS_RID_ADMINS,
                                   0, 0, 0, 0, 0, 0,
                                   &AdministratorsGroup))
    {
            // Initializing SID Failed.
        return false;
    }
        // Check whether the token is present in admin group.
    BOOL IsInAdminGroup = false;
    if( !CheckTokenMembership( NULL,AdministratorsGroup, &IsInAdminGroup ))
    {
            // Error occurred.
        IsInAdminGroup = false;
    }
        // Free SID and return.
    FreeSid(AdministratorsGroup);
    return IsInAdminGroup;
}

QString NcSettings::getStylePath()
{
    QString style = getValue<QString>(STYLE);
    if( style=="None" || style.isEmpty() ){

        return "";
    }else{

        return "style/" + style + "/style.qcss";
    }
}

bool _shutdown;
void NcSettings::setShutDown(bool __shutdown)
{
    _shutdown = __shutdown;
}

bool NcSettings::isShutDown()
{
    return _shutdown;
}


void NcSettings::save()
{
    NcSettings::settings->remove("CUSTOM");
    NcSettings::settings->sync();
}


void NcSettings::saveSettings()
{
    setValue(CONFIRM_CANCEL,ui->ck_cancel_confirm->isChecked());
    setValue(ALWAYS_ON_TOP,ui->ck_nc_onTop->isChecked());
    setValue(DELETE_DEST_CANCEL,ui->ck_delete_dest->isChecked());
    setValue(DELETE_TIME,ui->sb_sec_del->value());
    setValue(SHOW_TASKS,ui->sb_show_tasks->value());
    setValue(START_ON_CREATE,ui->ck_start->isChecked());
    setValue(DEFAULT_EXIST_HANDLE,ui->cb_default_handle->currentIndex());
    setValue(TIME_UNTIL_SHOW,ui->timeUntilShow->value());
    setValue(MAX_RECENT_TASKS,ui->sb_max_recentTasks->value());
    setValue(DEFAULT_ICONS,ui->defaultIcons->isChecked());
    setValue(CHECK_UPDATES,ui->ckUpdates->isChecked());
    setValue(APPEARANCE,ui->cbappearance->currentIndex());
    setValue(CONFIRM_DELETE,ui->cb_confirm_delete->isChecked());
    setValue(STYLE,ui->styleComboBox->currentText());
    setValue(TEST_CHECKSUM,ui->ckCheckSum->isChecked());
    setValue(CHECK_PERMISSIONS,ui->checkPermissionsCheckBox->isChecked());

    //reset "run with system"  first
    bool asadmin;
    bool start = startWithSystem(asadmin);
    if( start != ui->ck_start_wsystem->isChecked() || asadmin != ui->cbadmin->isChecked())
    {
        setStartWithSystem(false);
        if(ui->ck_start_wsystem->isChecked() == true )
            setStartWithSystem(true);
    }

    if(ui->cbBottomLeft->isChecked())
        setValue(POSITION,BOTTOM_LEFT);
    if(ui->cbBottomRight->isChecked())
        setValue(POSITION,BOTTOM_RIGHT);
    if(ui->cbTopLeft->isChecked())
        setValue(POSITION,TOP_LEFT);
    if(ui->cbTopRight->isChecked())
        setValue(POSITION,TOP_RIGHT);

    if(ui->checkPermissionsCheckBox->isChecked()){
        qt_ntfs_permission_lookup=1;
    }else{
        qt_ntfs_permission_lookup=0;
    }
}


void NcSettings::unload()
{
    delete NcSettings::settings;
}

void NcSettings::load()
{
    NcSettings::settings = new QSettings("ncsettings.ini",QSettings::IniFormat);
    if(getValue<bool>(USE_PLASTIQUE_STYLE))
    {
        defaultStyle = QApplication::style()->objectName();
        qDebug("NcSettings: style: "+defaultStyle.toUtf8());
        QApplication::setStyle("fusion");
    }

    QString lang = getValue<QString>(LANGUAGE);
    if(lang!="english")
    {
        if(NcSettings::currentTranslator){
            QApplication::removeTranslator(NcSettings::currentTranslator);
        }

        QTranslator* translator = new QTranslator();
        translator->load("languages/"+lang+".qm");
        QApplication::installTranslator(translator);
        NcSettings::currentTranslator = translator;
    }

    if(getValue<bool>(CHECK_PERMISSIONS)){
        qt_ntfs_permission_lookup=1;
    }else{
        qt_ntfs_permission_lookup=0;
    }
}

bool exiting = false;
bool NcSettings::isExiting()
{
    return exiting;
}

void NcSettings::setIsExiting(bool exit)
{
    exiting = exit;
}

NcSettings::~NcSettings()
{
    delete ui;
}

void NcSettings::setStartWithSystem(bool start_with_system)
{
    int version;
    version = QSysInfo::windowsVersion();
    if( version >= QSysInfo::WV_VISTA && (ui->cbadmin->isChecked() || start_with_system==false) )
    {
        QProcess process;
        if(start_with_system == true)
        {
            /*Create the xml file for the task scheduler*/
            //the program is in the resource
            process.start("createtasksheduler.exe");
            if( !process.waitForStarted() )
            {
                QMessageBox::information(this,
                                         tr("NiceCopier message"),
                                         "Failed to start process createtasksheduler.exe:\n"+process.errorString());
                return;
            }

            if( !process.waitForFinished() )
            {
                QMessageBox::information(this,
                                         tr("NiceCopier message"),
                                         "Failed to wait for process createtasksheduler.exe:\n"+process.errorString());
                return;
            }

            //Load the xml file with schtasks

            process.start("schtasks /Create /TN NiceCopier /XML NcSchtsk.xml");
        }
        else
            process.start("schtasks /Delete /TN NiceCopier /F");
        if( !process.waitForStarted() )
        {
            QMessageBox::information(this,
                                     tr("NiceCopier message"),
                                     "Failed to start process schtasks:\n"+process.errorString());
            return;
        }

        if( !process.waitForFinished() )
        {
            QMessageBox::information(this,
                                     tr("NiceCopier message"),
                                     "Failed to wait for process schtasks:\n"+process.errorString());
            return;
        }
    }

    if(version < QSysInfo::WV_VISTA || start_with_system==false || !ui->cbadmin->isChecked())
    {
        QString path = QDir::currentPath()+"/"+"NiceCopier.exe";
        path = path = "\""+path.replace('/','\\') +"\"";
        QSettings registry("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run\\", QSettings::NativeFormat);
        if(start_with_system==true)
            registry.setValue("NiceCopier",path);
        else
            registry.remove("NiceCopier");
    }
}

bool NcSettings::startWithSystem(bool &run_as_admin)
{
    int version;
    run_as_admin = false;
    version = QSysInfo::windowsVersion();
    if( version >= QSysInfo::WV_VISTA && isRunningAsAdmin())
    {
        QProcess process;
        process.start("schtasks /query");
        if( !process.waitForStarted() )
        {
            QMessageBox::information(NULL,tr("NiceCopier message"),"Failed to start process:\n"+process.errorString());
            return false;
        }

        if( !process.waitForFinished() )
        {
            QMessageBox::information(NULL,tr("NiceCopier message"),"Failed to wait for process:\n"+process.errorString());
            return false;
        }
        QString content;

        content = process.readAll();

        if( content.contains("NiceCopier") )
        {
            run_as_admin = true;
            return true;
        }
        //else check registry
    }


    QSettings registry("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run\\", QSettings::NativeFormat);
    if( registry.contains("NiceCopier") )
    {
        return true;
    }else
        return false;

}

void NcSettings::setValue(NcSettings::SettingsConsts setting, QVariant value)
{
    QString str = settingsConsts_str[setting].name;
    NcSettings::settings->setValue(str,value);
}


void NcSettings::on_btCancel_clicked()
{
    close();
}

void NcSettings::on_btApply_clicked()
{
    saveSettings();
    save();
    NcSettings::settings_changed();
}

void NcSettings::on_btOk_clicked()
{
    saveSettings();
    NcSettings::settings_changed();
    save();
    close();
}

QPoint NcSettings::screenCenter()
{
    QPoint pos = QCursor::pos();
    int desktop = QApplication::desktop()->screenNumber(pos);
    QDesktopWidget *monitor = QApplication::desktop();
    pos = monitor->availableGeometry(desktop).center();
    return pos;
}

void NcSettings::on_progressBarColor_clicked()
{
    QColor color = QColorDialog::getColor(Qt::green,this);
    NcSettings::settings->setValue("PB_COLOR",color);
    QPalette palette = ui->progressBar->palette();
    palette.setColor(QPalette::Highlight,color);
    ui->progressBar->setPalette(palette);
    QApplication::setStyle("fusion");
}


void NcSettings::on_usePlastiqueStyle_clicked(bool checked)
{
    NcSettings::settings->setValue("USE_PLASTIQUE_STYLE",checked);
    if(checked)
    {
        defaultStyle = QApplication::style()->objectName();
        qDebug("NcSettings: style: "+defaultStyle.toUtf8());
        QApplication::setStyle("fusion");
    }else{
        QApplication::setStyle(defaultStyle);
    }
}


void NcSettings::on_langCombobox_activated(const QString &lang)
{
    if(NcSettings::currentTranslator){
        QApplication::removeTranslator(NcSettings::currentTranslator);
    }

    if(lang==tr("english")){
        NcSettings::currentTranslator = NULL;
        return;
    }

    QTranslator* translator = new QTranslator();
    translator->load("languages/"+lang+".ts");
    QApplication::installTranslator(translator);
    NcSettings::currentTranslator = translator;

    ui->retranslateUi(this);
    this->repaint();
    this->update();
    this->show();
    setValue(LANGUAGE,lang);
    fillStyleComboBox();
}
