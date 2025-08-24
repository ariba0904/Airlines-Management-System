#ifndef HELPERS_H
#define HELPERS_H

#include <QString>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QMap>
#include <QVariant>

class Helpers
{
public:
    // Date and time formatting
    static QString formatDateTime(const QDateTime& dateTime, const QString& format = "dd/MM/yyyy hh:mm AP");
    static QString formatDate(const QDate& date, const QString& format = "dd/MM/yyyy");
    static QString formatTime(const QTime& time, const QString& format = "hh:mm AP");
    static QString formatDuration(const QDateTime& start, const QDateTime& end);
    
    // Currency formatting
    static QString formatCurrency(double amount, const QString& currency = "$");
    static QString formatPrice(double price);
    
    // String utilities
    static QString capitalize(const QString& text);
    static QString truncate(const QString& text, int maxLength);
    static QString generateRandomString(int length);
    static QString sanitizeInput(const QString& input);
    
    // Validation helpers
    static bool isNumeric(const QString& text);
    static bool isAlphaNumeric(const QString& text);
    static bool isValidDateRange(const QDate& start, const QDate& end);
    static bool isValidTimeRange(const QTime& start, const QTime& end);
    
    // Data conversion
    static QDateTime stringToDateTime(const QString& dateTimeStr);
    static QString dateTimeToString(const QDateTime& dateTime);
    static double stringToDouble(const QString& numberStr, bool* ok = nullptr);
    static int stringToInt(const QString& numberStr, bool* ok = nullptr);
    
    // Business logic helpers
    static double calculateTotalPrice(double basePrice, int seatCount);
    static int calculateAvailableSeats(int totalSeats, int bookedSeats);
    static double calculateOccupancyRate(int totalSeats, int bookedSeats);
    static QString getFlightStatus(const QDateTime& departureTime);
    static QString getBookingStatus(const QString& status);
    
    // UI helpers
    static QString getStatusColor(const QString& status);
    static QString getPriorityColor(int priority);
    static QString formatFileSize(qint64 bytes);
    static QString formatPercentage(double value);
};

#endif // HELPERS_H 