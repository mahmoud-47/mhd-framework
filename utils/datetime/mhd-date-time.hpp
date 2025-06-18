#ifndef MHD_DATETIME_HPP
#define MHD_DATETIME_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <vector>

#include "../Exception/exception.hpp"

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
    // Default constructor (current datetime)
    MhdDateTime();              
    // Copy constructor
    MhdDateTime(const MhdDateTime& dt);
    // Date only [d, m, y] (time set to 00:00:00)                                     
    MhdDateTime(int day, int month, int year);
    // Full datetime [d, m, y, h, mn, s], default seconds = 0                         
    MhdDateTime(int day, int month, int year, int hour, int minute, int second = 0); 
    // From string (various formats) either "yyyy-mm-dd hh:mm:ss" or "dd-mm-yyyy"
    MhdDateTime(const std::string& str);                                    
    
    // Destructor
    ~MhdDateTime() = default;
    
    // Getters - Date

    // get day [1 - 31]
    int get_day() const;
    // get month [1 - 12]
    int get_month() const;
    // gey year
    int get_year() const;
    // get day name [Monday, ..]
    std::string get_day_name() const;
    // get month // [January, ..]
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

    // Check if year is leap year
    bool is_bissextil() const;       
    // Get day of week (0=Monday, 6=Sunday)                   
    int get_day_week() const;    
    // Check if this date is today                       
    bool is_today() const;          
    // Check if this datetime is now (within 1 minute)                    
    bool is_now() const;                                
    
    // DateTime manipulation

    // Get next day (same time)
    MhdDateTime next_day() const;  
    // Get previous day (same time)                    
    MhdDateTime previous_day() const; 
    // Add hours                  
    MhdDateTime add_hours(int hours) const;  
    // Add minutes           
    MhdDateTime add_minutes(int minutes) const;   
    // Add seconds      
    MhdDateTime add_seconds(int seconds) const;         
    
    // Comparison and difference

    // Days difference
    int diff_days(const MhdDateTime& other) const;   
    // Hours difference   
    int diff_hours(const MhdDateTime& other) const;
    // Minutes difference     
    int diff_minutes(const MhdDateTime& other) const;
    // Seconds difference   
    int diff_seconds(const MhdDateTime& other) const;   
    
    // Formatting and conversion

    // format should be in ['sqlite', 'SQL', 'iso', 'ISO'] or if not specified = 'default' -> 17-06-2025 19:17:52
    std::string to_string(const std::string& format = "default") const;
    // format should be in ['iso', 'ISO'] -> 2025-06-17 or if not specified = 'default' -> 17-06-2025
    std::string to_date_string(const std::string& format = "default") const;
    // always hh:mm:ss
    std::string to_time_string(const std::string& format = "default") const;
    // SQLite datetime format -> 2025-06-17 19:17:52
    std::string to_sqlite_string() const;      
    // ISO 8601 format -> 2025-06-17T19:17:52         
    std::string to_iso_string() const;                  
    
    // Utility functions

    // years between [to_datetime - this date]
    int age_in_years(const MhdDateTime& to_datetime) const;
    // years between [today - this date]
    int age_in_years() const;
    // Convert to Unix timestamp
    time_t to_timestamp() const;           
    // Create from Unix timestamp             
    static MhdDateTime from_timestamp(time_t timestamp); 
    
    // Operators
    MhdDateTime& operator=(const MhdDateTime& dt);      // Assignment operator
    
    // Arithmetic operators
    MhdDateTime operator+(int seconds) const;           // Add seconds
    MhdDateTime operator-(int seconds) const;           // Subtract seconds
    // Seconds between datetimes
    int operator-(const MhdDateTime& dt) const;         
    
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