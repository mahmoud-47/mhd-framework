#ifndef MHD_DATETIME_HPP
#define MHD_DATETIME_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <vector>

// Custom exception class for datetime-related errors
class DateTimeException : public std::exception {
private:
    std::string message;
    
public:
    explicit DateTimeException(const std::string& msg) : message(msg) {}
    
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};

class MhdDateTime {
private:
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int second;
    
    mutable tm tm_;  // Cached tm structure for formatting operations
    
    // Private helper methods
    void is_correct() const;
    int get_days_in_month(int month, int year) const;
    bool is_leap_year(int year) const;
    void update_tm_structure() const;
    
public:
    // Constructors
    MhdDateTime();                                                           // Default constructor (current datetime)
    MhdDateTime(const MhdDateTime& dt);                                     // Copy constructor
    MhdDateTime(int day, int month, int year);                              // Date only (time set to 00:00:00)
    MhdDateTime(int day, int month, int year, int hour, int minute, int second = 0); // Full datetime
    MhdDateTime(const std::string& str);                                    // From string (various formats)
    
    // Destructor
    ~MhdDateTime() = default;
    
    // Getters - Date
    int get_day() const;
    int get_month() const;
    int get_year() const;
    std::string get_day_name() const;
    std::string get_month_name() const;
    
    // Getters - Time
    int get_hour() const;
    int get_minute() const;
    int get_second() const;
    
    // Setters
    void set_date(int day, int month, int year);
    void set_time(int hour, int minute, int second = 0);
    void set_datetime(int day, int month, int year, int hour, int minute, int second = 0);
    
    // DateTime validation and properties
    bool is_bissextil() const;                          // Check if year is leap year
    int get_day_week() const;                           // Get day of week (0=Monday, 6=Sunday)
    bool is_today() const;                              // Check if this date is today
    bool is_now() const;                                // Check if this datetime is now (within 1 minute)
    
    // DateTime manipulation
    MhdDateTime next_day() const;                       // Get next day (same time)
    MhdDateTime previous_day() const;                   // Get previous day (same time)
    MhdDateTime add_hours(int hours) const;             // Add hours
    MhdDateTime add_minutes(int minutes) const;         // Add minutes
    MhdDateTime add_seconds(int seconds) const;         // Add seconds
    
    // Comparison and difference
    int diff_days(const MhdDateTime& other) const;      // Days difference
    int diff_hours(const MhdDateTime& other) const;     // Hours difference
    int diff_minutes(const MhdDateTime& other) const;   // Minutes difference
    int diff_seconds(const MhdDateTime& other) const;   // Seconds difference
    
    // Formatting and conversion
    std::string to_string(const std::string& format = "default") const;
    std::string to_date_string(const std::string& format = "default") const;
    std::string to_time_string(const std::string& format = "default") const;
    std::string to_sqlite_string() const;               // SQLite datetime format
    std::string to_iso_string() const;                  // ISO 8601 format
    
    // Utility functions
    int age_in_years(const MhdDateTime& to_datetime) const;
    int age_in_years() const;
    time_t to_timestamp() const;                        // Convert to Unix timestamp
    static MhdDateTime from_timestamp(time_t timestamp); // Create from Unix timestamp
    
    // Operators
    MhdDateTime& operator=(const MhdDateTime& dt);      // Assignment operator
    
    // Arithmetic operators
    MhdDateTime operator+(int seconds) const;           // Add seconds
    MhdDateTime operator-(int seconds) const;           // Subtract seconds
    int operator-(const MhdDateTime& dt) const;         // Seconds between datetimes
    
    // Comparison operators
    bool operator==(const MhdDateTime& dt) const;
    bool operator<(const MhdDateTime& dt) const;
    bool operator<=(const MhdDateTime& dt) const;
    bool operator>(const MhdDateTime& dt) const;
    bool operator>=(const MhdDateTime& dt) const;
    
    // Stream operator
    friend std::ostream& operator<<(std::ostream& os, const MhdDateTime& dt);
};

// Utility functions
std::vector<std::string> split_string(const std::string& str, char delimiter);

#endif // MHD_DATETIME_HPP