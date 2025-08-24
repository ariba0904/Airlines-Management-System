#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QDateTime>

class Database : public QObject
{
    Q_OBJECT

public:
    static Database& getInstance();
    
    bool initialize(const QString& dbPath);
    bool createTables();
    bool isConnected() const;
    
    // User management
    bool addUser(const QString& username, const QString& password, const QString& email, 
                 const QString& fullName, const QString& role);
    bool authenticateUser(const QString& username, const QString& password, QString& role);
    bool userExists(const QString& username);
    bool updateUserProfile(const QString& username, const QString& email, const QString& fullName);
    bool changePassword(const QString& username, const QString& newPassword);
    QList<QMap<QString, QVariant>> getAllUsers();
    QMap<QString, QVariant> getUserByUsername(const QString& username);
    bool deleteUser(const QString& username);
    
    // Flight management
    bool addFlight(const QString& flightNumber, const QString& origin, const QString& destination,
                   const QDateTime& departureTime, const QDateTime& arrivalTime,
                   int totalSeats, double price, const QString& aircraftModel);
    bool updateFlight(const QString& flightNumber, const QString& origin, const QString& destination,
                      const QDateTime& departureTime, const QDateTime& arrivalTime,
                      int totalSeats, double price, const QString& aircraftModel);
    bool deleteFlight(const QString& flightNumber);
    QList<QMap<QString, QVariant>> getFlights(const QString& origin = "", const QString& destination = "",
                                              const QDate& date = QDate());
    QMap<QString, QVariant> getFlight(const QString& flightNumber);
    bool flightExists(const QString& flightNumber);
    
    // Booking management
    bool createBooking(const QString& username, const QString& flightNumber, 
                       int seatCount, double totalPrice, const QString& bookingDate);
    bool cancelBooking(const QString& bookingId);
    bool cancelBooking(const QString& bookingId, int seatsToCancel);
    QList<QMap<QString, QVariant>> getUserBookings(const QString& username);
    QList<QMap<QString, QVariant>> getAllBookings();
    QMap<QString, QVariant> getBooking(const QString& bookingId);
    QString generateBookingId();
    
    // Seat management
    bool updateSeatAvailability(const QString& flightNumber, int bookedSeats);
    int getAvailableSeats(const QString& flightNumber);
    bool reserveSeats(const QString& flightNumber, int seatCount);
    bool releaseSeats(const QString& flightNumber, int seatCount);
    
    // Reports and statistics
    QMap<QString, QVariant> getDashboardStats();
    QList<QMap<QString, QVariant>> getRevenueReport(const QDate& startDate, const QDate& endDate);
    QList<QMap<QString, QVariant>> getFlightReport(const QDate& date);
    
    QList<QMap<QString, QVariant>> searchFlights(const QString& origin, const QString& destination, 
                                                 const QDate& date, int passengers);
    QList<QMap<QString, QVariant>> getFlightsByDate(const QDate& date);

    // Expose password hashing for UI validation
    QString hashPassword(const QString& password);

private:
    Database(QObject *parent = nullptr);
    ~Database();
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    
    QSqlDatabase m_db;
    bool m_initialized;
    
    bool verifyPassword(const QString& password, const QString& hash);
};

#endif // DATABASE_H 