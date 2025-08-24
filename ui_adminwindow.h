/********************************************************************************
** Form generated from reading UI file 'adminwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADMINWINDOW_H
#define UI_ADMINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AdminWindow
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QTabWidget *tabWidget;
    QWidget *dashboardTab;
    QVBoxLayout *dashboardLayout;
    QGroupBox *statsGroup;
    QGridLayout *statsGrid;
    QLabel *totalFlightsLabel;
    QLabel *totalBookingsLabel;
    QLabel *totalRevenueLabel;
    QLabel *totalUsersLabel;
    QPushButton *refreshButton;
    QWidget *flightsTab;
    QVBoxLayout *flightsLayout;
    QGroupBox *flightFormGroup;
    QGridLayout *flightFormLayout;
    QLabel *flightNumberLabel;
    QLineEdit *flightNumberEdit;
    QLabel *originLabel;
    QLineEdit *originEdit;
    QLabel *destinationLabel;
    QLineEdit *destinationEdit;
    QLabel *priceLabel;
    QDoubleSpinBox *priceSpinBox;
    QHBoxLayout *flightButtonsLayout;
    QPushButton *addFlightButton;
    QPushButton *editFlightButton;
    QPushButton *deleteFlightButton;
    QSpacerItem *horizontalSpacer;
    QTableWidget *flightTable;
    QWidget *usersTab;
    QVBoxLayout *usersLayout;
    QTableWidget *userTable;
    QWidget *bookingsTab;
    QVBoxLayout *bookingsLayout;
    QTableWidget *bookingTable;
    QWidget *reportsTab;
    QVBoxLayout *reportsLayout;
    QGroupBox *dateRangeGroup;
    QHBoxLayout *dateRangeLayout;
    QLabel *fromLabel;
    QDateEdit *startDateEdit;
    QLabel *toLabel;
    QDateEdit *endDateEdit;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *reportButtonsLayout;
    QPushButton *generateReportButton;
    QPushButton *exportReportButton;
    QSpacerItem *horizontalSpacer_3;
    QTextEdit *reportTextEdit;

    void setupUi(QWidget *AdminWindow)
    {
        if (AdminWindow->objectName().isEmpty())
            AdminWindow->setObjectName("AdminWindow");
        AdminWindow->resize(1200, 800);
        verticalLayout = new QVBoxLayout(AdminWindow);
        verticalLayout->setObjectName("verticalLayout");
        titleLabel = new QLabel(AdminWindow);
        titleLabel->setObjectName("titleLabel");

        verticalLayout->addWidget(titleLabel);

        tabWidget = new QTabWidget(AdminWindow);
        tabWidget->setObjectName("tabWidget");
        dashboardTab = new QWidget();
        dashboardTab->setObjectName("dashboardTab");
        dashboardLayout = new QVBoxLayout(dashboardTab);
        dashboardLayout->setObjectName("dashboardLayout");
        statsGroup = new QGroupBox(dashboardTab);
        statsGroup->setObjectName("statsGroup");
        statsGrid = new QGridLayout(statsGroup);
        statsGrid->setObjectName("statsGrid");
        totalFlightsLabel = new QLabel(statsGroup);
        totalFlightsLabel->setObjectName("totalFlightsLabel");

        statsGrid->addWidget(totalFlightsLabel, 0, 0, 1, 1);

        totalBookingsLabel = new QLabel(statsGroup);
        totalBookingsLabel->setObjectName("totalBookingsLabel");

        statsGrid->addWidget(totalBookingsLabel, 0, 1, 1, 1);

        totalRevenueLabel = new QLabel(statsGroup);
        totalRevenueLabel->setObjectName("totalRevenueLabel");

        statsGrid->addWidget(totalRevenueLabel, 0, 2, 1, 1);

        totalUsersLabel = new QLabel(statsGroup);
        totalUsersLabel->setObjectName("totalUsersLabel");

        statsGrid->addWidget(totalUsersLabel, 0, 3, 1, 1);


        dashboardLayout->addWidget(statsGroup);

        refreshButton = new QPushButton(dashboardTab);
        refreshButton->setObjectName("refreshButton");

        dashboardLayout->addWidget(refreshButton);

        tabWidget->addTab(dashboardTab, QString());
        flightsTab = new QWidget();
        flightsTab->setObjectName("flightsTab");
        flightsLayout = new QVBoxLayout(flightsTab);
        flightsLayout->setObjectName("flightsLayout");
        flightFormGroup = new QGroupBox(flightsTab);
        flightFormGroup->setObjectName("flightFormGroup");
        flightFormLayout = new QGridLayout(flightFormGroup);
        flightFormLayout->setObjectName("flightFormLayout");
        flightNumberLabel = new QLabel(flightFormGroup);
        flightNumberLabel->setObjectName("flightNumberLabel");

        flightFormLayout->addWidget(flightNumberLabel, 0, 0, 1, 1);

        flightNumberEdit = new QLineEdit(flightFormGroup);
        flightNumberEdit->setObjectName("flightNumberEdit");

        flightFormLayout->addWidget(flightNumberEdit, 0, 1, 1, 1);

        originLabel = new QLabel(flightFormGroup);
        originLabel->setObjectName("originLabel");

        flightFormLayout->addWidget(originLabel, 0, 2, 1, 1);

        originEdit = new QLineEdit(flightFormGroup);
        originEdit->setObjectName("originEdit");

        flightFormLayout->addWidget(originEdit, 0, 3, 1, 1);

        destinationLabel = new QLabel(flightFormGroup);
        destinationLabel->setObjectName("destinationLabel");

        flightFormLayout->addWidget(destinationLabel, 1, 0, 1, 1);

        destinationEdit = new QLineEdit(flightFormGroup);
        destinationEdit->setObjectName("destinationEdit");

        flightFormLayout->addWidget(destinationEdit, 1, 1, 1, 1);

        priceLabel = new QLabel(flightFormGroup);
        priceLabel->setObjectName("priceLabel");

        flightFormLayout->addWidget(priceLabel, 1, 2, 1, 1);

        priceSpinBox = new QDoubleSpinBox(flightFormGroup);
        priceSpinBox->setObjectName("priceSpinBox");
        priceSpinBox->setRange(0.010000000000000);
        priceSpinBox->setMaximum(10000.000000000000000);
        priceSpinBox->setValue(299.990000000000009);
        priceSpinBox->setDecimals(2);

        flightFormLayout->addWidget(priceSpinBox, 1, 3, 1, 1);


        flightsLayout->addWidget(flightFormGroup);

        flightButtonsLayout = new QHBoxLayout();
        flightButtonsLayout->setObjectName("flightButtonsLayout");
        addFlightButton = new QPushButton(flightsTab);
        addFlightButton->setObjectName("addFlightButton");

        flightButtonsLayout->addWidget(addFlightButton);

        editFlightButton = new QPushButton(flightsTab);
        editFlightButton->setObjectName("editFlightButton");

        flightButtonsLayout->addWidget(editFlightButton);

        deleteFlightButton = new QPushButton(flightsTab);
        deleteFlightButton->setObjectName("deleteFlightButton");

        flightButtonsLayout->addWidget(deleteFlightButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        flightButtonsLayout->addItem(horizontalSpacer);


        flightsLayout->addLayout(flightButtonsLayout);

        flightTable = new QTableWidget(flightsTab);
        flightTable->setObjectName("flightTable");
        flightTable->setAlternatingRowColors(true);
        flightTable->setSelectionBehavior(QAbstractItemView::SelectRows);

        flightsLayout->addWidget(flightTable);

        tabWidget->addTab(flightsTab, QString());
        usersTab = new QWidget();
        usersTab->setObjectName("usersTab");
        usersLayout = new QVBoxLayout(usersTab);
        usersLayout->setObjectName("usersLayout");
        userTable = new QTableWidget(usersTab);
        userTable->setObjectName("userTable");
        userTable->setAlternatingRowColors(true);
        userTable->setSelectionBehavior(QAbstractItemView::SelectRows);

        usersLayout->addWidget(userTable);

        tabWidget->addTab(usersTab, QString());
        bookingsTab = new QWidget();
        bookingsTab->setObjectName("bookingsTab");
        bookingsLayout = new QVBoxLayout(bookingsTab);
        bookingsLayout->setObjectName("bookingsLayout");
        bookingTable = new QTableWidget(bookingsTab);
        bookingTable->setObjectName("bookingTable");
        bookingTable->setAlternatingRowColors(true);
        bookingTable->setSelectionBehavior(QAbstractItemView::SelectRows);

        bookingsLayout->addWidget(bookingTable);

        tabWidget->addTab(bookingsTab, QString());
        reportsTab = new QWidget();
        reportsTab->setObjectName("reportsTab");
        reportsLayout = new QVBoxLayout(reportsTab);
        reportsLayout->setObjectName("reportsLayout");
        dateRangeGroup = new QGroupBox(reportsTab);
        dateRangeGroup->setObjectName("dateRangeGroup");
        dateRangeLayout = new QHBoxLayout(dateRangeGroup);
        dateRangeLayout->setObjectName("dateRangeLayout");
        fromLabel = new QLabel(dateRangeGroup);
        fromLabel->setObjectName("fromLabel");

        dateRangeLayout->addWidget(fromLabel);

        startDateEdit = new QDateEdit(dateRangeGroup);
        startDateEdit->setObjectName("startDateEdit");
        startDateEdit->setCalendarPopup(true);

        dateRangeLayout->addWidget(startDateEdit);

        toLabel = new QLabel(dateRangeGroup);
        toLabel->setObjectName("toLabel");

        dateRangeLayout->addWidget(toLabel);

        endDateEdit = new QDateEdit(dateRangeGroup);
        endDateEdit->setObjectName("endDateEdit");
        endDateEdit->setCalendarPopup(true);

        dateRangeLayout->addWidget(endDateEdit);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        dateRangeLayout->addItem(horizontalSpacer_2);


        reportsLayout->addWidget(dateRangeGroup);

        reportButtonsLayout = new QHBoxLayout();
        reportButtonsLayout->setObjectName("reportButtonsLayout");
        generateReportButton = new QPushButton(reportsTab);
        generateReportButton->setObjectName("generateReportButton");

        reportButtonsLayout->addWidget(generateReportButton);

        exportReportButton = new QPushButton(reportsTab);
        exportReportButton->setObjectName("exportReportButton");

        reportButtonsLayout->addWidget(exportReportButton);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        reportButtonsLayout->addItem(horizontalSpacer_3);


        reportsLayout->addLayout(reportButtonsLayout);

        reportTextEdit = new QTextEdit(reportsTab);
        reportTextEdit->setObjectName("reportTextEdit");
        reportTextEdit->setReadOnly(true);

        reportsLayout->addWidget(reportTextEdit);

        tabWidget->addTab(reportsTab, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(AdminWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(AdminWindow);
    } // setupUi

    void retranslateUi(QWidget *AdminWindow)
    {
        AdminWindow->setWindowTitle(QCoreApplication::translate("AdminWindow", "Admin Dashboard", nullptr));
        titleLabel->setText(QCoreApplication::translate("AdminWindow", "Admin Dashboard", nullptr));
        titleLabel->setStyleSheet(QCoreApplication::translate("AdminWindow", "font-size: 24px; font-weight: bold; color: #2c3e50; margin-bottom: 10px;", nullptr));
        statsGroup->setTitle(QCoreApplication::translate("AdminWindow", "Statistics", nullptr));
        totalFlightsLabel->setText(QCoreApplication::translate("AdminWindow", "Total Flights: 0", nullptr));
        totalBookingsLabel->setText(QCoreApplication::translate("AdminWindow", "Total Bookings: 0", nullptr));
        totalRevenueLabel->setText(QCoreApplication::translate("AdminWindow", "Total Revenue: $0.00", nullptr));
        totalUsersLabel->setText(QCoreApplication::translate("AdminWindow", "Total Users: 0", nullptr));
        refreshButton->setText(QCoreApplication::translate("AdminWindow", "Refresh Dashboard", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(dashboardTab), QCoreApplication::translate("AdminWindow", "Dashboard", nullptr));
        flightFormGroup->setTitle(QCoreApplication::translate("AdminWindow", "Flight Information", nullptr));
        flightNumberLabel->setText(QCoreApplication::translate("AdminWindow", "Flight Number:", nullptr));
        flightNumberEdit->setPlaceholderText(QCoreApplication::translate("AdminWindow", "e.g., CW101", nullptr));
        originLabel->setText(QCoreApplication::translate("AdminWindow", "Origin:", nullptr));
        originEdit->setPlaceholderText(QCoreApplication::translate("AdminWindow", "e.g., New York", nullptr));
        destinationLabel->setText(QCoreApplication::translate("AdminWindow", "Destination:", nullptr));
        destinationEdit->setPlaceholderText(QCoreApplication::translate("AdminWindow", "e.g., Los Angeles", nullptr));
        priceLabel->setText(QCoreApplication::translate("AdminWindow", "Price ($):", nullptr));
        addFlightButton->setText(QCoreApplication::translate("AdminWindow", "Add Flight", nullptr));
        editFlightButton->setText(QCoreApplication::translate("AdminWindow", "Edit Flight", nullptr));
        deleteFlightButton->setText(QCoreApplication::translate("AdminWindow", "Delete Flight", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(flightsTab), QCoreApplication::translate("AdminWindow", "Flight Management", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(usersTab), QCoreApplication::translate("AdminWindow", "User Management", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(bookingsTab), QCoreApplication::translate("AdminWindow", "Booking Management", nullptr));
        dateRangeGroup->setTitle(QCoreApplication::translate("AdminWindow", "Date Range", nullptr));
        fromLabel->setText(QCoreApplication::translate("AdminWindow", "From:", nullptr));
        toLabel->setText(QCoreApplication::translate("AdminWindow", "To:", nullptr));
        generateReportButton->setText(QCoreApplication::translate("AdminWindow", "Generate Report", nullptr));
        exportReportButton->setText(QCoreApplication::translate("AdminWindow", "Export Report", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(reportsTab), QCoreApplication::translate("AdminWindow", "Reports", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AdminWindow: public Ui_AdminWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADMINWINDOW_H
