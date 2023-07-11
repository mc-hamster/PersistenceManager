#include "PersistenceManager.h"

// Constructor initializes the member variables and loads the JSON document
PersistenceManager::PersistenceManager(const char *filename, size_t maxJsonSize)
    : _filename(filename), _maxJsonSize(maxJsonSize), _doc(maxJsonSize)
{
    _mutex = xSemaphoreCreateMutex();

    load();
}

// Get method to retrieve a value for a key
JsonVariant PersistenceManager::get(const char *key)
{
    xSemaphoreTake(_mutex, portMAX_DELAY);
    JsonVariant value = _doc[key];
    xSemaphoreGive(_mutex);
    return value;
}

// Remove method to delete a key-value pair
bool PersistenceManager::remove(const char *key)
{
    xSemaphoreTake(_mutex, portMAX_DELAY);
    bool exists = _doc.containsKey(key);
    if (exists)
    {
        _doc.remove(key);
    }
    xSemaphoreGive(_mutex);
    return exists;
}

// Exists method to check if a key exists
bool PersistenceManager::exists(const char *key)
{
    xSemaphoreTake(_mutex, portMAX_DELAY);
    bool result = _doc.containsKey(key);
    xSemaphoreGive(_mutex);
    return result;
}

// Method to list all keys
String PersistenceManager::listKeys()
{
    xSemaphoreTake(_mutex, portMAX_DELAY);
    String keys = "";
    for (JsonPair kv : _doc.as<JsonObject>())
    {
        keys += kv.key().c_str();
        keys += "\n";
    }
    xSemaphoreGive(_mutex);
    return keys;
}

// Save method to persist the JSON document to the file
bool PersistenceManager::save()
{
    if (_checkAvailableSpace())
    {
        if (_beginTransaction())
        {
            File file = LittleFS.open(_filename, "w");
            if (!file)
            {
                Serial.println("Failed to open file for writing");
                return false;
            }
            serializeJson(_doc, file);
            file.close();
            _endTransaction();
            return true;
        }
    }
    return false;
}

// Clear method to delete all key-value pairs
bool PersistenceManager::clear()
{
    xSemaphoreTake(_mutex, portMAX_DELAY);
    _doc.clear();
    xSemaphoreGive(_mutex);
    return save();
}

// Method to create a backup of the data file
bool PersistenceManager::backup(const char *backupFilename)
{
    return LittleFS.rename(_filename, backupFilename);
}

// Load method to load the JSON document from the file
bool PersistenceManager::load()
{
    File file = LittleFS.open(_filename, "r");
    if (!file)
    {
        Serial.println("Failed to open file for reading 1");
        return false;
    }
    deserializeJson(_doc, file);
    file.close();
    return true;
}

// Check if there's enough available space on the file system
bool PersistenceManager::_checkAvailableSpace()
{
    // FSInfo fs_info;
    // LittleFS.info(fs_info);
    // return fs_info.totalBytes - fs_info.usedBytes > _maxJsonSize;

    // TODO: Estiamte available space

    // For now, always return true.
    return true;
}

// Begin a transaction by taking the mutex
bool PersistenceManager::_beginTransaction()
{
    return xSemaphoreTake(_mutex, portMAX_DELAY) == pdTRUE;
}

// End a transaction by giving the mutex back
void PersistenceManager::_endTransaction()
{
    xSemaphoreGive(_mutex);
}

size_t PersistenceManager::getFileSize()
{
    xSemaphoreTake(_mutex, portMAX_DELAY);

    File file = LittleFS.open(_filename, "r");
    if (!file)
    {
        Serial.println("Failed to open file for reading 2");
        return 0;
    }

    size_t fileSize = file.size();
    file.close();

    xSemaphoreGive(_mutex);
    return fileSize;
}

void PersistenceManager::printFileContents()
{
    // Obtain the lock
    xSemaphoreTake(_mutex, portMAX_DELAY);

    // Open the file
    File file = LittleFS.open(_filename, "r");
    if (!file)
    {
        Serial.println("Failed to open file for reading");
    }

    // Read from the file and print each line
    while (file.available())
    {
        String line = file.readStringUntil('\n');
        Serial.println(line);
    }

    // Close the file
    file.close();

    // Release the lock
    xSemaphoreGive(_mutex);
}
