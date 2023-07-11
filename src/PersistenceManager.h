#ifndef PersistenceManager_h
#define PersistenceManager_h

#include "Arduino.h"
#include <ArduinoJson.h>
#include "FS.h"
#include "LittleFS.h"

// The maximum size of the JSON document.
#define DEFAULT_JSON_SIZE 2048

class PersistenceManager
{
public:
    // Constructor now has an additional parameter for maximum JSON size
    PersistenceManager(const char *filename, size_t maxJsonSize = DEFAULT_JSON_SIZE);

    template <typename T>
    void set(const char *key, T value)
    {
        xSemaphoreTake(_mutex, portMAX_DELAY);
        _doc[key] = value;
        if (_autosave)
        {
            save();
        }
        xSemaphoreGive(_mutex);
    }
    // Method to get the value of a key.
    JsonVariant get(const char *key);

    // Method to remove a key-value pair.
    bool remove(const char *key);

    // Method to check if a key exists in the document.
    bool exists(const char *key);

    // Method to list all keys in the document.
    String listKeys();

    // Method to save the JSON document to the file.
    bool save();

    // Method to clear all key-value pairs.
    bool clear();

    // Method to backup the data to another file.
    bool backup(const char *backupFilename);

    // Method to load the JSON document from the file.
    bool load();

    // The JSON document.
    DynamicJsonDocument _doc;

    // Returns the size of the file associated with this PersistenceManager instance.
    size_t getFileSize();

    void printFileContents();

private:
    // The filename of the file to save to and load from.
    const char *_filename;

    size_t _maxJsonSize;

    // Semaphore for mutual exclusion.
    SemaphoreHandle_t _mutex;

    // Method to check if there is enough space on the file system.
    bool _checkAvailableSpace();

    // Method to begin a transaction.
    bool _beginTransaction();

    // Method to end a transaction.
    void _endTransaction();

    bool _autosave = false;
};

#endif
