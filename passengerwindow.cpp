#include "passengerwindow.h"
#include "database.h"
#include "utils/helpers.h"
#include "utils/validators.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QDateTime>
#include <QApplication>
#include <QInputDialog>
#include <QFileDialog>
#include <QTextDocument>
#include <QtPrintSupport/QPrinter>
#include <QDebug>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QKeySequence>

PassengerWindow::PassengerWindow(const QString& username, QWidget *parent)
    : QWidget(parent)
    , m_username(username)
    , m_welcomeLabel(nullptr)
    , m_upcomingFlightsLabel(nullptr)
    , m_totalBookingsLabel(nullptr)
    , m_totalSpentLabel(nullptr)
    , m_loyaltyProgressBar(nullptr)
    , m_flightTable(nullptr)
    , m_originSearchEdit(nullptr)
    , m_destinationSearchEdit(nullptr)
    , m_dateSearchEdit(nullptr)
    , m_searchButton(nullptr)
    , m_clearSearchButton(nullptr)
    , m_seatCountSpinBox(nullptr)
    , m_totalPriceLabel(nullptr)
    , m_confirmBookingButton(nullptr)
    , m_myBookingsTable(nullptr)
    , m_cancelBookingButton(nullptr)
    , m_viewDetailsButton(nullptr)
    , m_profileUsernameEdit(nullptr)
    , m_profileEmailEdit(nullptr)
    , m_profileFullNameEdit(nullptr)
    , m_updateProfileButton(nullptr)
    , m_currentPasswordEdit(nullptr)
    , m_newPasswordEdit(nullptr)
    , m_confirmPasswordEdit(nullptr)
    , m_changePasswordButton(nullptr)
    , m_tabWidget(nullptr)
    , m_selectedFlightNumber("")
    , m_selectedFlightPrice(0.0)
    , m_menuBar(nullptr)
    , m_logoutAction(nullptr)
{
    setupUI();
    setupConnections();
    loadUserData();
}

void PassengerWindow::loadDashboard()
{
    updateDashboardStats();
}

void PassengerWindow::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // Menu bar
    m_menuBar = new QMenuBar(this);
    QMenu* fileMenu = m_menuBar->addMenu("&File");
    m_logoutAction = new QAction("&Logout", this);
    m_logoutAction->setShortcut(QKeySequence("Ctrl+L"));
    fileMenu->addAction(m_logoutAction);
    mainLayout->setMenuBar(m_menuBar);
    
    // Title
    QLabel* titleLabel = new QLabel("Passenger Dashboard");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #2c3e50; margin-bottom: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Tab widget
    m_tabWidget = new QTabWidget();
    mainLayout->addWidget(m_tabWidget);
    
    setupDashboard();
    setupFlightSearch();
    setupMyBookings();
    setupProfile();
}

void PassengerWindow::setupDashboard()
{
    QWidget* dashboardWidget = new QWidget();
    QVBoxLayout* dashboardLayout = new QVBoxLayout(dashboardWidget);
    
    // Welcome section
    QGroupBox* welcomeGroup = new QGroupBox("Welcome");
    QVBoxLayout* welcomeLayout = new QVBoxLayout(welcomeGroup);
    m_welcomeLabel = new QLabel(QString("Welcome, %1!").arg(m_username));
    m_welcomeLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #3498db;");
    welcomeLayout->addWidget(m_welcomeLabel);
    dashboardLayout->addWidget(welcomeGroup);
    
    // Stats grid
    QGridLayout* statsGrid = new QGridLayout();
    
    // Upcoming flights
    QGroupBox* upcomingGroup = new QGroupBox("Upcoming Flights");
    QVBoxLayout* upcomingLayout = new QVBoxLayout(upcomingGroup);
    m_upcomingFlightsLabel = new QLabel("0");
    m_upcomingFlightsLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #e74c3c;");
    upcomingLayout->addWidget(m_upcomingFlightsLabel);
    statsGrid->addWidget(upcomingGroup, 0, 0);
    
    // Total bookings
    QGroupBox* bookingsGroup = new QGroupBox("Total Bookings");
    QVBoxLayout* bookingsLayout = new QVBoxLayout(bookingsGroup);
    m_totalBookingsLabel = new QLabel("0");
    m_totalBookingsLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #27ae60;");
    bookingsLayout->addWidget(m_totalBookingsLabel);
    statsGrid->addWidget(bookingsGroup, 0, 1);
    
    // Total spent
    QGroupBox* spentGroup = new QGroupBox("Total Spent");
    QVBoxLayout* spentLayout = new QVBoxLayout(spentGroup);
    m_totalSpentLabel = new QLabel("$0.00");
    m_totalSpentLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #f39c12;");
    spentLayout->addWidget(m_totalSpentLabel);
    statsGrid->addWidget(spentGroup, 0, 2);
    
    dashboardLayout->addLayout(statsGrid);
    
    // Loyalty progress
    QGroupBox* loyaltyGroup = new QGroupBox("Loyalty Progress");
    QVBoxLayout* loyaltyLayout = new QVBoxLayout(loyaltyGroup);
    m_loyaltyProgressBar = new QProgressBar();
    m_loyaltyProgressBar->setRange(0, 100);
    m_loyaltyProgressBar->setValue(0);
    m_loyaltyProgressBar->setFormat("Loyalty Level: %p%");
    loyaltyLayout->addWidget(m_loyaltyProgressBar);
    dashboardLayout->addWidget(loyaltyGroup);
    
    // Refresh button
    QPushButton* refreshButton = new QPushButton("Refresh Dashboard");
    refreshButton->setObjectName("refreshDashboardButton");
    refreshButton->setProperty("class", "success");
    dashboardLayout->addWidget(refreshButton);
    
    dashboardLayout->addStretch();
    
    m_tabWidget->addTab(dashboardWidget, "Dashboard");
}

void PassengerWindow::setupFlightSearch()
{
    QWidget* searchWidget = new QWidget();
    QVBoxLayout* searchLayout = new QVBoxLayout(searchWidget);
    
    // Search form
    QGroupBox* searchGroup = new QGroupBox("Search Flights");
    QGridLayout* searchFormLayout = new QGridLayout(searchGroup);
    
    searchFormLayout->addWidget(new QLabel("Origin:"), 0, 0);
    m_originSearchEdit = new QLineEdit();
    m_originSearchEdit->setPlaceholderText("e.g., New York");
    searchFormLayout->addWidget(m_originSearchEdit, 0, 1);
    
    searchFormLayout->addWidget(new QLabel("Destination:"), 0, 2);
    m_destinationSearchEdit = new QLineEdit();
    m_destinationSearchEdit->setPlaceholderText("e.g., Los Angeles");
    searchFormLayout->addWidget(m_destinationSearchEdit, 0, 3);
    
    searchFormLayout->addWidget(new QLabel("Date:"), 1, 0);
    m_dateSearchEdit = new QDateEdit();
    m_dateSearchEdit->setDate(QDate::currentDate());
    m_dateSearchEdit->setCalendarPopup(true);
    searchFormLayout->addWidget(m_dateSearchEdit, 1, 1);
    
    // Search buttons
    QHBoxLayout* searchButtonLayout = new QHBoxLayout();
    m_searchButton = new QPushButton("Search Flights");
    m_searchButton->setProperty("class", "success");
    m_clearSearchButton = new QPushButton("Clear Search");
    m_clearSearchButton->setProperty("class", "secondary");
    searchButtonLayout->addWidget(m_searchButton);
    searchButtonLayout->addWidget(m_clearSearchButton);
    searchButtonLayout->addStretch();
    searchFormLayout->addLayout(searchButtonLayout, 1, 2, 1, 2);
    
    searchLayout->addWidget(searchGroup);
    
    // Flight table
    m_flightTable = new QTableWidget();
    m_flightTable->setColumnCount(7);
    m_flightTable->setHorizontalHeaderLabels({
        "Flight #", "Origin", "Destination", "Departure", "Arrival", "Available Seats", "Price"
    });
    m_flightTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_flightTable->setAlternatingRowColors(false);
    m_flightTable->horizontalHeader()->setStretchLastSection(true);
    
    searchLayout->addWidget(m_flightTable);
    
    // Booking form
    QGroupBox* bookingGroup = new QGroupBox("Book Flight");
    QHBoxLayout* bookingLayout = new QHBoxLayout(bookingGroup);
    
    bookingLayout->addWidget(new QLabel("Number of Seats:"));
    m_seatCountSpinBox = new QSpinBox();
    m_seatCountSpinBox->setRange(1, 10);
    m_seatCountSpinBox->setValue(1);
    bookingLayout->addWidget(m_seatCountSpinBox);
    
    bookingLayout->addWidget(new QLabel("Total Price:"));
    m_totalPriceLabel = new QLabel("$0.00");
    m_totalPriceLabel->setStyleSheet("font-weight: bold; color: #e74c3c;");
    bookingLayout->addWidget(m_totalPriceLabel);
    
    m_confirmBookingButton = new QPushButton("Confirm Booking");
    m_confirmBookingButton->setProperty("class", "success");
    m_confirmBookingButton->setEnabled(false);
    bookingLayout->addWidget(m_confirmBookingButton);
    
    bookingLayout->addStretch();
    
    searchLayout->addWidget(bookingGroup);
    
    m_tabWidget->addTab(searchWidget, "Search & Book");
}

void PassengerWindow::setupMyBookings()
{
    QWidget* bookingsWidget = new QWidget();
    QVBoxLayout* bookingsLayout = new QVBoxLayout(bookingsWidget);
    
    // Booking actions
    QHBoxLayout* actionLayout = new QHBoxLayout();
    m_cancelBookingButton = new QPushButton("Cancel Booking");
    m_cancelBookingButton->setProperty("class", "danger");
    m_viewDetailsButton = new QPushButton("View Details");
    m_viewDetailsButton->setProperty("class", "info");
    QPushButton* refreshButton = new QPushButton("Refresh");
    refreshButton->setObjectName("bookingsRefreshButton");
    
    actionLayout->addWidget(m_cancelBookingButton);
    actionLayout->addWidget(m_viewDetailsButton);
    actionLayout->addWidget(refreshButton);
    actionLayout->addStretch();
    
    bookingsLayout->addLayout(actionLayout);
    
    // Bookings table
    m_myBookingsTable = new QTableWidget();
    m_myBookingsTable->setColumnCount(7);
    m_myBookingsTable->setHorizontalHeaderLabels({
        "Booking ID", "Flight", "Seats", "Total Price", "Date", "Status", "Receipt"
    });
    m_myBookingsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_myBookingsTable->setAlternatingRowColors(false);
    m_myBookingsTable->horizontalHeader()->setStretchLastSection(true);
    m_myBookingsTable->setColumnWidth(6, 150);
    
    bookingsLayout->addWidget(m_myBookingsTable);
    
    m_tabWidget->addTab(bookingsWidget, "My Bookings");
}

void PassengerWindow::setupProfile()
{
    QWidget* profileWidget = new QWidget();
    QVBoxLayout* profileLayout = new QVBoxLayout(profileWidget);
    
    // Profile information
    QGroupBox* profileGroup = new QGroupBox("Profile Information");
    QGridLayout* profileFormLayout = new QGridLayout(profileGroup);
    
    profileFormLayout->addWidget(new QLabel("Username:"), 0, 0);
    m_profileUsernameEdit = new QLineEdit();
    m_profileUsernameEdit->setReadOnly(true);
    profileFormLayout->addWidget(m_profileUsernameEdit, 0, 1);
    
    profileFormLayout->addWidget(new QLabel("Email:"), 1, 0);
    m_profileEmailEdit = new QLineEdit();
    profileFormLayout->addWidget(m_profileEmailEdit, 1, 1);
    
    profileFormLayout->addWidget(new QLabel("Full Name:"), 2, 0);
    m_profileFullNameEdit = new QLineEdit();
    profileFormLayout->addWidget(m_profileFullNameEdit, 2, 1);
    
    m_updateProfileButton = new QPushButton("Update Profile");
    m_updateProfileButton->setProperty("class", "success");
    profileFormLayout->addWidget(m_updateProfileButton, 3, 0, 1, 2);
    
    profileLayout->addWidget(profileGroup);
    
    // Change password
    QGroupBox* passwordGroup = new QGroupBox("Change Password");
    QGridLayout* passwordFormLayout = new QGridLayout(passwordGroup);
    
    passwordFormLayout->addWidget(new QLabel("Current Password:"), 0, 0);
    m_currentPasswordEdit = new QLineEdit();
    m_currentPasswordEdit->setEchoMode(QLineEdit::Password);
    passwordFormLayout->addWidget(m_currentPasswordEdit, 0, 1);
    
    passwordFormLayout->addWidget(new QLabel("New Password:"), 1, 0);
    m_newPasswordEdit = new QLineEdit();
    m_newPasswordEdit->setEchoMode(QLineEdit::Password);
    passwordFormLayout->addWidget(m_newPasswordEdit, 1, 1);
    
    passwordFormLayout->addWidget(new QLabel("Confirm Password:"), 2, 0);
    m_confirmPasswordEdit = new QLineEdit();
    m_confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    passwordFormLayout->addWidget(m_confirmPasswordEdit, 2, 1);
    
    m_changePasswordButton = new QPushButton("Change Password");
    m_changePasswordButton->setProperty("class", "warning");
    passwordFormLayout->addWidget(m_changePasswordButton, 3, 0, 1, 2);
    
    profileLayout->addWidget(passwordGroup);
    
    profileLayout->addStretch();
    
    m_tabWidget->addTab(profileWidget, "Profile");
}

void PassengerWindow::setupConnections()
{
    // Tab change
    connect(m_tabWidget, &QTabWidget::currentChanged, this, &PassengerWindow::onTabChanged);
    
    // Dashboard
    QPushButton* dashboardRefreshBtn = findChild<QPushButton*>("refreshDashboardButton");
    if (!dashboardRefreshBtn) qDebug() << "[CONNECT] Dashboard refresh button is nullptr!";
    else connect(dashboardRefreshBtn, &QPushButton::clicked, this, &PassengerWindow::refreshDashboard);
    
    // Flight search
    connect(m_searchButton, &QPushButton::clicked, this, &PassengerWindow::searchFlights);
    connect(m_clearSearchButton, &QPushButton::clicked, this, &PassengerWindow::refreshFlights);
    connect(m_flightTable, &QTableWidget::itemSelectionChanged, [this]() {
        QList<QTableWidgetItem*> selectedItems = m_flightTable->selectedItems();
        if (!selectedItems.isEmpty()) {
            int row = selectedItems.first()->row();
            m_selectedFlightNumber = m_flightTable->item(row, 0)->text();
            m_selectedFlightPrice = m_flightTable->item(row, 6)->text().remove("$").toDouble();
            m_confirmBookingButton->setEnabled(true);
            updateTotalPrice();
        } else {
            m_confirmBookingButton->setEnabled(false);
        }
    });
    connect(m_seatCountSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &PassengerWindow::updateTotalPrice);
    connect(m_confirmBookingButton, &QPushButton::clicked, this, &PassengerWindow::bookFlight);
    
    // My bookings
    connect(m_cancelBookingButton, &QPushButton::clicked, this, &PassengerWindow::cancelMyBooking);
    connect(m_viewDetailsButton, &QPushButton::clicked, this, &PassengerWindow::viewBookingDetails);
    QPushButton* bookingsRefreshBtn = findChild<QPushButton*>("bookingsRefreshButton");
    if (!bookingsRefreshBtn) qDebug() << "[CONNECT] Bookings refresh button is nullptr!";
    else connect(bookingsRefreshBtn, &QPushButton::clicked, this, &PassengerWindow::refreshMyBookings);
    
    // Profile
    connect(m_updateProfileButton, &QPushButton::clicked, this, &PassengerWindow::updateProfile);
    connect(m_changePasswordButton, &QPushButton::clicked, this, &PassengerWindow::changePassword);
    
    // Logout
    connect(m_logoutAction, &QAction::triggered, this, &PassengerWindow::logout);
}

void PassengerWindow::onTabChanged(int index)
{
    switch (index) {
        case 0: // Dashboard
            refreshDashboard();
            break;
        case 1: // Flight Search
            refreshFlights();
            break;
        case 2: // My Bookings
            refreshMyBookings();
            break;
        case 3: // Profile
            refreshProfile();
            break;
    }
}

void PassengerWindow::refreshDashboard()
{
    updateDashboardStats();
}

void PassengerWindow::updateDashboardStats()
{
    if (m_username.isEmpty()) {
        return;
    }
    
    Database& db = Database::getInstance();
    QList<QMap<QString, QVariant>> bookings = db.getUserBookings(m_username);
    
    // Count upcoming flights (only confirmed)
    int upcomingFlights = 0;
    int totalBookings = 0;
    double totalSpent = 0.0;
    QDateTime now = QDateTime::currentDateTime();
    
    for (const auto& booking : bookings) {
        QString status = booking["status"].toString().toUpper();
        if (status != "CONFIRMED") {
            continue;
        }
        QDateTime departureTime = booking["departure_time"].toDateTime();
        if (departureTime > now) {
            upcomingFlights++;
        }
        totalBookings++;
        totalSpent += booking["total_price"].toDouble();
    }
    
    m_welcomeLabel->setText(QString("Welcome, %1!").arg(m_username));
    m_upcomingFlightsLabel->setText(QString::number(upcomingFlights));
    m_totalBookingsLabel->setText(QString::number(totalBookings));
    m_totalSpentLabel->setText(Helpers::formatPrice(totalSpent));
    
    // Calculate loyalty level (simple calculation)
    int loyaltyLevel = qMin(100, static_cast<int>(totalSpent / 1000.0 * 10));
    m_loyaltyProgressBar->setValue(loyaltyLevel);
}

void PassengerWindow::searchFlights()
{
    QString origin = m_originSearchEdit->text().trimmed();
    QString destination = m_destinationSearchEdit->text().trimmed();
    QDate date = m_dateSearchEdit->date();
    
    Database& db = Database::getInstance();
    QList<QMap<QString, QVariant>> flights = db.getFlights(origin, destination, date);
    
    updateFlightTable(flights);
    if (flights.isEmpty()) {
        QMessageBox::information(this, "No Flights Found", "No flights match your search criteria.");
    }
}

void PassengerWindow::refreshFlights()
{
    m_originSearchEdit->clear();
    m_destinationSearchEdit->clear();
    m_dateSearchEdit->setDate(QDate::currentDate());
    
    Database& db = Database::getInstance();
    QList<QMap<QString, QVariant>> flights = db.getFlights();
    
    updateFlightTable(flights);
}

void PassengerWindow::updateFlightTable(const QList<QMap<QString, QVariant>>& flights)
{
    m_flightTable->setRowCount(flights.size());
    
    for (int i = 0; i < flights.size(); ++i) {
        const auto& flight = flights[i];
        
        m_flightTable->setItem(i, 0, new QTableWidgetItem(flight["flight_number"].toString()));
        m_flightTable->setItem(i, 1, new QTableWidgetItem(flight["origin"].toString()));
        m_flightTable->setItem(i, 2, new QTableWidgetItem(flight["destination"].toString()));
        m_flightTable->setItem(i, 3, new QTableWidgetItem(
            Helpers::formatDateTime(flight["departure_time"].toDateTime())));
        m_flightTable->setItem(i, 4, new QTableWidgetItem(
            Helpers::formatDateTime(flight["arrival_time"].toDateTime())));
        
        int availableSeats = flight["total_seats"].toInt() - flight["booked_seats"].toInt();
        m_flightTable->setItem(i, 5, new QTableWidgetItem(QString::number(availableSeats)));
        m_flightTable->setItem(i, 6, new QTableWidgetItem(
            Helpers::formatPrice(flight["price"].toDouble())));
    }
}

void PassengerWindow::updateTotalPrice()
{
    if (m_selectedFlightPrice > 0) {
        double totalPrice = m_selectedFlightPrice * m_seatCountSpinBox->value();
        m_totalPriceLabel->setText(Helpers::formatPrice(totalPrice));
    }
}

void PassengerWindow::bookFlight()
{
    if (m_username.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please login first.");
        return;
    }
    
    if (m_selectedFlightNumber.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a flight to book.");
        return;
    }
    
    if (!validateBookingForm()) {
        return;
    }
    
    int seatCount = m_seatCountSpinBox->value();
    double totalPrice = m_selectedFlightPrice * seatCount;
    
    // Confirmation dialog
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Confirm Booking",
        QString("Are you sure you want to book %1 seat(s) on flight %2 for %3?")
            .arg(seatCount)
            .arg(m_selectedFlightNumber)
            .arg(Helpers::formatPrice(totalPrice)),
        QMessageBox::Yes | QMessageBox::No
    );
    if (reply != QMessageBox::Yes) {
        return;
    }
    
    Database& db = Database::getInstance();
    
    // Check seat availability
    int availableSeats = db.getAvailableSeats(m_selectedFlightNumber);
    if (availableSeats < seatCount) {
        QMessageBox::warning(this, "Error", 
            QString("Only %1 seats available. Please select fewer seats.").arg(availableSeats));
        return;
    }
    
    bool success = db.createBooking(
        m_username,
        m_selectedFlightNumber,
        seatCount,
        totalPrice,
        QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")
    );
    
    if (success) {
        QMessageBox::information(this, "Success", 
            QString("Booking confirmed! Booking ID: %1").arg(db.generateBookingId()));
        clearBookingForm();
        refreshFlights();
        refreshMyBookings();
        refreshDashboard();
        emit bookingChanged();
    } else {
        QMessageBox::warning(this, "Error", "Failed to create booking. Please try again.");
    }
}

void PassengerWindow::refreshMyBookings()
{
    if (m_username.isEmpty()) {
        return;
    }
    
    updateMyBookingsTable();
}

void PassengerWindow::updateMyBookingsTable()
{
    Database& db = Database::getInstance();
    QList<QMap<QString, QVariant>> bookings = db.getUserBookings(m_username);
    
    m_myBookingsTable->setRowCount(bookings.size());
    
    for (int i = 0; i < bookings.size(); ++i) {
        const auto& booking = bookings[i];
        
        m_myBookingsTable->setItem(i, 0, new QTableWidgetItem(booking["booking_id"].toString()));
        m_myBookingsTable->setItem(i, 1, new QTableWidgetItem(booking["flight_number"].toString()));
        m_myBookingsTable->setItem(i, 2, new QTableWidgetItem(booking["seat_count"].toString()));
        m_myBookingsTable->setItem(i, 3, new QTableWidgetItem(
            Helpers::formatPrice(booking["total_price"].toDouble())));
        m_myBookingsTable->setItem(i, 4, new QTableWidgetItem(booking["booking_date"].toString()));
        
        QString status = booking["status"].toString();
        QTableWidgetItem* statusItem = new QTableWidgetItem(Helpers::getBookingStatus(status));
        m_myBookingsTable->setItem(i, 5, statusItem);
        // Add Download Receipt button
        QPushButton* receiptButton = new QPushButton("Download Receipt");
        receiptButton->setMinimumWidth(120);
        receiptButton->setStyleSheet("background-color: #fff; color: #007bff; border: 1px solid #007bff; border-radius: 4px; font-weight: bold; padding: 2px 4px; font-size: 12px;");
        connect(receiptButton, &QPushButton::clicked, this, [this, booking]() {
            this->downloadBookingReceipt(booking);
        });
        m_myBookingsTable->setCellWidget(i, 6, receiptButton);
        m_myBookingsTable->setRowHeight(i, 48);
    }
}

void PassengerWindow::cancelMyBooking()
{
    QList<QTableWidgetItem*> selectedItems = m_myBookingsTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a booking to cancel.");
        return;
    }
    int row = selectedItems.first()->row();
    QString bookingId = m_myBookingsTable->item(row, 0)->text();
    QString status = m_myBookingsTable->item(row, 5)->text();
    int seatCount = m_myBookingsTable->item(row, 2)->text().toInt();
    if (status.toUpper() == "CANCELLED") {
        QMessageBox::information(this, "Info", "This booking is already cancelled.");
        return;
    }
    // Ask user how many seats to cancel
    bool ok = false;
    int seatsToCancel = QInputDialog::getInt(this, "Cancel Booking", QString("How many seats do you want to cancel? (1-%1)").arg(seatCount), seatCount, 1, seatCount, 1, &ok);
    if (!ok) return;
    if (seatsToCancel == seatCount) {
        // Full cancellation
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Cancel", QString("Are you sure you want to cancel ALL %1 seats for booking %2?").arg(seatCount).arg(bookingId), QMessageBox::Yes | QMessageBox::No);
        if (reply != QMessageBox::Yes) return;
        Database& db = Database::getInstance();
        bool success = db.cancelBooking(bookingId);
        if (success) {
            QMessageBox::information(this, "Success", "Booking cancelled successfully!");
            refreshMyBookings();
            refreshDashboard();
            refreshFlights();
            emit bookingChanged();
        } else {
            QMessageBox::warning(this, "Error", "Failed to cancel booking.");
        }
    } else {
        // Partial cancellation
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Partial Cancel", QString("Are you sure you want to cancel %1 out of %2 seats for booking %3?").arg(seatsToCancel).arg(seatCount).arg(bookingId), QMessageBox::Yes | QMessageBox::No);
        if (reply != QMessageBox::Yes) return;
        Database& db = Database::getInstance();
        bool success = db.cancelBooking(bookingId, seatsToCancel);
        if (success) {
            QMessageBox::information(this, "Success", QString("%1 seat(s) cancelled successfully!").arg(seatsToCancel));
            refreshMyBookings();
            refreshDashboard();
            refreshFlights();
            emit bookingChanged();
        } else {
            QMessageBox::warning(this, "Error", "Failed to cancel seats.");
        }
    }
}

void PassengerWindow::viewBookingDetails()
{
    QList<QTableWidgetItem*> selectedItems = m_myBookingsTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a booking to view details.");
        return;
    }
    
    int row = selectedItems.first()->row();
    QString bookingId = m_myBookingsTable->item(row, 0)->text();
    
    Database& db = Database::getInstance();
    QMap<QString, QVariant> booking = db.getBooking(bookingId);
    if (booking.isEmpty()) {
        QMessageBox::warning(this, "Error", "Booking details not found.");
        return;
    }
    QString details = QString("Booking Details\n%1\n\nBooking ID: %2\nFlight: %3\nRoute: %4 → %5\nDeparture: %6\nArrival: %7\nSeats: %8\nPrice: %9\nStatus: %10")
        .arg(QString("=").repeated(30))
        .arg(booking["booking_id"].toString())
        .arg(booking["flight_number"].toString())
        .arg(booking["origin"].toString())
        .arg(booking["destination"].toString())
        .arg(Helpers::formatDateTime(booking["departure_time"].toDateTime()))
        .arg(Helpers::formatDateTime(booking["arrival_time"].toDateTime()))
        .arg(booking["seat_count"].toString())
        .arg(Helpers::formatPrice(booking["total_price"].toDouble()))
        .arg(Helpers::getBookingStatus(booking["status"].toString()));
    QMessageBox::information(this, "Booking Details", details);
}

void PassengerWindow::updateProfile()
{
    if (!validateProfileForm()) {
        return;
    }
    
    Database& db = Database::getInstance();
    bool success = db.updateUserProfile(
        m_username,
        m_profileEmailEdit->text().trimmed(),
        m_profileFullNameEdit->text().trimmed()
    );
    
    if (success) {
        QMessageBox::information(this, "Success", "Profile updated successfully!");
        refreshProfile();
    } else {
        // Check if email is already taken
        QMap<QString, QVariant> user = db.getUserByUsername(m_username);
        if (user["email"].toString() != m_profileEmailEdit->text().trimmed()) {
            QMessageBox::warning(this, "Error", "This email address is already in use by another account.");
        } else {
            QMessageBox::warning(this, "Error", "Failed to update profile.");
        }
    }
}

void PassengerWindow::changePassword()
{
    if (!validatePasswordForm()) {
        return;
    }
    
    // Prevent setting the same password as the old one
    Database& db = Database::getInstance();
    QString role;
    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = ?");
    query.addBindValue(m_username);
    if (query.exec() && query.next()) {
        QString oldHash = query.value(0).toString();
        QString newHash = db.hashPassword(m_newPasswordEdit->text());
        if (oldHash == newHash) {
            QMessageBox::warning(this, "Error", "New password must be different from the current password.");
            m_newPasswordEdit->clear();
            m_confirmPasswordEdit->clear();
            return;
        }
    }
    
    bool success = db.changePassword(m_username, m_newPasswordEdit->text());
    
    if (success) {
        QMessageBox::information(this, "Success", "Password changed successfully!");
        m_currentPasswordEdit->clear();
        m_newPasswordEdit->clear();
        m_confirmPasswordEdit->clear();
    } else {
        QMessageBox::warning(this, "Error", "Failed to change password. Please try again later or contact support.");
    }
}

void PassengerWindow::refreshProfile()
{
    if (m_username.isEmpty()) {
        return;
    }
    
    updateProfileForm();
}

void PassengerWindow::updateProfileForm()
{
    Database& db = Database::getInstance();
    QList<QMap<QString, QVariant>> users = db.getAllUsers();
    
    for (const auto& user : users) {
        if (user["username"].toString() == m_username) {
            m_profileUsernameEdit->setText(user["username"].toString());
            m_profileEmailEdit->setText(user["email"].toString());
            m_profileFullNameEdit->setText(user["full_name"].toString());
            break;
        }
    }
}

bool PassengerWindow::validateBookingForm()
{
    if (m_selectedFlightNumber.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a flight to book.");
        return false;
    }
    
    if (m_seatCountSpinBox->value() <= 0) {
        QMessageBox::warning(this, "Error", "Please select at least one seat.");
        return false;
    }
    
    return true;
}

bool PassengerWindow::validateProfileForm()
{
    if (m_profileEmailEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter an email address.");
        m_profileEmailEdit->setFocus();
        return false;
    }
    
    if (!Validators::isValidEmail(m_profileEmailEdit->text().trimmed())) {
        QMessageBox::warning(this, "Error", "Please enter a valid email address.");
        m_profileEmailEdit->setFocus();
        return false;
    }
    
    if (m_profileFullNameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter your full name.");
        m_profileFullNameEdit->setFocus();
        return false;
    }
    
    return true;
}

bool PassengerWindow::validatePasswordForm()
{
    if (m_currentPasswordEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter your current password.");
        m_currentPasswordEdit->setFocus();
        return false;
    }
    
    if (m_newPasswordEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter a new password.");
        m_newPasswordEdit->setFocus();
        return false;
    }
    
    if (m_newPasswordEdit->text().length() < 6) {
        QMessageBox::warning(this, "Error", "New password must be at least 6 characters long.");
        m_newPasswordEdit->setFocus();
        return false;
    }
    
    if (m_confirmPasswordEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please confirm your new password.");
        m_confirmPasswordEdit->setFocus();
        return false;
    }
    
    if (m_newPasswordEdit->text() != m_confirmPasswordEdit->text()) {
        QMessageBox::warning(this, "Error", "New passwords do not match.");
        m_confirmPasswordEdit->setFocus();
        return false;
    }
    
    // Check if username is set
    if (m_username.isEmpty()) {
        qDebug() << "[PASSENGER] Username is empty in validatePasswordForm!";
        QMessageBox::warning(this, "Error", "Internal error: Username is not set. Please log in again.");
        return false;
    }
    
    // Check if user exists
    Database& db = Database::getInstance();
    QMap<QString, QVariant> user = db.getUserByUsername(m_username);
    if (user.isEmpty()) {
        qDebug() << "[PASSENGER] User not found in database for username:" << m_username;
        QMessageBox::warning(this, "Error", "User does not exist. Please log in again.");
        return false;
    }
    
    // Verify current password
    QString role;
    if (!db.authenticateUser(m_username, m_currentPasswordEdit->text(), role)) {
        QMessageBox::warning(this, "Error", "Current password is incorrect.");
        m_currentPasswordEdit->setFocus();
        return false;
    }
    
    return true;
}

void PassengerWindow::clearBookingForm()
{
    m_seatCountSpinBox->setValue(1);
    m_totalPriceLabel->setText("$0.00");
    m_selectedFlightNumber = "";
    m_selectedFlightPrice = 0.0;
    m_confirmBookingButton->setEnabled(false);
}

void PassengerWindow::loadUserData()
{
    Database& db = Database::getInstance();
    QMap<QString, QVariant> userData = db.getUserByUsername(m_username);
    
    if (!userData.isEmpty()) {
        m_profileUsernameEdit->setText(userData["username"].toString());
        m_profileEmailEdit->setText(userData["email"].toString());
        m_profileFullNameEdit->setText(userData["full_name"].toString());
    }
}

void PassengerWindow::downloadBookingReceipt(const QMap<QString, QVariant>& booking)
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save Booking Receipt as PDF", QString("Booking_%1.pdf").arg(booking["booking_id"].toString()), "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;

    QTextDocument doc;
    QString html = QString("<h2>CloudWings Booking Receipt</h2>"
        "<hr>"
        "<b>Booking ID:</b> %1<br>"
        "<b>Flight:</b> %2<br>"
        "<b>Route:</b> %3 → %4<br>"
        "<b>Departure:</b> %5<br>"
        "<b>Arrival:</b> %6<br>"
        "<b>Seats:</b> %7<br>"
        "<b>Total Price:</b> %8<br>"
        "<b>Status:</b> %9<br>"
        "<b>Date:</b> %10<br>"
        "<hr>"
        "<i>Thank you for booking with CloudWings!</i>")
        .arg(booking["booking_id"].toString())
        .arg(booking["flight_number"].toString())
        .arg(booking["origin"].toString())
        .arg(booking["destination"].toString())
        .arg(Helpers::formatDateTime(booking["departure_time"].toDateTime()))
        .arg(Helpers::formatDateTime(booking["arrival_time"].toDateTime()))
        .arg(booking["seat_count"].toString())
        .arg(Helpers::formatPrice(booking["total_price"].toDouble()))
        .arg(Helpers::getBookingStatus(booking["status"].toString()))
        .arg(booking["booking_date"].toString());
    doc.setHtml(html);

    QPrinter printer(QPrinter::PrinterMode::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    doc.print(&printer);
}

void PassengerWindow::logout()
{
    emit logoutRequested();
    this->close();
} 