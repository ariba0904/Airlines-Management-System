/********************************************************************************
** Form generated from reading UI file 'passengerwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PASSENGERWINDOW_H
#define UI_PASSENGERWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PassengerWindow
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *welcomeLabel;
    QTabWidget *tabWidget;
    QWidget *searchTab;
    QVBoxLayout *searchLayout;
    QGroupBox *searchGroup;
    QGridLayout *searchGrid;
    QLabel *fromLabel;
    QComboBox *fromComboBox;
    QLabel *toLabel;
    QComboBox *toComboBox;
    QLabel *dateLabel;
    QDateEdit *dateEdit;
    QLabel *passengersLabel;
    QSpinBox *passengersSpinBox;
    QHBoxLayout *searchButtonsLayout;
    QPushButton *searchButton;
    QPushButton *clearButton;
    QSpacerItem *horizontalSpacer;
    QTableWidget *flightTable;
    QWidget *bookingsTab;
    QVBoxLayout *bookingsLayout;
    QTableWidget *bookingTable;
    QHBoxLayout *bookingButtonsLayout;
    QPushButton *cancelBookingButton;
    QPushButton *refreshBookingsButton;
    QSpacerItem *horizontalSpacer_2;
    QWidget *profileTab;
    QVBoxLayout *profileLayout;
    QGroupBox *profileGroup;
    QFormLayout *profileForm;
    QLabel *usernameLabel;
    QLineEdit *usernameEdit;
    QLabel *emailLabel;
    QLineEdit *emailEdit;
    QLabel *fullNameLabel;
    QLineEdit *fullNameEdit;
    QLabel *phoneLabel;
    QLineEdit *phoneEdit;
    QHBoxLayout *profileButtonsLayout;
    QPushButton *editProfileButton;
    QPushButton *changePasswordButton;
    QSpacerItem *horizontalSpacer_3;

    void setupUi(QWidget *PassengerWindow)
    {
        if (PassengerWindow->objectName().isEmpty())
            PassengerWindow->setObjectName("PassengerWindow");
        PassengerWindow->resize(1000, 700);
        verticalLayout = new QVBoxLayout(PassengerWindow);
        verticalLayout->setObjectName("verticalLayout");
        welcomeLabel = new QLabel(PassengerWindow);
        welcomeLabel->setObjectName("welcomeLabel");

        verticalLayout->addWidget(welcomeLabel);

        tabWidget = new QTabWidget(PassengerWindow);
        tabWidget->setObjectName("tabWidget");
        searchTab = new QWidget();
        searchTab->setObjectName("searchTab");
        searchLayout = new QVBoxLayout(searchTab);
        searchLayout->setObjectName("searchLayout");
        searchGroup = new QGroupBox(searchTab);
        searchGroup->setObjectName("searchGroup");
        searchGrid = new QGridLayout(searchGroup);
        searchGrid->setObjectName("searchGrid");
        fromLabel = new QLabel(searchGroup);
        fromLabel->setObjectName("fromLabel");

        searchGrid->addWidget(fromLabel, 0, 0, 1, 1);

        fromComboBox = new QComboBox(searchGroup);
        fromComboBox->setObjectName("fromComboBox");

        searchGrid->addWidget(fromComboBox, 0, 1, 1, 1);

        toLabel = new QLabel(searchGroup);
        toLabel->setObjectName("toLabel");

        searchGrid->addWidget(toLabel, 0, 2, 1, 1);

        toComboBox = new QComboBox(searchGroup);
        toComboBox->setObjectName("toComboBox");

        searchGrid->addWidget(toComboBox, 0, 3, 1, 1);

        dateLabel = new QLabel(searchGroup);
        dateLabel->setObjectName("dateLabel");

        searchGrid->addWidget(dateLabel, 1, 0, 1, 1);

        dateEdit = new QDateEdit(searchGroup);
        dateEdit->setObjectName("dateEdit");
        dateEdit->setCalendarPopup(true);

        searchGrid->addWidget(dateEdit, 1, 1, 1, 1);

        passengersLabel = new QLabel(searchGroup);
        passengersLabel->setObjectName("passengersLabel");

        searchGrid->addWidget(passengersLabel, 1, 2, 1, 1);

        passengersSpinBox = new QSpinBox(searchGroup);
        passengersSpinBox->setObjectName("passengersSpinBox");
        passengersSpinBox->setMinimum(1);
        passengersSpinBox->setMaximum(10);
        passengersSpinBox->setValue(1);

        searchGrid->addWidget(passengersSpinBox, 1, 3, 1, 1);


        searchLayout->addWidget(searchGroup);

        searchButtonsLayout = new QHBoxLayout();
        searchButtonsLayout->setObjectName("searchButtonsLayout");
        searchButton = new QPushButton(searchTab);
        searchButton->setObjectName("searchButton");

        searchButtonsLayout->addWidget(searchButton);

        clearButton = new QPushButton(searchTab);
        clearButton->setObjectName("clearButton");

        searchButtonsLayout->addWidget(clearButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        searchButtonsLayout->addItem(horizontalSpacer);


        searchLayout->addLayout(searchButtonsLayout);

        flightTable = new QTableWidget(searchTab);
        flightTable->setObjectName("flightTable");
        flightTable->setAlternatingRowColors(true);
        flightTable->setSelectionBehavior(QAbstractItemView::SelectRows);

        searchLayout->addWidget(flightTable);

        tabWidget->addTab(searchTab, QString());
        bookingsTab = new QWidget();
        bookingsTab->setObjectName("bookingsTab");
        bookingsLayout = new QVBoxLayout(bookingsTab);
        bookingsLayout->setObjectName("bookingsLayout");
        bookingTable = new QTableWidget(bookingsTab);
        bookingTable->setObjectName("bookingTable");
        bookingTable->setAlternatingRowColors(true);
        bookingTable->setSelectionBehavior(QAbstractItemView::SelectRows);

        bookingsLayout->addWidget(bookingTable);

        bookingButtonsLayout = new QHBoxLayout();
        bookingButtonsLayout->setObjectName("bookingButtonsLayout");
        cancelBookingButton = new QPushButton(bookingsTab);
        cancelBookingButton->setObjectName("cancelBookingButton");

        bookingButtonsLayout->addWidget(cancelBookingButton);

        refreshBookingsButton = new QPushButton(bookingsTab);
        refreshBookingsButton->setObjectName("refreshBookingsButton");

        bookingButtonsLayout->addWidget(refreshBookingsButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        bookingButtonsLayout->addItem(horizontalSpacer_2);


        bookingsLayout->addLayout(bookingButtonsLayout);

        tabWidget->addTab(bookingsTab, QString());
        profileTab = new QWidget();
        profileTab->setObjectName("profileTab");
        profileLayout = new QVBoxLayout(profileTab);
        profileLayout->setObjectName("profileLayout");
        profileGroup = new QGroupBox(profileTab);
        profileGroup->setObjectName("profileGroup");
        profileForm = new QFormLayout(profileGroup);
        profileForm->setObjectName("profileForm");
        usernameLabel = new QLabel(profileGroup);
        usernameLabel->setObjectName("usernameLabel");

        profileForm->setWidget(0, QFormLayout::ItemRole::LabelRole, usernameLabel);

        usernameEdit = new QLineEdit(profileGroup);
        usernameEdit->setObjectName("usernameEdit");
        usernameEdit->setReadOnly(true);

        profileForm->setWidget(0, QFormLayout::ItemRole::FieldRole, usernameEdit);

        emailLabel = new QLabel(profileGroup);
        emailLabel->setObjectName("emailLabel");

        profileForm->setWidget(1, QFormLayout::ItemRole::LabelRole, emailLabel);

        emailEdit = new QLineEdit(profileGroup);
        emailEdit->setObjectName("emailEdit");
        emailEdit->setReadOnly(true);

        profileForm->setWidget(1, QFormLayout::ItemRole::FieldRole, emailEdit);

        fullNameLabel = new QLabel(profileGroup);
        fullNameLabel->setObjectName("fullNameLabel");

        profileForm->setWidget(2, QFormLayout::ItemRole::LabelRole, fullNameLabel);

        fullNameEdit = new QLineEdit(profileGroup);
        fullNameEdit->setObjectName("fullNameEdit");
        fullNameEdit->setReadOnly(true);

        profileForm->setWidget(2, QFormLayout::ItemRole::FieldRole, fullNameEdit);

        phoneLabel = new QLabel(profileGroup);
        phoneLabel->setObjectName("phoneLabel");

        profileForm->setWidget(3, QFormLayout::ItemRole::LabelRole, phoneLabel);

        phoneEdit = new QLineEdit(profileGroup);
        phoneEdit->setObjectName("phoneEdit");
        phoneEdit->setReadOnly(true);

        profileForm->setWidget(3, QFormLayout::ItemRole::FieldRole, phoneEdit);


        profileLayout->addWidget(profileGroup);

        profileButtonsLayout = new QHBoxLayout();
        profileButtonsLayout->setObjectName("profileButtonsLayout");
        editProfileButton = new QPushButton(profileTab);
        editProfileButton->setObjectName("editProfileButton");

        profileButtonsLayout->addWidget(editProfileButton);

        changePasswordButton = new QPushButton(profileTab);
        changePasswordButton->setObjectName("changePasswordButton");

        profileButtonsLayout->addWidget(changePasswordButton);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        profileButtonsLayout->addItem(horizontalSpacer_3);


        profileLayout->addLayout(profileButtonsLayout);

        tabWidget->addTab(profileTab, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(PassengerWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(PassengerWindow);
    } // setupUi

    void retranslateUi(QWidget *PassengerWindow)
    {
        PassengerWindow->setWindowTitle(QCoreApplication::translate("PassengerWindow", "Passenger Dashboard", nullptr));
        welcomeLabel->setText(QCoreApplication::translate("PassengerWindow", "Welcome, Passenger!", nullptr));
        welcomeLabel->setStyleSheet(QCoreApplication::translate("PassengerWindow", "font-size: 24px; font-weight: bold; color: #2c3e50; margin-bottom: 10px;", nullptr));
        searchGroup->setTitle(QCoreApplication::translate("PassengerWindow", "Flight Search", nullptr));
        fromLabel->setText(QCoreApplication::translate("PassengerWindow", "From:", nullptr));
        fromComboBox->setPlaceholderText(QCoreApplication::translate("PassengerWindow", "Select departure city", nullptr));
        toLabel->setText(QCoreApplication::translate("PassengerWindow", "To:", nullptr));
        toComboBox->setPlaceholderText(QCoreApplication::translate("PassengerWindow", "Select arrival city", nullptr));
        dateLabel->setText(QCoreApplication::translate("PassengerWindow", "Date:", nullptr));
        passengersLabel->setText(QCoreApplication::translate("PassengerWindow", "Passengers:", nullptr));
        searchButton->setText(QCoreApplication::translate("PassengerWindow", "Search Flights", nullptr));
        clearButton->setText(QCoreApplication::translate("PassengerWindow", "Clear", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(searchTab), QCoreApplication::translate("PassengerWindow", "Search Flights", nullptr));
        cancelBookingButton->setText(QCoreApplication::translate("PassengerWindow", "Cancel Booking", nullptr));
        refreshBookingsButton->setText(QCoreApplication::translate("PassengerWindow", "Refresh", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(bookingsTab), QCoreApplication::translate("PassengerWindow", "My Bookings", nullptr));
        profileGroup->setTitle(QCoreApplication::translate("PassengerWindow", "Profile Information", nullptr));
        usernameLabel->setText(QCoreApplication::translate("PassengerWindow", "Username:", nullptr));
        emailLabel->setText(QCoreApplication::translate("PassengerWindow", "Email:", nullptr));
        fullNameLabel->setText(QCoreApplication::translate("PassengerWindow", "Full Name:", nullptr));
        phoneLabel->setText(QCoreApplication::translate("PassengerWindow", "Phone:", nullptr));
        editProfileButton->setText(QCoreApplication::translate("PassengerWindow", "Edit Profile", nullptr));
        changePasswordButton->setText(QCoreApplication::translate("PassengerWindow", "Change Password", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(profileTab), QCoreApplication::translate("PassengerWindow", "My Profile", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PassengerWindow: public Ui_PassengerWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PASSENGERWINDOW_H
