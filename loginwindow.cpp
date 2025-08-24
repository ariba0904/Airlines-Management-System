#include "loginwindow.h"
#include <QApplication>
#include <QScreen>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QTimer>
#include <QSizePolicy>
#include <QScrollArea>
#include <QDebug>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
    , m_usernameEdit(nullptr)
    , m_passwordEdit(nullptr)
    , m_loginButton(nullptr)
    , m_switchToRegisterButton(nullptr)
    , m_showPasswordCheckBox(nullptr)
    , m_regUsernameEdit(nullptr)
    , m_regEmailEdit(nullptr)
    , m_regFullNameEdit(nullptr)
    , m_regPasswordEdit(nullptr)
    , m_regConfirmPasswordEdit(nullptr)
    , m_registerButton(nullptr)
    , m_switchToLoginButton(nullptr)
    , m_showRegPasswordCheckBox(nullptr)
    , m_showConfirmPasswordCheckBox(nullptr)
    , m_tabWidget(nullptr)
{
    // Set gradient background on the LoginWindow itself
    this->setStyleSheet(
        "LoginWindow, QWidget {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "       stop:0 #667eea, stop:0.5 #764ba2, stop:1 #f093fb);"
        "}"
    );
    setupUI();
    setupConnections();
}

void LoginWindow::setupUI()
{
    // Main layout: center the card using stretches
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    QVBoxLayout* vCenter = new QVBoxLayout();
    vCenter->addStretch(1);
    QHBoxLayout* hCenter = new QHBoxLayout();
    hCenter->addStretch(1);

    // Card container (centered, not movable)
    QWidget* cardContainer = new QWidget(this);
    cardContainer->setFixedSize(450, 700);
    cardContainer->setStyleSheet(
        "QWidget {"
        "   background: rgba(255, 255, 255, 0.98);"
        "   border-radius: 20px;"
        "   border: 1px solid rgba(255, 255, 255, 0.3);"
        "}"
    );
    QVBoxLayout* cardLayout = new QVBoxLayout(cardContainer);
    // cardLayout->setSpacing(18);
    // cardLayout->setContentsMargins(32, 32, 32, 32);

    // // Close button inside the card, top-right
    // QHBoxLayout* closeRow = new QHBoxLayout();
    // closeRow->addStretch();
    // QPushButton* closeButton = new QPushButton("×", cardContainer);
    // closeButton->setFixedSize(32, 32);
    // closeButton->setStyleSheet(
    //     "QPushButton {"
    //     "   background: rgba(255, 255, 255, 0.9);"
    //     "   border: none;"
    //     "   border-radius: 16px;"
    //     "   color: #7f8c8d;"
    //     "   font-size: 20px;"
    //     "   font-weight: bold;"
    //     "   margin: 0;"
    //     "}"
    //     "QPushButton:hover {"
    //     "   background: rgba(231, 76, 60, 0.9);"
    //     "   color: white;"
    //     "}"
    // );
    // closeRow->addWidget(closeButton);
    // cardLayout->addLayout(closeRow);

    // Logo and title section (centered)
    QWidget* titleWidget = new QWidget(cardContainer);
    QVBoxLayout* titleLayout = new QVBoxLayout(titleWidget);
    titleLayout->setSpacing(10);
    titleLayout->setContentsMargins(0, 0, 0, 0);
    QLabel* logoLabel = new QLabel("✈", titleWidget);
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setStyleSheet(
        "font-size: 48px;"
        "color: #667eea;"
        "margin-bottom: 10px;"
    );
    QLabel* titleLabel = new QLabel("CloudWings", titleWidget);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "font-size: 32px;"
        "font-weight: bold;"
        "color: #2c3e50;"
        "margin-bottom: 5px;"
    );
    QLabel* subtitleLabel = new QLabel("Airline Management System", titleWidget);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet(
        "font-size: 14px;"
        "color: #7f8c8d;"
        "font-weight: 300;"
    );
    titleLayout->addWidget(logoLabel);
    titleLayout->addWidget(titleLabel);
    titleLayout->addWidget(subtitleLabel);
    cardLayout->addWidget(titleWidget, 0, Qt::AlignTop);

    // Tab widget (centered)
    m_tabWidget = new QTabWidget(cardContainer);
    m_tabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_tabWidget->setStyleSheet(
        "QTabWidget::pane {"
        "   border: none;"
        "   background: transparent;"
        "}"
        "QTabBar::tab {"
        "   background: transparent;"
        "   color: #7f8c8d;"
        "   padding: 15px 30px;"
        "   margin-right: 5px;"
        "   border-radius: 25px;"
        "   font-size: 16px;"
        "   font-weight: 500;"
        "   border: 2px solid transparent;"
        "}"
        "QTabBar::tab:selected {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #667eea, stop:1 #764ba2);"
        "   color: white;"
        "   border: 2px solid #667eea;"
        "}"
        "QTabBar::tab:hover:!selected {"
        "   background: rgba(102, 126, 234, 0.1);"
        "   color: #667eea;"
        "   border: 2px solid rgba(102, 126, 234, 0.3);"
        "}"
    );
    setupLoginTab();
    setupRegisterTab();
    cardLayout->addWidget(m_tabWidget, 1, Qt::AlignVCenter);
    // cardLayout->addStretch(1);

    // Card shadow
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setBlurRadius(24);
    shadowEffect->setColor(QColor(0, 0, 0, 80));
    shadowEffect->setOffset(0, 10);
    cardContainer->setGraphicsEffect(shadowEffect);

    hCenter->addWidget(cardContainer, 0, Qt::AlignCenter);
    hCenter->addStretch(1);
    vCenter->addLayout(hCenter);
    vCenter->addStretch(1);
    mainLayout->addLayout(vCenter);
}

void LoginWindow::setupLoginTab()
{
    QWidget* loginWidget = new QWidget(m_tabWidget);
    QVBoxLayout* loginLayout = new QVBoxLayout(loginWidget);
    // loginLayout->setSpacing(18);
    // loginLayout->setContentsMargins(32, 32, 32, 32);

    QLabel* usernameLabel = new QLabel("Username", loginWidget);
    usernameLabel->setStyleSheet(
        "font-weight: 600;"
        "color: #2c3e50;"
        "font-size: 15px;"
        "margin-bottom: 2px;"
    );
    m_usernameEdit = new QLineEdit(loginWidget);
    m_usernameEdit->setPlaceholderText("Enter your username");
    m_usernameEdit->setFixedHeight(44);
    m_usernameEdit->setMinimumWidth(180);
    m_usernameEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_usernameEdit->setStyleSheet(
        "QLineEdit {"
        "   padding: 0 8px;"
        "   border: 2px solid #e1e8ed;"
        "   border-radius: 8px;"
        "   font-size: 15px;"
        "   background: white;"
        "   color: #2c3e50;"
        "}"
        "QLineEdit:focus {"
        "   border-color: #667eea;"
        "   background: #f8f9ff;"
        "}"
        "QLineEdit::placeholder {"
        "   color: #bdc3c7;"
        "}"
    );

    QLabel* passwordLabel = new QLabel("Password", loginWidget);
    passwordLabel->setStyleSheet(
        "font-weight: 600;"
        "color: #2c3e50;"
        "font-size: 15px;"
        "margin-bottom: 2px;"
    );
    m_passwordEdit = new QLineEdit(loginWidget);
    m_passwordEdit->setPlaceholderText("Enter your password");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setFixedHeight(44);
    m_passwordEdit->setMinimumWidth(180);
    m_passwordEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_passwordEdit->setStyleSheet(
        "QLineEdit {"
        "   padding: 0 8px;"
        "   border: 2px solid #e1e8ed;"
        "   border-radius: 8px;"
        "   font-size: 15px;"
        "   background: white;"
        "   color: #2c3e50;"
        "}"
        "QLineEdit:focus {"
        "   border-color: #667eea;"
        "   background: #f8f9ff;"
        "}"
        "QLineEdit::placeholder {"
        "   color: #bdc3c7;"
        "}"
    );

    m_showPasswordCheckBox = new QCheckBox("Show Password", loginWidget);
    m_showPasswordCheckBox->setStyleSheet(
        "QCheckBox {"
        "   color: #7f8c8d;"
        "   font-size: 12px;"
        "   spacing: 8px;"
        "   margin: 10px 0;"
        "}"
        "QCheckBox::indicator {"
        "   width: 14px;"
        "   height: 14px;"
        "   border: 2px solid #bdc3c7;"
        "   border-radius: 4px;"
        "   background: white;"
        "}"
        "QCheckBox::indicator:checked {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #667eea, stop:1 #764ba2);"
        "   border-color: #667eea;"
        "}"
    );

    m_loginButton = new QPushButton("Sign In", loginWidget);
    m_loginButton->setMinimumHeight(40);
    m_loginButton->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #667eea, stop:1 #764ba2);"
        "   color: white;"
        "   padding: 10px;"
        "   border: none;"
        "   border-radius: 8px;"
        "   font-size: 16px;"
        "   font-weight: 600;"
        "   margin-top: 10px;"
        "   min-height: 40px;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #5a6fd8, stop:1 #6a4190);"
        "}"
        "QPushButton:pressed {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #4e5fc6, stop:1 #5e377e);"
        "}"
    );

    m_switchToRegisterButton = new QPushButton("Don't have an account? Create one", loginWidget);
    m_switchToRegisterButton->setStyleSheet(
        "QPushButton {"
        "   background: transparent;"
        "   border: none;"
        "   color: #667eea;"
        "   font-size: 14px;"
        "   text-decoration: underline;"
        "   margin-top: 18px;"
        "   padding: 10px;"
        "}"
        "QPushButton:hover {"
        "   color: #5a6fd8;"
        "}"
    );
    m_switchToRegisterButton->setCursor(Qt::PointingHandCursor);

    loginLayout->addWidget(usernameLabel);
    loginLayout->addWidget(m_usernameEdit);
    loginLayout->addWidget(passwordLabel);
    loginLayout->addWidget(m_passwordEdit);
    loginLayout->addWidget(m_showPasswordCheckBox);
    loginLayout->addWidget(m_loginButton);
    loginLayout->addStretch();
    loginLayout->addWidget(m_switchToRegisterButton, 0, Qt::AlignHCenter);

    loginWidget->setMinimumHeight(300);
    loginWidget->setStyleSheet("QWidget { background: rgba(255, 255, 255, 0.1); }");
    m_tabWidget->addTab(loginWidget, "Sign In");
}

void LoginWindow::setupRegisterTab()
{
    QWidget* registerWidget = new QWidget(m_tabWidget);
    QVBoxLayout* registerLayout = new QVBoxLayout(registerWidget);
    // registerLayout->setSpacing(18);
    // registerLayout->setContentsMargins(32, 32, 32, 32);

    // Username and Email side by side
    QHBoxLayout* row1 = new QHBoxLayout();
    QVBoxLayout* userCol = new QVBoxLayout();
    QLabel* regUsernameLabel = new QLabel("Username", registerWidget);
    regUsernameLabel->setStyleSheet(
        "font-weight: 500;"
        "color: #2c3e50;"
        "font-size: 12px;"
        "margin-bottom: 2px;"
    );
    m_regUsernameEdit = new QLineEdit(registerWidget);
    m_regUsernameEdit->setPlaceholderText("Choose a username");
    m_regUsernameEdit->setFixedHeight(30);
    // m_regUsernameEdit->setMinimumWidth(180);
    m_regUsernameEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_regUsernameEdit->setStyleSheet(
        "QLineEdit {"
        "   padding: 0 8px;"
        "   border: 2px solid #e1e8ed;"
        "   border-radius: 8px;"
        "   font-size: 15px;"
        "   background: white;"
        "   color: #2c3e50;"
        "}"
        "QLineEdit:focus {"
        "   border-color: #667eea;"
        "   background: #f8f9ff;"
        "}"
        "QLineEdit::placeholder {"
        "   color: #bdc3c7;"
        "}"
    );
    userCol->addWidget(regUsernameLabel);
    userCol->addWidget(m_regUsernameEdit);

    QVBoxLayout* emailCol = new QVBoxLayout();
    QLabel* regEmailLabel = new QLabel("Email", registerWidget);
    regEmailLabel->setStyleSheet(
        "font-weight: 500;"
        "color: #2c3e50;"
        "font-size: 12px;"
        "margin-bottom: 2px;"
    );
    m_regEmailEdit = new QLineEdit(registerWidget);
    m_regEmailEdit->setPlaceholderText("Enter your email address");
    m_regEmailEdit->setFixedHeight(30);
    // m_regEmailEdit->setMinimumWidth(180);
    m_regEmailEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_regEmailEdit->setStyleSheet(
        "QLineEdit {"
        "   padding: 0 8px;"
        "   border: 2px solid #e1e8ed;"
        "   border-radius: 8px;"
        "   font-size: 15px;"
        "   background: white;"
        "   color: #2c3e50;"
        "}"
        "QLineEdit:focus {"
        "   border-color: #667eea;"
        "   background: #f8f9ff;"
        "}"
        "QLineEdit::placeholder {"
        "   color: #bdc3c7;"
        "}"
    );
    emailCol->addWidget(regEmailLabel);
    emailCol->addWidget(m_regEmailEdit);

    row1->addLayout(userCol, 1);
    row1->addSpacing(12);
    row1->addLayout(emailCol, 1);
    registerLayout->addLayout(row1);

    // Full Name
    QLabel* regFullNameLabel = new QLabel("Full Name", registerWidget);
    regFullNameLabel->setStyleSheet(
        "font-weight: 500;"
        "color: #2c3e50;"
        "font-size: 12px;"
        "margin-bottom: 2px;"
    );
    m_regFullNameEdit = new QLineEdit(registerWidget);
    m_regFullNameEdit->setPlaceholderText("Enter your full name");
    m_regFullNameEdit->setFixedHeight(20);
    // m_regFullNameEdit->setMinimumWidth(180);
    m_regFullNameEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_regFullNameEdit->setStyleSheet(
        "QLineEdit {"
        "   padding: 0 8px;"
        "   border: 2px solid #e1e8ed;"
        "   border-radius: 8px;"
        "   font-size: 15px;"
        "   background: white;"
        "   color: #2c3e50;"
        "}"
        "QLineEdit:focus {"
        "   border-color: #667eea;"
        "   background: #f8f9ff;"
        "}"
        "QLineEdit::placeholder {"
        "   color: #bdc3c7;"
        "}"
    );
    registerLayout->addWidget(regFullNameLabel);
    registerLayout->addWidget(m_regFullNameEdit);

    // Password
    QLabel* regPasswordLabel = new QLabel("Password", registerWidget);
    regPasswordLabel->setStyleSheet(
        "font-weight: 500;"
        "color: #2c3e50;"
        "font-size: 12px;"
        "margin-bottom: 2px;"
    );
    m_regPasswordEdit = new QLineEdit(registerWidget);
    m_regPasswordEdit->setPlaceholderText("Choose a password");
    m_regPasswordEdit->setEchoMode(QLineEdit::Password);
    m_regPasswordEdit->setFixedHeight(30);
    // m_regPasswordEdit->setMinimumWidth(180);
    m_regPasswordEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_regPasswordEdit->setStyleSheet(
        "QLineEdit {"
        "   padding: 0 8px;"
        "   border: 2px solid #e1e8ed;"
        "   border-radius: 8px;"
        "   font-size: 15px;"
        "   background: white;"
        "   color: #2c3e50;"
        "}"
        "QLineEdit:focus {"
        "   border-color: #667eea;"
        "   background: #f8f9ff;"
        "}"
        "QLineEdit::placeholder {"
        "   color: #bdc3c7;"
        "}"
    );
    registerLayout->addWidget(regPasswordLabel);
    registerLayout->addWidget(m_regPasswordEdit);

    // Confirm Password
    QLabel* regConfirmPasswordLabel = new QLabel("Confirm Password", registerWidget);
    regConfirmPasswordLabel->setStyleSheet(
        "font-weight: 500;"
        "color: #2c3e50;"
        "font-size: 12px;"
        "margin-bottom: 2px;"
    );
    m_regConfirmPasswordEdit = new QLineEdit(registerWidget);
    m_regConfirmPasswordEdit->setPlaceholderText("Confirm your password");
    m_regConfirmPasswordEdit->setEchoMode(QLineEdit::Password);
    m_regConfirmPasswordEdit->setFixedHeight(30);
    // m_regConfirmPasswordEdit->setMinimumWidth(180);
    m_regConfirmPasswordEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_regConfirmPasswordEdit->setStyleSheet(
        "QLineEdit {"
        "   padding: 0 8px;"
        "   border: 2px solid #e1e8ed;"
        "   border-radius: 8px;"
        "   font-size: 15px;"
        "   background: white;"
        "   color: #2c3e50;"
        "}"
        "QLineEdit:focus {"
        "   border-color: #667eea;"
        "   background: #f8f9ff;"
        "}"
        "QLineEdit::placeholder {"
        "   color: #bdc3c7;"
        "}"
    );
    registerLayout->addWidget(regConfirmPasswordLabel);
    registerLayout->addWidget(m_regConfirmPasswordEdit);

    QHBoxLayout* passwordCheckboxesLayout = new QHBoxLayout();
    m_showRegPasswordCheckBox = new QCheckBox("Show Password", registerWidget);
    m_showConfirmPasswordCheckBox = new QCheckBox("Show Confirm", registerWidget);
    m_showRegPasswordCheckBox->setStyleSheet("color: #7f8c8d; font-size: 13px; margin-right: 12px;");
    m_showConfirmPasswordCheckBox->setStyleSheet("color: #7f8c8d; font-size: 13px;");
    passwordCheckboxesLayout->addWidget(m_showRegPasswordCheckBox);
    passwordCheckboxesLayout->addWidget(m_showConfirmPasswordCheckBox);
    passwordCheckboxesLayout->addStretch();

    m_registerButton = new QPushButton("Register", registerWidget);
    m_registerButton->setMinimumHeight(40);
    m_registerButton->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #667eea, stop:1 #764ba2);"
        "   color: white;"
        "   padding: 10px;"
        "   border: none;"
        "   border-radius: 8px;"
        "   font-size: 16px;"
        "   font-weight: 600;"
        "   margin-top: 10px;"
        "   min-height: 40px;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #5a6fd8, stop:1 #6a4190);"
        "}"
        "QPushButton:pressed {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "       stop:0 #4e5fc6, stop:1 #5e377e);"
        "}"
    );

    m_switchToLoginButton = new QPushButton("Already have an account? Sign in", registerWidget);
    m_switchToLoginButton->setStyleSheet(
        "QPushButton {"
        "   background: transparent;"
        "   border: none;"
        "   color: #667eea;"
        "   font-size: 14px;"
        "   text-decoration: underline;"
        "   margin-top: 18px;"
        "   padding: 10px;"
        "}"
        "QPushButton:hover {"
        "   color: #5a6fd8;"
        "}"
    );
    m_switchToLoginButton->setCursor(Qt::PointingHandCursor);

    registerLayout->addLayout(passwordCheckboxesLayout);
    registerLayout->addWidget(m_registerButton);
    registerLayout->addStretch();
    registerLayout->addWidget(m_switchToLoginButton, 0, Qt::AlignHCenter);

    registerWidget->setMinimumHeight(300);
    registerWidget->setStyleSheet("QWidget { background: rgba(255, 255, 255, 0.1); }");
    m_tabWidget->addTab(registerWidget, "Register");
}

void LoginWindow::setupConnections()
{
    // Login tab connections
    connect(m_loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(m_switchToRegisterButton, &QPushButton::clicked, this, &LoginWindow::switchToRegister);
    connect(m_showPasswordCheckBox, &QCheckBox::toggled, this, &LoginWindow::onShowPasswordToggled);
    connect(m_usernameEdit, &QLineEdit::returnPressed, this, &LoginWindow::onLoginClicked);
    connect(m_passwordEdit, &QLineEdit::returnPressed, this, &LoginWindow::onLoginClicked);
    
    // Register tab connections
    connect(m_registerButton, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);
    connect(m_switchToLoginButton, &QPushButton::clicked, this, &LoginWindow::switchToLogin);
    connect(m_showRegPasswordCheckBox, &QCheckBox::toggled, this, &LoginWindow::onShowRegisterPasswordToggled);
    connect(m_showConfirmPasswordCheckBox, &QCheckBox::toggled, this, &LoginWindow::onShowConfirmPasswordToggled);
    
    // Close button connection
    QList<QPushButton*> closeButtons = findChildren<QPushButton*>();
    for (QPushButton* button : closeButtons) {
        if (button->text() == "×") {
            connect(button, &QPushButton::clicked, this, &QWidget::close);
            break;
        }
    }
}

void LoginWindow::onLoginClicked()
{
    if (!validateLoginForm()) {
        return;
    }
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text();
    QString role;
    Database& db = Database::getInstance();
    if (db.authenticateUser(username, password, role)) {
        qDebug() << "Login as" << username << "role:" << role;
        if (role == "admin") {
            emit adminLoginSuccessful(username);
        } else if (role == "passenger") {
            emit passengerLoginSuccessful(username);
        }
        clearForms();
        emit loginSuccessful();
        close();
    } else {
        showError("Invalid username or password. Please try again.");
    }
}

void LoginWindow::onRegisterClicked()
{
    if (!validateRegisterForm()) {
        return;
    }
    
    QString username = m_regUsernameEdit->text().trimmed();
    QString email = m_regEmailEdit->text().trimmed();
    QString fullName = m_regFullNameEdit->text().trimmed();
    QString password = m_regPasswordEdit->text();
    
    Database& db = Database::getInstance();
    if (db.addUser(username, password, email, fullName, "passenger")) {
        QMessageBox::information(this, "Registration Successful", 
                                "Account created successfully! You can now login.");
        switchToLogin();
        clearForms();
    } else {
        showError("Registration failed. Username or email might already exist.");
    }
}

void LoginWindow::onShowPasswordToggled(bool checked)
{
    m_passwordEdit->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
}

void LoginWindow::onShowRegisterPasswordToggled(bool checked)
{
    m_regPasswordEdit->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
}

void LoginWindow::onShowConfirmPasswordToggled(bool checked)
{
    m_regConfirmPasswordEdit->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
}

void LoginWindow::switchToLogin()
{
    m_tabWidget->setCurrentIndex(0);
}

void LoginWindow::switchToRegister()
{
    m_tabWidget->setCurrentIndex(1);
}

bool LoginWindow::validateLoginForm()
{
    if (m_usernameEdit->text().trimmed().isEmpty()) {
        showError("Please enter a username.");
        m_usernameEdit->setFocus();
        return false;
    }
    
    if (m_passwordEdit->text().isEmpty()) {
        showError("Please enter a password.");
        m_passwordEdit->setFocus();
        return false;
    }
    
    return true;
}

bool LoginWindow::validateRegisterForm()
{
    if (m_regUsernameEdit->text().trimmed().isEmpty()) {
        showError("Please enter a username.");
        m_regUsernameEdit->setFocus();
        return false;
    }
    
    if (m_regUsernameEdit->text().trimmed().length() < 3) {
        showError("Username must be at least 3 characters long.");
        m_regUsernameEdit->setFocus();
        return false;
    }
    
    if (m_regEmailEdit->text().trimmed().isEmpty()) {
        showError("Please enter an email address.");
        m_regEmailEdit->setFocus();
        return false;
    }
    
    // Basic email validation
    QRegularExpression emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    if (!emailRegex.match(m_regEmailEdit->text().trimmed()).hasMatch()) {
        showError("Please enter a valid email address.");
        m_regEmailEdit->setFocus();
        return false;
    }
    
    if (m_regFullNameEdit->text().trimmed().isEmpty()) {
        showError("Please enter your full name.");
        m_regFullNameEdit->setFocus();
        return false;
    }
    
    if (m_regPasswordEdit->text().isEmpty()) {
        showError("Please enter a password.");
        m_regPasswordEdit->setFocus();
        return false;
    }
    
    if (m_regPasswordEdit->text().length() < 6) {
        showError("Password must be at least 6 characters long.");
        m_regPasswordEdit->setFocus();
        return false;
    }
    
    if (m_regConfirmPasswordEdit->text().isEmpty()) {
        showError("Please confirm your password.");
        m_regConfirmPasswordEdit->setFocus();
        return false;
    }
    
    if (m_regPasswordEdit->text() != m_regConfirmPasswordEdit->text()) {
        showError("Passwords do not match.");
        m_regConfirmPasswordEdit->setFocus();
        return false;
    }
    
    return true;
}

void LoginWindow::clearForms()
{
    // Clear login form
    m_usernameEdit->clear();
    m_passwordEdit->clear();
    m_showPasswordCheckBox->setChecked(false);
    
    // Clear register form
    m_regUsernameEdit->clear();
    m_regEmailEdit->clear();
    m_regFullNameEdit->clear();
    m_regPasswordEdit->clear();
    m_regConfirmPasswordEdit->clear();
    m_showRegPasswordCheckBox->setChecked(false);
    m_showConfirmPasswordCheckBox->setChecked(false);
}

void LoginWindow::showError(const QString& message)
{
    QMessageBox::warning(this, "Error", message);
} 
