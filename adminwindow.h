#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

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
#include <QDoubleSpinBox>
#include <QDateTimeEdit>
#include <QTextEdit>
#include <QMessageBox>
#include <QDateEdit>
#include <QProgressBar>
#include "database.h"

class AdminWindow : public QWidget
{
    Q_OBJECT

public:
    AdminWindow(QWidget *parent = nullptr);
    void loadDashboard();
    void updateDashboardStats();

private slots:
    void onTabChanged(int index);
    
    // Dashboard slots
    void refreshDashboard();
    
    // Flight management slots
    void addFlight();
    void editFlight();
    void deleteFlight();
    void refreshFlights();
    void searchFlights();
    
    // User management slots
    void refreshUsers();
    void deleteUser();
    void searchUsers();
    
    // Booking management slots
    void refreshBookings();
    void searchBookings();
    void cancelBooking();
    
    // Reports slots
    void generateRevenueReport();
    void generateFlightReport();
    void exportReport();

private:
    void setupUI();
    void setupDashboard();
    void setupFlightManagement();
    void setupUserManagement();
    void setupBookingManagement();
    void setupReports();
    void setupConnections();
    
    // void updateDashboardStats();
    void updateFlightTable();
    void updateUserTable();
    void updateBookingTable();
    
    bool validateFlightForm();
    void clearFlightForm();
    void populateFlightForm(const QMap<QString, QVariant>& flight);
    
    // Dashboard widgets
    QLabel* m_totalFlightsLabel;
    QLabel* m_totalBookingsLabel;
    QLabel* m_totalRevenueLabel;
    QLabel* m_totalUsersLabel;
    QProgressBar* m_occupancyProgressBar;
    QTextEdit* m_revenueSummaryText;
    
    // Flight management widgets
    QTableWidget* m_flightTable;
    QLineEdit* m_flightNumberEdit;
    QLineEdit* m_originEdit;
    QLineEdit* m_destinationEdit;
    QDateTimeEdit* m_departureTimeEdit;
    QDateTimeEdit* m_arrivalTimeEdit;
    QSpinBox* m_totalSeatsSpinBox;
    QDoubleSpinBox* m_priceSpinBox;
    QLineEdit* m_aircraftModelEdit;
    QPushButton* m_addFlightButton;
    QPushButton* m_editFlightButton;
    QPushButton* m_deleteFlightButton;
    QPushButton* m_clearFlightButton;
    QLineEdit* m_flightSearchEdit;
    
    // User management widgets
    QTableWidget* m_userTable;
    QLineEdit* m_userSearchEdit;
    QPushButton* m_deleteUserButton;
    
    // Booking management widgets
    QTableWidget* m_bookingTable;
    QLineEdit* m_bookingSearchEdit;
    QPushButton* m_cancelBookingButton;
    
    // Reports widgets
    QDateEdit* m_reportStartDate;
    QDateEdit* m_reportEndDate;
    QPushButton* m_generateRevenueReportButton;
    QPushButton* m_generateFlightReportButton;
    QTextEdit* m_reportTextEdit;
    QPushButton* m_exportReportButton;
    
    QTabWidget* m_tabWidget;
    QString m_currentFlightNumber;
    QString m_selectedUsername;
    QString m_selectedBookingId;
};

#endif // ADMINWINDOW_H 
