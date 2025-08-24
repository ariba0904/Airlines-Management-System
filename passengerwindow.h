#ifndef PASSENGERWINDOW_H
#define PASSENGERWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QTabWidget>
#include <QGroupBox>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDateEdit>
#include <QTextEdit>
#include <QMessageBox>
#include <QProgressBar>
#include <QMenuBar>
#include <QAction>
#include "database.h"

class PassengerWindow : public QWidget
{
    Q_OBJECT

public:
    PassengerWindow(const QString& username, QWidget *parent = nullptr);
    void loadDashboard();
    void loadUserData();
    void updateFlightTable(const QList<QMap<QString, QVariant>>& flights);
    void updateTotalPrice();

private slots:
    void onTabChanged(int index);
    
    // Dashboard slots
    void refreshDashboard();
    
    // Flight search slots
    void searchFlights();
    void refreshFlights();
    void bookFlight();
    
    // Booking management slots
    void refreshMyBookings();
    void cancelMyBooking();
    void viewBookingDetails();
    
    // Profile slots
    void updateProfile();
    void changePassword();
    void refreshProfile();
    void downloadBookingReceipt(const QMap<QString, QVariant>& booking);
    void logout();

private:
    void setupUI();
    void setupDashboard();
    void setupFlightSearch();
    void setupMyBookings();
    void setupProfile();
    void setupConnections();
    
    void updateDashboardStats();
    void updateMyBookingsTable();
    void updateProfileForm();
    
    bool validateBookingForm();
    bool validateProfileForm();
    bool validatePasswordForm();
    void clearBookingForm();
    
    // Member variables
    QString m_username;
    
    // Dashboard widgets
    QLabel* m_welcomeLabel;
    QLabel* m_upcomingFlightsLabel;
    QLabel* m_totalBookingsLabel;
    QLabel* m_totalSpentLabel;
    QProgressBar* m_loyaltyProgressBar;
    
    // Flight search widgets
    QTableWidget* m_flightTable;
    QLineEdit* m_originSearchEdit;
    QLineEdit* m_destinationSearchEdit;
    QDateEdit* m_dateSearchEdit;
    QPushButton* m_searchButton;
    QPushButton* m_clearSearchButton;
    
    // Booking widgets
    QSpinBox* m_seatCountSpinBox;
    QLabel* m_totalPriceLabel;
    QPushButton* m_confirmBookingButton;
    
    // My bookings widgets
    QTableWidget* m_myBookingsTable;
    QPushButton* m_cancelBookingButton;
    QPushButton* m_viewDetailsButton;
    
    // Profile widgets
    QLineEdit* m_profileUsernameEdit;
    QLineEdit* m_profileEmailEdit;
    QLineEdit* m_profileFullNameEdit;
    QPushButton* m_updateProfileButton;
    
    QLineEdit* m_currentPasswordEdit;
    QLineEdit* m_newPasswordEdit;
    QLineEdit* m_confirmPasswordEdit;
    QPushButton* m_changePasswordButton;
    
    QTabWidget* m_tabWidget;
    QString m_selectedFlightNumber;
    double m_selectedFlightPrice;
    
    QMenuBar* m_menuBar;
    QAction* m_logoutAction;

signals:
    void logoutRequested();
    void bookingChanged();
};

#endif // PASSENGERWINDOW_H 