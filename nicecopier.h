#ifndef NICECOPIER_H
#define NICECOPIER_H

#include <QMainWindow>
#include "mytaskbaricon.h"
#include "tasklisthandler.h"
#include "ncserver.h"
#include "copyinfopanel.h"
#include <QLibrary>
#include <QTimer>
#include "RecentTasks.h"
#include "updater.h"

namespace Ui {
	class NiceCopier;
}

class NiceCopier : public QWidget
{
		Q_OBJECT


	public:
		explicit NiceCopier(bool hideIcon = 0);
		~NiceCopier();


private:
		Ui::NiceCopier *ui;
		TaskListHandler *taskhandler;
		NCServer server;
		QList<CopyInfoPanel*> panels;

		MyTaskBarIcon *taskbar;
		Updater updater;


		void closeEvent(QCloseEvent *evt);
		void changeEvent(QEvent *e);

	public slots:
		void addTask(QIODevice &info, bool is_new = true);

	private slots:
		void removeTask( CopyInfoPanel *panel );
		void setFirstPanel(CopyInfoPanel *panel);

		void updateWindow();
		void taskIconClicked(QSystemTrayIcon::ActivationReason reason);
		void settingsChanged();

		void expandTask( CopyInfoPanel *panel );
		void contractTask( CopyInfoPanel *panel);

		void startTasks();
		void aboutToQuit();
};

#endif // NICECOPIER_H
