#include <QApplication>
#include <QStyleFactory>
#include <QDir>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include "mainwindow.h"
#include "database.h"
#include "loginwindow.h"
#include "passengerwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("CloudWings");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("CloudWings Airlines");
    
    // Set modern style
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // Load and apply stylesheet
    QFile styleFile(":/styles.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream styleStream(&styleFile);
        app.setStyleSheet(styleStream.readAll() + "\nQTableWidget, QTableView { background: #fff; }\nQTableWidget::item, QTableView::item { background: #fff; }\n");
        styleFile.close();
    }
    
    // Create application data directory
    QDir appDataDir = QDir::home();
    appDataDir.mkpath(".cloudwings");
    appDataDir.cd(".cloudwings");
    
    // Initialize database
    Database& db = Database::getInstance();
    if (!db.initialize(appDataDir.absolutePath() + "/cloudwings.db")) {
        QMessageBox::critical(nullptr, "Database Error", 
                            "Failed to initialize database. Application cannot start.");
        return -1;
    }
    
    LoginWindow login;
    MainWindow w;
    QObject::connect(&login, &LoginWindow::adminLoginSuccessful, [&](const QString& username) {
        w.show();
        login.close();
    });
    QObject::connect(&login, &LoginWindow::passengerLoginSuccessful, [&](const QString& username) {
        PassengerWindow* p = new PassengerWindow(username);
        QObject::connect(p, &PassengerWindow::logoutRequested, [&]() {
            login.show();
        });
        QObject::connect(p, &PassengerWindow::bookingChanged, [&]() {
            w.updateAdminDashboardStats();
        });
        p->show();
        login.close();
    });
    login.show();
    
    return app.exec();
} 