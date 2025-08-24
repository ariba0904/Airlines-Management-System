#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QGroupBox>
#include <QTabWidget>
#include <QMessageBox>
#include <QValidator>
#include <QMouseEvent>
#include <QPoint>
#include "database.h"

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);

signals:
    void adminLoginSuccessful(const QString& username);
    void passengerLoginSuccessful(const QString& username);
    void loginSuccessful();

protected:

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onShowPasswordToggled(bool checked);
    void onShowRegisterPasswordToggled(bool checked);
    void onShowConfirmPasswordToggled(bool checked);
    void switchToLogin();
    void switchToRegister();

private:
    void setupUI();
    void setupLoginTab();
    void setupRegisterTab();
    void setupConnections();
    bool validateLoginForm();
    bool validateRegisterForm();
    void clearForms();
    void showError(const QString& message);

    // Login tab widgets
    QLineEdit* m_usernameEdit;
    QLineEdit* m_passwordEdit;
    QPushButton* m_loginButton;
    QPushButton* m_switchToRegisterButton;
    QCheckBox* m_showPasswordCheckBox;
    
    // Register tab widgets
    QLineEdit* m_regUsernameEdit;
    QLineEdit* m_regEmailEdit;
    QLineEdit* m_regFullNameEdit;
    QLineEdit* m_regPasswordEdit;
    QLineEdit* m_regConfirmPasswordEdit;
    QPushButton* m_registerButton;
    QPushButton* m_switchToLoginButton;
    QCheckBox* m_showRegPasswordCheckBox;
    QCheckBox* m_showConfirmPasswordCheckBox;
    
    QTabWidget* m_tabWidget;
};

#endif // LOGINWINDOW_H 