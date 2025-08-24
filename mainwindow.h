#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QTimer>
#include <QLabel>
#include "loginwindow.h"
#include "adminwindow.h"
#include "passengerwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateAdminDashboardStats();

private slots:
    void showLogin();
    void showAdminInterface(const QString& username);
    void showPassengerInterface(const QString& username);
    void logout();
    void updateClock();

private:
    void setupUI();
    void createMenuBar();
    void createStatusBar();
    void setupConnections();
    void loadSampleData();

    QStackedWidget* m_stackedWidget;
    AdminWindow* m_adminWindow;
    PassengerWindow* m_passengerWindow;
    
    QTimer* m_clockTimer;
    QLabel* m_clockLabel;
    QLabel* m_statusLabel;
    
    QString m_currentUser;
    QString m_currentRole;
};

#endif // MAINWINDOW_H 