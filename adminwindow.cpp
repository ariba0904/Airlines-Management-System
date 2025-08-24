#include "adminwindow.h"
#include "database.h"
#include "utils/helpers.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QDateTime>
#include <QApplication>
#include <QtPrintSupport/QPrinter>
#include <QTextDocument>
#include <QDebug>

AdminWindow::AdminWindow(QWidget *parent)
    : QWidget(parent)
    , m_totalFlightsLabel(nullptr)
    , m_totalBookingsLabel(nullptr)
    , m_totalRevenueLabel(nullptr)
    , m_totalUsersLabel(nullptr)
    , m_occupancyProgressBar(nullptr)
    , m_revenueSummaryText(nullptr)
    , m_flightTable(nullptr)
    , m_flightNumberEdit(nullptr)
    , m_originEdit(nullptr)
    , m_destinationEdit(nullptr)
    , m_departureTimeEdit(nullptr)
    , m_arrivalTimeEdit(nullptr)
    , m_totalSeatsSpinBox(nullptr)
    , m_priceSpinBox(nullptr)
    , m_aircraftModelEdit(nullptr)
    , m_addFlightButton(nullptr)
    , m_editFlightButton(nullptr)
    , m_deleteFlightButton(nullptr)
    , m_clearFlightButton(nullptr)
    , m_flightSearchEdit(nullptr)
    , m_userTable(nullptr)
    , m_userSearchEdit(nullptr)
    , m_deleteUserButton(nullptr)
    , m_bookingTable(nullptr)
    , m_bookingSearchEdit(nullptr)
    , m_cancelBookingButton(nullptr)
    , m_reportStartDate(nullptr)
    , m_reportEndDate(nullptr)
    , m_generateRevenueReportButton(nullptr)
    , m_generateFlightReportButton(nullptr)
    , m_reportTextEdit(nullptr)
    , m_exportReportButton(nullptr)
    , m_tabWidget(nullptr)
    , m_currentFlightNumber("")
    , m_selectedUsername("")
    , m_selectedBookingId("")
{
    setupUI();
    setupConnections();
}

void AdminWindow::loadDashboard()
{
    updateDashboardStats();
}

void AdminWindow::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // Title
    QLabel* titleLabel = new QLabel("Admin Dashboard");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #2c3e50; margin-bottom: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Tab widget
    m_tabWidget = new QTabWidget();
    mainLayout->addWidget(m_tabWidget);
    
    setupDashboard();
    setupFlightManagement();
    setupUserManagement();
    setupBookingManagement();
    setupReports();
}

void AdminWindow::setupDashboard()
{
    QWidget* dashboardWidget = new QWidget();
    QVBoxLayout* dashboardLayout = new QVBoxLayout(dashboardWidget);
    
    // Stats grid
    QGridLayout* statsGrid = new QGridLayout();
    
    // Total flights
    QGroupBox* flightsGroup = new QGroupBox("Upcoming Flights");
    QVBoxLayout* flightsLayout = new QVBoxLayout(flightsGroup);
    m_totalFlightsLabel = new QLabel("0");
    m_totalFlightsLabel->setStyleSheet("font-size: 32px; font-weight: bold; color: #3498db;");
    flightsLayout->addWidget(m_totalFlightsLabel);
    statsGrid->addWidget(flightsGroup, 0, 0);
    
    // Total bookings
    QGroupBox* bookingsGroup = new QGroupBox("Total Bookings Today");
    QVBoxLayout* bookingsLayout = new QVBoxLayout(bookingsGroup);
    m_totalBookingsLabel = new QLabel("0");
    m_totalBookingsLabel->setStyleSheet("font-size: 32px; font-weight: bold; color: #27ae60;");
    bookingsLayout->addWidget(m_totalBookingsLabel);
    statsGrid->addWidget(bookingsGroup, 0, 1);
    
    // Total revenue
    QGroupBox* revenueGroup = new QGroupBox("Total Revenue Today");
    QVBoxLayout* revenueLayout = new QVBoxLayout(revenueGroup);
    m_totalRevenueLabel = new QLabel("$0.00");
    m_totalRevenueLabel->setStyleSheet("font-size: 32px; font-weight: bold; color: #e67e22;");
    revenueLayout->addWidget(m_totalRevenueLabel);
    statsGrid->addWidget(revenueGroup, 0, 2);
    
    // Total users
    QGroupBox* usersGroup = new QGroupBox("Total Passengers");
    QVBoxLayout* usersLayout = new QVBoxLayout(usersGroup);
    m_totalUsersLabel = new QLabel("0");
    m_totalUsersLabel->setStyleSheet("font-size: 32px; font-weight: bold; color: #9b59b6;");
    usersLayout->addWidget(m_totalUsersLabel);
    statsGrid->addWidget(usersGroup, 0, 3);
    
    dashboardLayout->addLayout(statsGrid);
    
    // Occupancy rate
    QGroupBox* occupancyGroup = new QGroupBox("Average Occupancy Rate");
    QVBoxLayout* occupancyLayout = new QVBoxLayout(occupancyGroup);
    m_occupancyProgressBar = new QProgressBar();
    m_occupancyProgressBar->setRange(0, 100);
    m_occupancyProgressBar->setValue(0);
    m_occupancyProgressBar->setFormat("%p%");
    occupancyLayout->addWidget(m_occupancyProgressBar);
    dashboardLayout->addWidget(occupancyGroup);
    
    // Revenue summary
    QGroupBox* summaryGroup = new QGroupBox("Revenue Summary");
    QVBoxLayout* summaryLayout = new QVBoxLayout(summaryGroup);
    m_revenueSummaryText = new QTextEdit();
    m_revenueSummaryText->setMaximumHeight(150);
    m_revenueSummaryText->setReadOnly(true);
    summaryLayout->addWidget(m_revenueSummaryText);
    dashboardLayout->addWidget(summaryGroup);
    
    // Refresh button
    QPushButton* refreshButton = new QPushButton("Refresh Dashboard");
    refreshButton->setObjectName("refreshDashboardButton");
    refreshButton->setProperty("class", "success");
    dashboardLayout->addWidget(refreshButton);
    
    dashboardLayout->addStretch();
    
    m_tabWidget->addTab(dashboardWidget, "Dashboard");
}

void AdminWindow::setupFlightManagement()
{
    QWidget* flightWidget = new QWidget();
    QVBoxLayout* flightLayout = new QVBoxLayout(flightWidget);
    
    // Flight form
    QGroupBox* formGroup = new QGroupBox("Flight Information");
    QGridLayout* formLayout = new QGridLayout(formGroup);
    
    formLayout->addWidget(new QLabel("Flight Number:"), 0, 0);
    m_flightNumberEdit = new QLineEdit();
    m_flightNumberEdit->setPlaceholderText("e.g., CW101");
    formLayout->addWidget(m_flightNumberEdit, 0, 1);
    
    formLayout->addWidget(new QLabel("Origin:"), 0, 2);
    m_originEdit = new QLineEdit();
    m_originEdit->setPlaceholderText("e.g., New York");
    formLayout->addWidget(m_originEdit, 0, 3);
    
    formLayout->addWidget(new QLabel("Destination:"), 1, 0);
    m_destinationEdit = new QLineEdit();
    m_destinationEdit->setPlaceholderText("e.g., Los Angeles");
    formLayout->addWidget(m_destinationEdit, 1, 1);
    
    formLayout->addWidget(new QLabel("Departure Time:"), 1, 2);
    m_departureTimeEdit = new QDateTimeEdit();
    m_departureTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(1));
    m_departureTimeEdit->setCalendarPopup(true);
    formLayout->addWidget(m_departureTimeEdit, 1, 3);
    
    formLayout->addWidget(new QLabel("Arrival Time:"), 2, 0);
    m_arrivalTimeEdit = new QDateTimeEdit();
    m_arrivalTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(1).addSecs(7200));
    m_arrivalTimeEdit->setCalendarPopup(true);
    formLayout->addWidget(m_arrivalTimeEdit, 2, 1);
    
    formLayout->addWidget(new QLabel("Total Seats:"), 2, 2);
    m_totalSeatsSpinBox = new QSpinBox();
    m_totalSeatsSpinBox->setRange(1, 500);
    m_totalSeatsSpinBox->setValue(180);
    formLayout->addWidget(m_totalSeatsSpinBox, 2, 3);
    
    formLayout->addWidget(new QLabel("Price ($):"), 3, 0);
    m_priceSpinBox = new QDoubleSpinBox();
    m_priceSpinBox->setRange(0.01, 10000.00);
    m_priceSpinBox->setValue(299.99);
    m_priceSpinBox->setDecimals(2);
    formLayout->addWidget(m_priceSpinBox, 3, 1);
    
    formLayout->addWidget(new QLabel("Aircraft Model:"), 3, 2);
    m_aircraftModelEdit = new QLineEdit();
    m_aircraftModelEdit->setPlaceholderText("e.g., Boeing 737");
    formLayout->addWidget(m_aircraftModelEdit, 3, 3);
    
    flightLayout->addWidget(formGroup);
    
    // Flight buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    m_addFlightButton = new QPushButton("Add Flight");
    m_addFlightButton->setProperty("class", "success");
    m_editFlightButton = new QPushButton("Update Flight");
    m_editFlightButton->setProperty("class", "warning");
    m_deleteFlightButton = new QPushButton("Delete Flight");
    m_deleteFlightButton->setProperty("class", "danger");
    m_clearFlightButton = new QPushButton("Clear Form");
    m_clearFlightButton->setProperty("class", "secondary");
    
    buttonLayout->addWidget(m_addFlightButton);
    buttonLayout->addWidget(m_editFlightButton);
    buttonLayout->addWidget(m_deleteFlightButton);
    buttonLayout->addWidget(m_clearFlightButton);
    buttonLayout->addStretch();
    
    flightLayout->addLayout(buttonLayout);
    
    // Search
    QHBoxLayout* searchLayout = new QHBoxLayout();
    searchLayout->addWidget(new QLabel("Search:"));
    m_flightSearchEdit = new QLineEdit();
    m_flightSearchEdit->setPlaceholderText("Search flights...");
    searchLayout->addWidget(m_flightSearchEdit);
    QPushButton* searchButton = new QPushButton("Search");
    searchButton->setProperty("class", "info");
    searchLayout->addWidget(searchButton);
    QPushButton* refreshButton = new QPushButton("Refresh");
    refreshButton->setProperty("class", "secondary");
    searchLayout->addWidget(refreshButton);
    
    flightLayout->addLayout(searchLayout);
    
    // Flight table
    m_flightTable = new QTableWidget();
    m_flightTable->setColumnCount(6);
    m_flightTable->setHorizontalHeaderLabels({
        "Flight #", "Origin", "Destination", "Departure", "Price", "Status"
    });
    m_flightTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_flightTable->setAlternatingRowColors(false);
    m_flightTable->horizontalHeader()->setStretchLastSection(true);
    
    flightLayout->addWidget(m_flightTable);
    
    m_tabWidget->addTab(flightWidget, "Flight Management");
}

void AdminWindow::setupUserManagement()
{
    QWidget* userWidget = new QWidget();
    QVBoxLayout* userLayout = new QVBoxLayout(userWidget);
    
    // Search
    QHBoxLayout* searchLayout = new QHBoxLayout();
    searchLayout->addWidget(new QLabel("Search:"));
    m_userSearchEdit = new QLineEdit();
    m_userSearchEdit->setPlaceholderText("Search users...");
    searchLayout->addWidget(m_userSearchEdit);
    QPushButton* searchButton = new QPushButton("Search");
    searchButton->setProperty("class", "info");
    searchLayout->addWidget(searchButton);
    QPushButton* refreshButton = new QPushButton("Refresh");
    refreshButton->setProperty("class", "secondary");
    searchLayout->addWidget(refreshButton);
    m_deleteUserButton = new QPushButton("Delete User");
    m_deleteUserButton->setProperty("class", "danger");
    searchLayout->addWidget(m_deleteUserButton);
    
    userLayout->addLayout(searchLayout);
    
    // User table
    m_userTable = new QTableWidget();
    m_userTable->setColumnCount(5);
    m_userTable->setHorizontalHeaderLabels({
        "Username", "Email", "Full Name", "Role", "Created Date"
    });
    m_userTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_userTable->setAlternatingRowColors(false);
    m_userTable->horizontalHeader()->setStretchLastSection(true);
    
    userLayout->addWidget(m_userTable);
    
    m_tabWidget->addTab(userWidget, "User Management");
}

void AdminWindow::setupBookingManagement()
{
    QWidget* bookingWidget = new QWidget();
    QVBoxLayout* bookingLayout = new QVBoxLayout(bookingWidget);
    
    // Search
    QHBoxLayout* searchLayout = new QHBoxLayout();
    searchLayout->addWidget(new QLabel("Search:"));
    m_bookingSearchEdit = new QLineEdit();
    m_bookingSearchEdit->setPlaceholderText("Search bookings...");
    searchLayout->addWidget(m_bookingSearchEdit);
    QPushButton* searchButton = new QPushButton("Search");
    searchButton->setProperty("class", "info");
    searchLayout->addWidget(searchButton);
    QPushButton* refreshButton = new QPushButton("Refresh");
    refreshButton->setObjectName("bookingsRefreshButton");
    searchLayout->addWidget(refreshButton);
    m_cancelBookingButton = new QPushButton("Cancel Booking");
    m_cancelBookingButton->setProperty("class", "danger");
    searchLayout->addWidget(m_cancelBookingButton);
    
    bookingLayout->addLayout(searchLayout);
    
    // Booking table
    m_bookingTable = new QTableWidget();
    m_bookingTable->setColumnCount(7);
    m_bookingTable->setHorizontalHeaderLabels({
        "Booking ID", "Passenger", "Flight", "Seats", "Total Price", "Date", "Status"
    });
    m_bookingTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_bookingTable->setAlternatingRowColors(false);
    m_bookingTable->horizontalHeader()->setStretchLastSection(true);
    
    bookingLayout->addWidget(m_bookingTable);
    
    m_tabWidget->addTab(bookingWidget, "Booking Management");
}

void AdminWindow::setupReports()
{
    QWidget* reportWidget = new QWidget();
    QVBoxLayout* reportLayout = new QVBoxLayout(reportWidget);
    
    // Date range
    QGroupBox* dateGroup = new QGroupBox("Report Date Range");
    QHBoxLayout* dateLayout = new QHBoxLayout(dateGroup);
    dateLayout->addWidget(new QLabel("From:"));
    m_reportStartDate = new QDateEdit();
    m_reportStartDate->setDate(QDate::currentDate().addDays(-30));
    m_reportStartDate->setCalendarPopup(true);
    dateLayout->addWidget(m_reportStartDate);
    dateLayout->addWidget(new QLabel("To:"));
    m_reportEndDate = new QDateEdit();
    m_reportEndDate->setDate(QDate::currentDate());
    m_reportEndDate->setCalendarPopup(true);
    dateLayout->addWidget(m_reportEndDate);
    dateLayout->addStretch();
    
    reportLayout->addWidget(dateGroup);
    
    // Report buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    m_generateRevenueReportButton = new QPushButton("Generate Revenue Report");
    m_generateRevenueReportButton->setProperty("class", "success");
    m_generateFlightReportButton = new QPushButton("Generate Flight Report");
    m_generateFlightReportButton->setProperty("class", "info");
    m_exportReportButton = new QPushButton("Export Report");
    m_exportReportButton->setProperty("class", "warning");
    
    buttonLayout->addWidget(m_generateRevenueReportButton);
    buttonLayout->addWidget(m_generateFlightReportButton);
    buttonLayout->addWidget(m_exportReportButton);
    buttonLayout->addStretch();
    
    reportLayout->addLayout(buttonLayout);
    
    // Report text area
    QGroupBox* reportGroup = new QGroupBox("Report Output");
    QVBoxLayout* reportTextLayout = new QVBoxLayout(reportGroup);
    m_reportTextEdit = new QTextEdit();
    m_reportTextEdit->setReadOnly(true);
    reportTextLayout->addWidget(m_reportTextEdit);
    
    reportLayout->addWidget(reportGroup);
    
    m_tabWidget->addTab(reportWidget, "Reports");
}

void AdminWindow::setupConnections()
{
    // Tab change
    connect(m_tabWidget, &QTabWidget::currentChanged, this, &AdminWindow::onTabChanged);
    
    // Dashboard
    QPushButton* dashboardRefreshBtn = findChild<QPushButton*>("refreshDashboardButton");
    if (!dashboardRefreshBtn) qDebug() << "[CONNECT] Dashboard refresh button is nullptr!";
    else connect(dashboardRefreshBtn, &QPushButton::clicked, this, &AdminWindow::refreshDashboard);
    
    // Flight management
    connect(m_addFlightButton, &QPushButton::clicked, this, &AdminWindow::addFlight);
    connect(m_editFlightButton, &QPushButton::clicked, this, &AdminWindow::editFlight);
    connect(m_deleteFlightButton, &QPushButton::clicked, this, &AdminWindow::deleteFlight);
    connect(m_clearFlightButton, &QPushButton::clicked, this, &AdminWindow::clearFlightForm);
    
    // User management
    connect(m_deleteUserButton, &QPushButton::clicked, this, &AdminWindow::deleteUser);
    
    // Booking management
    connect(m_cancelBookingButton, &QPushButton::clicked, this, &AdminWindow::cancelBooking);
    
    // Reports
    connect(m_generateRevenueReportButton, &QPushButton::clicked, this, &AdminWindow::generateRevenueReport);
    connect(m_generateFlightReportButton, &QPushButton::clicked, this, &AdminWindow::generateFlightReport);
    connect(m_exportReportButton, &QPushButton::clicked, this, &AdminWindow::exportReport);

    connect(m_flightTable, &QTableWidget::itemSelectionChanged, this, [this]() {
        QList<QTableWidgetItem*> selectedItems = m_flightTable->selectedItems();
        if (!selectedItems.isEmpty()) {
            int row = selectedItems.first()->row();
            QString flightNumber = m_flightTable->item(row, 0)->text();
            Database& db = Database::getInstance();
            QMap<QString, QVariant> flight = db.getFlight(flightNumber);
            populateFlightForm(flight);
        } else {
            clearFlightForm();
        }
    });

    connect(m_flightSearchEdit, &QLineEdit::textChanged, this, &AdminWindow::searchFlights);

    connect(m_userTable, &QTableWidget::itemSelectionChanged, this, [this]() {
        QList<QTableWidgetItem*> selectedItems = m_userTable->selectedItems();
        if (!selectedItems.isEmpty()) {
            int row = selectedItems.first()->row();
            m_selectedUsername = m_userTable->item(row, 0)->text();
        } else {
            m_selectedUsername.clear();
        }
    });
    connect(m_userSearchEdit, &QLineEdit::textChanged, this, &AdminWindow::searchUsers);

    connect(m_bookingTable, &QTableWidget::itemSelectionChanged, this, [this]() {
        QList<QTableWidgetItem*> selectedItems = m_bookingTable->selectedItems();
        if (!selectedItems.isEmpty()) {
            int row = selectedItems.first()->row();
            m_selectedBookingId = m_bookingTable->item(row, 0)->text();
        } else {
            m_selectedBookingId.clear();
        }
    });
    connect(m_bookingSearchEdit, &QLineEdit::textChanged, this, &AdminWindow::searchBookings);
    QPushButton* bookingsRefreshBtn = findChild<QPushButton*>("bookingsRefreshButton");
    if (!bookingsRefreshBtn) qDebug() << "[CONNECT] Bookings refresh button is nullptr!";
    else connect(bookingsRefreshBtn, &QPushButton::clicked, this, &AdminWindow::refreshBookings);
}

void AdminWindow::onTabChanged(int index)
{
    switch (index) {
        case 0: // Dashboard
            refreshDashboard();
            break;
        case 1: // Flight Management
            refreshFlights();
            break;
        case 2: // User Management
            refreshUsers();
            break;
        case 3: // Booking Management
            refreshBookings();
            break;
        case 4: // Reports
            // Reports are generated on demand
            break;
    }
}

void AdminWindow::refreshDashboard()
{
    updateDashboardStats();
}

void AdminWindow::updateDashboardStats()
{
    Database& db = Database::getInstance();
    QMap<QString, QVariant> stats = db.getDashboardStats();
    
    m_totalFlightsLabel->setText(stats["flights_today"].toString());
    m_totalBookingsLabel->setText(stats["bookings_made_today"].toString());
    m_totalRevenueLabel->setText(Helpers::formatPrice(stats["revenue_made_today"].toDouble()));
    m_totalUsersLabel->setText(stats["total_passengers"].toString());
    
    // Calculate average occupancy
    QList<QMap<QString, QVariant>> flights = db.getFlights();
    double totalOccupancy = 0.0;
    int flightCount = 0;
    
    for (const auto& flight : flights) {
        int totalSeats = flight["total_seats"].toInt();
        int bookedSeats = flight["booked_seats"].toInt();
        if (totalSeats > 0) {
            totalOccupancy += Helpers::calculateOccupancyRate(totalSeats, bookedSeats);
            flightCount++;
        }
    }
    
    double avgOccupancy = flightCount > 0 ? totalOccupancy / flightCount : 0.0;
    m_occupancyProgressBar->setValue(static_cast<int>(avgOccupancy));
    
    // Revenue summary
    QString summary = QString("Summary for flights departing today (%1):\n\n")
                     .arg(QDate::currentDate().toString("dd/MM/yyyy"));
    summary += QString("• Bookings: %1\n").arg(stats["bookings_departing_today"].toString());
    summary += QString("• Revenue: %1\n").arg(Helpers::formatPrice(stats["revenue_departing_today"].toDouble()));
    summary += QString("• Unique Passengers: %1\n").arg(stats["passengers_departing_today"].toString());
    
    m_revenueSummaryText->setText(summary);
}

void AdminWindow::addFlight()
{
    if (!validateFlightForm()) {
        QMessageBox::warning(this, "Invalid Input", "Please fill in all required flight details correctly.");
        return;
    }
    QString flightNumber = m_flightNumberEdit->text().trimmed();
    QString origin = m_originEdit->text().trimmed();
    QString destination = m_destinationEdit->text().trimmed();
    QDateTime departureTime = m_departureTimeEdit->dateTime();
    QDateTime arrivalTime = m_arrivalTimeEdit->dateTime();
    int totalSeats = m_totalSeatsSpinBox->value();
    double price = m_priceSpinBox->value();
    QString aircraftModel = m_aircraftModelEdit->text().trimmed();

    Database& db = Database::getInstance();
    if (!db.addFlight(flightNumber, origin, destination, departureTime, arrivalTime, totalSeats, price, aircraftModel)) {
        QMessageBox::critical(this, "Error", "Failed to add flight. Flight number may already exist.");
        return;
    }
    QMessageBox::information(this, "Success", "Flight added successfully.");
    clearFlightForm();
    updateFlightTable();
    updateDashboardStats();
}

void AdminWindow::editFlight()
{
    if (m_currentFlightNumber.isEmpty()) {
        QMessageBox::warning(this, "No Selection", "Please select a flight to update.");
        return;
    }
    if (!validateFlightForm()) {
        QMessageBox::warning(this, "Invalid Input", "Please fill in all required flight details correctly.");
        return;
    }
    QString origin = m_originEdit->text().trimmed();
    QString destination = m_destinationEdit->text().trimmed();
    QDateTime departureTime = m_departureTimeEdit->dateTime();
    QDateTime arrivalTime = m_arrivalTimeEdit->dateTime();
    int totalSeats = m_totalSeatsSpinBox->value();
    double price = m_priceSpinBox->value();
    QString aircraftModel = m_aircraftModelEdit->text().trimmed();

    Database& db = Database::getInstance();
    if (!db.updateFlight(m_currentFlightNumber, origin, destination, departureTime, arrivalTime, totalSeats, price, aircraftModel)) {
        QMessageBox::critical(this, "Error", "Failed to update flight.");
        return;
    }
    QMessageBox::information(this, "Success", "Flight updated successfully.");
    clearFlightForm();
    updateFlightTable();
    updateDashboardStats();
}

void AdminWindow::deleteFlight()
{
    if (m_currentFlightNumber.isEmpty()) {
        QMessageBox::warning(this, "No Selection", "Please select a flight to delete.");
        return;
    }
    int ret = QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete this flight? This action cannot be undone.");
    if (ret != QMessageBox::Yes) return;
    Database& db = Database::getInstance();
    if (!db.deleteFlight(m_currentFlightNumber)) {
        QMessageBox::critical(this, "Error", "Failed to delete flight.");
        return;
    }
    QMessageBox::information(this, "Success", "Flight deleted successfully.");
    clearFlightForm();
    updateFlightTable();
    updateDashboardStats();
}

void AdminWindow::refreshFlights()
{
    updateFlightTable();
}

void AdminWindow::searchFlights()
{
    QString searchText = m_flightSearchEdit->text().trimmed();
    Database& db = Database::getInstance();
    QList<QMap<QString, QVariant>> flights;
    if (searchText.isEmpty()) {
        flights = db.getFlights();
    } else {
        // Search by flight number, origin, or destination (case-insensitive)
        QList<QMap<QString, QVariant>> allFlights = db.getFlights();
        for (const auto& flight : allFlights) {
            if (flight["flight_number"].toString().contains(searchText, Qt::CaseInsensitive) ||
                flight["origin"].toString().contains(searchText, Qt::CaseInsensitive) ||
                flight["destination"].toString().contains(searchText, Qt::CaseInsensitive)) {
                flights.append(flight);
            }
        }
    }
    m_flightTable->setRowCount(flights.size());
    for (int i = 0; i < flights.size(); ++i) {
        const auto& flight = flights[i];
        m_flightTable->setItem(i, 0, new QTableWidgetItem(flight["flight_number"].toString()));
        m_flightTable->setItem(i, 1, new QTableWidgetItem(flight["origin"].toString()));
        m_flightTable->setItem(i, 2, new QTableWidgetItem(flight["destination"].toString()));
        m_flightTable->setItem(i, 3, new QTableWidgetItem(Helpers::formatDateTime(flight["departure_time"].toDateTime())));
        m_flightTable->setItem(i, 4, new QTableWidgetItem(Helpers::formatDateTime(flight["arrival_time"].toDateTime())));
        m_flightTable->setItem(i, 5, new QTableWidgetItem(QString("%1/%2").arg(flight["booked_seats"].toInt()).arg(flight["total_seats"].toInt())));
        QString status = Helpers::getFlightStatus(flight["departure_time"].toDateTime());
        m_flightTable->setItem(i, 6, new QTableWidgetItem(status));
    }
}

void AdminWindow::refreshUsers()
{
    updateUserTable();
}

void AdminWindow::deleteUser()
{
    if (m_selectedUsername.isEmpty()) {
        QMessageBox::warning(this, "No Selection", "Please select a user to delete.");
        return;
    }
    int ret = QMessageBox::question(this, "Confirm Delete", QString("Are you sure you want to delete user '%1'? This action cannot be undone.").arg(m_selectedUsername));
    if (ret != QMessageBox::Yes) return;
    Database& db = Database::getInstance();
    if (!db.deleteUser(m_selectedUsername)) {
        QMessageBox::critical(this, "Error", "Failed to delete user.");
        return;
    }
    QMessageBox::information(this, "Success", "User deleted successfully.");
    m_selectedUsername.clear();
    updateUserTable();
    updateDashboardStats();
}

void AdminWindow::searchUsers()
{
    QString searchText = m_userSearchEdit->text().trimmed();
    Database& db = Database::getInstance();
    QList<QMap<QString, QVariant>> users = db.getAllUsers();
    QList<QMap<QString, QVariant>> filtered;
    if (searchText.isEmpty()) {
        filtered = users;
    } else {
        for (const auto& user : users) {
            if (user["username"].toString().contains(searchText, Qt::CaseInsensitive) ||
                user["email"].toString().contains(searchText, Qt::CaseInsensitive) ||
                user["full_name"].toString().contains(searchText, Qt::CaseInsensitive)) {
                filtered.append(user);
            }
        }
    }
    m_userTable->setRowCount(filtered.size());
    for (int i = 0; i < filtered.size(); ++i) {
        const auto& user = filtered[i];
        m_userTable->setItem(i, 0, new QTableWidgetItem(user["username"].toString()));
        m_userTable->setItem(i, 1, new QTableWidgetItem(user["email"].toString()));
        m_userTable->setItem(i, 2, new QTableWidgetItem(user["full_name"].toString()));
        m_userTable->setItem(i, 3, new QTableWidgetItem(user["role"].toString()));
        m_userTable->setItem(i, 4, new QTableWidgetItem(user["created_date"].toString()));
    }
}

void AdminWindow::updateUserTable()
{
    Database& db = Database::getInstance();
    QList<QMap<QString, QVariant>> users = db.getAllUsers();
    
    m_userTable->setRowCount(users.size());
    
    for (int i = 0; i < users.size(); ++i) {
        const auto& user = users[i];
        
        m_userTable->setItem(i, 0, new QTableWidgetItem(user["username"].toString()));
        m_userTable->setItem(i, 1, new QTableWidgetItem(user["email"].toString()));
        m_userTable->setItem(i, 2, new QTableWidgetItem(user["full_name"].toString()));
        m_userTable->setItem(i, 3, new QTableWidgetItem(user["role"].toString()));
        m_userTable->setItem(i, 4, new QTableWidgetItem(user["created_date"].toString()));
    }
}

void AdminWindow::refreshBookings()
{
    updateBookingTable();
}

void AdminWindow::searchBookings()
{
    QString searchText = m_bookingSearchEdit->text().trimmed();
    Database& db = Database::getInstance();
    QList<QMap<QString, QVariant>> bookings = db.getAllBookings();
    QList<QMap<QString, QVariant>> filtered;
    if (searchText.isEmpty()) {
        filtered = bookings;
    } else {
        for (const auto& booking : bookings) {
            if (booking["booking_id"].toString().contains(searchText, Qt::CaseInsensitive) ||
                booking["full_name"].toString().contains(searchText, Qt::CaseInsensitive) ||
                booking["flight_number"].toString().contains(searchText, Qt::CaseInsensitive) ||
                booking["status"].toString().contains(searchText, Qt::CaseInsensitive)) {
                filtered.append(booking);
            }
        }
    }
    m_bookingTable->setRowCount(filtered.size());
    for (int i = 0; i < filtered.size(); ++i) {
        const auto& booking = filtered[i];
        QString passenger = booking["full_name"].toString();
        if (passenger.isEmpty()) passenger = booking["username"].toString();
        m_bookingTable->setItem(i, 0, new QTableWidgetItem(booking["booking_id"].toString()));
        m_bookingTable->setItem(i, 1, new QTableWidgetItem(passenger));
        m_bookingTable->setItem(i, 2, new QTableWidgetItem(booking["flight_number"].toString()));
        m_bookingTable->setItem(i, 3, new QTableWidgetItem(booking["seat_count"].toString()));
        m_bookingTable->setItem(i, 4, new QTableWidgetItem(Helpers::formatPrice(booking["total_price"].toDouble())));
        m_bookingTable->setItem(i, 5, new QTableWidgetItem(booking["booking_date"].toString()));
        QString status = booking["status"].toString();
        m_bookingTable->setItem(i, 6, new QTableWidgetItem(Helpers::getBookingStatus(status)));
    }
}

void AdminWindow::cancelBooking()
{
    if (m_selectedBookingId.isEmpty()) {
        QMessageBox::warning(this, "No Selection", "Please select a booking to cancel.");
        return;
    }
    int ret = QMessageBox::question(this, "Confirm Cancel", QString("Are you sure you want to cancel booking '%1'? This action cannot be undone.").arg(m_selectedBookingId));
    if (ret != QMessageBox::Yes) return;
    Database& db = Database::getInstance();
    if (!db.cancelBooking(m_selectedBookingId)) {
        QMessageBox::critical(this, "Error", "Failed to cancel booking.");
        return;
    }
    QMessageBox::information(this, "Success", "Booking cancelled successfully.");
    m_selectedBookingId.clear();
    updateBookingTable();
    updateDashboardStats();
}

void AdminWindow::updateBookingTable()
{
    Database& db = Database::getInstance();
    QList<QMap<QString, QVariant>> bookings = db.getAllBookings();
    
    m_bookingTable->setRowCount(bookings.size());
    
    for (int i = 0; i < bookings.size(); ++i) {
        const auto& booking = bookings[i];
        QString passenger = booking["full_name"].toString();
        if (passenger.isEmpty()) passenger = booking["username"].toString();
        m_bookingTable->setItem(i, 0, new QTableWidgetItem(booking["booking_id"].toString()));
        m_bookingTable->setItem(i, 1, new QTableWidgetItem(passenger));
        m_bookingTable->setItem(i, 2, new QTableWidgetItem(booking["flight_number"].toString()));
        m_bookingTable->setItem(i, 3, new QTableWidgetItem(booking["seat_count"].toString()));
        m_bookingTable->setItem(i, 4, new QTableWidgetItem(Helpers::formatPrice(booking["total_price"].toDouble())));
        m_bookingTable->setItem(i, 5, new QTableWidgetItem(booking["booking_date"].toString()));
        QString status = booking["status"].toString();
        m_bookingTable->setItem(i, 6, new QTableWidgetItem(Helpers::getBookingStatus(status)));
    }
}

void AdminWindow::generateRevenueReport()
{
    QDate startDate = m_reportStartDate->date();
    QDate endDate = m_reportEndDate->date();
    Database& db = Database::getInstance();
    QList<QMap<QString, QVariant>> report = db.getRevenueReport(startDate, endDate);
    QString html = "<div style='font-size:14pt; font-family:Arial;'>";
    html += QString("<h2>Revenue Report</h2>");
    html += QString("<b>From:</b> %1 &nbsp; <b>To:</b> %2<br><br>")
        .arg(startDate.toString("yyyy-MM-dd")).arg(endDate.toString("yyyy-MM-dd"));
    double totalRevenue = 0;
    int totalBookings = 0;
    for (const auto& row : report) {
        html += QString("<b>Date:</b> %1<br>").arg(row["date"].toString());
        html += QString("<b>Bookings:</b> %1<br>").arg(row["bookings"].toInt());
        html += QString("<b>Revenue:</b> %1<br><hr style='border:1px solid #ccc;'><br>")
            .arg(Helpers::formatPrice(row["revenue"].toDouble()));
        totalRevenue += row["revenue"].toDouble();
        totalBookings += row["bookings"].toInt();
    }
    html += QString("<b>Total Bookings:</b> %1<br><b>Total Revenue:</b> %2</div>")
        .arg(totalBookings).arg(Helpers::formatPrice(totalRevenue));
    m_reportTextEdit->setHtml(html);
}

void AdminWindow::generateFlightReport()
{
    QDate startDate = m_reportStartDate->date();
    QDate endDate = m_reportEndDate->date();
    Database& db = Database::getInstance();
    QString html = "<div style='font-size:14pt; font-family:Arial;'>";
    html += QString("<h2>Flight Report</h2>");
    html += QString("<b>From:</b> %1 &nbsp; <b>To:</b> %2<br><br>")
        .arg(startDate.toString("yyyy-MM-dd")).arg(endDate.toString("yyyy-MM-dd"));
    int totalFlights = 0;
    int totalSeats = 0;
    int totalBooked = 0;
    for (QDate date = startDate; date <= endDate; date = date.addDays(1)) {
        QList<QMap<QString, QVariant>> report = db.getFlightReport(date);
        if (!report.isEmpty()) {
            html += QString("<b>Date:</b> %1<br>").arg(date.toString("yyyy-MM-dd"));
        }
        for (const auto& row : report) {
            html += QString("<b>Flight #:</b> %1<br>").arg(row["flight_number"].toString());
            html += QString("<b>Origin:</b> %1<br>").arg(row["origin"].toString());
            html += QString("<b>Destination:</b> %1<br>").arg(row["destination"].toString());
            html += QString("<b>Departure:</b> %1<br>").arg(Helpers::formatDateTime(row["departure_time"].toDateTime()));
            html += QString("<b>Total Seats:</b> %1<br>").arg(row["total_seats"].toInt());
            html += QString("<b>Booked:</b> %1<br>").arg(row["booked_seats"].toInt());
            html += QString("<b>Available:</b> %1<br>").arg(row["available_seats"].toInt());
            html += QString("<b>Occupancy:</b> %1%<br><hr style='border:1px solid #ccc;'><br>")
                .arg(row["occupancy_rate"].toDouble());
            totalFlights++;
            totalSeats += row["total_seats"].toInt();
            totalBooked += row["booked_seats"].toInt();
        }
    }
    html += QString("<b>Total Flights:</b> %1<br><b>Total Seats:</b> %2<br><b>Total Booked:</b> %3</div>")
        .arg(totalFlights).arg(totalSeats).arg(totalBooked);
    m_reportTextEdit->setHtml(html);
}

void AdminWindow::exportReport()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export Report as PDF", "report.pdf", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;
    QTextDocument doc;
    doc.setHtml(m_reportTextEdit->toHtml());
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageMargins(QMarginsF(20, 20, 20, 20));
    doc.setDefaultFont(QFont("Arial", 14));
    doc.print(&printer);
    QMessageBox::information(this, "Exported", "Report exported as PDF successfully.");
}

void AdminWindow::clearFlightForm()
{
    m_flightNumberEdit->clear();
    m_originEdit->clear();
    m_destinationEdit->clear();
    m_departureTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(1));
    m_arrivalTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(1).addSecs(7200));
    m_totalSeatsSpinBox->setValue(180);
    m_priceSpinBox->setValue(299.99);
    m_aircraftModelEdit->clear();
    m_currentFlightNumber = "";
}

void AdminWindow::populateFlightForm(const QMap<QString, QVariant>& flight)
{
    m_currentFlightNumber = flight["flight_number"].toString();
    m_flightNumberEdit->setText(m_currentFlightNumber);
    m_originEdit->setText(flight["origin"].toString());
    m_destinationEdit->setText(flight["destination"].toString());
    m_departureTimeEdit->setDateTime(flight["departure_time"].toDateTime());
    m_arrivalTimeEdit->setDateTime(flight["arrival_time"].toDateTime());
    m_totalSeatsSpinBox->setValue(flight["total_seats"].toInt());
    m_priceSpinBox->setValue(flight["price"].toDouble());
    m_aircraftModelEdit->setText(flight["aircraft_model"].toString());
}

bool AdminWindow::validateFlightForm()
{
    // Implement validation logic here
    return true; // Placeholder return, actual implementation needed
}

void AdminWindow::updateFlightTable()
{
    Database& db = Database::getInstance();
    QList<QMap<QString, QVariant>> flights = db.getFlights();
    m_flightTable->setRowCount(flights.size());
    for (int i = 0; i < flights.size(); ++i) {
        const auto& flight = flights[i];
        m_flightTable->setItem(i, 0, new QTableWidgetItem(flight["flight_number"].toString()));
        m_flightTable->setItem(i, 1, new QTableWidgetItem(flight["origin"].toString()));
        m_flightTable->setItem(i, 2, new QTableWidgetItem(flight["destination"].toString()));
        m_flightTable->setItem(i, 3, new QTableWidgetItem(Helpers::formatDateTime(flight["departure_time"].toDateTime())));
        m_flightTable->setItem(i, 4, new QTableWidgetItem(Helpers::formatDateTime(flight["arrival_time"].toDateTime())));
        m_flightTable->setItem(i, 5, new QTableWidgetItem(QString("%1/%2").arg(flight["booked_seats"].toInt()).arg(flight["total_seats"].toInt())));
        QString status = Helpers::getFlightStatus(flight["departure_time"].toDateTime());
        m_flightTable->setItem(i, 6, new QTableWidgetItem(status));
    }
} 