#include "mainwindow.h"
#include "database.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QStatusBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QDateTime>
#include <QApplication>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_stackedWidget(nullptr)
    , m_adminWindow(nullptr)
    , m_passengerWindow(nullptr)
    , m_clockTimer(nullptr)
    , m_clockLabel(nullptr)
    , m_statusLabel(nullptr)
{
    setupUI();
    createMenuBar();
    createStatusBar();
    setupConnections();
    loadSampleData();
    
    // Start with login screen
    showLogin();
}

MainWindow::~MainWindow()
{
    if (m_clockTimer) {
        m_clockTimer->stop();
    }
}

void MainWindow::setupUI()
{
    setWindowTitle("CloudWings - Airline Management System");
    setMinimumSize(1200, 800);
    resize(1400, 900);
    
    // Center the window
    QRect screenGeometry = QApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
    
    // Create stacked widget for different interfaces
    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget);
    
    // Create admin window
    m_adminWindow = new AdminWindow(this);
    m_stackedWidget->addWidget(m_adminWindow);
    
    // Create passenger window
    m_passengerWindow = new PassengerWindow("demo_passenger", this);
    m_stackedWidget->addWidget(m_passengerWindow);
}

void MainWindow::createMenuBar()
{
    QMenuBar* menuBar = this->menuBar();
    
    // File menu
    QMenu* fileMenu = menuBar->addMenu("&File");
    
    QAction* logoutAction = new QAction("&Logout", this);
    logoutAction->setShortcut(QKeySequence("Ctrl+L"));
    fileMenu->addAction(logoutAction);
    
    fileMenu->addSeparator();
    
    QAction* exitAction = new QAction("E&xit", this);
    exitAction->setShortcut(QKeySequence("Ctrl+Q"));
    fileMenu->addAction(exitAction);
    
    // Help menu
    QMenu* helpMenu = menuBar->addMenu("&Help");
    
    QAction* aboutAction = new QAction("&About", this);
    helpMenu->addAction(aboutAction);
    
    // Connect actions
    connect(logoutAction, &QAction::triggered, this, &MainWindow::logout);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    connect(aboutAction, &QAction::triggered, [this]() {
        QMessageBox::about(this, "About CloudWings",
                          "CloudWings Airline Management System\n\n"
                          "Version 1.0.0\n"
                          "A comprehensive airline management solution\n"
                          "built with Qt6 and C++.\n\n"
                          "© 2024 CloudWings Airlines");
    });
}

void MainWindow::createStatusBar()
{
    QStatusBar* statusBar = this->statusBar();
    
    // Create status label
    m_statusLabel = new QLabel("Ready");
    statusBar->addWidget(m_statusLabel);
    
    // Create clock label
    m_clockLabel = new QLabel();
    m_clockLabel->setMinimumWidth(200);
    statusBar->addPermanentWidget(m_clockLabel);
    
    // Setup clock timer
    m_clockTimer = new QTimer(this);
    connect(m_clockTimer, &QTimer::timeout, this, &MainWindow::updateClock);
    m_clockTimer->start(1000); // Update every second
    updateClock();
}

void MainWindow::setupConnections()
{
    // Remove all connections to m_loginWindow, as login is handled in main.cpp
}

void MainWindow::loadSampleData()
{
    Database& db = Database::getInstance();
    
    // Add sample flights if none exist
    if (db.getFlights().isEmpty()) {
        QDateTime now = QDateTime::currentDateTime();
        
        // Sample flights for today
        db.addFlight("CW101", "New York", "Los Angeles", 
                     now.addSecs(3600), now.addSecs(7200), 180, 299.99, "Boeing 737");
        db.addFlight("CW102", "Chicago", "Miami", 
                     now.addSecs(7200), now.addSecs(10800), 150, 199.99, "Airbus A320");
        db.addFlight("CW103", "Seattle", "Denver", 
                     now.addSecs(10800), now.addSecs(14400), 120, 149.99, "Boeing 737");
        
        // Sample flights for tomorrow
        QDateTime tomorrow = now.addDays(1);
        db.addFlight("CW201", "Boston", "San Francisco", 
                     tomorrow.addSecs(3600), tomorrow.addSecs(7200), 200, 399.99, "Boeing 777");
        db.addFlight("CW202", "Atlanta", "Las Vegas", 
                     tomorrow.addSecs(7200), tomorrow.addSecs(10800), 160, 249.99, "Airbus A321");
        
        // Sample flights for next week
        QDateTime nextWeek = now.addDays(7);
        db.addFlight("CW301", "Dallas", "Phoenix", 
                     nextWeek.addSecs(3600), nextWeek.addSecs(7200), 140, 179.99, "Boeing 737");
        db.addFlight("CW302", "Houston", "Orlando", 
                     nextWeek.addSecs(7200), nextWeek.addSecs(10800), 130, 159.99, "Airbus A320");
    }
    
    // Add sample passenger if none exist
    if (!db.userExists("demo")) {
        db.addUser("demo", "demo123", "demo@example.com", "Demo User", "passenger");
    }
}

void MainWindow::showLogin()
{
    // This function is no longer needed for the new login flow.
}

void MainWindow::showAdminInterface(const QString& username)
{
    m_currentUser = username;
    m_currentRole = "admin";
    setCentralWidget(m_adminWindow);
    if (menuBar()) menuBar()->show();
    if (statusBar()) statusBar()->show();
    m_adminWindow->loadDashboard();
    m_statusLabel->setText(QString("Logged in as Admin: %1").arg(username));
    setWindowTitle(QString("CloudWings - Admin Dashboard (%1)").arg(username));
}

void MainWindow::showPassengerInterface(const QString& username)
{
    m_currentUser = username;
    m_currentRole = "passenger";
    // Remove old passenger window if it exists
    if (m_passengerWindow) {
        m_stackedWidget->removeWidget(m_passengerWindow);
        delete m_passengerWindow;
        m_passengerWindow = nullptr;
    }
    // Create new passenger window with correct username
    m_passengerWindow = new PassengerWindow(username, this);
    setCentralWidget(m_passengerWindow);
    if (menuBar()) menuBar()->show();
    if (statusBar()) statusBar()->show();
    m_passengerWindow->loadDashboard();
    m_statusLabel->setText(QString("Logged in as Passenger: %1").arg(username));
    setWindowTitle(QString("CloudWings - Passenger Dashboard (%1)").arg(username));
}

void MainWindow::logout()
{
    m_currentUser.clear();
    m_currentRole.clear();

    // Create a new login window
    LoginWindow* login = new LoginWindow();
    QObject::connect(login, &LoginWindow::loginSuccessful, [login]() {
        MainWindow* w = new MainWindow();
        w->show();
        login->close();
        login->deleteLater();
    });
    login->show();

    // Close the current main window
    this->close();
}

void MainWindow::updateClock()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timeString = currentTime.toString("dd/MM/yyyy hh:mm:ss AP");
    m_clockLabel->setText(timeString);
}

void MainWindow::updateAdminDashboardStats()
{
    if (m_adminWindow) m_adminWindow->updateDashboardStats();
} 